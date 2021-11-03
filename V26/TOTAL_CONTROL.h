#pragma once



#include "tools.h"

class TOTAL_CONTROL
{
public:
	unsigned int SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 0;
	unsigned int SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 0;
	unsigned int SERVICE_PIPE_MASTER_THREAD_EXECUTE_2 = 0;
	unsigned int SERVICE_last_agent_active = 0;
	unsigned int SERVICE_PIPE_WRITE_INFO_THREAD_EXECUTE = 0;

	unsigned int AGENT_PIPE_READ_SERVICE_INFO_EXECUTE_master_detect_ok = 0;
	unsigned int AGENT_PIPE_READ_SERVICE_INFO_EXECUTE_master_detect_fail = 0;

	SOCKET udp_socket__service = 0;
	struct sockaddr_in udp_socket_adr__service;

	SOCKET udp_socket__indicator = 0;
	struct sockaddr_in udp_socket_adr__indicator;

	void send_udp_SERVICE();
	void send_udp_AGENT();
	void send_udp_DESKTOP();
	void send_udp_INDICATOR();

	boost::thread* thread_EXECUTE = nullptr;

	bool EXECUTE_is_run = false;
	void EXECUTE();

	void start_EXECUTE();

	TOTAL_CONTROL();
	~TOTAL_CONTROL();
};
