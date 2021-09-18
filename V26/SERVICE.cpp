#include "APPLICATION_ATTRIBUTES.h"
#include "SERVICE.h"
#include "tools.h"
#include "NET_SERVER_SESSION.h"
//#include "NET_SERVER_SESSION_POOL.h"
//#include "mem.h"

extern bool GLOBAL_STOP;
//extern ALL_THREAD_LIST thread_list;
extern SERVICE *service;
extern APPLICATION_ATTRIBUTES app_attributes;
//extern NET_SERVER_SESSION_POOL *session_pool;
//extern TOTAL_CONTROL total_control;



wchar_t *strServiceName = L"VISIATOR";

SERVICE_STATUS_HANDLE nServiceStatusHandle;
DWORD nServiceCurrentStatus;
BOOL nServiceRunning;
HANDLE killServiceEvent;
HANDLE hServiceThread;

DWORD ServiceExecutionThread(LPDWORD pp);
bool UpdateServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwServiceSpecificExitCode, DWORD dwCheckPoint, DWORD dwWaitHint);

void KillService(void)
{
	sudp("KillService");
	nServiceRunning = false;
	SetEvent(killServiceEvent);
	UpdateServiceStatus(SERVICE_STOPPED, NO_ERROR, 0, 0, 0);
}


bool UpdateServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwServiceSpecificExitCode, DWORD dwCheckPoint, DWORD dwWaitHint) {
	sudp("UpdateServiceStatus()");
	SERVICE_STATUS nServiceStatus;
	nServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	nServiceStatus.dwCurrentState = dwCurrentState;
	
	if (dwCurrentState == SERVICE_START_PENDING) { 
		nServiceStatus.dwControlsAccepted = 0; 
	}
	else { 
		nServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN; 
	}

	if (dwServiceSpecificExitCode == 0) { 
		nServiceStatus.dwWin32ExitCode = dwWin32ExitCode; 
	}
	else { 
		nServiceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR; 
	}
	
	nServiceStatus.dwServiceSpecificExitCode = dwServiceSpecificExitCode;
	nServiceStatus.dwCheckPoint = dwCheckPoint;
	nServiceStatus.dwWaitHint = dwWaitHint;

	sudp("u1");
	if (!SetServiceStatus(nServiceStatusHandle, &nServiceStatus)) 
	{ 
		sudp("u2");
		KillService();	
		sudp("u3");
		return false; 
	}
	else {
		sudp("u4");
		return true;
	};
}


void ServiceCtrlHandler(DWORD nControlCode)
{
	sudp("ServiceCtrlHandler()");
	BOOL success;
	switch (nControlCode)
	{
	case SERVICE_CONTROL_SHUTDOWN:
		sudp("ServiceCtrlHandler( SERVICE_CONTROL_SHUTDOWN )");
		break;
	case SERVICE_CONTROL_STOP:
		sudp("ServiceCtrlHandler( SERVICE_CONTROL_STOP ) GLOBAL_STOP = true");

		service->interaction_with_agent_STOP_AGENT();

		sudp("***********************************");

		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		for (int i = 0; i < 100; i++) {
			//send_udp("stoping...");
			if (service->is_ALL_THREAD_IS_STOP() == true) {
				//send_udp("all treads is stop");
				i = 100;
			}
			/*
			if (all_threads_is_stoping()) {
				send_udp("all_threads_is_stoping !");
				break;
			}

			Sleep(500);*/
		}
		sudp("1");
		nServiceCurrentStatus = SERVICE_STOP_PENDING;
		sudp("2");
		success = UpdateServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 1, 3000);
		sudp("3");

		return;
	default:
		//send_udp("ServiceCtrlHandler( default )");
		break;
	}
	UpdateServiceStatus(nServiceCurrentStatus, NO_ERROR, 0, 0, 0);
}

/*2019 10 bool StartServiceThread(void)
{
	send_udp("StartServiceThread() 12");
	DWORD id;
	hServiceThread = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)ServiceExecutionThread, 0, 0, &id, "StartServiceThread");
	send_udp("StartServiceThread() 12 ee");
	if (hServiceThread == 0) { send_udp("ServiceExecutionThread FAIL"); return false; }
	else { nServiceRunning = true;	send_udp("nServiceRunning = true;");	return true; }
}*/


void ServiceMain(DWORD argc, LPTSTR *argv)
{
	sudp("ServiceMain()");

	nServiceStatusHandle = RegisterServiceCtrlHandler(strServiceName, (LPHANDLER_FUNCTION)ServiceCtrlHandler);
	
	if (!nServiceStatusHandle) { return; }
	
	if (!UpdateServiceStatus(SERVICE_START_PENDING, NO_ERROR, 0, 1, 3000)) { return; }
	
	killServiceEvent = CreateEvent(0, TRUE, FALSE, 0);
	
	if (killServiceEvent == NULL) { return; }
	
	if (!UpdateServiceStatus(SERVICE_START_PENDING, NO_ERROR, 0, 2, 1000)) { return; }
	
	nServiceCurrentStatus = SERVICE_RUNNING;
	
	if (!UpdateServiceStatus(SERVICE_RUNNING, NO_ERROR, 0, 0, 0)) { return; }

	if (service == NULL) { service = new SERVICE(); }

	service->RUN();

	/*if (!StartServiceThread()) {
		send_udp("r1");
		UpdateServiceStatus(SERVICE_STOPPED, NO_ERROR, 0, 0, 0);
		return;
	}
	else {
		send_udp("r2");
	}*/
	sudp("ServiceMain...");
	WaitForSingleObject(killServiceEvent, INFINITE);
	sudp("ServiceMain end");
	CloseHandle(killServiceEvent);
}


