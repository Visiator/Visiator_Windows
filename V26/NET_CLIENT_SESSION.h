#pragma once

#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "GUI_low_level.h"
#include "tools.h"
#include "CRYPTO.h"
#include "EVENT_QUEUE.h"

#define CLIENT_RAUND_1_result_OK 1 
#define CLIENT_RAUND_1_result_SKIP 2 
#define CLIENT_RAUND_1_result_OVERWRITE 3
#define CLIENT_RAUND_1_result_CANCEL 4
#define CLIENT_RAUND_1_result_FATAL_ERROR 5
#define CLIENT_RAUND_1_result_OK_finish_ZEROSIZE 6

class PERFOMANCE_COUNTER
{
private:
	int max_count = 300;
	int values[2][300]; // в одном элемента значение скорости, в другом жлементе отметка времени

public:
	DWORD last_time = 0;
	void add(int val);
	int get_per_sec();

	void clean();

	PERFOMANCE_COUNTER();
	~PERFOMANCE_COUNTER();
};


class NET_CLIENT_SESSION
{
public:
	PERFOMANCE_COUNTER perf_counter;

	uint64_t my_pub_id = 0, my_priv_id = 0;
	unsigned int SERVER_VER = 0;

	std::wstring status;
	void set_status(wchar_t *v);
	GUI_low_level *parent_low_level = nullptr;
	MY_AES aes_partner;
	uint64_t recv__counter = 0, send__countern = 0;

	bool need_start_sendCLIPBOARD_to_server = false;
	bool need_start_requestCLIPBOARD_from_server = false;
	int  need_set_mouse_x = -1, need_set_mouse_y = -1;

	DWORD recv_responce_screen = 0;
	bool need_send_ALL_KEY_UP_event = false;

	bool need_send_TRANSFER_CANCELED = false;

	bool partner_pass_and_id_is_set = false;
	uint64_t partner_id = 0;
	unsigned char  partner_pass_encripted[32];
	DWORD recv__counter__time = 0, send__counter__time = 0;

	EVENT_QUEUE out_event_queue;

	bool connection_to_partner_established = false;
	bool need_start_screenflow_from_server = false;
	unsigned int need_start_screenflow_from_server_FORMAT_VER = 0;

	unsigned char *llow_level_encoded_buffer_read = NULL;
	unsigned int    low_level_encoded_buffer_read_idx = 0;
	unsigned int   llow_level_encoded_buffer_read_max_count = 0;
	void  increase_llow_level_encoded_buffer_read(int add_size);

	unsigned char *llow_level_buffer_read = NULL;
	unsigned int   llow_level_buffer_read_max_count = 0;
	unsigned int    low_level_buffer_read_idx = 0;
	void  increase_llow_level_buffer_read(int add_size);

	int  Client_Main_Loop(SOCKET sos);
	int  READ(byte *buffer, int buffer_size);
	void add_to_low_level_encoded_buffer(unsigned char *buf, unsigned int size);

	bool  detect_full_command_in_to_low_level_buffer();
	void  analiz_command(unsigned char *buf);

	void set_partner_pass_and_id(uint64_t partner_id_, uint8_t *partner_pass_encripted_len32);

	boost::thread* thread_EXECUTE = nullptr;
	bool EXECUTE_is_run = false;
	void EXECUTE();

	int  Connect_to_server( unsigned long long partner_id, unsigned long long public_id, unsigned long long private_id, unsigned char pass_hash16[16], unsigned int proxy_ip);

	void(*parent_func__arrived_screen)(unsigned char *buf, int buf_size, unsigned int _FORMAT_VER) = nullptr;
	void(*parent_func__connect)(void) = nullptr;
	void(*parent_func__disconnect)(void) = nullptr;

	void get_statistic(char *ss, int ss_max_size_);

	void mouse_move(GUI_low_level *low_level, int mx, int my);
	void mouse_whell(GUI_low_level *low_level, int msg, int wp, int lp);

	void mouse_left_button_down(GUI_low_level *low_level, int mx, int my);
	void mouse_left_button_up(GUI_low_level *low_level, int mx, int my);
	void mouse_right_button_down(GUI_low_level *low_level, int mx, int my);
	void mouse_right_button_up(GUI_low_level *low_level, int mx, int my);

	void send_CtrlAltDel();
	void send_Change_LNG();
	
	void char_keydown(GUI_low_level *low_level, int msg, int wp, int lp);
	void char_keyup(GUI_low_level *low_level, int msg, int wp, int lp);

	

	void add_to_low_level_buffer(unsigned char *buf, int size);
	DWORD last_set_need_set_mouse = 0;
	unsigned int last_commit_scr_id = -1;
	//**************************************************************
	bool    need_request_files_list = false;
	wchar_t need_request_files_list_DIR[5100];
	bool    need_request_FilesList(wchar_t *folder_name);
	//**************************************************************

	//*****************************************************************************************
	bool buf_tdn_need_send = false;
	unsigned char *buf_tdn = nullptr;
	unsigned int buf_tdn_max_size = 0;
	unsigned int buf_tdn_len = 0;

	int responce_from_partner_arrived = 0;
	unsigned int responce_from_partner_part_szie = 0;
	int responce_from_partner_result = 0;
	unsigned int responce_from_partner_FILE_ID = 0;
	int responce_from_partner_intermediate = 0;
	unsigned long long responce_from_partner_FILE_size = 0, responce_from_partner_FILE_date = 0;
	bool transfer_DIRECTORY_to_parnter(wchar_t *dest_target, wchar_t *local_path, wchar_t *name);
	bool transfer_DIRECTORY_NAME(wchar_t *full_name, unsigned int file_ID);

	bool transfer_files_canceled = false;

	unsigned char *transfer_file_buffer = nullptr;
	unsigned int            transfer_file_buffer_max_size = 500000;
	int            transfer_file_buffer_len = 0;
	bool           transfer_file_buffer_need_send = false;
	int            transfer_file_buffer_commit_send_from_partner = false;
	bool transfer_FILE_to_parnter(wchar_t *dest_target, wchar_t *local_path, wchar_t *name, bool overwrite_this_file);

	int  transfer_FILE_to_parnter_RAUND_1(wchar_t *dest_full_name, unsigned long long FileSize, unsigned long long LastWriteTime, unsigned int file_ID, unsigned int allow_overwrite);
	bool transfer_FILE_to_parnter_RAUND_2(unsigned char *transfer_file_buffer, int len, int kusok_idx, unsigned int file_ID);
	void transfer_FILE_to_parnter_RAUND_cancel_transfer(unsigned int file_ID);
	//*****************************************************************************************

	unsigned int request_filefolder_stat(wchar_t *partner_name_, unsigned int *_is_file_folder, unsigned long long *_size, unsigned long long *_date, unsigned int file_ID, int *modal_result); // запросим у партнера информацию о файле/папке размер, дата, тд
	bool request_file_part(unsigned int file_ID, unsigned char *buf, unsigned int buf_size, unsigned int *buf_size_readed, unsigned int start_from, int *modal_result);
	bool request_folder_content(unsigned int file_ID, wchar_t *partner_folder_name_, int *modal_result);


	bool need_send_delete_cancel = false;
	void delete_files_list_from_partner(unsigned char *buf, unsigned int buf_len);


	NET_CLIENT_SESSION();
	~NET_CLIENT_SESSION();
};

