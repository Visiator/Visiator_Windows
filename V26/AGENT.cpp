#include "stdafx.h"

#include "AGENT.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "SERVICE.h"
#include "TOTAL_CONTROL.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;
extern TOTAL_CONTROL *total_control;

void zero_128_s(MASTER_AGENT_PACKET_HEADER *v);

AGENT::AGENT() {

}

AGENT::~AGENT() {

}

void AGENT::RUN() {

	sudp("AGENT::RUN()");

	app_attributes.is_agent = true;

	is_run_MAIN_THREAD = true;

	init_encode_color_matrix_all();

	/*
	start_tread_PIPE_CONTROL(); void AGENT::PIPE_CONTROL_EXECUTE() 
	start_tread_PIPE_READ_SERVICE_INFO(); PIPE_READ_SERVICE_INFO_EXECUTE()
	start_tread_PIPE_SLAVE();  PIPE_SLAVE_EXECUTE_2()
	*/

	thread_EXECUTE_control = app_attributes.tgroup.create_thread( boost::bind(&AGENT::EXECUTE_control, this) ); //  start_tread_PIPE_CONTROL(); void AGENT::PIPE_CONTROL_EXECUTE()
	
	thread_EXECUTE_read_service_info = app_attributes.tgroup.create_thread(boost::bind(&AGENT::EXECUTE_read_service_info, this)); // start_tread_PIPE_READ_SERVICE_INFO(); PIPE_READ_SERVICE_INFO_EXECUTE()

	thread_EXECUTE_read_write = app_attributes.tgroup.create_thread( boost::bind(&AGENT::EXECUTE_read_write, this) ); // start_tread_PIPE_SLAVE();  PIPE_SLAVE_EXECUTE_2()
	
	boost::posix_time::milliseconds SleepTime(10);

	while (GLOBAL_STOP == false) {

		//r = my_pipe.read_service_info(&service_info);

		boost::this_thread::sleep(SleepTime);
	}

	is_run_MAIN_THREAD = false;
}

void AGENT::EXECUTE_read_service_info() {
	EXECUTE_read_service_info_is_run = true;

	SERVICE_INFO service_info;
	bool r;

	DWORD last_read_service_info = 0;

	last_detect_MASTER = GetTickCount();



	boost::posix_time::milliseconds SleepTime(100);

	while (GLOBAL_STOP == false) {

		if (last_read_service_info + 500 < GetTickCount()) {


			//if (my_FileExists("c:\\1\\a.txt") == false) {

			pipe_handle_read_info_TIMEOUT = GetTickCount();

			zero_unsigned_char((unsigned char *)&service_info, 128);

			service_info.who_is_asked = IT_IS_agent;

			r = read_service_info(&service_info, &pipe_handle_read_info_);

			pipe_handle_read_info_TIMEOUT = 0;

			if (r == true) {

				//send_udp2("read_service_info() last_detect_MASTER");
				last_detect_MASTER = GetTickCount();
				total_control->AGENT_PIPE_READ_SERVICE_INFO_EXECUTE_master_detect_ok++;

				last_read_service_info = GetTickCount();
			}
			else {
				//send_udp2("read_service_info() fails");
				total_control->AGENT_PIPE_READ_SERVICE_INFO_EXECUTE_master_detect_fail++;
				boost::this_thread::sleep(SleepTime);
			}

			//};
		};

		if (last_detect_MASTER + 5000 < GetTickCount()) {
			// 
			sudp("MASTER is stoped? (b) GLOBAL_STOP = true");
			set_GLOBAL_STOP_true();
		}


		boost::this_thread::sleep(SleepTime);
	};

	EXECUTE_read_service_info_is_run = false;
}