void RUN_SERVICE(void) {
	sudp("RUN_SERVICE begin");

	//my_pipe.start_service_write_info_pipe();
	//my_pipe.start_MASTER_pipe();

	//my_pipe.start_test_MASTER();

	SERVICE_TABLE_ENTRY servicetable[] =
	{
		{ strServiceName,(LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL,NULL }
	};
	if (!StartServiceCtrlDispatcher(servicetable)) {
		//send_udp("RUN_SERVICE end 2");
		return;
	}
	sudp("RUN_SERVICE end");
}


DWORD ServiceExecutionThread(LPDWORD pp)
{

	//////////////////////////////////////////////////////////////////////////////////////////////


	while (GLOBAL_STOP == false) {
		
		//total_control.SERVICE_ServiceExecutionThread++;

		Sleep(100);
	};

	KillService();

	//thread_list.th_finish();
	//
	//////////////////////////////////////////////////////////////////////////////////////////////

	return 0;

}

//-----------------------------------------------------------------------------------------------
// SERVICE
SERVICE::SERVICE()
{

}


//------------------------------------------------------------------------------------------------------
// MAIN_THREAD
void SERVICE::MAIN_THREAD_EXECUTE() {

	//**********************************************************************
	// 1. загрузить ID и PASS
	// 1.1. если нет ID запросить новый у прокси

	//send_udp("SERVICE::MAIN_THREAD_EXECUTE() begin...");

	is_run_MAIN_THREAD = true;

	DWORD last_interaction_with_agent = GetTickCount();

	while (GLOBAL_STOP == false) {

		if (PUBLIC_ID == 0 || PRIVATE_ID == 0) {
			LOAD_ID_or_REGISTER();
		}
		else {
			if (PASS_IS_EMPTY(PASS)) {
				LOAD_PASS();
			}
			else {
				START_NET_POOL();
			}
		}

		/*if (MASTER_is_agent_connected == true) {
			if (last_interaction_with_agent + 100 < GetTickCount()) {
				last_interaction_with_agent = GetTickCount();

				send_udp("SERVICE main thread, interaction with agent...");



				interaction_with_agent_2();



				send_udp("SERVICE main thread, interaction with agent - finish");
			}
		}*/
		//send_udp("44");
		Sleep(1);
		//send_udp("400");
	}
	is_run_MAIN_THREAD = false;

	
	
	while (is_ALL_THREAD_IS_STOP() == false) {
		Sleep(1);
	}
	
	KillService();
	
	//send_udp("SERVICE::MAIN_THREAD_EXECUTE() finish");
}
DWORD SERVICE_MAIN_THREAD_executionThread(LPDWORD pp) {

	try {
		service->MAIN_THREAD_EXECUTE();
	}
	catch (...) {
		// 2021 09 crash_log("service->MAIN_THREAD_EXECUTE()");
	};

	// 2021 09 thread_list.th_finish();
	return 0;
}
void SERVICE::start_MAIN_THREAD() {
	
	// 2021 09 MAIN_THREAD_h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)SERVICE_MAIN_THREAD_executionThread, 0, 0, &MAIN_THREAD_id, "SERVICE::start_MAIN_THREAD()");
}
// end



//------------------------------------------------------------------------------------------------------
// SERVICE pipe CONTROL
void SERVICE::PIPE_CONTROL_THREAD_EXECUTE() {
	/* 2021 09
	

	DWORD ll, last_check_agent = 0;

	char c1, c2, c3, c4;
	

	ll = 0;

	send_udp("CONTROL THREAD run");

	is_run_PIPE_CONTROL_THREAD = true;

	while (GLOBAL_STOP == false) {
		
		total_control.SERVICE_PIPE_CONTROL_THREAD_EXECUTE++;

		if (interaction_with_INDICATOR_TIMEOUT != 0 &&
			interaction_with_INDICATOR_TIMEOUT + 5000 < GetTickCount()) {
			// произошел Timeout 
			set_interaction_with_INDICATOR_TIMEOUT( 0 ); 
			
			
			send_udp2("CONTROL interaction_with_INDICATOR_TIMEOUT! DisconnectNamedPipe(pipe_indicator)");
			
			Disconnect_Named_Pipe(pipe_indicator, "p control 1");
			enter_crit(26);
			set_MASTER_is_indicator_connected( false );
			leave_crit(26);
		}

		if (last_check_agent + 5000 < GetTickCount()) {
			
			CHECK_and_RUN_AGENT_AS_CONSOLE();
			last_check_agent = GetTickCount();
		};
		if (last_active_INDICATOR + 5000 < GetTickCount()) {
			
			CHECK_and_RUN_INDICATOR_AS_CONSOLE();
			last_active_INDICATOR = GetTickCount();  send_udp2("last_active_INDICATOR detect (now running)");
		};

		if ( ll + 5000 < GetTickCount() ) {
			 ll = GetTickCount();

			if (interaction_with_agent_IN_USE) { c1 = '+'; } else { c1 = '-'; };
			if (interaction_with_agent_TIMEOUT > 0) { c2 = '+'; } else { c2 = '-'; };
			if (MASTER_is_agent_connected) { c3 = '+'; } else { c3 = '-'; };
			if (write_info_connect_TIMEOUT > 0) { c4 = '+'; } else { c4 = '-'; };

			//sprintf_ s(ss, 290, "CONTROL - ia_in_USE=%c ia_TIME=%c agent_con=%c w_info_con_TIME=%c last_agent_active=%d" , c1, c2, c3, c4, (GetTickCount() - last_agent_active) );
			//send_udp(ss);
		}
		//send_udp("2");
		if (write_info_connect_TIMEOUT > 0) {
			if (write_info_connect_TIMEOUT + 5000 < GetTickCount()) {
				write_info_connect_TIMEOUT = 0; 
				send_udp2("write_info_connect_TIMEOUT = 0 (to)");
				send_udp2("CONTROL write_info_connect_TIMEOUT ! DisconnectNamedPipe(pipe_write_service_info)");
				Disconnect_Named_Pipe(pipe_write_service_info, "p control 1");
				send_udp2("+++3");
			}
		}
		//send_udp("3");
		if (interaction_with_agent_TIMEOUT > 0) {
			if (interaction_with_agent_TIMEOUT + 5000 < GetTickCount()) {
				interaction_with_agent_TIMEOUT = 0;
				send_udp2("CONTROL interaction_with_agent_TIMEOUT ! DisconnectNamedPipe(pipe_master)");
				Disconnect_Named_Pipe(pipe_master, "p control 2");

				enter_crit(27);
				MASTER_is_agent_connected = false;
				leave_crit(27);

				//send_udp("SERVICE::PIPE_CONTROL_THREAD_EXECUTE() (interaction_with_agent_TIMEOUT!) MASTER_is_agent_connected = false");


			}
		}
		if (last_agent_active + 5000 < GetTickCount() && MASTER_is_agent_connected == true ) {
			
			Disconnect_Named_Pipe(pipe_master, "p control ag_last_act");

			enter_crit(28);
			MASTER_is_agent_connected = false;
			leave_crit(28);

		}
		//send_udp2("004");
		try
		{
			total_control.send_udp_SERVICE();
		}
		catch (...) {
			send_udp("catch(...) total_control.send_udp_SERVICE() ");
		}
		//send_udp2("4");
		Sleep(500);
		//send_udp2("100");
	}

	send_udp("1000");

	Sleep(1000);

	if (app_attributes.WINDOWS_is_XP()) {

		if (is_run_PIPE_INDICATOR_THREAD) {
			is_run_PIPE_INDICATOR_THREAD = false;
			my_Terminate_Thread(PIPE_INDICATOR_THREAD_handle);
		};
		if (is_run_PIPE_WRITE_INFO_THREAD) {
			is_run_PIPE_WRITE_INFO_THREAD = false;
			my_Terminate_Thread(WRITE_INFO_THREAD_h);
		};
		if (is_run_MAIN_THREAD) {
			is_run_MAIN_THREAD = false;
			my_Terminate_Thread(MAIN_THREAD_h);
		};

	};
	//send_udp2("CONTROL close all pipes ...");
	close_pipe_handle(&pipe_write_service_info, "pipe control finish write_service_info");
	close_pipe_handle(&pipe_master, "pipe control finish master");
	close_pipe_handle(&pipe_indicator, "pipe control finish pipe_indicator");



	//send_udp2("CONTROL close all pipes finish");

	is_run_PIPE_CONTROL_THREAD = false;

	send_udp2("CONTROL THREAD finish");
	*/
}
DWORD SERVICE_PIPE_CONTROL_THREAD_executionThread(LPDWORD pp) {

	try
	{
		service->PIPE_CONTROL_THREAD_EXECUTE();
	}
	catch (...) {
		//crash_log("service->PIPE_CONTROL_THREAD_EXECUTE()");
	};
	//thread_list.th_finish();
	return 0;
}
void SERVICE::start_PIPE_CONTROL_THREAD() {
	HANDLE h;
	DWORD id;
	// 2021 09 h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)SERVICE_PIPE_CONTROL_THREAD_executionThread, 0, 0, &id, "SERVICE::start_PIPE_CONTROL_THREAD()");
}
// end

void SERVICE::set_MASTER_is_indicator_connected(bool val) {
	if (val) {
		//send_udp2("set --- MASTER_is_indicator_connected = true");
	}
	else {
		//send_udp2("set --- MASTER_is_indicator_connected = false");
	}
	MASTER_is_indicator_connected = val;
}

void  SERVICE::set_interaction_with_INDICATOR_TIMEOUT(int val) {
	//total_control.SERVICE_interaction_with_INDICATOR_TIMEOUT = val;
	interaction_with_INDICATOR_TIMEOUT = val;
}

//------------------------------------------------------------------------------------------------------
// PIPE MASTER
void SERVICE::PIPE_MASTER_THREAD_EXECUTE() {
	
	//*******************************************************************************************
	/*

	Создаем PIPE для связи с AGENT. Ждем когда подключится AGENT. Выставляем флаг - MASTER_is_agent_connected
	

	*/
	//*******************************************************************************************

	//send_udp("PIPE_MASTER_THREAD_EXECUTE run thread");

	
	/*** 2021 09
	is_run_PIPE_MASTER_THREAD = true;

	SECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);


	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = TRUE;

	if (MASTER_is_pipe_open == false) {
		//send_udp("create master pipe...");
	
		
		
		pipe_master = CreateNamedPipe(L"\\\\.\\pipe\\$visiator_master$", PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 1000000, 1000000, 500, &sa);// NMPWAIT_USE_DEFAULT_WAIT, NULL);

		

		if (pipe_master == INVALID_HANDLE_VALUE) {
			pipe_master = 0;
			//send_udp("pipe_master == INVALID_HANDLE_VALUE");
			
			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		};
		//send_udp("MASTER_is_pipe_open = true    <---------------------------------------------- ");
		MASTER_is_pipe_open = true;
	}

	BOOL x;

	while (GLOBAL_STOP == false) {

		total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE++;

		total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 1;

		send_udp("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     MASTER pipe connect...");

		x = ConnectNamedPipe(pipe_master, NULL);

		total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 2;

		if (x == TRUE) {
			send_udp("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     MASTER_is_agent_connected = true");

			total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 3;

			SET_last_agent_active("connect MASTER");

			MASTER_is_agent_connected = true;

			while (GLOBAL_STOP == false && MASTER_is_agent_connected == true) {

				total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_2++;

				total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 4;

				Sleep(1);
			};

			total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 5;

			if (MASTER_is_agent_connected == true) {

				total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 6;

				enter_crit(29);
				MASTER_is_agent_connected = false;
				leave_crit(29);
				
				send_udp("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     MASTER_is_agent_connected = false Disconnect...");

				Disconnect_Named_Pipe(pipe_master, "p master 1");

				total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 7;

			}

		};

		total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 8;

		send_udp("master 4    <----------------------------------------------");
		Sleep(1);
		send_udp("master 100    <----------------------------------------------");
	}

	total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 9;

	is_run_PIPE_MASTER_THREAD = false;
	send_udp("PIPE_MASTER_THREAD_EXECUTE() finish");
	***/
}

DWORD SERVICE_PIPE_MASTER_THREAD_executionThread(LPDWORD pp) {

	try {
		service->PIPE_MASTER_THREAD_EXECUTE();
	}
	catch (...) {
		//crash_log("service->PIPE_MASTER_THREAD_EXECUTE()");
	};
	service->MASTER_THREAD_id = 0;
	//thread_list.th_finish();
	return 0;
}
void SERVICE::start_PIPE_MASTER_THREAD() {
	HANDLE h;
	
	// 2021 09 h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)SERVICE_PIPE_MASTER_THREAD_executionThread, 0, 0, &MASTER_THREAD_id, "SERVICE::start_PIPE_MASTER_THREAD()");
}
// end

void SERVICE::reconnect_master_pipe() {

	// 2021 09 Disconnect_Named_Pipe(pipe_master, "reconnect_master_pipe");
	MASTER_is_agent_connected = false;

}

//------------------------------------------------------------------------------------------------------
// PIPE WRITE INFO
void SERVICE::PIPE_WRITE_INFO_THREAD_EXECUTE() {
	
	/* 2021 09 
	is_run_PIPE_WRITE_INFO_THREAD = true;



	SECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);


	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = TRUE;


	SERVICE_INFO service_info;
	DWORD w, r;
	BOOL x;

	pipe_write_service_info = CreateNamedPipe(L"\\\\.\\pipe\\$visiator_wsi$", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 128, 128, 500, &sa);// NMPWAIT_USE_DEFAULT_WAIT, NULL);

	if (pipe_write_service_info == INVALID_HANDLE_VALUE) {
		pipe_write_service_info = 0;
		fatal_error("pipe_write_service_info == INVALID_HANDLE_VALUE");
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
	};




	while (GLOBAL_STOP == false) {

		total_control.SERVICE_PIPE_WRITE_INFO_THREAD_EXECUTE++;

		//send_udp2("Connect wait...");

		x = ConnectNamedPipe(pipe_write_service_info, NULL);
		
		

		if (x == TRUE) {
			
			
			
			

			write_info_connect_TIMEOUT = GetTickCount();
			
			//send_udp2("r1...");

			x = ReadFile(pipe_write_service_info, &service_info, 128, &r, NULL);
			if (x == TRUE) {
				//send_udp2("r1 true ", r);
			}
			else {
				//send_udp2("r1 false ", r);
			}
			if (x == TRUE && r == 128) {
				
				//sprintf_ s(ss, 200, "SERVICE::PIPE_WRITE_INFO_THREAD_EXECUTE() --- %d ", service_info.who_is_asked );
				//send_udp2(ss);
				if (service_info.who_is_asked == IT_IS_agent) {
					//last_active_AGENT = GetTickCount();
					SET_last_agent_active("conn write info");
					//send_udp2("SERVICE::PIPE_WRITE_INFO_THREAD_EXECUTE() --- detect agent activity !");
				}
				else {
					if (service_info.who_is_asked == IT_IS_indicator) {
						
						last_active_INDICATOR = GetTickCount(); send_udp2("last_active_INDICATOR detect 1");

						//if (service_info.INDICATOR_detect_clipboard_is_changed != 0) {
						//	INDICATOR_SAY_clipboard_is_changed();
						//}

						//send_udp2("SERVICE::PIPE_WRITE_INFO_THREAD_EXECUTE() --- detect indicator activity !");
					}
					else {
						if (service_info.who_is_asked == IT_IS_desktop) {
							last_active_DESKTOP = GetTickCount();
							//send_udp2("SERVICE::PIPE_WRITE_INFO_THREAD_EXECUTE() --- detect desktop activity !");
						}
						else {
							send_udp2("SERVICE::PIPE_WRITE_INFO_THREAD_EXECUTE() --- ???");
						}
					}
				}
				
				zero_void((void*)&service_info, 128);
				get_service_info(&service_info);

				//send_udp2("write info... 128 info");

				x = WriteFile(pipe_write_service_info, &service_info, 128, &w, NULL);
				if (x == TRUE) {
					//send_udp2("TRUE w=", w);
				}
				else {
					//send_udp2("FALSE w=", w);
				}

				if (x == TRUE && w == 128) {
					//send_udp2("___1");
				}
				else {
					//send_udp2("___2");
				}

				//send_udp2("read2 info... 128 info");

				x = ReadFile(pipe_write_service_info, &service_info, 128, &r, NULL);
				if (x == TRUE) {
					//send_udp2("read true ", r);
				}
				else {
					//send_udp2("read false ", r);
				}
			}
			else {
				send_udp2("r1 FALSEE");
			}
			//if (write_info_connect_TIMEOUT > 0) {
				//write_info_connect_TIMEOUT = 0; //send_udp("write_info_connect_TIMEOUT = 0 (as)");
				//Sleep(333);
			//send_udp2("Disconnect_Named_Pipe - -");
			Disconnect_Named_Pipe(pipe_write_service_info, "1");
			write_info_connect_TIMEOUT = 0;
			//};
		}
		else {
			send_udp2("SERVICE write info Connect error");
			
		}


		Sleep(1);
	}

	is_run_PIPE_WRITE_INFO_THREAD = false;

	send_udp2("SERVICE write info thread finish");
	***/
}
DWORD SERVICE_PIPE_WRITE_INFO_THREAD_executionThread(LPDWORD pp) {

	try
	{
		service->PIPE_WRITE_INFO_THREAD_EXECUTE();
	}
	catch (...) {
		//crash_log("service->PIPE_WRITE_INFO_THREAD_EXECUTE()");
	};
	service->WRITE_INFO_THREAD_id = 0;
	//thread_list.th_finish();
	return 0;
}
void SERVICE::start_PIPE_WRITE_INFO_THREAD() {
	
	
	// 2021 09 WRITE_INFO_THREAD_h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)SERVICE_PIPE_WRITE_INFO_THREAD_executionThread, 0, 0, &WRITE_INFO_THREAD_id, "SERVICE::start_PIPE_WRITE_INFO_THREAD()");
}
// end

void SERVICE::PIPE_INDICATOR_EXECUTE() {
	
	//********************************************************************************************************
	/*
	В ЭТОЙ ПРОЦЕДУРЕ:
	Создаем PIPE для связи с INDICATOR. Ждем подключения клиента. Когда клиент подключился, выставляем флаг - MASTER_is_indicator_connected
	
	
	НЕ В ЭТОЙ ПРОЦЕДУРЕ:
	Если стоит флаг MASTER_is_indicator_connected, значит можно асинхронно осуществлять транзакции с INDICATOR.
	Сами транзакции делаются не в этой процедуре, а где то еще, возможно из других потоков.
	Для того, что бы параллельные транзакции не накладывались друг на друга, используется флаг - interaction_with_INDICATOR_IN_USE.
	Установка этого флага (interaction_with_INDICATOR_IN_USE) делается через процедуру - lock_interaction_with_INDICATOR();
	Сброс флага interaction_with_INDICATOR_IN_USE делается непосредственно interaction_with_INDICATOR_IN_USE = false;

	Транзакция это две последовательные операции: write и read. 
	Пояснение для операции write: Передаем данные от MASTER к INDICATOR. Минимальный размер 128 bite. Первые 4 byte это UINT с указанием размера всего блока write.
	Пояснение для операции read: Получаем данные от MASTER к INDICATOR. Сначала читаем 128 byte. Из них берем первые 4 и определяем полный размер блока данных. Если размер больше 128, делаем еще одну операцию чтения.

	*/
	//********************************************************************************************************

	/* 2021 09

	total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 1;

	is_run_PIPE_INDICATOR_THREAD = true;

	SECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);


	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = TRUE;

	pipe_indicator = CreateNamedPipe(L"\\\\.\\pipe\\$visiator_indicator$", PIPE_ACCESS_DUPLEX , PIPE_TYPE_BYTE | PIPE_WAIT, 1, 1000000, 1000000, 500, &sa);

	if (pipe_indicator == INVALID_HANDLE_VALUE) {
		pipe_indicator = 0;
		send_udp("pipe_indicator == INVALID_HANDLE_VALUE");
		total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 2000;
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
	}
	else {
		total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 2; 
	}
		
		
	

	BOOL x;

	while (GLOBAL_STOP == false) {

		total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 3;
		send_udp2("wait INDICATOR connect...");
		x = ConnectNamedPipe(pipe_indicator, NULL);
		send_udp2("INDICATOR connect DETECT!");
		total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 4;

		if (x == TRUE) {

			set_MASTER_is_indicator_connected( true );

			total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 5;

			while (GLOBAL_STOP == false && MASTER_is_indicator_connected == true) {
				Sleep(1);
			};
			send_udp2("+++++++++++++++++++ 5->6");
			total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 6;

			if (MASTER_is_indicator_connected == true) {

				total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 7;

				enter_crit(30);
				set_MASTER_is_indicator_connected( false );
				leave_crit(30);

				

				Disconnect_Named_Pipe(pipe_indicator, "p master 1");

				total_control.SERVICE_PIPE_INDICATOR_THREAD_EXECUTE_status = 8;

			}

		};

		Sleep(1);
	}

	
	is_run_PIPE_INDICATOR_THREAD = false;
	
	***/
}
DWORD SERVICE_PIPE_INDICATOR_EXECUTE_executionThread(LPDWORD pp) {

	try
	{
		service->PIPE_INDICATOR_EXECUTE();
	}
	catch (...) {
		//crash_log("service->PIPE_INDICATOR_EXECUTE()");
	};
	service->PIPE_INDICATOR_THREAD_id = 0;
	//thread_list.th_finish();
	return 0;
}
void SERVICE::start_PIPE_INDICATOR_EXECUTE() {
	
	
	// 2021 09 PIPE_INDICATOR_THREAD_handle = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)SERVICE_PIPE_INDICATOR_EXECUTE_executionThread, 0, 0, &PIPE_INDICATOR_THREAD_id, "SERVICE::start_PIPE_INDICATOR_EXECUTE()");
}

void SERVICE::SEND_INFO_to_INDICATOR_EXECUTE() {
	is_run_SEND_INFO_to_INDOCATOR_THREAD = true;

	char packet_send[550];
	char packet_recv[550];

	while (GLOBAL_STOP == false) {

		if (MASTER_is_indicator_connected == true) {
			SEND_INFO_to_INDICATOR(packet_send, packet_recv);
		};

		my_Slip(500);
	}

	is_run_SEND_INFO_to_INDOCATOR_THREAD = false;
}

DWORD SERVICE_END_INFO_to_INDICATOR_EXECUTE_executionThread(LPDWORD pp) {

	try {
		service->SEND_INFO_to_INDICATOR_EXECUTE();
	}
	catch (...) {
		//crash_log("service->SEND_INFO_to_INDICATOR_EXECUTE()");
	};

	//thread_list.th_finish();
	return 0;
}
void SERVICE::start_SEND_INFO_to_INDICATOR_EXECUTE() {
	HANDLE h;
	DWORD id;
	// 2021 09 h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)SERVICE_END_INFO_to_INDICATOR_EXECUTE_executionThread, 0, 0, &id, "SERVICE::start_SEND_INFO_to_INDICATOR_EXECUTE()");
}

void SERVICE::get_service_info(SERVICE_INFO *service_info) {
	
	service_info->type = 1286011; // признак
	
	service_heart_beat++;
	service_info->heart_beat = service_heart_beat;
	service_info->service_id = PUBLIC_ID;
	if (is_sync_clipboards) service_info->is_sync_clipboards = 1;
	else service_info->is_sync_clipboards = 0;
	
	int client_connected_count = 0, ready_to_client_connect = 0;

	/* 2021 09 
	if (session_pool == NULL) {
		service_info->is_client_connected = 0;
		service_info->is_ready_to_client_connect = 0;
	} else{
		client_connected_count = 0;
		ready_to_client_connect = 0;

		session_pool->client_connected_count(&client_connected_count, &ready_to_client_connect);

		service_info->is_client_connected = client_connected_count;
		service_info->is_ready_to_client_connect = ready_to_client_connect;
		
	}
	*/
	 

}

void SERVICE::start_indicator() {

}

bool SERVICE::check_run_indicator() {

	

	if (check_mutex(L"Global\\vim")) {
		//send_udp("INDICATOR--- free");
	}
	else {
		//send_udp("INDICATOR--- locked");
	}

	return false;
}

void SERVICE::INDICATOR_THREAD_EXECUTE() {

	/* 2021 09 
	send_udp2("INDICATOR_THREAD_EXECUTE start");

	is_run_INDICATOR_THREAD = true;

	while (GLOBAL_STOP == false) {
		
		total_control.SERVICE_INDICATOR_THREAD_EXECUTE++;

		if ( check_run_indicator() == false) {

		}
		
		my_Slip(5000);
	}

	is_run_INDICATOR_THREAD = false;

	send_udp2("INDICATOR_THREAD_EXECUTE stop");
	*/
}

DWORD SERVICE_INDICATOR_THREAD_executionThread(LPDWORD pp) {

	try
	{
		service->INDICATOR_THREAD_EXECUTE();
	}
	catch (...) {
		//crash_log("service->INDICATOR_THREAD_EXECUTE()");
	}
	//thread_list.th_finish();
	return 0;
}

void SERVICE::start_INDICATOR_THREAD() {
	HANDLE h;
	DWORD id;
	// 2021 09 h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)SERVICE_INDICATOR_THREAD_executionThread, 0, 0, &id, "SERVICE::start_INDICATOR_THREAD()");
}

//------------------------------------------------------------------------------------------------------
bool SERVICE::is_ALL_THREAD_IS_STOP() {
	/* 2021 09 
	if (is_run_MAIN_THREAD == true) {
		//send_udp2("01"); 
		return false;
	}
	if (is_run_PIPE_CONTROL_THREAD == true) {
		//send_udp2("02"); 
		return false;
	}
	if (is_run_PIPE_WRITE_INFO_THREAD == true) {
		//send_udp2("03"); 
		return false;
	}
	if (is_run_PIPE_MASTER_THREAD == true) {
		//send_udp2("04"); 
		return false;
	};
	if (is_run_INDICATOR_THREAD == true) { send_udp2("1"); return false; };
	if (is_run_PIPE_INDICATOR_THREAD == true) { send_udp2("2"); return false; };
    if (is_run_SEND_INFO_to_INDOCATOR_THREAD == true) { send_udp2("3"); return false; };
	*/

	
	return true;
}

void SERVICE::save_in_registry_last_run_ver_and_date() {

	//HKEY_LOCAL_MACHINE, L"Software\\VisiatorService"

	HKEY hkey;
	LSTATUS rr;

	hkey = 0;    //HKEY_USERS    .DEFAULT\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections
	rr = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\VisiatorService", 0, KEY_READ | KEY_WRITE | 0x0100, &hkey);
	if (rr == 2) { // нет ключа
		rr = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"Software\\VisiatorService", 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | 0x0100, NULL, &hkey, NULL);
	};
	if (hkey == 0) {
		return;
	}

	unsigned int yy[10], *z, vv;
	for (int i = 0; i < 10; i++) yy[i] = 0;

	SYSTEMTIME st;
	GetLocalTime(&st);

	z = yy;

	*z++ = st.wYear;
	*z++ = st.wMonth;
	*z++ = st.wDay;

	*z++ = st.wHour;
	*z++ = st.wMinute;
	*z++ = st.wSecond;

	DWORD size;
	DWORD type;

	size = 4*6;
	type = REG_BINARY;

	rr = RegSetValueEx(hkey, L"lrd", 0, type, (BYTE *)yy, size); // last run date

	size = 4;
	vv = app_attributes.VER;
	rr = RegSetValueEx(hkey, L"lrv", 0, type, (BYTE *)&vv, size); // last run VER


	RegCloseKey(hkey);


}

