#pragma once

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

//#include "PIPES_SERVER_POOL.h"

#define packet_type_PING_MASTER_to_AGENT 100
#define packet_type_REQUEST_SCREEN_one_byte 101
#define packet_type_RESPONCE_SCREEN_one_byte 102
#define packet_type_REQUEST_STOP_AGENT 103
#define packet_type_RESPONCE_STOP_AGENT 104

#define packet_type_REQUEST_EVENT 105
#define packet_type_RESPONCE_EVENT 106

#define packet_type_REQUEST_CLIPBOARD 107
#define packet_type_RESPONCE_CLIPBOARD 108
#define packet_type_SET_CLIPBOARD 109

#define packet_type_SERVICE_INFO 110

#define sizeof_MASTER_AGENT_PACKET_HEADER 128
#define sizeof_ENCODED_SCREEN_8bit_header 84

#define packet_type_REQUEST_SCREEN_12bit 111
#define packet_type_RESPONCE_SCREEN_12bit 112


struct MASTER_AGENT_PACKET_HEADER
{
	unsigned int packet_size;
	unsigned int packet_type;
	char reserv[128];

};

struct MASTER_AGENT_PACKET_HEADER_event
{
	int session_no;
	unsigned int event_type;
	int global_type;
	//+12
	unsigned long long msg, wparam, lparam;
	//+24

	//=36
};


void zero_128_s(MASTER_AGENT_PACKET_HEADER *v);

class GUI_element;
class NET_SERVER_SESSION;
class NET_SERVER_SESSION_POOL;
class SCREEN_LIGHT_one_byte;
struct ENCODED_SCREEN_8bit_header;


int SERVICE___CHECK_STATUS(wchar_t *service_name);

int  ServiceUnInstallLocal();
int  ServiceInstallLocal();



#define IT_IS_agent 1
#define IT_IS_indicator 2
#define IT_IS_desktop 3

struct SERVICE_INFO
{
	unsigned long long service_id;
	unsigned int is_client_connected;
	unsigned int agent_need_stop;
	unsigned int heart_beat;
	unsigned int is_ready_to_client_connect;
	unsigned int who_is_asked;
	//unsigned int INDICATOR_detect_clipboard_is_changed;
	unsigned int is_sync_clipboards;
	unsigned int type;
	unsigned char reserv[128];
};

void RUN_SERVICE(void);

class SERVICE
{
public:
	// for "total_control"
	int interaction_with_agent_GET_SCREEN_counter = 0;
	int interaction_with_agent_GET_SCREEN_status = 0;
	unsigned int SERVICE_PIPE_CONTROL_THREAD_EXECUTE = 0;

	NET_SERVER_SESSION_POOL *net_server_session_pool = nullptr;

	//PIPES_SERVER_POOL *pipes_server_pool = nullptr;

	bool is_sync_clipboards = false; // нужно автоматически снхронизировать clipboard между сервером и клиентом (или не нужно)

	unsigned long long PUBLIC_ID = 0, PRIVATE_ID = 0;
	unsigned char *PASS = NULL;

	unsigned int service_heart_beat = 0;
	void get_service_info(SERVICE_INFO *service_info);

	void LOAD_ID_or_REGISTER();
	void LOAD_PASS();
	void START_NET_POOL();


	DWORD last_agent_active = 0, last_active_INDICATOR = 0, last_active_DESKTOP = 0;
	void SET_last_agent_active(char *info);

	HANDLE pipe_write_service_info = 0;
	//HANDLE pipe_master = 0;
	HANDLE pipe_indicator = 0;

	bool is_run_MAIN_THREAD = false;
	bool is_run_PIPE_CONTROL_THREAD = false;
	bool is_run_PIPE_WRITE_INFO_THREAD = false;  DWORD write_info_connect_TIMEOUT = 0;
	bool is_run_PIPE_MASTER_THREAD = false;
	bool is_run_INDICATOR_THREAD = false;
	bool is_run_PIPE_INDICATOR_THREAD = false;
	bool is_run_SEND_INFO_to_INDOCATOR_THREAD = false;

	bool is_ALL_THREAD_IS_STOP();

	void INDICATOR_SAY_clipboard_is_changed();

	//-------------------------------------------------------
	// indicator thread
	bool check_run_indicator();
	void start_indicator();
	

	//-------------------------------------------------------
	// main thread
	DWORD read_MASTER_pipe_TIMEOUT = 0, write_MASTER_pipe_TIMEOUT = 0;
	DWORD MAIN_THREAD_id = 0;
	HANDLE MAIN_THREAD_h = 0;
	void MAIN_THREAD_EXECUTE();
	void start_MAIN_THREAD();
	//-------------------------------------------------------
	// master pipe
	bool MASTER_is_pipe_open = false;
	bool MASTER_is_agent_connected = false;
	bool MASTER_is_indicator_connected = false;
	DWORD interaction_with_agent_TIMEOUT = 0;
	bool  interaction_with_agent_IN_USE = false;
	
	void set_MASTER_is_indicator_connected(bool val);

	DWORD interaction_with_INDICATOR_TIMEOUT = 0;
	bool  interaction_with_INDICATOR_IN_USE = false;
	void  set_interaction_with_INDICATOR_TIMEOUT(int val);
	

	
	




	//-------------------------------------------------------
	// write info pipe
	
	HANDLE PIPE_INDICATOR_THREAD_handle = 0;
	DWORD  WRITE_INFO_THREAD_id = 0;
	HANDLE WRITE_INFO_THREAD_h = 0;
	
	//-------------------------------------------------------
	
