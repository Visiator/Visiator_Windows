#pragma once

#include "tools.h"
#include "CRYPTO.h"
#include "HIGHLEVEL_COMMAND_QUEUE.h"
#include "SERVICE.h"
#include "FILE__LIST.h"

struct MASTER_AGENT_PACKET_HEADER;
struct MASTER_AGENT_PACKET_HEADER_event;



#define PACKET_TYPE_request_screen 20001

#define PACKET_TYPE_responce_screen_ver11 20002
#define PACKET_TYPE_responce_screen_ver22 20053
#define PACKET_TYPE_responce_screen_ver33 20055

#define PACKET_TYPE_responce_screen_test 20003

#define PACKET_TYPE_request_start_screenflow_ver11 20004
#define PACKET_TYPE_request_start_screenflow_ver22 20052
#define PACKET_TYPE_request_start_screenflow_ver33 20054

#define PACKET_TYPE_responce_mouse_cursor_full_format 20005
#define PACKET_TYPE_responce_mouse_cursor_short_format 20006

#define PACKET_TYPE_send_event_from_client_to_server 20007

#define PACKET_TYPE_send_mouse_from_client_to_server 20011
#define PACKET_TYPE_send_keyboard_event_list_from_client_to_server 20012
#define PACKET_TYPE_request_userlogin_list 20013
#define PACKET_TYPE_responce_userlogin_list 20014

#define PACKET_TYPE_request_userCLIPBOARD_list 20015
#define PACKET_TYPE_responce_userCLIPBOARD_list 20016

#define PACKET_TYPE_need_create_folder 20030
#define PACKET_TYPE_need_create_folder_commit 20031

#define PACKET_TYPE_transfer_file_ROUND_1 20032
#define PACKET_TYPE_transfer_file_ROUND_1_commit 20033
#define PACKET_TYPE_transfer_file_ROUND_2 20034
#define PACKET_TYPE_transfer_file_ROUND_2_commit 20035
#define PACKET_TYPE_transfer_file_ROUND_2_commit_finish 20036

#define PACKET_TYPE_transfer_file_ROUND_1_cancel_transfer 20037

#define PACKET_TYPE_need_delete_files_list 20040
#define PACKET_TYPE_need_delete_files_list_commit 20041
#define PACKET_TYPE_need_delete_files_list_intermediate 20042
#define PACKET_TYPE_need_delete_files_list_FINISH 20043
#define PACKET_TYPE_need_delete_files_list_CANCEL 20044

#define PACKET_TYPE_request_filefolder_stat 20045
#define PACKET_TYPE_responce_filefolder_stat 20046

#define PACKET_TYPE_request_filefolder_part 20047
#define PACKET_TYPE_responce_filefolder_part 20048

#define PACKET_TYPE_request_file_list_1 20017
#define PACKET_TYPE_responce_file_list_1 20018

#define PACKET_TYPE_request_file_list_2 20049
#define PACKET_TYPE_responce_file_list_2 20050

#define PACKET_TYPE_TRANSFER_from_SRV_to_CLI_CANCELED 20051

#define filefolder_part_buf_max_size 150000


#define ROUND_1_RESULT_OK_target_file_created 1
#define ROUND_1_RESULT_ERR_target_file_already_exists 2
#define ROUND_1_RESULT_ERR_fatal 3
#define ROUND_1_RESULT_OK_target_file_finish_ZEROSIZE 4


// 20052 already used
// 20053 already used
// 20054 already used
// 20055 already used

struct PACKET_LEVEL0
{
	unsigned int zero;
	unsigned int crc32;
	unsigned int sol;
	unsigned int packet_type;
	unsigned int sol2;
	unsigned char body[108];
};
struct PACKET_LEVEL1_1002_request // get_proxy_ip
{
	unsigned long long sub_type; // 101 - ?????? ip ??????? ??? ??????????? ?????? ????????
						   // 102 - ?????? ip ??????? ??? ??????????? ???????
						   // 103 - ?????? ip ??????? ?? ??????? ??????? ?????? ?????????? ????????
	unsigned long long im_public_id;
	unsigned long long im_private_id;
	unsigned char AES_pass[32];
	unsigned char body[52];
};
struct PACKET_LEVEL1_1002_responce // get_proxy_ip
{
	unsigned int  sub_type;
	unsigned int  ip4;
	
