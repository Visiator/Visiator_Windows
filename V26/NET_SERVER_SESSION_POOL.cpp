#include "NET_SERVER_SESSION_POOL.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "tools.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;

NET_SERVER_SESSION_POOL::NET_SERVER_SESSION_POOL() {

}

NET_SERVER_SESSION_POOL::~NET_SERVER_SESSION_POOL() {

}

void NET_SERVER_SESSION_POOL::RUN(unsigned long long PUBLIC_ID_, unsigned long long PRIVATE_ID_, unsigned char *PASS_ENCODED_) {

	PUBLIC_ID = PUBLIC_ID_;
	PRIVATE_ID = PRIVATE_ID_;
	for (int i = 0; i < 32; i++) PASS_ENCODED[i] = PASS_ENCODED_[i];

	thread_EXECUTE = app_attributes.tgroup.create_thread(boost::bind(&NET_SERVER_SESSION_POOL::EXECUTE, this));
}

void NET_SERVER_SESSION_POOL::EXECUTE() {
	
	NET_SERVER_SESSION *n;

	EXECUTE_is_run = true;

	boost::posix_time::milliseconds SleepTime(10);

	n = add_element();
	n->RUN(PUBLIC_ID, PRIVATE_ID, PASS_ENCODED);

	while (GLOBAL_STOP == false) {

		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_is_run = false;
}

NET_SERVER_SESSION *NET_SERVER_SESSION_POOL::add_element() {

	elements.emplace_back();
	return &(elements.back());

}