void SERVICE::RUN() {
	// send_udp("SERVICE::RUN()");

	PROXY_LIST *proxy_list = app_attributes.proxy_list;

	proxy_list->load_from_registry(HKEY_USERS, L".DEFAULT\\Software\\Visiator");
	proxy_list->Load_ProxySettings();
	proxy_list->save_in_registry(HKEY_USERS, L".DEFAULT\\Software\\Visiator");

	save_in_registry_last_run_ver_and_date();
	
	thread_EXECUTE = app_attributes.tgroup.create_thread(boost::bind(&SERVICE::EXECUTE, this));

	//if (pipes_server_pool == nullptr) pipes_server_pool = new PIPES_SERVER_POOL();
	//pipes_server_pool->RUN();



	/* 2021 09
	start_INDICATOR_THREAD();
	start_PIPE_CONTROL_THREAD();
	start_PIPE_WRITE_INFO_THREAD();
	start_PIPE_MASTER_THREAD(); // service->PIPE_MASTER_THREAD_EXECUTE();  --->  CreateNamedPipe(...) , ConnectNamedPipe(...) ждем когда подключитсмя клиент
	start_PIPE_INDICATOR_EXECUTE();
	start_MAIN_THREAD(); // service->MAIN_THREAD_EXECUTE(); ---> LOAD_ID_or_REGISTER() , LOAD_PASS() , START_NET_POOL();
	
	start_SEND_INFO_to_INDICATOR_EXECUTE();
	***/
	// send_udp("SERVICE::RUN() finish");
}