	unsigned char body[100];
};
struct PACKET_LEVEL1_1003_request // register neww partner
{
	unsigned char AES_pass[32];
	unsigned char body[76];
};
struct PACKET_LEVEL1_1003_responce // register neww partner
{
	unsigned long long public_id;
	unsigned long long private_id;
	unsigned char body[92];
};
struct PACKET_LEVEL1_1004_request // server connecting
{
	unsigned long long my_public_id;
	unsigned long long my_private_id;
	unsigned long long server_ver;
	unsigned char AES_passs[32];
	unsigned char body[52];
};
struct PACKET_LEVEL1_1004_responce // server connecting
{
	unsigned long long result;
	unsigned int  key;
	unsigned int  current_visiator_exe_ver;
	unsigned char body[92];
};
struct PACKET_LEVEL1_1005_request // client connecting
{
	unsigned long long client_public_id;
	unsigned long long client_private_id;
	unsigned long long client_ver;
	unsigned long long partner_public_id;
	unsigned char AES_passs[32];
	unsigned char body[44];
};
struct PACKET_LEVEL1_1005_responce // client connecting
{
	unsigned long long result;
	unsigned int  key;
	unsigned int  srv_ver;
	unsigned char body[92];
};
struct PACKET_LEVEL1_1006_responce // server connecting
{
	unsigned long long result;
	unsigned int  sol;
	unsigned char body[96];
};
struct PACKET_LEVEL1_1006_request // server connecting
{
	unsigned int key;
	unsigned int sol;
	unsigned char body[100];
};
struct PACKET_LEVEL1_1007_request // decode pass to attributes
{
	unsigned char pass_unic_id[16];
	unsigned char pin[4];
	unsigned char pin2[4];
	unsigned int  get_or_change; // 1 - check pin. 2 - change pin
	unsigned int  sol;
	unsigned char AES_pass[32];
	unsigned char body[44];


	/*unsigned char pass_unic_id[16];
	unsigned char pin[4];
	unsigned int  sol;
	unsigned char AES_pass[32];
	unsigned char body[52];*/
};
struct PACKET_LEVEL1_1007_responce
{
	unsigned char pass_to_decode_attr_part1[8];
	unsigned int  sol1;
	unsigned int  sol2;
	unsigned char pass_to_decode_attr_part2[8];
	unsigned int  result;
	unsigned int  is_prem;
	unsigned char body[72];
};
struct PACKET_LEVEL1_1008_request // get exe file
{
	unsigned int file_type; // 201 - visiator.exe 
	unsigned int fragment_idx;
	unsigned long long im_public_id;
	unsigned long long im_private_id;
	unsigned char AES_pass[32];
	unsigned int  sol;
	unsigned char body[52];

};
struct PACKET_LEVEL1_1008_responce {
	unsigned int sub_type;
	unsigned int fragment_idx;
	unsigned int fragment_size;
	unsigned int fragment_crc;
	unsigned int file_ver;
	unsigned char body[1088];
};


class NET_SERVER_SESSION
{
public:
	HIGHLEVEL_COMMAND_QUEUE out_queue_command;
	MULTIDISPLAY multidisplay;

	unsigned long long PUBLIC_ID = 0, PRIVATE_ID = 0;
	unsigned char PASS_ENCODED[32];
	MY_AES aes_partner;

	SCREEN_LIGHT_encoded_8bit_first   *screen_encoded_8bit_first  = nullptr;
	SCREEN_LIGHT_encoded_8bit_second  *screen_encoded_8bit_second = nullptr;

	SCREEN_LIGHT_one_byte *screen_one_byte_ = nullptr;
	
	SCREEN_LIGHT_12bit *screen_12bit = nullptr;
	SCREEN_LIGHT_encoded_12bit *screen_encoded_12bit = nullptr;

	unsigned char *llow_level_buffer_read = nullptr;
	unsigned int   llow_level_buffer_read_max_count = 0;
	unsigned int    low_level_buffer_read_idx = 0;
	void  increase_llow_level_buffer_read(int add_size);

	unsigned char *llow_level_encoded_buffer_read = nullptr;
	unsigned int    low_level_encoded_buffer_read_idx = 0;
	unsigned int   llow_level_encoded_buffer_read_max_count = 0;
	void  increase_llow_level_encoded_buffer_read(int add_size);


	char  dummy_packet[15];
	int   dummy_packet_size = 15;
	unsigned int ip_to_server_connect = 0;
	int last_set_mouse_x = 0, last_set_mouse_y = 0;

	boost::thread* thread_EXECUTE = nullptr;
	bool EXECUTE_is_run = false;
	void EXECUTE();

	void RUN(unsigned long long PUBLIC_ID_, unsigned long long PRIVATE_ID_, unsigned char *PASS_ENCODED_);

