#include "NET_SERVER_SESSION.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "tools.h"


extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;

NET_SERVER_SESSION::NET_SERVER_SESSION() {
	for (int i = 0; i < 15; i++) dummy_packet[i] = (char)i;
	dummy_packet_size = 15;

}

NET_SERVER_SESSION::~NET_SERVER_SESSION() {

	if (llow_level_buffer_read != NULL) delete[] llow_level_buffer_read;
	llow_level_buffer_read_max_count = 0;
	low_level_buffer_read_idx = 0;


	if (llow_level_encoded_buffer_read != NULL) delete[] llow_level_encoded_buffer_read;
	llow_level_encoded_buffer_read_max_count = 0;
	low_level_encoded_buffer_read_idx = 0;


	delete[] screen_encoded;
	delete[] screen_one_byte_;

}

void NET_SERVER_SESSION::RUN(unsigned long long PUBLIC_ID_, unsigned long long PRIVATE_ID_) {

	PUBLIC_ID = PUBLIC_ID_;
	PRIVATE_ID = PRIVATE_ID_;

	thread_EXECUTE = app_attributes.tgroup.create_thread(boost::bind(&NET_SERVER_SESSION::EXECUTE, this));
}

void NET_SERVER_SESSION::EXECUTE() {
	
	EXECUTE_is_run = true;

	boost::posix_time::milliseconds SleepTime(1000);

	while (GLOBAL_STOP == false) {

		sudp("NET_SERVER_SESSION::EXECUTE()");

		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_is_run = false;
}

void NET_SERVER_SESSION::increase_llow_level_buffer_read(int add_size) {
	unsigned char *q;
	unsigned int ss;
	unsigned int i;
	ss = llow_level_buffer_read_max_count + add_size;

	if (ss > 5000000) {
		fatal_error("NET_CLIENT_SESSION::increase_llow_level_buffer_read() ss > 5000000");
		return;
	}

	q = new unsigned char[ss];

	for (i = 0; i < llow_level_buffer_read_max_count; i++) {
		q[i] = llow_level_buffer_read[i];
	}
	while (i < ss) {
		q[i] = 0;
		i++;
	}
	if (llow_level_buffer_read != NULL) delete[] llow_level_buffer_read;
	llow_level_buffer_read = q;
	llow_level_buffer_read_max_count = ss;
}

void NET_SERVER_SESSION::increase_llow_level_encoded_buffer_read(int add_size) {
	unsigned char *q;
	unsigned int ss;
	unsigned int i;
	ss = llow_level_encoded_buffer_read_max_count + add_size;

	if (ss > 5000000) {
		fatal_error("NET_CLIENT_SESSION::increase_llow_level_encoded_buffer_read() ss > 5000000");
		return;
	}

	q = new unsigned char[ss];
	for (i = 0; i < llow_level_encoded_buffer_read_max_count; i++) {
		q[i] = llow_level_encoded_buffer_read[i];
	}
	while (i < ss) {
		q[i] = 0;
		i++;
	}
	if (llow_level_encoded_buffer_read != NULL) delete[] llow_level_encoded_buffer_read;
	llow_level_encoded_buffer_read = q;
	llow_level_encoded_buffer_read_max_count = ss;
}