/*
void SERVICE::interaction_with_agent() {

	total_control.SERVICE_interaction_with_agent_status = 1;

	MASTER_AGENT_PACKET_HEADER request_packet_header, responce_packet_header;
	
	SCREEN_LIGHT_one_byte screen_one_byte;
	//screen_one_byte.buf_one_byte = NULL;
	//int sizeof_SCREEN_LIGHT_one_byte, sizeof_MASTER_AGENT_PACKET_HEADER;

	//sizeof_SCREEN_LIGHT_one_byte = sizeof(SCREEN_LIGHT_one_byte);
	//sizeof_MASTER_AGENT_PACKET_HEADER = sizeof(MASTER_AGENT_PACKET_HEADER);

	//char *b5m;
	//b5m = neww char[5000000];

	char ss[300];
	BOOL x;
	DWORD r, w;
	int a;
	a = 0;
	while (a == 0 ) {	if (GLOBAL_STOP == true) return;

		

		total_control.SERVICE_interaction_with_agent_status = 2;

		if (interaction_with_agent_IN_USE == false) {

			total_control.SERVICE_interaction_with_agent_status = 3;

			enter_crit();
			interaction_with_agent_IN_USE = true;
			a = 1;
			leave_crit();
		}
		else {
			total_control.SERVICE_interaction_with_agent_status = 4;
			// collision !
			Sleep(1);
		}

		total_control.SERVICE_interaction_with_agent_status = 5;

	};

	total_control.SERVICE_interaction_with_agent_status = 6;

	interaction_with_agent_TIMEOUT = GetTickCount();
	
	//--------------------------------------------------------------------------------------
	// с начала посылаем запрос
	zero(&request_packet_header, 128);
	
	request_packet_header.packet_size = 128;
	request_packet_header.packet_type = packet_type_REQUEST_SCREEN_one_byte;
	w = 0;

	x = WriteFile(pipe_master,&request_packet_header, 128, &w, NULL);
	
	if (x == false || w != 128) {
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;

		Disconnect_Named_Pipe(pipe_master, "ia 1");

		enter_crit();
		MASTER_is_agent_connected = false;
		leave_crit();

		send_udp("SERVICE::interaction_with_agent() (w) MASTER_is_agent_connected = false");

		

		return;
	}

	send_udp("SERVICE::interaction_with_agent() write OK 128");
	//--------------------------------------------------------------------------------------
	// теперь читаем ответ

	//zero(&responce_packet_header, sizeof_MASTER_AGENT_PACKET_HEADER);
	//r = 0;

	total_control.SERVICE_interaction_with_agent_status = 100;
	
	zero(&responce_packet_header, sizeof_MASTER_AGENT_PACKET_HEADER);
	r = 0;
	
	x = Read_Pipe_MASTER(pipe_master, &responce_packet_header, sizeof_MASTER_AGENT_PACKET_HEADER, &r);

	total_control.SERVICE_interaction_with_agent_status = 200;

	//x = ReadFile(pipe_master, &responce_packet_header, sizeof_MASTER_AGENT_PACKET_HEADER, &r, NULL);
	if (x == false || r != 128) {
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;

		Disconnect_Named_Pipe(pipe_master, "ia 2");

		enter_crit();
		MASTER_is_agent_connected = false;
		leave_crit();

		send_udp("SERVICE::interaction_with_agent() (r) MASTER_is_agent_connected = false");



		return;
	}

	total_control.SERVICE_interaction_with_agent_status = 300;
	ss[0] = 0;
	//sprintf_ s(ss,290,"ReadFile (1) 128 ok type=%d ", responce_packet_header.packet_type);
	send_udp(ss);

	if (responce_packet_header.packet_type == packet_type_RESPONCE_SCREEN_one_byte) {
		ss[0] = 0;
		//sprintf_ s(ss,290,"MASTER * detect packet_type_RESPONCE_SCREEN. ReadFile %d ...", sizeof_ENCODED_SCREEN_8bit_header);
		send_udp(ss);
		
		total_control.SERVICE_interaction_with_agent_status = 500;

		r = 0;
		
		x = Read_Pipe_MASTER(pipe_master, &screen_one_byte, sizeof_ENCODED_SCREEN_8bit_header, &r);
		//x = ReadFile(pipe_master, &screen_one_byte, sizeof_SCREEN_LIGHT_one_byte, &r, NULL);
		if (x == false || r != sizeof_ENCODED_SCREEN_8bit_header) {
			interaction_with_agent_TIMEOUT = 0;
			interaction_with_agent_IN_USE = false;

			Disconnect_Named_Pipe(pipe_master, "ia 3");

			enter_crit();
			MASTER_is_agent_connected = false;
			leave_crit();

			//sprintf_ s(ss, 290, "SERVICE::interaction_with_agent() (r2) MASTER_is_agent_connected = false %d ", r);
			//send_udp(ss);
			return;
		};

		total_control.SERVICE_interaction_with_agent_status = 600;

		//sprintf_ s(ss, 290, "ReadFile (2) %d|%d ok", r, sizeof_ENCODED_SCREEN_8bit_header);
		//send_udp(ss);

		if (screen_one_byte.header.w < 0 || screen_one_byte.header.w > 5500 ||
			screen_one_byte.header.h < 0 || screen_one_byte.header.h > 5500) {
			fatal_error("123558ccv GLOBAL_STOP = true;");
			
			interaction_with_agent_TIMEOUT = 0;
			interaction_with_agent_IN_USE = false;
			
			Disconnect_Named_Pipe(pipe_master, "ia 4");

			enter_crit();
			MASTER_is_agent_connected = false;
			leave_crit();

			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
			return;
		}

		total_control.SERVICE_interaction_with_agent_status = 700;

		//sprintf_ s(ss, 290, "%d | %d | %d ", screen_one_byte.header.w, screen_one_byte.header.h, responce_packet_header.packet_size);
		//send_udp(ss);

		if (screen_one_byte.header.w * screen_one_byte.header.h != responce_packet_header.packet_size - 208) {
			send_udp("errrrrrrrr1");
		}
		else {
			send_udp("ooooooooook1");
		}

		if (screen_one_byte.buf_one_byte_size < 0 || screen_one_byte.buf_one_byte_size > 55000000) {
			interaction_with_agent_TIMEOUT = 0;
			interaction_with_agent_IN_USE = false;

			Disconnect_Named_Pipe(pipe_master, "ia 3");

			enter_crit();
			MASTER_is_agent_connected = false;
			leave_crit();

			send_udp("interaction_with_agent() WTF? 1 GLOBAL_STOP = true;");
			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
			return;
		}

		total_control.SERVICE_interaction_with_agent_status = 800;

		screen_one_byte.buf_one_byte_NEW( screen_one_byte.buf_one_byte_size );

		int need_read;
		need_read = screen_one_byte.buf_one_byte_size;
		r = 0;

		//sprintf_ s(ss, 290, "ReadFile... %d ", screen_one_byte.buf_one_byte_size);
		//send_udp(ss);
		//Sleep(1550);
		//      x = ReadFile(pipe_master, screen_one_byte.buf_one_byte, 100, &r, NULL);
		x = Read_Pipe_MASTER(pipe_master, screen_one_byte.get_buf_one_byte(), screen_one_byte.buf_one_byte_size, &r);

		if (x == TRUE) {
			send_udp("** x == true");
		}
		else {
			send_udp("** x == false");
		}

		total_control.SERVICE_interaction_with_agent_status = 900;

		if (x == false || r != screen_one_byte.buf_one_byte_size) {
			
			if (screen_one_byte.get_buf_one_byte() != NULL) screen_one_byte.buf_one_byte_DELETE();
			
			interaction_with_agent_TIMEOUT = 0;
			interaction_with_agent_IN_USE = false;

			Disconnect_Named_Pipe(pipe_master, "ia 5");

			enter_crit();
			MASTER_is_agent_connected = false;
			leave_crit();

			//sprintf_ s(ss, 290, "SERVICE::interaction_with_agent() (r2) MASTER_is_agent_connected = false %d ", r);
			//send_udp(ss);
			return;
		};

		total_control.SERVICE_interaction_with_agent_status = 1000;

		//sprintf_ s(ss, 290, "ReadFile (3) ok %d ", r);
		//send_udp(ss);
	}
	//--------------------------------------------------------------------------------------

	total_control.SERVICE_interaction_with_agent_status = 1100;

	send_udp("interaction_with_agent() OK ");
	if (responce_packet_header.packet_type == packet_type_RESPONCE_SCREEN_one_byte) {
		send_udp("detect packet_type_RESPONCE_SCREEN");
		
		//sprintf_ s(ss, 290, "%d ", responce_packet_header.packet_size);
		//send_udp(ss);
	}
	else {

	}

	total_control.SERVICE_interaction_with_agent_status = 1200;

	if (screen_one_byte.get_buf_one_byte() != NULL) screen_one_byte.buf_one_byte_DELETE();

	interaction_with_agent_TIMEOUT = 0;

	interaction_with_agent_IN_USE = false;

	total_control.SERVICE_interaction_with_agent_status = 2000;

}
*/