	void Connect_to_proxy_as_server(unsigned long long public_id, unsigned long long private_id, unsigned int ip_to_server_connect, unsigned char pass_hash16[16]);
	bool main_loop_is_strated = false;
	void NetSession_Main_Loop(SOCKET sos);
	void SEND_SCREEN_FROM_SERVER_TO_CLIENT_8bit_first(MASTER_AGENT_PACKET_HEADER *w_buf, MASTER_AGENT_PACKET_HEADER *r_buf, ENCODED_SCREEN_8bit_header *scr_head_buf);
	void SEND_SCREEN_FROM_SERVER_TO_CLIENT_12bit_first(MASTER_AGENT_PACKET_HEADER *w_buf, MASTER_AGENT_PACKET_HEADER *r_buf, ENCODED_SCREEN_12bit_header *scr_head_buf);
	void SEND_SCREEN_FROM_SERVER_TO_CLIENT_8bit_second(MASTER_AGENT_PACKET_HEADER *w_buf, MASTER_AGENT_PACKET_HEADER *r_buf, ENCODED_SCREEN_8bit_header *scr_head_buf);

	

	int  READ(byte *buffer, int buffer_size);
	void add_to_low_level_encoded_buffer(unsigned char *buf, unsigned int size);
	void add_to_low_level_buffer(unsigned char *buf, int size);
	bool detect_full_command_in_to_low_level_buffer();
	void analiz_command(unsigned char *buf);
	void RECEIVE_KEYBOARD_EVENT_LIST_from_client_to_server(unsigned char *buf);
	void send_event_in_to_session(int session_no, unsigned int event_type, int global_type, unsigned long long msg, unsigned long long wparam, unsigned long long lparam);

	void clean_();

	void unpress_all_pressed_keys();
	bool ss_need_disconnect = false;
	unsigned int need_start_screenflow_count = 0;
	bool need_start_screenflow = false;
	unsigned int need_start_screenflow_FORMAT_VER = 0;
	int responce_screen_in_queue = 0;

	unsigned long long recv_counter = 0, send_counter = 0;

	//**********************************************************

	unsigned char *filefolder_part_buf = nullptr;
	HANDLE       last_filefolder = 0;
	unsigned int last_filefolder_start_from = 0;
	unsigned long long filefolder_size = 0;
	void request_filefolder_stat(unsigned char *buf);
	void request_filefolder_part(unsigned char *in_buf);

	void request_folder_content(unsigned char *buf);

	void TRANSFER_from_SRV_to_CLI_CANCELED();

	//**
	//************************************************************************************************

	//**********************************************************
	FILE__LIST  FL_1;
	wchar_t need_send_file_list_FOLDER_1[5100];
	bool need_send_file_list_1 = false;

	FILE__LIST  FL_2;
	wchar_t need_send_file_list_FOLDER_2[5100];
	bool need_send_file_list_2 = false;
	//**********************************************************

	void SEND_file_list_FROM_SERVER_to_CLIENT_1(wchar_t *need_request_files_list_DIR);
	void SEND_file_list_FROM_SERVER_to_CLIENT_2(wchar_t *need_request_files_list_DIR);

	//************************************************************************************************
	//**
	wchar_t *target_file_full_name = nullptr;
	//wchar_t *tmp_file_full_name = nullptr;
	HANDLE hFILE = 0;
	DWORD  hFILE_time = 0; // GetTickCount() ????? ??????? ????????? ????, ????? ??? TimeOut
	unsigned long long  hFILE_writed_size = 0;
	unsigned long long  target_file_size = 0;
	unsigned long long  target_file_date = 0;
	bool TRANSFER_FILE_ROUND_1(unsigned char *buf);
	bool TRANSFER_FILE_ROUND_2(unsigned char *buf);
	void TRANSFER_FILE_ROUND_cancel_transfer();
	void FinishFile();
	bool NEED_CREATE_FOLDER(unsigned char *buf);
	//************************************************************************************************
	//**
	int NET_SERVER_SESSION_delete_status = 0;
	int files_list_for_delete_size = 0;
	wchar_t *files_list_for_delete = nullptr;
	bool need_delete_files_list = false;
	bool need_delete_files_list_CANCEL = false;
	unsigned int files_list_for_delete_file_ID = 0;

	void DELETE_FILE_LIST_cancel();
	void DELETE_FILE_LIST(wchar_t *w, int sz, unsigned int file_ID);

	bool delete_file_or_folder(wchar_t *name, int sz);
	void delete_file_or_folder_send_intermediate(int val); // ????????? ????????????? ??????????. "?? ?? ??????????. ??????? ????."
	void delete_file_or_folder_send_FINISH();
	unsigned int deleted_dirsfiles_count = 0;
	bool my_deletedir(wchar_t *path, wchar_t *name);
	bool my_deletefile(wchar_t *path, wchar_t *name);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	NET_SERVER_SESSION();
	~NET_SERVER_SESSION();
};

