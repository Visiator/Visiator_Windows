#include "PIPES_SERVER_POOL.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "tools.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;

void zero_128_s(MASTER_AGENT_PACKET_HEADER *v) {
	v->packet_size = 0;
	v->packet_type = 0;
	for (int i = 0; i < 128; i++) {
		v->reserv[i] = 0;
	}
}

PIPES_SERVER_POOL::PIPES_SERVER_POOL() {

}

PIPES_SERVER_POOL::~PIPES_SERVER_POOL() {

}

void PIPES_SERVER_POOL::close_handle(HANDLE *h) {
	if (*h == 0) return;
	if (*h == INVALID_HANDLE_VALUE) return;
	enter_crit(122);
	if (*h != 0) {

		CloseHandle(*h);
		*h = 0;
	}
	leave_crit(122);
}


void PIPES_SERVER_POOL::RUN() {

	thread_EXECUTE_control = app_attributes.tgroup.create_thread( boost::bind(&PIPES_SERVER_POOL::EXECUTE_control, this) );
	
	thread_EXECUTE_MASTER_AGENT_reconnect_pipe = app_attributes.tgroup.create_thread( boost::bind(&PIPES_SERVER_POOL::EXECUTE_MASTER_AGENT_reconnect_pipe, this) );

}


void PIPES_SERVER_POOL::EXECUTE_control() {

	EXECUTE_control_is_run = true;

	boost::posix_time::milliseconds SleepTime(100);

	while (GLOBAL_STOP == false) {



		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_control_is_run = false;
}


void PIPES_SERVER_POOL::EXECUTE_MASTER_AGENT_reconnect_pipe() {
	EXECUTE_MASTER_AGENT_reconnect_pipe_is_run = true;

	///////////////////////////////////////////////////////////////

	BOOL x;

	SECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);


	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = TRUE;

	pipe_master_to_agent = CreateNamedPipe(L"\\\\.\\pipe\\$visiator_master$", PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 128, 128, 500, &sa);

	if (pipe_master_to_agent == INVALID_HANDLE_VALUE) {
		pipe_master_to_agent = 0;
		//fatal_error("pipe_master_to_agent == INVALID_HANDLE_VALUE");
		GLOBAL_STOP = true;
	};


	///////////////////////////////////////////////////////////////

	boost::posix_time::milliseconds SleepTime(100);

	while (GLOBAL_STOP == false) {

		MASTER_AGENT_connected = false;

		x = ConnectNamedPipe(pipe_master_to_agent, nullptr);

		if (x == TRUE) {
			//send_udp("MASTER connect ok ! MASTER_is_agent_connected = true");

			MASTER_AGENT_connected = true;

			while ( GLOBAL_STOP == false && MASTER_AGENT_connected == true ) {
				Sleep(1);
			}

			try {

				DisconnectNamedPipe( pipe_master_to_agent );
				//send_udp("MASTER disconnect");
			}
			catch (...) {};

		}
		else {
			if (GLOBAL_STOP == true) { // send_udp("MASTER connect fail. GLOBAL_STOP == true"); 
			}
			else { //send_udp("MASTER connect fail. GLOBAL_STOP != true  ERROR?"); 
			};

		}


		boost::this_thread::sleep( SleepTime );
	}

	close_handle( &pipe_master_to_agent );

	EXECUTE_MASTER_AGENT_reconnect_pipe_is_run = false;
}


bool PIPES_SERVER_POOL::send_ping_from_MASTER_to_AGENT() {
	BOOL x;
	DWORD w, r;
	MASTER_AGENT_PACKET_HEADER server_agent_packet_header, server_agent_packet_header_responce;

	//send_udp("ping");

	if (MASTER_AGENT_connected == false) return false;

	while (true) {
		//send_udp("ping while");
		if (MASTER_is_use == true) {
			Sleep(0);
		}
		else {
			enter_crit(331);
			if (MASTER_is_use == true) {
				leave_crit(331);
				Sleep(0);
			}
			else {
				MASTER_is_use = true;
				leave_crit(0);

				zero_128_s(&server_agent_packet_header);

				server_agent_packet_header.packet_size = 128;
				server_agent_packet_header.packet_type = packet_type_PING_SERVER_to_AGENT;

				w = 0;
				x = WriteFile(pipe_master_to_agent, &server_agent_packet_header, 128, &w, NULL);
				if (x != TRUE || w != 128) {

					DisconnectNamedPipe(pipe_master_to_agent);
					//send_udp("11211 MASTER_is_agent_connected = false");
					MASTER_AGENT_connected = false;
					MASTER_is_use = false;
					return false;
				};

				zero_128_s(&server_agent_packet_header_responce);

				r = 0;
				x = ReadFile(pipe_master_to_agent, &server_agent_packet_header_responce, 128, &r, NULL);
				if (x != TRUE || r != 128) {
					DisconnectNamedPipe(pipe_master_to_agent);
					//send_udp("11212 MASTER_is_agent_connected = false");
					MASTER_AGENT_connected = false;
					MASTER_is_use = false;
					return false;
				};

				MASTER_is_use = false;
				return true;
			}
		}
	}

	return false;
}