extern int lock_interaction_with_INDICATOR_status;

void SERVICE::lock_interaction_with_INDICATOR() {
	
	/* 2021 09
	lock_interaction_with_INDICATOR_status = 1;
	while (GLOBAL_STOP == false) {
		lock_interaction_with_INDICATOR_status = 2;
		if (interaction_with_INDICATOR_IN_USE == false) {
			lock_interaction_with_INDICATOR_status = 3;
			enter_crit(40);
			lock_interaction_with_INDICATOR_status = 4;
			if (interaction_with_INDICATOR_IN_USE == false) {
				lock_interaction_with_INDICATOR_status = 5;
				interaction_with_INDICATOR_IN_USE = true;
				lock_interaction_with_INDICATOR_status = 6;
				leave_crit(40);
				lock_interaction_with_INDICATOR_status = 7;
				//send_udp2("lock_interaction_with_INDICATOR() lock");
				return;
			};
			lock_interaction_with_INDICATOR_status = 8;
			leave_crit(40);
			lock_interaction_with_INDICATOR_status = 9;
		}
		else {
			lock_interaction_with_INDICATOR_status = 10;
			send_udp2("lock_interaction_with_INDICATOR() collision !");
			Sleep(100);
			lock_interaction_with_INDICATOR_status = 101;
		}
		lock_interaction_with_INDICATOR_status = 11;
	};
	lock_interaction_with_INDICATOR_status = 12;
	send_udp2("lock_interaction_with_INDICATOR() GLOBAL_STOP == true ");
	*/
}

void SERVICE::lock_interaction_with_AGENT() {

	/* 2021 09 
	while (GLOBAL_STOP == false) {
		if (interaction_with_agent_IN_USE == false) {
			enter_crit(41);
			if (interaction_with_agent_IN_USE == false) {
				interaction_with_agent_IN_USE = true;
				leave_crit(41);
				return;
			};
			leave_crit(41);
		}
		else {
			// collision !
			Sleep(1);
		}
	};

	*/
}

void SERVICE::interaction_with_agent_STOP_AGENT() {
	/* 2021 09

	total_control.SERVICE_interaction_with_agent_STOP_AGENT_status = 1;

	bool x;
	DWORD r, w;

	char *w_buf_1 = NULL;
	char *r_buf_2 = NULL;

	w_buf_1 = new_char(sizeof_MASTER_AGENT_PACKET_HEADER, 385);
	r_buf_2 = new_char(sizeof_MASTER_AGENT_PACKET_HEADER, 386);

	MASTER_AGENT_PACKET_HEADER *packet_send, *packet_recv;

	packet_send = (MASTER_AGENT_PACKET_HEADER *)w_buf_1;
	packet_recv = (MASTER_AGENT_PACKET_HEADER *)r_buf_2;

	if (MASTER_is_agent_connected == false) {
		send_udp("======> => => => => => => => interaction_with_agent_STOP_AGENT() end (1)");
		delete_(&w_buf_1); delete_(&r_buf_2); 
		return;
	}

	total_control.SERVICE_interaction_with_agent_STOP_AGENT_status = 100;

	lock_interaction_with_AGENT(); // там внутри   interaction_with_agent_IN_USE = true;
    //send_udp("======> => => => => => => => interaction_with_agent_STOP_AGENT() begin");

	interaction_with_agent_TIMEOUT = GetTickCount();

	//******************************************************************************
	// write 128 (1)
	zero_char(w_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER);
	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_REQUEST_STOP_AGENT;

	x = write_pipe(pipe_master, w_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (1-) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		delete_(&w_buf_1); delete_(&r_buf_2); 
		return;
	}

	total_control.SERVICE_interaction_with_agent_STOP_AGENT_status = 200;

	//-----------------------------------------------------------------------------
	// read 128 (2)

	x = read_pipe(pipe_master, r_buf_2, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_MASTER_pipe_TIMEOUT, "i1");
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (2-) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		delete_(&w_buf_1); delete_(&r_buf_2); 
		return;
	}

	total_control.SERVICE_interaction_with_agent_STOP_AGENT_status = 300;

	interaction_with_agent_TIMEOUT = 0;

	//send_udp("interaction_with_agent_2() end");
	interaction_with_agent_IN_USE = false;

	delete_(&w_buf_1); delete_(&r_buf_2);

	//send_udp("======> => => => => => => => interaction_with_agent_STOP_AGENT() end\r\n");

	total_control.SERVICE_interaction_with_agent_STOP_AGENT_status = 400;

	return;
	***/
}

bool SERVICE::interaction_with_agent_SEND_EVENT(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv) {
	

	/* 2021 09
	total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 1;

	if (MASTER_is_agent_connected == false) {
		send_udp("======> => => => => => => => interaction_with_agent_2() end (1)");
		//delete_ w_buf_1; delete_ r_buf_2; //delete_ r_buf_3;

		total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 2;

		return false;
	}

	lock_interaction_with_AGENT(); // там внутри   interaction_with_agent_IN_USE = true;
	
	//send_udp("======> => => => => => => => interaction_with_agent_2() (100) begin ");

	interaction_with_agent_TIMEOUT = GetTickCount();

	total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 100;

	DWORD r, w;
	bool x;

	x = write_pipe(pipe_master, packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (1--) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 102;
		return false;
	}

	total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 200;

	//-----------------------------------------------------------------------------
	// read 128 (2) получаем ответ (заголовок)

	x = read_pipe(pipe_master, packet_recv, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_MASTER_pipe_TIMEOUT, "i2");
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (2--) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 202;
		return false;
	}

	total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 300;

	//send_udp("======> => => => => => => => interaction_with_agent_2() (100) end\r\n");

	interaction_with_agent_TIMEOUT = 0;
	interaction_with_agent_IN_USE = false;

	total_control.SERVICE_interaction_with_agent_SEND_EVENT_status = 400;
	***/
	return true;


}

bool SERVICE::interaction_with_indicator_SET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char *buf, unsigned int buf_size) {

	/* 2021 09

	send_udp2("interaction_with_indicator_SET_CLIPBOARD() buf_size=", buf_size);


	lock_interaction_with_INDICATOR();

	set_interaction_with_INDICATOR_TIMEOUT( GetTickCount() );

	DWORD w;
	bool x;
	unsigned int *ii;

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() write...");
	// write

	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_SET_CLIPBOARD;
	ii = (unsigned int *)&packet_send->reserv[0];
	*ii = buf_size;
	w = 0;

	send_udp2("packet_type_SET_CLIPBOARD (HEADER 128) packet_send->reserv[0] =", buf_size);

	x = write_pipe(pipe_indicator, packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true || w != sizeof_MASTER_AGENT_PACKET_HEADER) {

		set_interaction_with_INDICATOR_TIMEOUT( 0 );
		interaction_with_INDICATOR_IN_USE = false;


		Disconnect_Named_Pipe(pipe_indicator, "p control 13251");
		enter_crit(42);
		set_MASTER_is_indicator_connected( false );
		leave_crit(42);

		return false;
	}


	set_interaction_with_INDICATOR_TIMEOUT( GetTickCount() );

	send_udp2("packet_type_SET_CLIPBOARD (BODY ) need_write_size = ", buf_size);

	w = 0;
	x = write_pipe(pipe_indicator, buf, buf_size, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true || w != buf_size) {

		send_udp2("write fail w=", w);

		set_interaction_with_INDICATOR_TIMEOUT( 0 );
		interaction_with_INDICATOR_IN_USE = false;


		Disconnect_Named_Pipe(pipe_indicator, "p control 132t");
		enter_crit(43);
		set_MASTER_is_indicator_connected(false);
		leave_crit(43);

		return false;
	}

	set_interaction_with_INDICATOR_TIMEOUT(0);

	interaction_with_INDICATOR_IN_USE = false;
	*/
	return true;
}


bool SERVICE::interaction_with_indicator_GET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char **read_buf, unsigned int *read_buf_size) {

	/* 2021 09

	send_udp2("interaction_with_indicator_GET_CLIPBOARD()");

	//send_udp2("interaction_with_INDICATOR_SEND_INFO()...");

	lock_interaction_with_INDICATOR();

	set_interaction_with_INDICATOR_TIMEOUT( GetTickCount() );

	DWORD r, w;
	bool x;

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() write...");
	// write

	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_REQUEST_CLIPBOARD;
	send_udp2("interaction_with_INDICATOR_SEND_INFO() write1... ", sizeof_MASTER_AGENT_PACKET_HEADER);
	w = 0;
	x = write_pipe(pipe_indicator, packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {

		set_interaction_with_INDICATOR_TIMEOUT( 0 );
		interaction_with_INDICATOR_IN_USE = false;


		Disconnect_Named_Pipe(pipe_indicator, "p control 13252");
		enter_crit(44);
		set_MASTER_is_indicator_connected( false);
		leave_crit(44);

		return false;
	}

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() write ok ", w);



	//-----------------------------------------------------------------------------
	// read 128 (2) получаем ответ (заголовок)

	set_interaction_with_INDICATOR_TIMEOUT( GetTickCount() );

	send_udp2("interaction_with_INDICATOR_SEND_INFO() read1...", sizeof_MASTER_AGENT_PACKET_HEADER);

	r = 0;
	x = read_pipe(pipe_indicator, packet_recv, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_MASTER_pipe_TIMEOUT, "i2");
	if (x != true) {

		set_interaction_with_INDICATOR_TIMEOUT( 0 );
		interaction_with_INDICATOR_IN_USE = false;

		return false;
	}

	send_udp2("read1 ok packet_recv->packet_size = ", packet_recv->packet_size);

	if (packet_recv->packet_size > 128) {

		send_udp2("neww[] ", packet_recv->packet_size - 128 + 32);

		unsigned int zz;

		zz = (packet_recv->packet_size - 128) / 16;
		zz *= 16;
		if (zz < (packet_recv->packet_size - 128)) zz += 16;

		zz += 32;

		*read_buf = new_unsigned_char(zz, 4511);

		send_udp2("interaction_with_INDICATOR_SEND_INFO() read2...", packet_recv->packet_size - 128);

		r = 0;
		x = read_pipe(pipe_indicator, *read_buf + 32, packet_recv->packet_size - 128, &r, &read_MASTER_pipe_TIMEOUT, "i2");

		*read_buf_size = zz;

		//send_udp2("read2 ", r);
		unsigned int *ii;
		ii = (unsigned int *)(*read_buf + 16);
		*ii = packet_recv->packet_size - 128;
		//send_udp2("___ *ii=", *ii);
		 
	}

	last_active_INDICATOR = GetTickCount();  send_udp2("last_active_INDICATOR detect 2");

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() read ok", r);


	set_interaction_with_INDICATOR_TIMEOUT( 0 );

	interaction_with_INDICATOR_IN_USE = false;
	//send_udp2("interaction_with_INDICATOR_SEND_INFO() ok");

	***/

	return true;

}


