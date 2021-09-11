#include "NET_CLIENT_SESSION.h"

extern bool GLOBAL_STOP;

NET_CLIENT_SESSION::NET_CLIENT_SESSION() {
	
}

NET_CLIENT_SESSION::~NET_CLIENT_SESSION() {

}

void NET_CLIENT_SESSION::mouse_left_button_down(int mx, int my) {

}
void NET_CLIENT_SESSION::mouse_left_button_up(int mx, int my) {

}
void NET_CLIENT_SESSION::mouse_right_button_down(int mx, int my) {

}
void NET_CLIENT_SESSION::mouse_right_button_up(int mx, int my) {

}
void NET_CLIENT_SESSION::get_statistic(char *ss, int ss_max_size_) {

	sprintf__s_ull_ull(ss, ss_max_size_, "rcv:%lld snd:%lld ", recv__counter, send__countern);

}
void NET_CLIENT_SESSION::mouse_move(int mx, int my) {

}
void NET_CLIENT_SESSION::mouse_whell(int msg, int wp, int lp) {

}

void NET_CLIENT_SESSION::EXECUTE() {

	boost::posix_time::milliseconds SleepTime(100);

	EXECUTE_is_run = true;

	while (GLOBAL_STOP == false) {

		if (partner_pass_and_id_is_set == true) {

		} 


		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_is_run = false;

}


void NET_CLIENT_SESSION::set_status(wchar_t *v) {
	status = v;
	if (parent_low_level != nullptr) parent_low_level->invalidate();
}