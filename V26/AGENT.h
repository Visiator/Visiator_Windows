#pragma once

#include "tools.h"
#include "SERVICE.h"

#include <boost/thread.hpp>




class AGENT
{
public:
	HANDLE pipe_handle_read_info_ = 0; DWORD pipe_handle_read_info_TIMEOUT = 0;
	bool is_run_MAIN_THREAD = false;

	DWORD last_detect_MASTER = 0;

	HANDLE pipe_AGENT = 0;
	DWORD read_pipe_TIMEOUT = 0, write_pipe_TIMEOUT = 0, write_SLAVE_pipe_TIMEOUT = 0;
	

	boost::thread* thread_EXECUTE_control = nullptr;
	boost::thread* thread_EXECUTE_read_service_info = nullptr;
	boost::thread* thread_EXECUTE_read_write = nullptr;

	void EXECUTE_control();
	bool EXECUTE_control_is_run = false;

	void EXECUTE_read_service_info();
	bool EXECUTE_read_service_info_is_run = false;

	void EXECUTE_read_write();
	bool EXECUTE_read_write_is_run = false;

	void PIPE_SLAVE_EXECUTE_2_____FINAL_SCREEN_one_byte();
	void PIPE_SLAVE_EXECUTE_2_____FINAL_STOP_AGENT();
	void PIPE_SLAVE_EXECUTE_2_____FINAL_REQUEST_EVENT(MASTER_AGENT_PACKET_HEADER_event *event);
	void PIPE_SLAVE_EXECUTE_2_____FINAL_PING();

	void RUN();

	AGENT();
	~AGENT();
};

