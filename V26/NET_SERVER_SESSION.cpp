#include "NET_SERVER_SESSION.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "tools.h"


extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;

NET_SERVER_SESSION::NET_SERVER_SESSION() {

}

NET_SERVER_SESSION::~NET_SERVER_SESSION() {

}

void NET_SERVER_SESSION::RUN() {

	EXECUTE_is_run = true;

	boost::posix_time::milliseconds SleepTime(10);

	while (GLOBAL_STOP == false) {

		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_is_run = false;
}