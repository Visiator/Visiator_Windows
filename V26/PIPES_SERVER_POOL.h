#pragma once

#include "tools.h"

#include <boost/thread.hpp>

#define packet_type_PING_SERVER_to_AGENT 100
#define packet_type_REQUEST_SCREEN 101


struct SERVER_AGENT_PACKET_HEADER
{
	unsigned int header_size;
	unsigned int packet_type;
	char reserv[128];

};


class PIPES_SERVER_POOL
{
public:
	// ����� ����� �������� � �������:
	// ������������ � ���� ����������. ������ �������� ������ � ���� ����� �� ������. ���� ����� ����� � �� ��������, ��� �������� 
	// ���������� ��������� �����.
	// � ��������� ������ �������������� ����������� � pipe

	// ��������� ����� ��� �������� �������� ����������
	
	HANDLE pipe_server_to_agent = 0;
	HANDLE pipe_server_to_indicator = 0;

	// ����� control
	boost::thread* thread_EXECUTE_control = nullptr;
	bool is_run_thread_control;
	bool EXECUTE_control_is_run = false;
	void EXECUTE_control();
	bool SERVER_AGENT_connected = false;

	// ���� SERVER<->AGENT
	boost::thread* thread_EXECUTE_SERVER_AGENT_reconnect_pipe = nullptr;
	void                  EXECUTE_SERVER_AGENT_reconnect_pipe();
	bool                  EXECUTE_SERVER_AGENT_reconnect_pipe_is_run = false;
	bool SERVER_is_use = false;

	bool send_ping_from_SERVER_to_AGENT();

	void RUN();

	void close_handle(HANDLE *h);

	PIPES_SERVER_POOL();
	~PIPES_SERVER_POOL();
};

