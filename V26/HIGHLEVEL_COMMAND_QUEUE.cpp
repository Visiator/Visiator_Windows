
#include "HIGHLEVEL_COMMAND_QUEUE.h"
#include "PIPES_SERVER_POOL.h"
//#include "mem.h"

extern void fatal_error(char *p);
extern void enter_crit(int id);
extern void leave_crit(int id);

HIGHLEVEL_COMMAND_QUEUE_element *new_HIGHLEVEL_COMMAND_QUEUE_element(int count, int id) {
	HIGHLEVEL_COMMAND_QUEUE_element *q;

	int sz;
	sz = sizeof(HIGHLEVEL_COMMAND_QUEUE_element);

	q = new HIGHLEVEL_COMMAND_QUEUE_element[count];

	//add_all_alloc((unsigned int)q, sz, id);

	return q;
}

void delete_(HIGHLEVEL_COMMAND_QUEUE_element **q) {
	HIGHLEVEL_COMMAND_QUEUE_element *a;
	a = *q;
	//_delete_((unsigned char *)a);
	delete a;
	*q = NULL;
}


HIGHLEVEL_COMMAND_QUEUE_element::HIGHLEVEL_COMMAND_QUEUE_element(void) {
	//command_type = COMMAND_nop;
	command_data = NULL;
	command_data_size = 0;

	command_info = 0;

	//status[0] = 0;
};


void HIGHLEVEL_COMMAND_QUEUE::lock(void) {
	
	while (true) {
		if (is_locked_ == false) {
			enter_crit(112983);
			if (is_locked_ == false) {
				is_locked_ = true;
				leave_crit(112983);
				return;
			}
			leave_crit(112983);
		}
		else {
			::Sleep(0);
		}
	}

	is_locked_ = true;
};

void HIGHLEVEL_COMMAND_QUEUE::unlock(void) {
	is_locked_ = false;
};


void HIGHLEVEL_COMMAND_QUEUE::free(void) {
	lock();
	int i;
	i = 0;
	while (i < queue_max_size) {
		if (elements[i].command_data != NULL) delete elements[i].command_data;
		elements[i].command_data = NULL;
		elements[i].command_data_size = 0;
		i++;
	};
	len = 0;
	unlock();
};


bool HIGHLEVEL_COMMAND_QUEUE::add_element_(unsigned int type, int info, unsigned char *command_data, unsigned int command_data_size) {//, int tmp_) {

	

	//ALOG("HIGHLEVEL_COMMAND_QUEUE::add_element() size = %d ", command_data_size );

	if (command_data_size < 0) {
		fatal_error((char *)"HIGHLEVEL_COMMAND_QUEUE::add_element  command_data_size < 0");
		return false;
	};
	if (command_data == NULL && command_data_size > 0) {
		fatal_error((char *)"HIGHLEVEL_COMMAND_QUEUE::add_element  command_data == NULL && command_data_size > 0");
		return false;
	};
	if (command_data == NULL && command_data_size == 0) {
		return add_element_low_level( type, 0, NULL, 0);
	};



	if (command_data != NULL && command_data_size > 0) {
		unsigned char *q;
		unsigned int sz;
		sz = command_data_size / 16;
		sz *= 16;

		if (sz != command_data_size) {
			//send_udp( "sz != command_data_size");
		}


		

		q = new unsigned char[ command_data_size ]; //neew( command_data_size, q);
		//q = new_u_char(command_data_size);
		if (q == NULL) {

			

			//AnsiString s;
			//  s = "123";
			//s = "HIGHLEVEL_COMMAND_QUEUE::add_element  neww char["+String(command_data_size)+"] == NULL";
			fatal_error((char *)"HIGHLEVEL_COMMAND_QUEUE::add_element  neww char[xxx] == NULL");
			return false;
		};

		

		try
		{

			for (unsigned int i = 0; i < command_data_size; i++) q[i] = command_data[i];
		}
		catch (...) {
			//crash_log("C2 sgkdjhsgksgkj");
		}
		

		return add_element_low_level(type, info, q, command_data_size); // encr +
	};

	fatal_error((char *)"HIGHLEVEL_COMMAND_QUEUE::add_element  WTF??");
	return false;

};

bool HIGHLEVEL_COMMAND_QUEUE::add_element_low_level(unsigned int type, int info, unsigned char *command_data_, unsigned int command_data_size_ ) {
	
	/*if (type == PACKET_TYPE_responce_userCLIPBOARD_list) {
		sudp("add_element_low_level() PACKET_TYPE_responce_userCLIPBOARD_list START");
	}*/

	int jj;
	jj = command_data_size_ / 16;
	jj *= 16;
	if (jj != command_data_size_) {
		fatal_error("add_element_low_level <> 16");
		return false;
	}

	
	lock();
	int i;
	i = 0;

	while (i < queue_max_size && elements[i].command_data != NULL) {
		i++;
	};

	if (i >= queue_max_size) {
		unlock();
		fatal_error((char *)"queue owerfull !");
		return false;
	};


	elements[i].command_data = command_data_;
	elements[i].command_data_size = command_data_size_;
	elements[i].command_type = type;
	elements[i].command_info = info;



	counter++;
	len++;
	unlock();
	//set_status(L"ADD type = ", type);

	/*if (type == PACKET_TYPE_responce_userCLIPBOARD_list) {
		send_udp2("add_element_low_level() PACKET_TYPE_responce_userCLIPBOARD_list END");
	}*/

	return true;
};

bool HIGHLEVEL_COMMAND_QUEUE::get_element_low_level(int &type_, int &info_, unsigned char * &command_data, unsigned int &command_data_size) {
	//ALOG("HIGHLEVEL_COMMAND_QUEUE::get_element_low_level() start");
	lock();
	
	if (elements[0].command_data == NULL) {
		//ALOG("HIGHLEVEL_COMMAND_QUEUE::get_element_low_level() end 2");
		command_data = NULL;
		command_data_size = 0;
		unlock();
		return false;
	};

	//command_type_ = elements[0].command_type;
	command_data = elements[0].command_data;
	command_data_size = elements[0].command_data_size;
	type_ = elements[0].command_type;
	info_ = elements[0].command_info;
	int i;
	for (i = 0; i < queue_max_size - 1 && elements[i + 1].command_data != NULL; i++) {
		elements[i].command_type = elements[i + 1].command_type;
		elements[i].command_data = elements[i + 1].command_data;
		elements[i].command_data_size = elements[i + 1].command_data_size;
	};
	//elements[i].command_type = COMMAND_nop;
	elements[i].command_data = NULL;
	elements[i].command_data_size = 0;
	elements[i].command_type = 0;
	elements[i].command_info = 0;
	len--;
	unlock();
	//set_status(L"GET type_ = ", type_);
	//ALOG("HIGHLEVEL_COMMAND_QUEUE::get_element_low_level() end - found!");
	return true;
};

HIGHLEVEL_COMMAND_QUEUE::~HIGHLEVEL_COMMAND_QUEUE() {
	delete_(&elements);
}

HIGHLEVEL_COMMAND_QUEUE::HIGHLEVEL_COMMAND_QUEUE(void) {

	name = NULL;
	queue_max_size = 120;
	elements = new_HIGHLEVEL_COMMAND_QUEUE_element(queue_max_size, 780);// neew(sizeof(HIGHLEVEL_COMMAND_QUEUE_element), elements, "HIGHLEVEL_COMMAND_QUEUE::HIGHLEVEL_COMMAND_QUEUE()");
	

	//memo = 0;
	counter = 0;
	len = 0;
};


void HIGHLEVEL_COMMAND_QUEUE::clean(void) {
	free();
}