#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef WIN32

#define WIN32_LEAN_AND_MEAN
#define WIN32_WINNT 0x0501


#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_



#endif

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#include <signal.h>

#include "tools.h"
#include "GUI_low_level.h"
#include "GUI.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "DESKTOP.h"
#include "VIEWER.h"
#include "SERVICE.h"
#include "KERNEL.h"
#include "AGENT.h"
#include "TOTAL_CONTROL.h"

#include <boost/lambda/lambda.hpp>

#pragma comment(lib,"vfw32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "cryptlib.lib")

FONT *font[10] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
bool GLOBAL_STOP = false;
APPLICATION_ATTRIBUTES app_attributes;
DESKTOP  *desktop = nullptr;
VIEWER *viewer = nullptr;
SERVICE *service = nullptr;
KERNEL kernel;
CMDLINE cmd_line;
AGENT *agent = nullptr;
TOTAL_CONTROL *total_control = nullptr;

void WinMain_finish();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//using namespace boost::lambda;

	init_crit_section();
	
	if (init_net() == false) { return -1; }

	

	app_attributes.get_all_parametrs(hInstance);
	
	sudp("start");

	cmd_line.decode((char *)lpCmdLine);
	
	load_fonts();

	total_control = new TOTAL_CONTROL();
	total_control->start_EXECUTE();

	/*
	thread_list = new ALL_THREAD_LIST();

	view_visiator_com = new char[50];
	zero_char(view_visiator_com, 50);
	my_strcpy((unsigned char *)view_visiator_com, (unsigned char *)"view.visiator.com");

	

	cmd_line.decode((char *)lpCmdLine);
	*/

	/***
	unsigned char partner_id[32];
	unsigned char pass_encripted[32];
	unsigned char pass_no_encripted[32];

	zero_unsigned_char(pass_encripted, 32);
	zero_unsigned_char(pass_no_encripted, 32);

	my_strcpy(partner_id, (unsigned char *)"239231241");                               
	my_strcpy(pass_no_encripted, (unsigned char *)"Bara*}Oke");

	viewer = new VIEWER();
	viewer->RUN_VIEWER(partner_id, pass_encripted, pass_no_encripted);
	***/

	if (cmd_line.compare_param_by_no(0, "reinstall")) {

		ServiceUnInstallLocal();
		ServiceInstallLocal();

		return 0;
	}

	//--------------------------------------------------------------------------------------------------------
	// AGENT

	if (cmd_line.count == 1 && cmd_line.compare_param_by_no(0, "agent")) {

		/*if (service == NULL) { service = neww SERVICE(); }
		service->RUN();*/

		if (agent == NULL) { agent = new AGENT(); };

		agent->RUN();

		WinMain_finish();

		sudp("AGENT finish");

		return 0;
	}


	if (app_attributes.is_service) {
		//send_udp("is_service");
		RUN_SERVICE();
		//send_udp("service is stoped");

		WinMain_finish();

		sudp("SERVICE finish");

		return 0;
	};

	
	if (desktop == NULL) desktop = new DESKTOP();
	desktop->RUN();
	
	sudp("DESKTOP finish");

	WinMain_finish();
	return 0;
};

void WinMain_finish() {


}