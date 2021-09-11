#pragma once

#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "GUI_low_level.h"
#include "tools.h"

class NET_CLIENT_SESSION
{
public:
	uint64_t my_pub_id = 0, my_priv_id = 0;
	
	std::wstring status;
	void set_status(wchar_t *v);
	GUI_low_level *parent_low_level = nullptr;

	uint64_t recv__counter = 0, send__countern = 0;

	DWORD recv_responce_screen = 0;
	bool need_send_ALL_KEY_UP_event = false;

	bool partner_pass_and_id_is_set = false;
	uint64_t partner_id = 0;
	unsigned char  partner_pass_encripted[32];

	void set_partner_pass_and_id(uint64_t partner_id_, uint8_t *partner_pass_encripted_len32) {
		for (int i = 0; i < 32; i++) partner_pass_encripted_len32[i] = 0;
		for (int i = 0; i < 16; i++) {
			partner_pass_encripted[i] = partner_pass_encripted_len32[i];
		}
		partner_id = partner_id_;
		set_status(L"partner_id received");
		partner_pass_and_id_is_set = true;
	}

	boost::thread* thread_EXECUTE = nullptr;
	bool EXECUTE_is_run = false;
	void EXECUTE();

	void(*parent_func__arrived_screen)(unsigned char *buf, int buf_size) = nullptr;
	void(*parent_func__connect)(void) = nullptr;
	void(*parent_func__disconnect)(void) = nullptr;

	void get_statistic(char *ss, int ss_max_size_);

	void mouse_move(int mx, int my);
	void mouse_whell(int msg, int wp, int lp);

	void mouse_left_button_down(int mx, int my);
	void mouse_left_button_up(int mx, int my);
	void mouse_right_button_down(int mx, int my);
	void mouse_right_button_up(int mx, int my);

	NET_CLIENT_SESSION();
	~NET_CLIENT_SESSION();
};

