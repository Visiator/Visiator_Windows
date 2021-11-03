#pragma once

#include "tools.h"

#include <boost/thread.hpp>

#define packet_type_PING_MASTER_to_AGENT 100
#define packet_type_PONG_MASTER_to_AGENT 10000

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


//#define packet_type_PING_SERVER_to_AGENT 100
#define packet_type_REQUEST_SCREEN 101

#define sizeof_MASTER_AGENT_PACKET_HEADER 128
#define sizeof_ENCODED_SCREEN_8bit_header 84

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


class PIPES_SERVER_POOL
{
public:
	// Обмен между мастером и подчиненным (клиентом):
	
	// Первый поток создает канал (pipe) и вызывает функцию ConnectNamedPipe. Эта функция блокирует поток и ждет когда подключится клиент.
	// когда клиент подключился, поток выставляет переменную client_is_connected = true переходит в бесконечный цикл.
	
	// 

	// Производится в виде транзакций. Сервер посылает запрос и ждет ответ от агента. Если агент завис и не отвечает, эту ситуацию 
	// обработает отдельный поток и разорвет соединение.
	// В отдельном потоке контролируется подключение к pipe

	// отдельный поток для контроля зависших транзакций по таймауту
	

	//*****************************************************
	// отдельный канал для основной связи с агентом. Запрос от агента экрана и передача агенту нажатых клавишь и мыши
	// когда агент подключился к мастеру, канал все время остается открытым
	
	HANDLE pipe_master_to_agent = 0;
	
	boost::thread* thread_EXECUTE_MASTER_AGENT_reconnect_pipe = nullptr;
	void                  EXECUTE_MASTER_AGENT_reconnect_pipe();
	bool                  EXECUTE_MASTER_AGENT_reconnect_pipe_is_run = false;
	bool MASTER_is_use = false;


	//
	//*****************************************************



	
	HANDLE pipe_master_to_indicator = 0;

	// поток control
	boost::thread* thread_EXECUTE_control = nullptr;
	bool is_run_thread_control;
	bool EXECUTE_control_is_run = false;
	void EXECUTE_control();
	bool MASTER_AGENT_connected = false;

	
	bool send_ping_from_MASTER_to_AGENT();

	void RUN();

	void close_handle(HANDLE *h);

	PIPES_SERVER_POOL();
	~PIPES_SERVER_POOL();
};