/*
// 2020
bool SERVICE::interaction_with_agent_GET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char **buf, unsigned int *buf_size) {


	send_udp2("interaction_with_agent_GET_CLIPBOARD()...[[100]]");

	char ss[300];
	bool x;
	DWORD w, r, d1, d2;
	d1 = GetTickCount();
	//----------------------------------------------------------------------------------

	//send_udp("interaction_with_agent_2() begin");

	if (MASTER_is_agent_connected == false) {
		send_udp("interaction_with_agent_2() end (1)");

		return false;
	}



	lock_interaction_with_AGENT(); // там внутри   interaction_with_agent_IN_USE = true;

	//send_udp("======> => => => => => => => interaction_with_agent_2() (10) begin");

	interaction_with_agent_TIMEOUT = GetTickCount();

	//******************************************************************************
	// write 128 (1) посылаем запрос (заголовок)

	//zero(packet_send, sizeof_MASTER_AGENT_PACKET_HEADER);
	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_REQUEST_CLIPBOARD;
	send_udp2("write_pipe (p1)...");
	x = write_pipe(pipe_master, packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (1--+) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;

		//MASTER_is_agent_connected = false;
		reconnect_master_pipe();

		return false;
	}

	//-----------------------------------------------------------------------------
	// read 128 (2) получаем ответ (заголовок)
	send_udp2("read_pipe (p2)... 128");
	x = read_pipe(pipe_master, packet_recv, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_MASTER_pipe_TIMEOUT, "i3");
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (2---) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		return false;
	}
	int *ii;
	ii = (int *)&(packet_recv->reserv[0]);

	if(*ii < 0 || *ii > 20000000) {
		send_udp2("*ii < 0 || *ii > 20000000");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		return false;
	}

	send_udp2("read_pipe (p2) TRUE ",*ii);

	*buf_size = (*ii);
	*buf = new_unsigned_char(*ii + 16, 387);

	bool flag = false;

	if (flag == false && packet_recv->packet_size == sizeof_MASTER_AGENT_PACKET_HEADER && packet_recv->packet_type == packet_type_RESPONCE_CLIPBOARD) {
		flag = true;
	}

	if (flag == false) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (не распознанный пакет) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		return false;
	}

	//-----------------------------------------------------------------------------
	// 

	

	send_udp2("CLIPBOARD %d read...\r\n", *ii);
	
	if (*ii > 0) {
		send_udp2("read_pipe (p3)... ", *ii);
		x = read_pipe(pipe_master, (char *)(*buf + 16), *ii, &r, &read_MASTER_pipe_TIMEOUT, "i4");
		if (x != true) {
			send_udp("======> => => => => => => => interaction_with_agent_2() (2----) end ");
			interaction_with_agent_TIMEOUT = 0;
			interaction_with_agent_IN_USE = false;
			return false;
		}
		send_udp2("read_pipe (p3) TRUE ", r);
	};
	


	//-----------------------------------------------------------------------------
	// read 2000000 (4) получаем тело скрина

	
	
	interaction_with_agent_TIMEOUT = 0;

	//send_udp("======> => => => => => => => interaction_with_agent_2() (10) end\r\n");
	interaction_with_agent_IN_USE = false;
	d2 = GetTickCount();

	//sprintf_ s(ss, 90, "IA = %d ", d2 - d1);
	//send_udp(ss);

	return true;
}
*/
bool SERVICE::interaction_with_agent_GET_SCREEN(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, ENCODED_SCREEN_8bit_header *scr_head_buf, SCREEN_LIGHT_one_byte *screen_light) {

	if (screen_light == NULL) return false;

	/* 2021 09 

	total_control.SERVICE_interaction_with_agent_GET_SCREEN++;

	total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 1;

	//char ss[300];
	bool x;
	DWORD w, r, d1, d2;
	d1 = GetTickCount();
	//----------------------------------------------------------------------------------

	//send_udp("interaction_with_agent_2() begin");

	if (MASTER_is_agent_connected == false) {
		send_udp("interaction_with_agent_2() end (1)");
		
		total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 2;
		return false;
	}

	

	lock_interaction_with_AGENT(); // там внутри   interaction_with_agent_IN_USE = true;

	//send_udp("======> => => => => => => => interaction_with_agent_2() (10) begin");

	interaction_with_agent_TIMEOUT = GetTickCount();

	//******************************************************************************
	// write 128 (1) посылаем запрос (заголовок)

	//zero(packet_send, sizeof_MASTER_AGENT_PACKET_HEADER);
	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_REQUEST_SCREEN_one_byte;
	
	total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 100;

	x = write_pipe(pipe_master, packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (1--+) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		
		//MASTER_is_agent_connected = false;
		reconnect_master_pipe();
		total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 102;
		return false;
	}

	//-----------------------------------------------------------------------------
	// read 128 (2) получаем ответ (заголовок)

	total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 200;

	x = read_pipe(pipe_master, packet_recv, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_MASTER_pipe_TIMEOUT, "i3");
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (2---) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 202;
		return false;
	}

	bool flag = false;

	if (flag == false && packet_recv->packet_size == sizeof_MASTER_AGENT_PACKET_HEADER && packet_recv->packet_type == packet_type_RESPONCE_SCREEN_one_byte) {
		flag = true;
	}

	if (flag == false) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (не распознанный пакет) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		return false;
	}

	//-----------------------------------------------------------------------------
	// read 128 (3) получаем шапку скрина
	total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 300;
	x = read_pipe(pipe_master, (char *)scr_head_buf, sizeof_ENCODED_SCREEN_8bit_header, &r, &read_MASTER_pipe_TIMEOUT, "i4");
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (2----) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 301;
		return false;
	}

	if (scr_head_buf->w <= 0 || scr_head_buf->w > 5500 ||
		scr_head_buf->h <= 0 || scr_head_buf->h > 5500) {
		send_udp("======> => => => => => => => receive screen size is bad ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		return false;
	}

	total_control.SERVICE_interaction_with_agent_GET_SCREEN_w = screen_light->header.w;
	total_control.SERVICE_interaction_with_agent_GET_SCREEN_h = screen_light->header.h;
	total_control.SERVICE_interaction_with_agent_GET_SCREEN_cursor = screen_light->header.mouse_cursor_type_id;

	if (screen_light->header.w != scr_head_buf->w ||
		screen_light->header.h != scr_head_buf->h) {
		if (screen_light->get_buf_one_byte() != NULL) screen_light->buf_one_byte_DELETE();
		//screen_light->buf_one_byte = NULL;

		screen_light->header.w = scr_head_buf->w;
		screen_light->header.h = scr_head_buf->h;

		screen_light->buf_one_byte_size = screen_light->header.w * screen_light->header.h;
		screen_light->buf_one_byte_NEW( screen_light->buf_one_byte_size );
	}

	screen_light->header.keyboard_location = scr_head_buf->keyboard_location;
	screen_light->header.itis_user_move_mouse = scr_head_buf->itis_user_move_mouse;
	screen_light->header.mouse_cursor_type_id = scr_head_buf->mouse_cursor_type_id;
	screen_light->header.mouse_x = scr_head_buf->mouse_x;
	screen_light->header.mouse_y = scr_head_buf->mouse_y;



	//-----------------------------------------------------------------------------
	// read 2000000 (4) получаем тело скрина

	total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 400;

	x = read_pipe(pipe_master, screen_light->get_buf_one_byte(), screen_light->buf_one_byte_size, &r, &read_MASTER_pipe_TIMEOUT, "i5");
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (3) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;
		total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 401;
		return false;
	}

	interaction_with_agent_TIMEOUT = 0;

	//send_udp("======> => => => => => => => interaction_with_agent_2() (10) end\r\n");
	interaction_with_agent_IN_USE = false;
	d2 = GetTickCount();

	//sprintf_ s(ss, 90, "IA = %d ", d2 - d1);
	//send_udp(ss);

	total_control.SERVICE_interaction_with_agent_GET_SCREEN_status = 500;
	*/
	return true;
}

void SERVICE::SET_last_agent_active(char *info) {
	last_agent_active = GetTickCount();
	
	// 2021 09 total_control.SERVICE_last_agent_active = last_agent_active;

	//char ss[100];
	//sprintf_ s(ss, 90, "SET last_agent_active = GetTickCount(); %s ", info);
	//send_udp2(ss);
	
}
void SERVICE::LOAD_ID_or_REGISTER() {
	
	Load_private_id_and_public_id_from_SERVICE_registry(&PUBLIC_ID, &PRIVATE_ID);

	if (PUBLIC_ID == 0) {
		Register_new_partner_on_proxy();
		Load_private_id_and_public_id_from_SERVICE_registry(&PUBLIC_ID, &PRIVATE_ID);
	}
	//char ss[200];
	//generate_ID_to_text(ss, PUBLIC_ID);
	//send_udp(ss);
}
void SERVICE::LOAD_PASS() {
	if (PASS == nullptr) PASS = new unsigned char[32];
	zero_unsigned_char(PASS, 32);
	load_service_pass_hash16(PASS);
}
void SERVICE::START_NET_POOL() {
	//send_udp("START_NET_POOL() begin");

	/* 2021 09 TODO!
	if (session_pool == NULL) {
		session_pool = new_NET_SERVER_SESSION_POOL(&PUBLIC_ID, &PRIVATE_ID, PASS, 730);
	}

	
	session_pool->ACTION(NULL, NULL);

	send_udp("START_NET_POOL() end");
	***/
}

void SERVICE::set_clipboard_in_to_session(int session_no, unsigned char *buf, unsigned int buf_size) {
	
	/* 2021 09
	bool r;

	MASTER_AGENT_PACKET_HEADER *packet_send, *packet_recv;

	packet_send = (MASTER_AGENT_PACKET_HEADER *)packet_send_event;
	packet_recv = (MASTER_AGENT_PACKET_HEADER *)packet_recv_event;

	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_REQUEST_EVENT;

	// 2020 r = interaction_with_agent_SET_CLIPBOARD(packet_send, packet_recv, buf+16, buf_size-16);

	unsigned int *ii;
	ii = (unsigned int *)&buf[16];

	r = interaction_with_indicator_SET_CLIPBOARD(packet_send, packet_recv, buf + 32, *ii);

	if (r == false) {
		send_udp("interaction_with_agent_GET_CLIPBOARD() err");
		return;
	}
	*/

}

