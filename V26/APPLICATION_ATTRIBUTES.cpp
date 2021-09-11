#include "APPLICATION_ATTRIBUTES.h"

APPLICATION_ATTRIBUTES::APPLICATION_ATTRIBUTES() {
	proxy_list = new PROXY_LIST();
};

void APPLICATION_ATTRIBUTES::get_all_parametrs(HINSTANCE hInstance_) {
	hInstance = hInstance_;

	global_my_proc_id = GetCurrentProcessId();
	
	/*** 2021
	int bb;
	bb = GetModuleFileName(NULL, my_exe_file_name, 2500);



	im_is_admin = check_admin();
	if (im_is_admin) { send_udp("is admin"); }
	else { send_udp("is no admin"); }



	is_32_or_64_bit_system = check_IsWow64(global_my_proc_id);



	is_service = check_run_as_service();



	if (is_service == true) {
		//if (update_exe_service == NULL) {
		//	update_exe_service = neww UPDATE_EXE(); neew( sizeof(UPDATE_EXE), update_exe_service);
		//};
		//update_exe_service->Start_update_service();
	}



	getRealWindowsVersion();



	_SetProcessDPIAware();



	get_programm_version();



	wchar_t pr[5550];// , *q, *qq;
	for (int i = 0; i < 5550; i++) pr[i] = 0;

	GetEnvironmentVariable(L"PR", pr, 5500);
	if (pr[0] != 0) {
		//send_udp2( "PR=", pr);
		set_startup_paramert_pr(pr);
	};
	***/

}