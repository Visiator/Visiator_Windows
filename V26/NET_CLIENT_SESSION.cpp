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

	bool local_stop = false;
	int connect_try_count = 0;

	boost::posix_time::milliseconds SleepTime(100);

	EXECUTE_is_run = true;

	set_status(L"waiting for the partner_id");

	while (GLOBAL_STOP == false && local_stop == false) {

		if (partner_pass_and_id_is_set == true) {

			connect_try_count++;

			if (my_priv_id == 0) {

				set_status(L"Get local id... ");

				Load_private_id_and_public_id_from_USER_registry(&my_pub_id, &my_priv_id);

				if (my_pub_id == 0) {

					set_status(L"Register... ");

					Register_new_partner_on_proxy();
				}
			}
		} 


		boost::this_thread::sleep(SleepTime);
	}
	set_status(L"stoped");
	EXECUTE_is_run = false;

}


void NET_CLIENT_SESSION::set_status(wchar_t *v) {
	status = v;
	if (parent_low_level != nullptr) parent_low_level->invalidate();
}