void SERVICE::SEND_INFO_to_INDICATOR(char *packet_send, char *packet_recv) {

	/* 2021 09
	if (MASTER_is_indicator_connected == false) return;

	//send_udp2("SEND_INFO_to_INDICATOR()...");

	

	//packet_send = new_char(500, 392);
	//packet_recv = new_char(500, 393);

	MASTER_AGENT_PACKET_HEADER *p_send, *p_recv;

	p_send = (MASTER_AGENT_PACKET_HEADER *)packet_send;
	p_recv = (MASTER_AGENT_PACKET_HEADER *)packet_recv;

	p_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	p_send->packet_type = packet_type_REQUEST_EVENT;

	MASTER_AGENT_PACKET_HEADER_event *ev;
	ev = (MASTER_AGENT_PACKET_HEADER_event *)p_send->reserv;

	ev->session_no = 1;
	ev->event_type = 2;
	ev->global_type = 3;
	ev->msg = 4;
	ev->wparam = 5;
	ev->lparam = 6;

	//send_udp("interaction_with_agent_SEND_EVENT() begin...");

	unsigned char *send_buf = NULL;
	unsigned int send_buf_size = 0;
	unsigned char *read_buf = NULL;
	unsigned int read_buf_size = 0;

	SERVICE_INFO *service_info;

	service_info = (SERVICE_INFO *)&p_send->reserv[0];

	zero_void((void*)p_send, 128);
	get_service_info(service_info);

	//send_udp2("write info... 128 info");

	//x = WriteFile(pipe_write_service_info, &service_info, 128, &w, NULL);

	read_buf_size = 0;

	p_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	p_send->packet_type = packet_type_SERVICE_INFO;

	interaction_with_INDICATOR_SEND_INFO(p_send, p_recv, send_buf, send_buf_size, &read_buf, &read_buf_size);
	
	if (interaction_with_INDICATOR_IN_USE == true) {
		send_udp2("WTF?? interaction_with_INDICATOR_IN_USE == true");
	}

	if (read_buf_size > 0) {
		//send_udp2("клиент что то нам передал");
		//send_udp2("r2 = ", read_buf_size);
		delete_(&read_buf);
	}

	//delete_(&packet_send);
	//delete_(&packet_recv);

	*/

}

bool SERVICE::interaction_with_INDICATOR_SEND_INFO(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char *write_buf, unsigned int write_buf_size, unsigned char **read_buf, unsigned int *read_buf_size) {

	/* 2021 09

	//send_udp2("interaction_with_INDICATOR_SEND_INFO()...");

	lock_interaction_with_INDICATOR();

	set_interaction_with_INDICATOR_TIMEOUT( GetTickCount() );

	DWORD r, w;
	bool x;

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() write...");
	// write



	w = 0;
	x = write_pipe(pipe_indicator, packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {
		
		//send_udp2("interaction_with_INDICATOR_SEND_INFO() write... err1");

		interaction_with_INDICATOR_TIMEOUT = 0;
		interaction_with_INDICATOR_IN_USE = false;
		

		Disconnect_Named_Pipe(pipe_indicator, "p control 13253");
		enter_crit(45);
		set_MASTER_is_indicator_connected(false);
		leave_crit(45);

		return false;
	}

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() write ok ", w);

	

	//-----------------------------------------------------------------------------
	// read 128 (2) получаем ответ (заголовок)

	set_interaction_with_INDICATOR_TIMEOUT( GetTickCount() );

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() read...");

	r = 0;
	x = read_pipe(pipe_indicator, packet_recv, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_MASTER_pipe_TIMEOUT, "i2");
	if (x != true) {
		
		set_interaction_with_INDICATOR_TIMEOUT( 0 );
		interaction_with_INDICATOR_IN_USE = false;
		
		send_udp2("error 111222");

		return false;
	}

	if (packet_recv->packet_size > 128) {
		
		*read_buf = new_unsigned_char(packet_recv->packet_size - 128, 4511);

		
		r = 0;
		x = read_pipe(pipe_indicator, *read_buf, packet_recv->packet_size - 128, &r, &read_MASTER_pipe_TIMEOUT, "i2");
		
		*read_buf_size = r;

	}

	last_active_INDICATOR = GetTickCount();  //send_udp2("last_active_INDICATOR detect 3");

	if (packet_recv->reserv[0] == 1) {
		send_udp2("clipboard_is_changed DETECTED!");
		if (session_pool != NULL) session_pool->INDICATOR_SAY_clipboard_is_changed();
	}

	//send_udp2("interaction_with_INDICATOR_SEND_INFO() read ok", r);


	set_interaction_with_INDICATOR_TIMEOUT( 0 );

	interaction_with_INDICATOR_IN_USE = false;
	//send_udp2("interaction_with_INDICATOR_SEND_INFO() ok");

	*/

	return true;
}

/*
// 2020

bool SERVICE::interaction_with_agent_SET_CLIPBOARD(MASTER_AGENT_PACKET_HEADER *packet_send, MASTER_AGENT_PACKET_HEADER *packet_recv, unsigned char *buf, unsigned int buf_size) {
	
	send_udp("interaction_with_agent_SET_CLIPBOARD()..."); 
	
	//char ss[300];
	unsigned int *ii;
	bool x;
	DWORD   d1, d2, r,w;
	d1 = GetTickCount();

	if (MASTER_is_agent_connected == false) {
		send_udp("interaction_with_agent_2() end (1)");
		return false;
	}

	lock_interaction_with_AGENT(); // там внутри   interaction_with_agent_IN_USE = true;

	interaction_with_agent_TIMEOUT = GetTickCount();

	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_SET_CLIPBOARD;
	ii = (unsigned int *)&(packet_send->reserv[0]);
	*ii = buf_size;

	send_udp("write_pipe packet_type_SET_CLIPBOARD ");

	x = write_pipe(pipe_master, packet_send, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (1--+) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;

		//MASTER_is_agent_connected = false;
		reconnect_master_pipe();

		return false;
	}
	send_udp("write buf...");
	x = write_pipe(pipe_master, buf, buf_size, &w, &write_MASTER_pipe_TIMEOUT);
	if (x != true) {
		send_udp("======> => => => => => => => interaction_with_agent_2() (1--+) end ");
		interaction_with_agent_TIMEOUT = 0;
		interaction_with_agent_IN_USE = false;

		//MASTER_is_agent_connected = false;
		reconnect_master_pipe();

		return false;
	}
	send_udp("write buf ok");
	
	interaction_with_agent_TIMEOUT = 0;


	interaction_with_agent_IN_USE = false;
	d2 = GetTickCount();

	return true;
};
*/
void SERVICE::get_clipboard_from_session(int session_no, unsigned char **buf, unsigned int *buf_size) {

	/* 2021 09

	bool r;
	
	//send_udp2("SERVICE::get_clipboard_from_session [[1]]");

	MASTER_AGENT_PACKET_HEADER *packet_send, *packet_recv;

	if (packet_send_event == NULL) packet_send_event = new_char(500, 390);
	if (packet_recv_event == NULL) packet_recv_event = new_char(500, 391);

	packet_send = (MASTER_AGENT_PACKET_HEADER *)packet_send_event;
	packet_recv = (MASTER_AGENT_PACKET_HEADER *)packet_recv_event;

	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_REQUEST_EVENT;

	//send_udp2("SERVICE::get_clipboard_from_session [[2]]");

	// 2020 r = interaction_with_agent_GET_CLIPBOARD(packet_send, packet_recv, buf, buf_size);
	r = interaction_with_indicator_GET_CLIPBOARD(packet_send, packet_recv, buf, buf_size);

	send_udp2("SERVICE::get_clipboard_from_session [[3]]");

	if (r == false) {
		send_udp("interaction_with_agent_GET_CLIPBOARD() err");
		return;
	}

	*/
}

void SERVICE::send_unpress_all_pressed_keys() {

	int session_no = 0;
	unsigned int event_type = 0;
	int global_type = 0;
	unsigned long long msg = 5556;
	unsigned long long wparam = 0;
	unsigned long long lparam = 0;

	send_event_in_to_session( session_no, event_type, global_type, msg, wparam, lparam);
}

void SERVICE::send_event_in_to_session(int session_no, unsigned int event_type, int global_type, unsigned long long msg, unsigned long long wparam, unsigned long long lparam) {
	/* 2021 09
	if (packet_send_event == NULL) packet_send_event = new_char(500, 392);
	if (packet_recv_event == NULL) packet_recv_event = new_char(500, 393);

	MASTER_AGENT_PACKET_HEADER *packet_send, *packet_recv;

	packet_send = (MASTER_AGENT_PACKET_HEADER *)packet_send_event;
	packet_recv = (MASTER_AGENT_PACKET_HEADER *)packet_recv_event;

	packet_send->packet_size = sizeof_MASTER_AGENT_PACKET_HEADER;
	packet_send->packet_type = packet_type_REQUEST_EVENT;

	MASTER_AGENT_PACKET_HEADER_event *ev;
	ev = (MASTER_AGENT_PACKET_HEADER_event *)packet_send->reserv;

	ev->session_no = session_no;
	ev->event_type = event_type;
	ev->global_type = global_type;
	ev->msg = msg;
	ev->wparam = wparam;
	ev->lparam = lparam;

	//send_udp("interaction_with_agent_SEND_EVENT() begin...");

	interaction_with_agent_SEND_EVENT(packet_send, packet_recv);

	//send_udp("interaction_with_agent_SEND_EVENT() end");
	*/
}

//****************************************************************************
//**