	//*******************************************************
	//** interaction with INDICATOR

	void lock_interaction_with_INDICATOR();

	bool interaction_with_INDICATOR_SEND_INFO(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char *write_buf, unsigned int write_buf_size, unsigned char **read_buf, unsigned int *read_buf_size);

	bool interaction_with_indicator_GET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char **buf, unsigned int *buf_size);
	bool interaction_with_indicator_SET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char *buf, unsigned int buf_size);


	void SEND_INFO_to_INDICATOR(char *packet_send, char *packet_recv);

	//*******************************************************
	//** interaction with AGENT
	void lock_interaction_with_AGENT();

	

	void set_clipboard_in_to_session(int session_no, unsigned char *buf, unsigned int buf_size);
	// 2020 bool interaction_with_agent_SET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char *buf, unsigned int buf_size);
	
	void get_clipboard_from_session(int session_no_, unsigned char **buf, unsigned int *buf_size);
	// 2020 bool interaction_with_agent_GET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char **buf, unsigned int *buf_size);
	

	bool interaction_with_agent_GET_SCREEN_8bit(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, ENCODED_SCREEN_8bit_header *scr_head_buf, SCREEN_LIGHT_one_byte *screen_light);
	bool interaction_with_agent_GET_SCREEN_12bit(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, ENCODED_SCREEN_12bit_header *scr_head_buf, SCREEN_LIGHT_12bit *screen_light_12bit);

	//bool interaction_with_agent_GET_SCREEN(MASTER_AGENT_PACKET_HEADER *w_buf, MASTER_AGENT_PACKET_HEADER *r_buf, ENCODED_SCREEN_8bit_header *scr_head_buf, SCREEN_LIGHT_one_byte *screen_light);
	bool interaction_with_agent_SEND_EVENT(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv);

	void interaction_with_agent_STOP_AGENT();

	//** interaction with AGENT
	//**********************************************************

	void save_in_registry_last_run_ver_and_date();

	//*****************************************************************************
	

	//*****************************************************
	// Thread для основного потока (reconnect) MASTER<->AGENT
	boost::thread* thread_EXECUTE_main_MASTER_AGENT = nullptr;
	bool EXECUTE_main_MASTER_AGENT_reconnect_is_run = false;
	void EXECUTE_main_MASTER_AGENT_reconnect();

	void RUN();

	// MAIN Thread
	boost::thread* thread_EXECUTE = nullptr;
	bool EXECUTE_is_run = false;
	void EXECUTE(); // MAIN Thread	
	HANDLE pipe_MASTER = 0;
	bool pipe_MASTER_is_open = false;
	//
	//*****************************************************************************

	// CONTROL Thread
	boost::thread* thread_EXECUTE_CONTROL = nullptr;
	bool EXECUTE_CONTROL_is_run = false;
	void EXECUTE_CONTROL(); // CONTROL Thread	


	// TEST Thread
	boost::thread* thread_EXECUTE_test = nullptr;
	bool EXECUTE_test_is_run = false;
	void EXECUTE_test(); // CONTROL Thread	

	boost::thread* INDICATOR_EXECUTE_thread = nullptr;
	void INDICATOR_THREAD_EXECUTE();
	void start_INDICATOR_THREAD();

	//
	bool interaction_with_agent_PING();
	int SERVICE_interaction_with_agent_PING_try = 0;
	int SERVICE_interaction_with_agent_PING_ok  = 0;

	//-------------------------------------------------------
	// control
	boost::thread* CONTROL_thread = nullptr;
	void PIPE_CONTROL_THREAD_EXECUTE();
	void start_PIPE_CONTROL_THREAD();


	// indicator pipe
	boost::thread* PIPE_INDICATOR_EXECUTE_thread = nullptr;
	void PIPE_INDICATOR_EXECUTE();                                  // Создаем PIPE для связи с INDICATOR.Ждем подключения клиента.Когда клиент подключился, выставляем флаг - MASTER_is_indicator_connected
	void start_PIPE_INDICATOR_EXECUTE();


	boost::thread* SEND_INFO_to_INDICATOR_EXECUTE_thread = nullptr;
	void SEND_INFO_to_INDICATOR_EXECUTE();                           // 2 раза в секунду передаем в INDICATOR текущий статус SERVICE
	void start_SEND_INFO_to_INDICATOR_EXECUTE();


	boost::thread* PIPE_MASTER_EXECUTE_thread;
	void PIPE_MASTER_THREAD_EXECUTE();                               // Создаем PIPE для связи с AGENT.Ждем когда подключится AGENT.Выставляем флаг - MASTER_is_agent_connected
	DWORD MASTER_THREAD_id = 0;
	void start_PIPE_MASTER_THREAD();
	void reconnect_master_pipe();
	//-------------------------------------------------------

	boost::thread* PIPE_WRITE_INFO_THREAD_EXECUTE_thread;
	void PIPE_WRITE_INFO_THREAD_EXECUTE();
	void start_PIPE_WRITE_INFO_THREAD();
	//-------------------------------------------------------

	char *packet_send_event = NULL, *packet_recv_event = NULL;
	void send_event_in_to_session(int session_no, unsigned int event_type, int global_type, unsigned long long msg, unsigned long long wparam, unsigned long long lparam);

	void send_unpress_all_pressed_keys();

	void RUN_AGENT_AS_CONSOLE(wchar_t *user_for_attach_session);
	void CHECK_and_RUN_AGENT_AS_CONSOLE(wchar_t *user_for_attach_session);

	SERVICE();
};

void restart_service();