#include "tools.h"
#include "TOTAL_CONTROL.h"
#include "APPLICATION_ATTRIBUTES.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;


TOTAL_CONTROL::TOTAL_CONTROL() {

};

TOTAL_CONTROL::~TOTAL_CONTROL() {

};

char d__[300];

char *dddd(char *p) {
	int i, j;
	i = 55 - my_strlen((unsigned char *)p);

	j = 0;
	while (j < i) {
		d__[j] = '.';
		j++;
	}
	d__[j] = 0;
	return d__;
}

void add(char *ss, char *p, unsigned int v) {

	char s[300];

	sprintf__s_c_c_ui(s, 290, "%s %s %d\r\n", p, dddd(p), v);

	int i, j;
	i = 0;
	while (i < 3000 - 10 && ss[i] != 0) i++;
	j = 0;
	while (s[j] != 0 && i < 3000 - 10) {

		ss[i++] = s[j++];


	}
	ss[i] = 0;
}


void TOTAL_CONTROL::send_udp_SERVICE() {




		char ss[13000];
		char s[300];

		zero_unsigned_char((unsigned char *)ss, 13000);

		add(ss, "global_my_proc_id", app_attributes.global_my_proc_id);
		add(ss, "agent_process_id", app_attributes.agent_process_id);
		add(ss, "indicator_process_id", app_attributes.indicator_process_id);

		/*
		add(ss, "SERVICE_ServiceExecutionThread", SERVICE_ServiceExecutionThread);

		add(ss, "SERVICE_PIPE_CONTROL_THREAD_EXECUTE", SERVICE_PIPE_CONTROL_THREAD_EXECUTE);
		add(ss, "SERVICE_PIPE_MASTER_THREAD_EXECUTE", SERVICE_PIPE_MASTER_THREAD_EXECUTE);
		add(ss, "SERVICE_PIPE_MASTER_THREAD_EXECUTE_2", SERVICE_PIPE_MASTER_THREAD_EXECUTE_2);
		add(ss, "SERVICE_PIPE_MASTER_THREAD_EXECUTE_status", SERVICE_PIPE_MASTER_THREAD_EXECUTE_status);
		add(ss, "SERVICE_PIPE_WRITE_INFO_THREAD_EXECUTE", SERVICE_PIPE_WRITE_INFO_THREAD_EXECUTE);
		add(ss, "SERVICE_INDICATOR_THREAD_EXECUTE", SERVICE_INDICATOR_THREAD_EXECUTE);
		add(ss, "SERVICE_interaction_with_agent_status", SERVICE_interaction_with_agent_status);
		add(ss, "SERVICE_interaction_with_agent_STOP_AGENT_status", SERVICE_interaction_with_agent_STOP_AGENT_status);
		add(ss, "SERVICE_interaction_with_agent_SEND_EVENT_status", SERVICE_interaction_with_agent_SEND_EVENT_status);
		add(ss, "SERVICE_interaction_with_agent_GET_SCREEN_status", SERVICE_interaction_with_agent_GET_SCREEN_status);
		add(ss, "SERVICE_interaction_with_agent_GET_SCREEN", SERVICE_interaction_with_agent_GET_SCREEN);
		add(ss, "SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status", SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status);
		add(ss, "SERVICE_last_agent_active", SERVICE_last_agent_active);
		add(ss, "SERVICE_interaction_with_INDICATOR_TIMEOUT", SERVICE_interaction_with_INDICATOR_TIMEOUT);

		add(ss, "lock_interaction_with_INDICATOR_status", lock_interaction_with_INDICATOR_status);
		add(ss, "enter_crit_id", enter_crit_id);
		*/

		/*
		sprintf__s_ui_ui(s, 290, "w=%d h=%d cursor", SERVICE_interaction_with_agent_GET_SCREEN_w, SERVICE_interaction_with_agent_GET_SCREEN_h);
		add(ss, s, SERVICE_interaction_with_agent_GET_SCREEN_cursor);

		NET_SERVER_SESSION *q;

		int j;
		if (session_pool != NULL) {
			for (j = 0; j < 5; j++) {
				q = session_pool->session[j];
				if (q != NULL) {
					sprintf__s_i_c_c_ull_ull(s, 290, "session[%d] is_run = %s , main_loop_is_strated = %s , recv = %d , send_ = %d\r\n", j, decode_bool(q->is_run_), decode_bool(q->main_loop_is_strated)
						, q->recv_counter, q->send_counter);
					add(ss, s);
				}
				else {
					sprintf__s_i(s, 290, "session[%d] NULL\r\n", j);
					add(ss, s);
				}
			}
		};
		*/

		if (udp_socket__service == 0) {
			udp_socket__service = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			udp_socket_adr__service.sin_family = AF_INET;
			udp_socket_adr__service.sin_port = htons(7780);
			udp_socket_adr__service.sin_addr.S_un.S_addr = inet_addr("192.168.7.7");

		};

		int i;
		i = 0;

		while (ss[i] != 0 && i < 13000 - 10) i++;

		sendto(udp_socket__service, ss, i, 0, (struct sockaddr *)&udp_socket_adr__service, sizeof(udp_socket_adr__service));




}


void TOTAL_CONTROL::EXECUTE() {

	EXECUTE_is_run = true;

	boost::posix_time::milliseconds SleepTime(100);

	while (GLOBAL_STOP == false) {

		send_udp_SERVICE();

		for (int i = 0; i < 10; i++) {

			if (GLOBAL_STOP == true) {
				EXECUTE_is_run = false;
				return;
			}

			boost::this_thread::sleep(SleepTime);
		};
	}

	EXECUTE_is_run = false;

}

void TOTAL_CONTROL::start_EXECUTE() {

	thread_EXECUTE = app_attributes.tgroup.create_thread(boost::bind(&TOTAL_CONTROL::EXECUTE, this));

}