int SERVICE___UNINSTALL(wchar_t *service_name) {


	SC_HANDLE schService;
	SC_HANDLE schSCManager;

	// SC_MANAGER_CREATE_SERVICE SC_MANAGER_ENUMERATE_SERVICE
	schSCManager = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);

	if (!schSCManager) { return -1000; }
	// DELETE
	schService = OpenService(schSCManager, service_name, DELETE);

	if (!schService) { CloseServiceHandle(schSCManager); return -11; }

	if (!DeleteService(schService)) {
		// TODO описание ошибки
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return -12;
	}

	CloseServiceHandle(schService);

	CloseServiceHandle(schSCManager);

	return 0;
}
int SERVICE___CHECK_STATUS(wchar_t *service_name) {

	SC_HANDLE schService;
	SC_HANDLE schSCManager;
	SERVICE_STATUS_PROCESS service_status;
	BOOL r;
	int retval;
	retval = -11111;

	schSCManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!schSCManager) { return -10; }
	schService = OpenService(schSCManager, service_name, SERVICE_QUERY_STATUS);
	if (!schService) {
		CloseServiceHandle(schSCManager);
		return -11;
	}

	DWORD ll;
	ll = 111;
	r = QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (BYTE *)&service_status, sizeof(SERVICE_STATUS_PROCESS), &ll);
	if (r == FALSE) { return -12; };

	retval = service_status.dwCurrentState;

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return retval;

}
int SERVICE___STOP(wchar_t *service_name) {
	SC_HANDLE schService;
	SC_HANDLE schSCManager;
	//SERVICE_STATUS_PROCESS service_status;
	BOOL r;
	int retval;
	retval = -11111;

	schSCManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!schSCManager) {
		return -10;
	}
	schService = OpenService(schSCManager, service_name, SERVICE_STOP);
	if (!schService) {
		CloseServiceHandle(schSCManager);
		return -11;
	}

	SERVICE_STATUS ServiceStatus;


	r = ControlService(schService, SERVICE_CONTROL_STOP, &ServiceStatus);

	SERVICE_STATUS svc_status;
	unsigned long dwCheckPoint;

	if (QueryServiceStatus(schService, &svc_status))
	{
		while (svc_status.dwCurrentState != SERVICE_STOPPED)
		{
			dwCheckPoint = svc_status.dwCheckPoint;
			my_Slip(svc_status.dwWaitHint);
			if (!QueryServiceStatus(schService, &svc_status))
			{
				break;
			}
			if (svc_status.dwCheckPoint < dwCheckPoint)
			{
				break;
			}
		}
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return retval;
}
int  ServiceUnInstallLocal() {

	/*
	#define SERVICE_STOPPED                        0x00000001
	#define SERVICE_START_PENDING                  0x00000002
	#define SERVICE_STOP_PENDING                   0x00000003
	#define SERVICE_RUNNING                        0x00000004
	#define SERVICE_CONTINUE_PENDING               0x00000005
	#define SERVICE_PAUSE_PENDING                  0x00000006
	#define SERVICE_PAUSED                         0x00000007
	*/
	//char ss[500];


	int v, rr;
	int i = 0;
	do
	{
		//sprintf__s_i(ss, 450, "uninstall service - check... %d ", i);
		//if (e != NULL) e->set_label(ss);

		v = SERVICE___CHECK_STATUS(strServiceName);
		if (v == -11) { return 0; };// служба не установлена
		if (v != SERVICE_STOPPED && v != -11) {
			if (v == SERVICE_RUNNING) {

				//sprintf__s_i(ss, 450, "uninstall service - stop... %d ", i);
				//if (e != NULL) e->set_label(ss);

				rr = SERVICE___STOP(strServiceName);
				if (rr == -11) {
					//sprintf__s(ss, 450, "uninstall service - no permission");
					//if (e != NULL) e->set_label(ss);

					return -100;
				}
				my_Slip(1000);
			}
			else {
				Sleep(100);
			};
			i++;
			if (i > 15) {
				//sprintf__s(ss, 450, "uninstall service - error");
				//if (e != NULL) e->set_label(ss);
				return -100;
			};
		};


	} while (v != SERVICE_STOPPED && GLOBAL_STOP == false);

	int ii;
	ii = 0;
	do
	{
		//sprintf__s(ss, 450, "uninstall service....");

		i = SERVICE___UNINSTALL(strServiceName);
		if (i == -1000) { // нет адмнинских прав?
			//send_udp("permission denied? ");
			//sprintf__s(ss, 450, "uninstall service - permission denied?");
			//if (e != NULL) e->set_label(ss);
			return -101;
		};
		if (i != 0) my_Slip(1000);
		ii++;
		if (ii > 3) return -100;

	} while (i != 0 && GLOBAL_STOP == false);

	//SERVICE___START(strServiceName);

	return 0;
}
int SERVICE___START(wchar_t *service_name) {

	SC_HANDLE schService;
	SC_HANDLE schSCManager;
	//SERVICE_STATUS_PROCESS service_status;
	BOOL r;
	int retval;
	retval = -11111;

	schSCManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!schSCManager) { return -10; }
	schService = OpenService(schSCManager, service_name, SERVICE_START);
	if (!schService) { CloseServiceHandle(schSCManager);		return -11; }

	r = StartService(schService, 0, 0);
	//int cc;
	//cc = 0;
	//while (r != 1 && cc < 4 && GLOBAL_STOP == false) {
		//my_Slip(1000);
		//r = StartService(schService, 0, 0);
		//cc++;
	//}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return 0;

}
int ServiceInstallLocal()
{
	// send_udp("ServiceInstall()1...");
	SC_HANDLE schSCManager;
	SC_HANDLE schService;

	TCHAR dest_file[MAX_PATH + 9];


	if (app_attributes.is_32_or_64_bit_system != 32 && app_attributes.is_32_or_64_bit_system != 64) {
		//send_udp("-10");
		return -10;

	}

	//char tCantInst[] = "Cannot install service (%d)";


	int k;
	k = 0;
	while (app_attributes.my_exe_file_name[k] != 0) k++;
	while (k > 0 && app_attributes.my_exe_file_name[k] != '\\') k--;
	if (k > 0 && app_attributes.my_exe_file_name[k] == '\\') k++;
	//wcscat_s(szPath, MAX_PATH + 9, L" -service");
	//send_udp("OpenSCManager...");
	schSCManager = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);

	if (!schSCManager) {
		//PrintError(tCantInst);	
		//send_udp("OpenSCManager() error ! Permission denied? ");
		return -1000;
	}
	//send_udp("OpenSCManager ok");
	bool res;
	int dd;

	if (app_attributes.is_32_or_64_bit_system == 64) {



		dd = GetSystemWow64Directory(dest_file, MAX_PATH);
		//send_udp(L"sys 64 dir ", dest_file);
		wcscat_s(dest_file, MAX_PATH, L"\\");
		wcscat_s(dest_file, MAX_PATH, app_attributes.my_exe_file_name + k);
		int kk;
		kk = 0;
		while (my_copyfile(app_attributes.my_exe_file_name, dest_file) == false) {
			Sleep(500);
			kk++;
			if (kk > 30) {
				//send_udp("-13");
				return -13;
			}
		};

	}
	else {


		dd = GetSystemDirectory(dest_file, MAX_PATH);
		//send_udp(L"sys 32 dir ", dest_file);
		wcscat_s(dest_file, MAX_PATH, L"\\");
		wcscat_s(dest_file, MAX_PATH, app_attributes.my_exe_file_name + k);

		res = my_copyfile(app_attributes.my_exe_file_name, dest_file); // TODO надо несколько попыток скопировать
		if (res == false) {
			//send_udp("-14");
			return -14;
		};
	}
	//send_udp("CreateService...");
	schService = CreateService(schSCManager, strServiceName, L"VISIATOR",
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL, dest_file,
		0, 0, 0, 0, 0);
	if (!schService) {
		//PrintError(tCantInst);	
		//send_udp("CreateService error");
		CloseServiceHandle(schSCManager);

		return -15;
	}
	else {
		//send_udp("CreateService ok");
	}



	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	SERVICE___START(strServiceName);

	return 0;
}
int get_service_VISIATOR_status() { // получим статус службы 0-не установлена, 1-установлена но не запущена, 2-запущена
	int v;
	v = SERVICE___CHECK_STATUS(strServiceName);
	if (v < 0) { // нет прав?
		return 0;
	}
	if (v == SERVICE_RUNNING) { return 2; }
	if (v == SERVICE_STOPPED) { return 1; }
	if (v == SERVICE_START_PENDING) { return 1; }
	if (v == SERVICE_STOP_PENDING) { return 1; }
	if (v == SERVICE_CONTINUE_PENDING) { return 1; }
	if (v == SERVICE_PAUSE_PENDING) { return 1; }
	if (v == SERVICE_PAUSED) { return 1; }

	return 0;
	/*
	SERVICE_STOPPED	Служба является не работающей.
	SERVICE_START_PENDING	Служба является запущенной.
	SERVICE_STOP_PENDING	Служба является остановленной.
	SERVICE_RUNNING	Служба является работающей.
	SERVICE_CONTINUE_PENDING	Служба продолжает ожидать.
	SERVICE_PAUSE_PENDING	Приостановленная служба является ожидающей.
	SERVICE_PAUSED	Служба является приостановленной.
	*/
};
void restart_service() {
	
	SERVICE___STOP(strServiceName);
	
	SERVICE___START(strServiceName);

}


void SERVICE::INDICATOR_SAY_clipboard_is_changed() {
	//send_udp2("INDICATOR_SAY_clipboard_is_changed()");
	// 2021 09 if(session_pool != NULL) session_pool->INDICATOR_SAY_clipboard_is_changed();

}

void SERVICE::EXECUTE() {

	// 2021 09 
	
	// MAIN Thread

	EXECUTE_is_run = true;

	boost::posix_time::milliseconds SleepTime(10);

	//sudp("LOAD_ID_or_REGISTER()...");

	while (GLOBAL_STOP == false) {

		if (PUBLIC_ID == 0 || PRIVATE_ID == 0) {
			LOAD_ID_or_REGISTER();
		}
		else {
			break;
		}

		boost::this_thread::sleep(SleepTime);
	};

	//sudp("LOAD_ID_or_REGISTER() ok");

	while (GLOBAL_STOP == false) {

		if (PASS_IS_EMPTY(PASS)) {
			LOAD_PASS();
		}
		else {
			break;
		}

		boost::this_thread::sleep(SleepTime);
	};

	if(GLOBAL_STOP == false) thread_EXECUTE_main_MASTER_AGENT = app_attributes.tgroup.create_thread(boost::bind(&SERVICE::EXECUTE_main_MASTER_AGENT, this));

	//sudp("LOAD_PASS() ok");

	while (GLOBAL_STOP == false) {

		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_is_run = false;

	KillService();
}

void SERVICE::EXECUTE_main_MASTER_AGENT() {

	// Thread для основного потока (reconnect) MASTER<->AGENT

	EXECUTE_main_MASTER_AGENT_is_run = true;

	SECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = TRUE;

	if (MASTER_is_pipe_open == false) {
		//send_udp("create master pipe...");

		pipe_MASTER = CreateNamedPipe(L"\\\\.\\pipe\\$visiator_master$", PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 1000000, 1000000, 500, &sa);// NMPWAIT_USE_DEFAULT_WAIT, NULL);

		if (pipe_MASTER == INVALID_HANDLE_VALUE) {
			pipe_MASTER = 0;
			sudp("pipe_MASTER == INVALID_HANDLE_VALUE");

			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		};
		sudp("MASTER_is_pipe_open = true    <---------------------------------------------- ");
		pipe_MASTER_is_open = true;
	}

	BOOL x;


	boost::posix_time::milliseconds SleepTime(10);

	while (GLOBAL_STOP == false) {


		sudp("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     MASTER pipe connect...");

		x = ConnectNamedPipe(pipe_MASTER, NULL);

		//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 2;

		if (x == TRUE) {
			sudp("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     MASTER_is_agent_connected = true");

			//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 3;

			SET_last_agent_active("connect MASTER");

			MASTER_is_agent_connected = true;

			while (GLOBAL_STOP == false && MASTER_is_agent_connected == true) {

				//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_2++;

				//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 4;

				boost::this_thread::sleep(SleepTime);
			};

			//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 5;

			if (MASTER_is_agent_connected == true) {

				//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 6;

				enter_crit(29);
				MASTER_is_agent_connected = false;
				leave_crit(29);

				

				Disconnect_Named_Pipe(pipe_MASTER, "p master 1");

				//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 7;

			}

		};

		//total_control.SERVICE_PIPE_MASTER_THREAD_EXECUTE_status = 8;

		sudp("master 4    <----------------------------------------------");
		boost::this_thread::sleep(SleepTime);
		sudp("master 100    <----------------------------------------------");

		
	}

	

	EXECUTE_main_MASTER_AGENT_is_run = false;

	Disconnect_Named_Pipe(pipe_MASTER, "c1");

}