#pragma once



#include "tools.h"

class TOTAL_CONTROL
{
public:
	


	SOCKET udp_socket__service = 0;
	struct sockaddr_in udp_socket_adr__service;

	void send_udp_SERVICE();
	void send_udp_DESKTOP();
	void send_udp_INDICATOR();

	boost::thread* thread_EXECUTE = nullptr;

	bool EXECUTE_is_run = false;
	void EXECUTE();

	void start_EXECUTE();

	TOTAL_CONTROL();
	~TOTAL_CONTROL();
};