void AGENT::EXECUTE_control() {  // PIPE_CONTROL_EXECUTE()
	EXECUTE_control_is_run = true;

	DWORD dd;
	dd = GetTickCount();


	boost::posix_time::milliseconds SleepTime(100);

	while (GLOBAL_STOP == false) {

		if (dd + 500 < GetTickCount()) {
			dd = GetTickCount();
			total_control->send_udp_AGENT();

		}

		if (pipe_handle_read_info_TIMEOUT > 0) {
			if (pipe_handle_read_info_TIMEOUT + 5000 < GetTickCount()) {
				// �� �� ������ �������� ����� �� MASTER-� . ��� ������
				sudp("MASTER is stoped? (a) GLOBAL_STOP = true");
				set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
			}
		}


		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_control_is_run = false;
}

void AGENT::EXECUTE_read_write() {
	
	DWORD err;
	char *r_buf_1 = nullptr;
	bool x;
	DWORD r;


	EXECUTE_read_write_is_run = true;

	r_buf_1 = new char[sizeof_MASTER_AGENT_PACKET_HEADER];
	MASTER_AGENT_PACKET_HEADER *packet_recv = nullptr;
	
	packet_recv = (MASTER_AGENT_PACKET_HEADER *)r_buf_1;


	boost::posix_time::milliseconds SleepTime(100);

	pipe_AGENT = CreateFile(L"\\\\.\\pipe\\$visiator_master$", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (pipe_AGENT == INVALID_HANDLE_VALUE) {

		err = GetLastError();

		sudp("info open pipe_error $visiator_master$");

		if (err == ERROR_ACCESS_DENIED) { sudp("ERROR_ACCESS_DENIED"); };
		if (err == ERROR_PIPE_BUSY) {  sudp("ERROR_PIPE_BUSY"); };
		if (err == ERROR_FILE_NOT_FOUND) { sudp("ERROR_FILE_NOT_FOUND"); };
		pipe_AGENT = 0;

		//send_udp("AGENT::PIPE_SLAVE_EXECUTE() open pipe error ! GLOBAL_STOP = true");

		delete[] r_buf_1; 

		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		EXECUTE_read_write_is_run = false;
		return;
	}
	else {
		sudp("AGENT::PIPE_SLAVE_EXECUTE_2() open pipe OK $visiator_master$");
	}


	while (GLOBAL_STOP == false) {

		x = read_pipe(pipe_AGENT, r_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_pipe_TIMEOUT, "i01");
		if (x == false) {
			// send_udp("PIPE_SLAVE_EXECUTE_2() (1) x == false  GLOBAL_STOP = true;");

			delete[] r_buf_1; //delete_ w_buf_2; delete_ w_buf_3; if (w_buf_4 != NULL) delete_ w_buf_4;

			EXECUTE_read_write_is_run = false;

			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;

			return;
		}

		///

		//***********************************************************************************************
		// analiz
		bool flag = false;

		if (flag == false && packet_recv->packet_size == 128 && packet_recv->packet_type == packet_type_REQUEST_SCREEN_one_byte) {
			flag = true;
			//send_udp("DETECT packet_type_REQUEST_SCREEN_one_byte");

			last_detect_MASTER = GetTickCount();

			//total_control.AGENT_PIPE_SLAVE_EXECUTE_2_screen++;

			PIPE_SLAVE_EXECUTE_2_____FINAL_SCREEN_one_byte();

		}
		if (flag == false && packet_recv->packet_size == 128 && packet_recv->packet_type == packet_type_REQUEST_STOP_AGENT) {
			flag = true;
			// send_udp("DETECT packet_type_REQUEST_STOP_AGENT   GLOBAL_STOP = true;");

			last_detect_MASTER = GetTickCount();

			PIPE_SLAVE_EXECUTE_2_____FINAL_STOP_AGENT();
			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;

		}
		if (flag == false && packet_recv->packet_size == 128 && packet_recv->packet_type == packet_type_REQUEST_EVENT) {
			flag = true;
			//send_udp("DETECT packet_type_REQUEST_EVENT");
			MASTER_AGENT_PACKET_HEADER_event *ev;
			ev = (MASTER_AGENT_PACKET_HEADER_event *)packet_recv->reserv;

			last_detect_MASTER = GetTickCount();

			//total_control.AGENT_PIPE_SLAVE_EXECUTE_2_request_event++;

			PIPE_SLAVE_EXECUTE_2_____FINAL_REQUEST_EVENT(ev);

			exec_event_in_to_session(ev->session_no, ev->event_type, ev->global_type, ev->msg, ev->wparam, ev->lparam); // 1
		}

		if (flag == false && packet_recv->packet_size == 128 && packet_recv->packet_type == packet_type_PING_MASTER_to_AGENT) {
			flag = true;

			PIPE_SLAVE_EXECUTE_2_____FINAL_PING();

		};

		if (flag == false) {
			//send_udp2("PIPE_SLAVE_EXECUTE_2() �� ���������� ������  GLOBAL_STOP = true;");
			delete[] r_buf_1; //delete_ w_buf_2; delete_ w_buf_3; if (w_buf_4 != NULL) delete_ w_buf_4;
			EXECUTE_read_write_is_run = false;
			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
			return;
		}

		///

		//boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_read_write_is_run = false;
}

void AGENT::PIPE_SLAVE_EXECUTE_2_____FINAL_SCREEN_one_byte() {

	bool x;
	DWORD w;

	MASTER_AGENT_PACKET_HEADER packet_send;
	SCREEN_LIGHT_one_byte screen_header;


	//***********************************************************************************************
	// write 128 (2)
	zero_128_s(&packet_send);
	//zero_void(&packet_send, sizeof_MASTER_AGENT_PACKET_HEADER);
	packet_send.packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send.packet_type = packet_type_RESPONCE_SCREEN_one_byte;

	x = write_pipe(pipe_AGENT, &packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_SLAVE_pipe_TIMEOUT);
	if (x == false) {
		//send_udp("PIPE_SLAVE_EXECUTE_2() (2-) x == false  GLOBAL_STOP = true;");
		//delete[] w_buf_2; 
		//delete[] w_buf_3; //if (w_buf_4 != NULL) delete[] w_buf_4;
		EXECUTE_read_write_is_run = false;
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		return;
	}

	//***********************************************************************************************
	// write 128 (3)
	clean_ENCODED_SCREEN_8bit_header(&screen_header.header);

	//zero(&screen_header.header, sizeof_ENCODED_SCREEN_8bit_header);
	bool result;
	result = get_screenshot(&screen_header, nullptr);
	if (result == false) {
		screen_header.emulate_dark_blue();

		CHECK_DESKTOP();

	}
	/*if (result == false) {
		send_udp("get_screenshot() == false  GLOBAL_STOP = true;");
		delete_ w_buf_2;
		delete_ w_buf_3; //if (w_buf_4 != NULL) delete_ w_buf_4;
		is_run_PIPE_SLAVE = false;
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		return;

	}*/

	x = write_pipe(pipe_AGENT, &screen_header.header, sizeof_ENCODED_SCREEN_8bit_header, &w, &write_SLAVE_pipe_TIMEOUT);
	if (x == false) {
		//send_udp("PIPE_SLAVE_EXECUTE_2() (2--) x == false  GLOBAL_STOP = true;");

		screen_header.clean();

		
		EXECUTE_read_write_is_run = false;
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		return;
	}

	//***********************************************************************************************
		// write 2000 (4)

	//w_buf_4 = neww char[2000000];

	if (screen_header.buf_one_byte_size > 0) {

		x = write_pipe(pipe_AGENT, screen_header.get_buf_one_byte(), screen_header.buf_one_byte_size, &w, &write_SLAVE_pipe_TIMEOUT);
		if (x == false) {
			//send_udp("PIPE_SLAVE_EXECUTE_2() (3) x == false  GLOBAL_STOP = true;");

			screen_header.clean();


			EXECUTE_read_write_is_run = false;

			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;

			return;
		}

	};

	screen_header.clean();

	// 2021 09    WTF?       bis_run_PIPE_SLAVE = false;

}

void AGENT::PIPE_SLAVE_EXECUTE_2_____FINAL_STOP_AGENT() {
	bool x;
	DWORD w;

	unsigned char *w_buf_2 = NULL;


	w_buf_2 = new unsigned char[sizeof_MASTER_AGENT_PACKET_HEADER];


	MASTER_AGENT_PACKET_HEADER *packet_send;



	packet_send = (MASTER_AGENT_PACKET_HEADER *)w_buf_2;


	//***********************************************************************************************
	// write 128 (2)
	zero_unsigned_char(w_buf_2, sizeof_MASTER_AGENT_PACKET_HEADER);
	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_RESPONCE_STOP_AGENT;

	x = write_pipe(pipe_AGENT, w_buf_2, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_SLAVE_pipe_TIMEOUT);
	if (x == false) {
		//send_udp("PIPE_SLAVE_EXECUTE_2() (2) x == false  GLOBAL_STOP = true;");
		delete[] w_buf_2;
		EXECUTE_read_write_is_run = false;
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		return;
	}

	delete[] w_buf_2;
	// 2021 09   WTF?  is_run_PIPE_SLAVE = false;

};

void AGENT::PIPE_SLAVE_EXECUTE_2_____FINAL_REQUEST_EVENT(MASTER_AGENT_PACKET_HEADER_event *event) {
	bool x;
	DWORD w;

	unsigned char *w_buf_2 = NULL;

	w_buf_2 = new unsigned char[sizeof_MASTER_AGENT_PACKET_HEADER * 2];

	MASTER_AGENT_PACKET_HEADER *packet_send;
	packet_send = (MASTER_AGENT_PACKET_HEADER *)w_buf_2;

	//***********************************************************************************************
	// write 128 (2)
	zero_unsigned_char(w_buf_2, sizeof_MASTER_AGENT_PACKET_HEADER);
	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_RESPONCE_EVENT;

	x = write_pipe(pipe_AGENT, w_buf_2, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_SLAVE_pipe_TIMEOUT);
	if (x == false) {
		//send_udp("PIPE_SLAVE_EXECUTE_2() (2) x == false  GLOBAL_STOP = true;");
		delete[] w_buf_2;
		EXECUTE_read_write_is_run = false;
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		return;
	}

	delete[] w_buf_2;
	// 2021 09   WTF?  is_run_PIPE_SLAVE = false;

	//send_udp("receive EVENT from service");
}

void AGENT::PIPE_SLAVE_EXECUTE_2_____FINAL_PING() {
	bool x;
	DWORD w;

	unsigned char *w_buf_2 = NULL;


	w_buf_2 = new unsigned char[sizeof_MASTER_AGENT_PACKET_HEADER];


	MASTER_AGENT_PACKET_HEADER *packet_send;



	packet_send = (MASTER_AGENT_PACKET_HEADER *)w_buf_2;


	//***********************************************************************************************
	// write 128 (2)
	zero_unsigned_char(w_buf_2, sizeof_MASTER_AGENT_PACKET_HEADER);
	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_PONG_MASTER_to_AGENT;

	x = write_pipe(pipe_AGENT, w_buf_2, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_SLAVE_pipe_TIMEOUT);
	if (x == false) {
		//send_udp("PIPE_SLAVE_EXECUTE_2() (2) x == false  GLOBAL_STOP = true;");
		delete[] w_buf_2;
		EXECUTE_read_write_is_run = false;
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		return;
	}

	delete[] w_buf_2;
}