#include "stdafx.h"

#include "NET_SERVER_SESSION.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "tools.h"
#include "CRYPTO.h"
#include "SERVICE.h"
#include "KERNEL.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;
extern SERVICE *service;
extern KEY_PRESSED key_pressed;
extern KERNEL kernel;

NET_SERVER_SESSION::NET_SERVER_SESSION() {
	for (int i = 0; i < 15; i++) dummy_packet[i] = (char)i;
	dummy_packet_size = 15;

}

NET_SERVER_SESSION::~NET_SERVER_SESSION() {

	if (llow_level_buffer_read != NULL) delete[] llow_level_buffer_read;
	llow_level_buffer_read_max_count = 0;
	low_level_buffer_read_idx = 0;


	if (llow_level_encoded_buffer_read != NULL) delete[] llow_level_encoded_buffer_read;
	llow_level_encoded_buffer_read_max_count = 0;
	low_level_encoded_buffer_read_idx = 0;


	delete[] screen_encoded_8bit_first;
	delete[] screen_one_byte_;

}

void NET_SERVER_SESSION::RUN(unsigned long long PUBLIC_ID_, unsigned long long PRIVATE_ID_, unsigned char *PASS_ENCODED_) {

	PUBLIC_ID = PUBLIC_ID_;
	PRIVATE_ID = PRIVATE_ID_;
	for (int i = 0; i < 32; i++) {
		PASS_ENCODED[i] = PASS_ENCODED_[i];
	}

	thread_EXECUTE = app_attributes.tgroup.create_thread(boost::bind(&NET_SERVER_SESSION::EXECUTE, this));
}

void NET_SERVER_SESSION::EXECUTE() {
	
	EXECUTE_is_run = true;

	ip_to_server_connect;


	boost::posix_time::milliseconds SleepTime(1000);

	while (GLOBAL_STOP == false) {

		sudp("NET_SERVER_SESSION::EXECUTE()");

		ip_to_server_connect = 0;

		if (PRIVATE_ID != 0) {
			if (ip_to_server_connect == 0) {

				ip_to_server_connect = GetFromProxy_ip_to_server_connect(PUBLIC_ID, PRIVATE_ID);

				if (ip_to_server_connect == 0) { // не удалось получить от прокси ip для поключения в качестве сервера. нет инета?
					sudp("NET_SERVER_SESSION::EXECUTE() proxy ip not get ");

					app_attributes.proxy_list->Load_ProxySettings();
					app_attributes.proxy_list->save_in_registry(HKEY_CURRENT_USER, L"Software\\Visiator");


					if (GLOBAL_STOP == false) my_Slip(5000);
				}
			}

		}

		if (PRIVATE_ID != 0 && ip_to_server_connect != 0) {
			//sudp("NET_SERVER_SESSION::EXECUTE() connect to proxy...");
			Connect_to_proxy_as_server(PUBLIC_ID, PRIVATE_ID, ip_to_server_connect, PASS_ENCODED);
		}


		if (GLOBAL_STOP == false) my_Slip(3000);


		boost::this_thread::sleep(SleepTime);
	}

	EXECUTE_is_run = false;
}

void NET_SERVER_SESSION::increase_llow_level_buffer_read(int add_size) {
	unsigned char *q;
	unsigned int ss;
	unsigned int i;
	ss = llow_level_buffer_read_max_count + add_size;

	if (ss > 5000000) {
		fatal_error("NET_CLIENT_SESSION::increase_llow_level_buffer_read() ss > 5000000");
		return;
	}

	q = new unsigned char[ss];

	for (i = 0; i < llow_level_buffer_read_max_count; i++) {
		q[i] = llow_level_buffer_read[i];
	}
	while (i < ss) {
		q[i] = 0;
		i++;
	}
	if (llow_level_buffer_read != NULL) delete[] llow_level_buffer_read;
	llow_level_buffer_read = q;
	llow_level_buffer_read_max_count = ss;
}

void NET_SERVER_SESSION::increase_llow_level_encoded_buffer_read(int add_size) {
	unsigned char *q;
	unsigned int ss;
	unsigned int i;
	ss = llow_level_encoded_buffer_read_max_count + add_size;

	if (ss > 5000000) {
		fatal_error("NET_CLIENT_SESSION::increase_llow_level_encoded_buffer_read() ss > 5000000");
		return;
	}

	q = new unsigned char[ss];
	for (i = 0; i < llow_level_encoded_buffer_read_max_count; i++) {
		q[i] = llow_level_encoded_buffer_read[i];
	}
	while (i < ss) {
		q[i] = 0;
		i++;
	}
	if (llow_level_encoded_buffer_read != NULL) delete[] llow_level_encoded_buffer_read;
	llow_level_encoded_buffer_read = q;
	llow_level_encoded_buffer_read_max_count = ss;
}

void NET_SERVER_SESSION::Connect_to_proxy_as_server(unsigned long long public_id, unsigned long long private_id, unsigned int ip_to_server_connect, unsigned char pass_hash16[16]) {

	FILE *f;
	fopen_s(&f, "c:\\1\\NSS.txt", "ab");
	if (f != NULL) {
		fprintf(f, "start %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\r\n"
			, pass_hash16[0], pass_hash16[1], pass_hash16[2], pass_hash16[3]
			, pass_hash16[0], pass_hash16[1], pass_hash16[2], pass_hash16[3]
			, pass_hash16[0], pass_hash16[1], pass_hash16[2], pass_hash16[3]
			, pass_hash16[0], pass_hash16[1], pass_hash16[2], pass_hash16[3]
			);
		fclose(f);
	};

	if (public_id == 0 || private_id == 0 || ip_to_server_connect == 0) {
		// TODO save log
		sudp("Connect_to_proxy_as_server() err 1");
		return;
	}
	// TODO check - pass_hash is empty?

	unsigned int crc32;
	unsigned char xx[500], bb[500];
	PACKET_LEVEL0 *r0;
	PACKET_LEVEL1_1004_responce *r1004;
	PACKET_LEVEL1_1006_responce *r1006;


	PACKET_LEVEL0 *p0;
	PACKET_LEVEL1_1004_request *p1004;
	PACKET_LEVEL1_1006_request *p1006;




	SOCKET sos;
	sockaddr_in dest_addr;
	DWORD tmout;
	MY_AES aes;
	MY_CRC crc;
	MY_RSA rsaa;
	rsaa.init();

	//hexstr_to_char16((char *)pass_hash, bb);

	aes_partner.set_key_16_byte(pass_hash16, "NSS Connect_to_proxy_as_server 1-aes_partner");


	int res, snd;

	if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
	{ //3
		//status = "create socket error";
		sudp("Connect_to_proxy_as_server() err 2");
		return;
	} //3

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	dest_addr.sin_addr.S_un.S_addr = ip_to_server_connect;// proxy_address );

	PROXY_LIST *proxy_list;




	//status = "connect...";
	if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+
	{//4
		closesocket(sos);
		sos = 0;
		proxy_list = app_attributes.proxy_list;
		if (proxy_list != NULL) {
			sos = proxy_list->try_connect(ip_to_server_connect); // пробуем подключиться через прокси +
		};
		if (sos == 0) {
			closesocket(sos);
			sudp("Connect_to_proxy_as_server() err 3");
			return;
		};
	}//4



	setnonblocking(sos);

	zero_unsigned_char(bb, 128);

	p0 = (PACKET_LEVEL0 *)bb;
	p0->packet_type = 1004;
	p1004 = (PACKET_LEVEL1_1004_request *)p0->body;
	p1004->my_private_id = private_id;
	p1004->my_public_id = public_id;
	p1004->server_ver = app_attributes.VER;
	//for (int i = 0; i < 16; i++) p1004->AES_passs[i] = '0' + i;
	my_random(p1004->AES_passs, 16);

	aes.set_key_16_byte(p1004->AES_passs, "NSS Connect_to_proxy_as_server 2 aes");

	p0->crc32 = crc.calc(&bb[8], 120);

	rsaa.encode_128_byte(bb, xx);

	unsigned long long rc = 0;

	snd = 0;
	do
	{
		snd = my_send( sos, (unsigned char *)xx, 128, 0, "", &rc);//ok
	} while (snd != 128);


	do
	{
		res = my_recv(sos, xx, 128, &rc);
	} while (res != 128 && res != -1);
	aes.decrypt_128_byte(xx);


	crc32 = crc.calc(&xx[8], 120);


	r0 = (PACKET_LEVEL0 *)xx;
	if (r0->crc32 == crc32)
	{
		r1004 = (PACKET_LEVEL1_1004_responce *)r0->body;
		if (r1004->result == 1011) { // Ok

			//app_attributes.SET_current_visiator_exe_ver_on_proxy(r1004->current_visiator_exe_ver);

			zero_unsigned_char(bb, 128);

			p0 = (PACKET_LEVEL0 *)bb;
			p0->packet_type = 1006;

			p1006 = (PACKET_LEVEL1_1006_request *)p0->body;
			p1006->key = r1004->key;
			p1006->sol = 0; // get sol

			aes.encrypt_16_byte((unsigned char *)p1006);
			
			tmout = GetTickCount();
			snd = 0;
			do
			{
				snd = my_send(sos, (unsigned char *)p1006, 16, 0, "", &rc);//ok
				if (res == 0 && tmout + 3000 < GetTickCount()) res = -1;
			} while (snd != 16 && snd != -1);

			if (snd == 16) {
				
				tmout = GetTickCount();
				do
				{
					res = my_recv(sos, xx, 128, &rc);
					if (res == 0 && tmout + 3000 < GetTickCount()) {
						res = -1;
						sudp("NSS timeout (1)");
					};
				} while (res != 16 && res != -1);
				aes.decrypt_16_byte(xx);

				if (res == 16) {

					r1006 = (PACKET_LEVEL1_1006_responce *)xx;

					if (r1006->result == 1022) {
						sudp("Connect_to_proxy_as_server() NetSession_Main_Loop()...");

						try {
							sudp("NSS start Main_Loop (1)");
							NetSession_Main_Loop(sos);
							need_start_screenflow_count = 0;
							sudp("NSS end   Main_Loop (1)");
						}
						catch (...) {
							// crash_log("NET_SERVER_SESSION::Connect_to_proxy_as_server() NetSession_Main_Loop(sos)");
						}
						sudp("Connect_to_proxy_as_server() NetSession_Main_Loop() end");
						snd = 222;
					}
				};
			};

		};


	}
	else {
		//status = "crc err";
		sudp("crc err");
	};
	struct linger lng = { 1, 0 };
	setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

	int ee;
	ee = shutdown(sos, SD_BOTH);
	closesocket(sos);

	clean_();

	unpress_all_pressed_keys();

	sudp("Connect_to_proxy_as_server() finish");
}

void NET_SERVER_SESSION::clean_() {

	//commit_from_client_LAST_SCREEN_ID = 0;

	ip_to_server_connect = 0;

	last_set_mouse_x = 0;
	last_set_mouse_y = 0;

	ss_need_disconnect = false;
	need_start_screenflow = false;
	need_start_screenflow_count = 0;

	low_level_buffer_read_idx = 0;
	low_level_encoded_buffer_read_idx = 0;

	recv_counter = 0;
	send_counter = 0;

	if (screen_encoded_8bit_first != nullptr) screen_encoded_8bit_first->clean_();
	if (screen_one_byte_ != nullptr) screen_one_byte_->clean_();
	
	//in_queue_command.clean();
	out_queue_command.clean();

	// 2019 11 TODO if (screen_for_client != NULL) screen_for_client->Clean();

	responce_screen_in_queue = 0;

	// 2019 11 TODO local_stop = false;

}

void NET_SERVER_SESSION::unpress_all_pressed_keys() {

	if (service != NULL) {
		service->send_unpress_all_pressed_keys(); // 2
	}
	else {
		if (app_attributes.is_desktop) {
			key_pressed.unpress_all_pressed_keys();
		}
	};
}

void NET_SERVER_SESSION::NetSession_Main_Loop(SOCKET sos) {

	/*MASTER_AGENT_PACKET_HEADER w_buf;
	MASTER_AGENT_PACKET_HEADER r_buf;
	ENCODED_SCREEN_8bit_header scr_head_buf;*/

	//char ss[100];
	//sprintf_ s(ss, 90, " recv_counter=%lld ", recv_counter);

	if (screen_encoded_8bit_first != nullptr) screen_encoded_8bit_first->clean_();
	if (screen_encoded_8bit_second != nullptr) screen_encoded_8bit_second->clean_();
	if (screen_encoded_12bit != nullptr) screen_encoded_12bit->clean_();
	if (screen_12bit != nullptr) screen_12bit->clean_();

	unsigned char *w_buf, *r_buf, *scr_head_buf;
	w_buf = new unsigned char[512];
	r_buf = new unsigned char[512];
	scr_head_buf = new unsigned char[512];

	int sz;

	sz = sizeof(w_buf);

	sz = sizeof(MASTER_AGENT_PACKET_HEADER);


	sz = sizeof(scr_head_buf);

	sz = sizeof(ENCODED_SCREEN_8bit_header);

	// start Async Thread
	unsigned char *bb;
	bb = new unsigned char[500000]; //neew(50000, bb, "NET_SERVER_SESSION::NetSession_Main_Loop()");

	int res;
	bool local_stop;

	local_stop = false;

	main_loop_is_strated = true;

	need_start_screenflow_count = 0;

	//DWORD LastSendScreen_into_Client = 0;
	DWORD last_send_heartbeat, tc;
	last_send_heartbeat = GetTickCount() + 3000;

	boost::posix_time::milliseconds SleepTime(10);

	int forced_send_screen;
	forced_send_screen = 0;
	while (GLOBAL_STOP == false && local_stop == false && ss_need_disconnect == false) {

		//send_udp2("1");

		res = my_recv(sos, bb, 500000, &recv_counter);
		if (res > 0) { // arrived

			//recv_counter += res;
			//sudp("READ");
			READ(bb, res);

		}
		else {
			boost::this_thread::sleep(SleepTime);
		}
		if (res < 0) { // disconnect 
			sudp("local_stop = true (1)");
			local_stop = true;
		}

		
		if (need_send_file_list_1 == true) {
			//send_udp2("[[0000]] DETECT  need_send_file_list_1 == true");
			SEND_file_list_FROM_SERVER_to_CLIENT_1(need_send_file_list_FOLDER_1);
			//send_udp2("[[6666]] need_send_file_list_1 = false");
			need_send_file_list_1 = false;
		}
		if (need_send_file_list_2 == true) {
			//send_udp2(L"[[0000]] DETECT  need_send_file_list_2 == true ", need_send_file_list_FOLDER_2);

			SEND_file_list_FROM_SERVER_to_CLIENT_2(need_send_file_list_FOLDER_2);

			//send_udp2("[[6666]] need_send_file_list_2 = false");
			need_send_file_list_2 = false;
		}
		/*** 2021 if (need_request_CLIPBOARD == true) {
			need_request_CLIPBOARD = false;

			try {

				SEND_CLIPBOARD_FROM_SERVER_to_CLIENT();
			}
			catch (...) {
				crash_log("NET_SERVER_SESSION::NetSession_Main_Loop() SEND_CLIPBOARD_FROM_SERVER_to_CLIENT()");
			}
			//send_udp2("SERVICE::get_clipboard_from_session [[7]]");
		};***/
		

		if (need_start_screenflow == true) {
			//sudp("need_start_screenflow == true");

			if (responce_screen_in_queue == 0) {

				if (need_start_screenflow_FORMAT_VER == PACKET_TYPE_request_start_screenflow_ver11) {
					SEND_SCREEN_FROM_SERVER_TO_CLIENT_8bit_first((MASTER_AGENT_PACKET_HEADER *)w_buf, (MASTER_AGENT_PACKET_HEADER *)r_buf, (ENCODED_SCREEN_8bit_header *)scr_head_buf);
				}
				if (need_start_screenflow_FORMAT_VER == PACKET_TYPE_request_start_screenflow_ver22) {
					SEND_SCREEN_FROM_SERVER_TO_CLIENT_12bit_first((MASTER_AGENT_PACKET_HEADER *)w_buf, (MASTER_AGENT_PACKET_HEADER *)r_buf, (ENCODED_SCREEN_12bit_header *)scr_head_buf);
				};
				if (need_start_screenflow_FORMAT_VER == PACKET_TYPE_request_start_screenflow_ver33) {
					// SEND_SCREEN_FROM_SERVER_TO_CLIENT_8bit_second((MASTER_AGENT_PACKET_HEADER *)w_buf, (MASTER_AGENT_PACKET_HEADER *)r_buf, (ENCODED_SCREEN_12bit_header *)scr_head_buf);
					SEND_SCREEN_FROM_SERVER_TO_CLIENT_8bit_second((MASTER_AGENT_PACKET_HEADER *)w_buf, (MASTER_AGENT_PACKET_HEADER *)r_buf, (ENCODED_SCREEN_8bit_header *)scr_head_buf);
				};
			}
			forced_send_screen = 1;
		}

		////////////////////////////////////////
		int ss, tt, info, cccc;

		tc = GetTickCount();
		if (tc > last_send_heartbeat + 3000 || tc < last_send_heartbeat) {
			last_send_heartbeat = tc - 1;

			if (recv_counter == 0) {
				//send_udp("send_dummy");
				ss = my_send(sos, (unsigned char *)dummy_packet, dummy_packet_size, 0, "dummy_packet", &send_counter); // TODO
			}


		}


		unsigned char *command_data;
		unsigned int   command_data_size;
		command_data = NULL;

		ss = 0;
		tt = 0;

		cccc = 0;

		//char a[300];

		//sprintf_ s(a, 90, "idx=%d 100", idx);
		//send_udp2(a);

		while (local_stop == false && out_queue_command.get_element_low_level(tt, info, command_data, command_data_size)) {
			cccc++;

			/*if (tt == PACKET_TYPE_responce_userCLIPBOARD_list) {
				send_udp2("get_element_low_level() PACKET_TYPE_responce_userCLIPBOARD_list DETECT ", command_data_size);
			}*/

			unsigned int send_commit;
			send_commit = 0;
			int so;
			so = 0;

			if (tt == PACKET_TYPE_responce_screen_ver11 ||
				tt == PACKET_TYPE_responce_screen_ver22 ||
				tt == PACKET_TYPE_responce_screen_ver33

				) {
				//sudp("sss begin");

			};

			while (local_stop == false && send_commit < command_data_size) {

				/*if (tt == PACKET_TYPE_responce_userCLIPBOARD_list) {
					send_udp2("get_element_low_level() PACKET_TYPE_responce_userCLIPBOARD_list SEND... ", command_data_size - send_commit);
				}*/

				ss = my_send(sos, (unsigned char *)(command_data + send_commit), command_data_size - send_commit, 0, "work", &send_counter); // TODO
				if (ss == -1) {
					sudp("local_stop = true (2)");
					local_stop = true;
				}
				else
				{
					if (ss > 0) {
						send_commit += ss;
						send_counter += ss;
					}
					else {
						//Sleep(0);
					}
				}

				/*if (tt == PACKET_TYPE_responce_userCLIPBOARD_list) {
					send_udp2("get_element_low_level() PACKET_TYPE_responce_userCLIPBOARD_list SEND commit ", send_commit);
				}*/

			};

			if (tt == PACKET_TYPE_responce_screen_ver11 ||
				tt == PACKET_TYPE_responce_screen_ver22 ||
				tt == PACKET_TYPE_responce_screen_ver33

				) {
				//sudp("sss end");

				if (responce_screen_in_queue > 0) {
					enter_crit(20);
					responce_screen_in_queue--;
					leave_crit(20);
				};

			}
			delete[] command_data;
			//send_udp2( "del queue element ");
		}

		//sprintf_ s(a, 90, "idx=%d 1000", idx);
		//send_udp2(a);
	}

	/*** 2021 09
	if (hFILE != 0) {
		send_udp2("file delete");
		CloseHandle(hFILE);
		hFILE = 0;
		int i = 0;
		while (i < 5 && my_FileExists(target_file_full_name)) {
			if (i > 0) ::Sleep(500);
			DeleteFile(target_file_full_name);
			i++;
		};
		target_file_full_name[0] = 0;
	};

	if (last_filefolder != 0) {
		CloseHandle(last_filefolder);
		last_filefolder = 0;
		last_filefolder_start_from = 0;
		send_udp2("CloseHandle(last_filefolder) ~~~~~~~~~~~~~~ last_filefolder=0");
	}
	***/

	delete[] bb;
	main_loop_is_strated = false;

	// send_udp2("main_loop_is_strated = false (1)");

	// stop Async Thread

	delete[] w_buf;
	delete[] r_buf;
	delete[] scr_head_buf;

}

int jj = 0;


int  NET_SERVER_SESSION::READ(byte *buffer, int buffer_size) {

	add_to_low_level_encoded_buffer((unsigned char *)buffer, buffer_size);

	while (detect_full_command_in_to_low_level_buffer() == true) //  <<<<==================
	{

	}

	return 0;
}

void NET_SERVER_SESSION::add_to_low_level_encoded_buffer(unsigned char *buf, unsigned int size) {
	unsigned int i, j, jj;

	//ALOG("add_to_low_level_encoded_buffer size=%d ", size );

	if (low_level_encoded_buffer_read_idx + size + 500 > llow_level_encoded_buffer_read_max_count) {
		increase_llow_level_encoded_buffer_read(size + 500);
	}

	i = 0;
	while (i < size) {

		llow_level_encoded_buffer_read[low_level_encoded_buffer_read_idx++] = buf[i++];

		if (low_level_encoded_buffer_read_idx >= llow_level_encoded_buffer_read_max_count) {

			fatal_error("NET_SESSION::add_to_low_level_buffer fatal error");
			return;

		};
	};

	if (low_level_encoded_buffer_read_idx < 16) {

		return;
	};

	j = 0;
	while (j + 16 <= low_level_encoded_buffer_read_idx) {
		//for (int k = 0; k < 16; k++) low_level_encoded_buffer_read[j + k] = '*';
		aes_partner.decrypt_16_byte(llow_level_encoded_buffer_read + j);
		add_to_low_level_buffer(llow_level_encoded_buffer_read + j, 16);
		j += 16;
	}

	if (j == low_level_encoded_buffer_read_idx) {
		low_level_encoded_buffer_read_idx = 0;

		return;
	}

	i = 0;
	jj = j;
	while (j < low_level_encoded_buffer_read_idx) {
		llow_level_encoded_buffer_read[i++] = llow_level_encoded_buffer_read[j++];
	}
	low_level_encoded_buffer_read_idx -= jj;

}

void NET_SERVER_SESSION::add_to_low_level_buffer(unsigned char *buf, int size) {
	//ALOG("add_to_low_level_buffer size=%d ", size );

	//char ss[500];
	//sprintf_ s(ss, 450, "add_to_low_level_buffer size=%d _%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X_ ! ", size, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9]);
	//send_udp(ss);

	if (low_level_buffer_read_idx + size + 500 > llow_level_buffer_read_max_count) {
		increase_llow_level_buffer_read(size + 500);
	}

	int i;
	i = 0;
	while (i < size) {
		llow_level_buffer_read[low_level_buffer_read_idx++] = buf[i++];
		if (low_level_buffer_read_idx >= llow_level_buffer_read_max_count) {
			fatal_error("NET_SESSION::add_to_low_level_buffer fatal error");
			return;
		};
	};
}

bool NET_SERVER_SESSION::detect_full_command_in_to_low_level_buffer() {

	//ALOG("detect_full_command_in_to_low_level_buffer... %d ", low_level_buffer_read_idx);

	/*
	FILE *f;
	fopen_s( &f, "c:\\1\\detect.txt", "wb");
	if (f != NULL) {
		fprintf(f, "low_level_buffer_read_idx = %d\r\n", low_level_buffer_read_idx);
		unsigned int i;
		unsigned int *uu;
		i = 0;
		while (i < low_level_buffer_read_idx) {
			uu = (unsigned int *)&(low_level_buffer_read[i]);
			fprintf(f, "%08X ", *uu);
			i += 4;
		}
		fprintf(f, "\r\n");
		fclose(f);
	};
	*/
	unsigned int *sz, *crc;

	sz = (unsigned int *)&(llow_level_buffer_read[0]);

	crc = (unsigned int *)&(llow_level_buffer_read[4]);

	if (*sz > 33000000) {
		//ALOG("detect_full_command_in_to_low_level_buffer whf?? *sz > 3000000");
		//send_udp2("*sz = ", *sz);
		fatal_error("detect_full_command_in_to_low_level_buffer whf?? *sz > 3000000");
		sudp("ss_need_disconnect = true (1)");
		ss_need_disconnect = true;
		return false;
	}

	if (*sz > low_level_buffer_read_idx) {
		//if (MODE == MODE_server) set_status(L"not complete command ");
		//ALOG("not complete command");
		return false;
	}

	analiz_command(llow_level_buffer_read);  //  <<<< =================



	if (*sz == low_level_buffer_read_idx) {
		//ALOG("@@@@@@@@@@@@@    CLEAN ALL %d ", low_level_buffer_read_idx );
		low_level_buffer_read_idx = 0;
		//ALOG("2---");
		return false;
	}

	unsigned int i, j, jj;
	i = 0;
	j = *sz;
	jj = *sz;



	while (j < low_level_buffer_read_idx) {
		llow_level_buffer_read[i++] = llow_level_buffer_read[j++];
	}
	low_level_buffer_read_idx -= jj;

	//ALOG("@@@@@@@@@@@@@    REMOVE %d [%d] ", jj, low_level_buffer_read_idx );

	return true;
}

void NET_SERVER_SESSION::analiz_command(unsigned char *buf) {

	//2019+ ENCODED_SCREEN_8bit_header *header;

	unsigned int *sz, *crc, *type, *reserv3, *reserv4;// , *old_screen_id;

	sz = (unsigned int *)&(buf[0]);
	crc = (unsigned int *)&(buf[4]);
	type = (unsigned int *)&(buf[8]);

	/*char ss[500];
	sprintf_s(ss, 450, "analiz_command() sz=%d crc=%d type=%d ", (int)sz, (int)crc, (int)type);
	sudp(ss);*/

	if (*crc != 0) {
		/*c1 = CRC32_short((void *)&(buf[8]), (8 + 16));
		if (c1 != *crc) {
			ALOG("analiz_command() fatal_error");
			fatal_error("analiz_command error 1 crc");
			return;
		}*/
	};

	//ALOG("type = %X (%d) ", *type, *type );

	
	if (*type == PACKET_TYPE_TRANSFER_from_SRV_to_CLI_CANCELED) {
		//send_udp2(L"*type == PACKET_TYPE_TRANSFER_CANCELED");
		TRANSFER_from_SRV_to_CLI_CANCELED();

		return;
	}

	if (*type == PACKET_TYPE_request_filefolder_part) {
		//send_udp2(L"*type == PACKET_TYPE_request_filefolder_part");
		request_filefolder_part(buf);

		return;
	}

	if (*type == PACKET_TYPE_request_filefolder_stat) {
		//send_udp2(L"*type == PACKET_TYPE_request_filefolder_stat");
		request_filefolder_stat(buf);

		return;
	}

	if (*type == PACKET_TYPE_need_delete_files_list_CANCEL) {
		//send_udp2(L"*type == PACKET_TYPE_need_delete_files_list_CANCEL");
		DELETE_FILE_LIST_cancel();

		return;
	}

	if (*type == PACKET_TYPE_need_delete_files_list) {
		wchar_t *w;
		w = (wchar_t *)&buf[32];
		//send_udp2(L"PACKET_TYPE_need_delete_files_list ", w);
		//send_udp2("*sz - 32 = ", *sz - 32);
		unsigned int file_ID, *ii;

		ii = (unsigned int *)&buf[16];
		file_ID = *ii;

		DELETE_FILE_LIST(w, *sz - 32, file_ID);

		return;
	}

	if (*type == PACKET_TYPE_transfer_file_ROUND_1_cancel_transfer) {
		//send_udp2("PACKET_TYPE_transfer_file_ROUND_1_cancel_transfer");

		TRANSFER_FILE_ROUND_cancel_transfer();

		return;
	};

	if (*type == PACKET_TYPE_transfer_file_ROUND_1) {
		//send_udp2("DETECT PACKET_TYPE_transfer_file_ROUND_1");
		//send_udp2((wchar_t *)&buf[40]);

		TRANSFER_FILE_ROUND_1(buf);

		return;
	};

	if (*type == PACKET_TYPE_transfer_file_ROUND_2) {
		//send_udp2("DETECT PACKET_TYPE_transfer_file_ROUND_2");

		if (TRANSFER_FILE_ROUND_2(buf) == false) {
			//send_udp2("TRANSFER_FILE_ROUND_2(buf) == false");
			//send_udp2("ss_need_disconnect = true (2)");
			ss_need_disconnect = true;
		}

		return;
	};

	if (*type == PACKET_TYPE_need_create_folder) {

		//send_udp2("DETECT PACKET_TYPE_need_create_folder");
		//send_udp2((wchar_t *)&buf[16]);

		NEED_CREATE_FOLDER(buf);

		return;
	};

	if (*type == PACKET_TYPE_request_file_list_1) {


		wchar_t *ww;

		ww = (wchar_t *)&(buf[16]);

		//send_udp2(ww);

		need_send_file_list_FOLDER_1[0] = 0;
		int i;
		i = 0;
		while (i < 5000 && ww[i] != 0) {
			need_send_file_list_FOLDER_1[i + 1] = 0;
			need_send_file_list_FOLDER_1[i] = ww[i];
			i++;
		}
		need_send_file_list_FOLDER_1[i] = 0;

		//send_udp2("~~~~~~~~~~~~~~~~~~~~~ > > need_send_file_list_1 = true");

		need_send_file_list_1 = true;

		return;
	};

	if (*type == PACKET_TYPE_request_file_list_2) {


		wchar_t *ww;

		ww = (wchar_t *)&(buf[24]);

		//send_udp2(L"*type == PACKET_TYPE_request_file_list_2", ww);

		need_send_file_list_FOLDER_2[0] = 0;
		int i;
		i = 0;
		while (i < 5000 && ww[i] != 0) {
			need_send_file_list_FOLDER_2[i + 1] = 0;
			need_send_file_list_FOLDER_2[i] = ww[i];
			i++;
		}
		need_send_file_list_FOLDER_2[i] = 0;

		//send_udp2("~~~~~~~~~~~~~~~~~~~~~ > > need_send_file_list_2 = true");

		need_send_file_list_2 = true;

		return;
	};
	

	if ( *type == PACKET_TYPE_request_start_screenflow_ver11 ||
		 *type == PACKET_TYPE_request_start_screenflow_ver22 ||
		 *type == PACKET_TYPE_request_start_screenflow_ver33
		) {
		reserv3 = (unsigned int *)&(buf[24]);
		reserv4 = (unsigned int *)&(buf[28]);


		if (service != NULL) {
			if (*reserv4 == 1 && service->is_sync_clipboards == false) {
				service->is_sync_clipboards = true;
			}
			if (*reserv4 != 1 && service->is_sync_clipboards == true) {
				service->is_sync_clipboards = false;
			}
		}
		
		/* 2021 09
		if (desktop != NULL) {
			if (*reserv4 == 1 && desktop->is_sync_clipboards == false) {
				desktop->is_sync_clipboards = true;
			}
			if (*reserv4 != 1 && desktop->is_sync_clipboards == true) {
				desktop->is_sync_clipboards = false;
			}
		}
		*/

		//sudp("SRV recv PACKET_TYPE_request_start_screenflow ");

		//commit_from_client_LAST_SCREEN_ID = *reserv3;
		need_start_screenflow_FORMAT_VER = *type;
		
		need_start_screenflow_count++;
		need_start_screenflow = true;
		return;
	}



	if (*type == PACKET_TYPE_responce_screen_test) {
		//ALOG("PACKET_TYPE_responce_screen_test");
	}

	if (*type == PACKET_TYPE_responce_mouse_cursor_short_format) {
		//ALOG("PACKET_TYPE_responce_mouse_cursor_short_format");
	}
	if (*type == PACKET_TYPE_responce_mouse_cursor_full_format) {
		//ALOG("PACKET_TYPE_responce_mouse_cursor_full_format");
	}
	if (*type == PACKET_TYPE_responce_screen_ver11) {
		//send_udp("PACKET_TYPE_responce_screen");
		/* 2019 +
		//header = (ENCODED_SCREEN_8bit_header *)&(buf[0]);

		//ALOG("PACKET_TYPE_responce_screen ╨┐╤А╨╕╤И╨╡╨╗ ╤Б╨║╤А╨╕╨╜ %d ", header->screen_id);

		//screen_from_server.decode(buf, *sz, &server_screen);
		//server_screen.scr_id = header->screen_id;
		*/

	}
	if (*type == PACKET_TYPE_send_event_from_client_to_server) {
		//ALOG("PACKET_TYPE_send_event_from_client_to_server");
	};
	if (*type == PACKET_TYPE_send_mouse_from_client_to_server) {
		//ALOG("PACKET_TYPE_send_mouse_from_client_to_server");
	}

	if (*type == PACKET_TYPE_send_keyboard_event_list_from_client_to_server) {
		//ALOG("PACKET_TYPE_send_keyboard_event_list_from_client_to_server");
		//send_udp("RECEIVE_KEYBOARD_EVENT_LIST_from_client_to_server 1121");

		RECEIVE_KEYBOARD_EVENT_LIST_from_client_to_server(buf);
		return;
	}

	if (*type == PACKET_TYPE_request_userlogin_list) {
		//ALOG("PACKET_TYPE_send_keyboard_event_list_from_client_to_server");
		//send_udp(L"PACKET_TYPE_request_userlogin_list ssss");

		// 2019 11 TODO RECEIVE_request_userlogin_list_from_client_to_server();

		return;
	}

	/* 2021 09
	if (*type == PACKET_TYPE_request_userCLIPBOARD_list) {
		//send_udp2("PACKET_TYPE_request_userCLIPBOARD_list ssssssssssssssssssssss");

		need_request_CLIPBOARD = true;

		//GET_CLIPBOARD_FROM_SERVET_to_CLIENT();

		return;
	}
	*/

	/* 2021 09
	if (*type == PACKET_TYPE_responce_userCLIPBOARD_list) {


		send_udp2("PACKET_TYPE_responce_userCLIPBOARD_list $$$$$$$$$$$$$$$$$$$$$+++");


		RECEIVE_responce_userCLIPBOARD_from_client_to_server(buf, *sz);

		return;
	}
	*/
	int x;
	x = 1123;

};

void NET_SERVER_SESSION::RECEIVE_KEYBOARD_EVENT_LIST_from_client_to_server(unsigned char *buf) {
	unsigned long long *iii, global_type, msg, wparam, lparam;

	iii = (unsigned long long *)&(buf[16]);

	char ss[100];
	int i, j;
	j = (int)(*iii);

	iii = (unsigned long long *)&(buf[32]);

	for (i = 0; i < j; i++) {

		msg = *iii; iii++;
		wparam = *iii; iii++;
		lparam = *iii; iii++;
		global_type = msg;
		global_type = global_type >> 32;
		msg = msg & 0xffffffff;

		//send_udp("READ global_type = ", (int)global_type);

		//send_udp( "KEYB EV ", msg, wparam, lparam);

		if (msg == 15 && wparam == 1 && lparam == 1) {
			sprintf__s_D(ss, 90, "CAD Detected F ! pid = %d ", app_attributes.global_my_proc_id);
			//send_udp2(ss);
			//send__udp("CAD Detected F ! pid = ", app_attributes.global_my_proc_id);
			kernel.CAD();
			return;
		};

		if (msg == 16 && wparam == 1 && lparam == 1) {
			//send_udp("request CLIPBOARD detected! pid = ", app_attributes.global_my_proc_id);
			//sprintf__s_D(ss, 90, "request CLIPBOARD detected! ! pid = %d ", app_attributes.global_my_proc_id);
			//send_udp2(ss);
			//kernel.CAD();
			//return;
		};

		//in_queue_command.add_element

		//2019+ 
		int screenflow_from_server_to_client_session_no;

		screenflow_from_server_to_client_session_no = 0;

		send_event_in_to_session(screenflow_from_server_to_client_session_no, EVENT_keyboard, (int)global_type, msg, wparam, lparam); // 1

		//screen_for_client->send_event_in_to_session(screenflow_from_server_to_client_session_no, EVENT_keyboard, msg, wparam, lparam);

	}

}

void NET_SERVER_SESSION::send_event_in_to_session(int session_no, unsigned int event_type, int global_type, unsigned long long msg, unsigned long long wparam, unsigned long long lparam) {
	if (service != NULL) {
		service->send_event_in_to_session(session_no, event_type, global_type, msg, wparam, lparam); // 2
	}
	else {
		if (app_attributes.is_desktop) {
			exec_event_in_to_session(&multidisplay, session_no, event_type, global_type, msg, wparam, lparam); // 2
		}
	}
	if (msg == 5555) {
		if (wparam >= 0 && wparam <= 3500 &&
			lparam >= 0 && lparam <= 3500)
		{
			last_set_mouse_x = (int)wparam;
			last_set_mouse_y = (int)lparam;
		}

	}
}

////
void NET_SERVER_SESSION::SEND_SCREEN_FROM_SERVER_TO_CLIENT_8bit_first(MASTER_AGENT_PACKET_HEADER *w_buf, MASTER_AGENT_PACKET_HEADER *r_buf, ENCODED_SCREEN_8bit_header *scr_head_buf) {

	if (screen_one_byte_ == nullptr) screen_one_byte_ = new SCREEN_LIGHT_one_byte();


	//screen_one_byte->emulate_red();

	if (jj < 3) {
		screen_one_byte_->emulate_red();
		jj++;
	}
	else {
		if (app_attributes.is_desktop == true) {

			init_encode_color_matrix_all();

			if (get_screenshot(&multidisplay, screen_one_byte_, nullptr) == false) {

				screen_one_byte_->emulate_red();
			}
		}
		else {
			if (service->interaction_with_agent_GET_SCREEN_8bit(w_buf, r_buf, scr_head_buf, screen_one_byte_) == false) {
				screen_one_byte_->emulate_red();
			}
		};
	};
	screen_one_byte_->screen_id++;

	if (screen_encoded_8bit_first == NULL) screen_encoded_8bit_first = new SCREEN_LIGHT_encoded_8bit_first();

	screen_encoded_8bit_first->encode_screen_ONE_BYTE(screen_one_byte_, last_set_mouse_x, last_set_mouse_y);

	//char ss[300];

	//sprintf_ s(ss, 290, "send_ screen %d %d %d", screen_one_byte_->header.w, screen_one_byte_->header.h, screen_one_byte_->header.mouse_cursor_type_id );
	//send_udp(ss);

	/*

	if (responce_screen_in_queue > 0) { return; };

	if (screen_for_client == NULL) return;

	//send_udp("get screen");

	if (screen_for_client->get_raw_screen_from_session() == false) {
		return;
	}

	//send_udp("encode");

	if (screen_for_client->encode() == false) {
		fatal_error("screen_for_client->encode() error");
		return;
	}
    */
	unsigned char *buf;
	unsigned int buf_len, *sz, *crc, *sol, *type, zz;


	buf = screen_encoded_8bit_first->encoded_buffer;
	buf_len = screen_encoded_8bit_first->encoded_buffer_len;

	zz = buf_len / 16;
	zz *= 16;
	if (zz < buf_len) zz += 16;
	buf_len = zz;

	sz = (unsigned int *)&(buf[0]);
	crc = (unsigned int *)&(buf[4]);
	type = (unsigned int *)&(buf[8]);
	sol = (unsigned int *)&(buf[12]);
	ENCODED_SCREEN_8bit_header *hhh;

	*sz = buf_len;
	*crc = 0;
	*type = PACKET_TYPE_responce_screen_ver11;
	*sol = get_sol();
	hhh = (ENCODED_SCREEN_8bit_header *)screen_encoded_8bit_first->encoded_buffer;

	unsigned int sscr_id;
	sscr_id = hhh->screen_id;
	//send_udp( "SCREEN prepared " , hhh->screen_id );

	//sprintf_ s(ss, 290, "encoded = %d", hhh->mouse_cursor_type_id);
	//send_udp(ss);

	responce_screen_in_queue++;

	//send_udp("encrypt");

	aes_partner.encrypt_stream(buf, buf_len);

	//c3 = clock();

	//wchar_t cc[ 250 ];
	//wsprintf(cc, L"encode  %d  %d ", c2 - c1, c3-c2 );
	//set_status(cc);

	//send_udp("add SCREEN into queue");
					   // 100003
	if (out_queue_command.add_element_(PACKET_TYPE_responce_screen_ver11, sscr_id, buf, buf_len) == false) {  // TODO encr ???
		responce_screen_in_queue--;
	}
	else {
		need_start_screenflow = false;
	}

}


void NET_SERVER_SESSION::SEND_SCREEN_FROM_SERVER_TO_CLIENT_12bit_first(MASTER_AGENT_PACKET_HEADER *w_buf, MASTER_AGENT_PACKET_HEADER *r_buf, ENCODED_SCREEN_12bit_header *scr_head_buf) {

	/*char ss[550];
	sprintf_s(ss, 450, "SEND_SCREEN_FROM_SERVER_TO_CLIENT_12bit_first");
	sudp(ss);*/

	if (screen_12bit == nullptr) screen_12bit = new SCREEN_LIGHT_12bit();

	if (jj < 3) {
		screen_12bit->emulate_red();
		jj++;
	}
	else {
		if (app_attributes.is_desktop == true) {

			if (get_screenshot(&multidisplay, nullptr, screen_12bit) == false) {

				screen_12bit->emulate_red();
			}
			//screen_12bit->emulate_red();
		}
		else {
			
			if (service->interaction_with_agent_GET_SCREEN_12bit(w_buf, r_buf, scr_head_buf, screen_12bit) == false) {
				screen_12bit->emulate_red();
			}
			
		};
	};
	screen_12bit->screen_id++;

	if (screen_encoded_12bit == NULL) screen_encoded_12bit = new SCREEN_LIGHT_encoded_12bit();
	
	sudp("encode_screen_12bit...");
	screen_encoded_12bit->encode_screen_12bit(screen_12bit, last_set_mouse_x, last_set_mouse_y);
	sudp("encode_screen_12bit ok");

	unsigned char *_buf;
	unsigned int _buf_len, *sz, *crc, *sol, *type, zz;


	_buf = screen_encoded_12bit->encoded_buffer;
	_buf_len = screen_encoded_12bit->encoded_buffer_len + sizeof(ENCODED_SCREEN_12bit_header);

	zz = _buf_len / 16;
	zz *= 16;
	if (zz < _buf_len) zz += 16;
	_buf_len = zz;

	sz = (unsigned int *)&(_buf[0]);
	crc = (unsigned int *)&(_buf[4]);
	type = (unsigned int *)&(_buf[8]);
	sol = (unsigned int *)&(_buf[12]);
	ENCODED_SCREEN_12bit_header *hhh;

	*sz = _buf_len;
	*crc = 0;
	*type = PACKET_TYPE_responce_screen_ver22;
	*sol = get_sol();
	hhh = (ENCODED_SCREEN_12bit_header *)screen_encoded_12bit->encoded_buffer;

	unsigned int sscr_id;
	sscr_id = hhh->screen_id;


	responce_screen_in_queue++;


	aes_partner.encrypt_stream(_buf, _buf_len);


	if (out_queue_command.add_element_(PACKET_TYPE_responce_screen_ver22, sscr_id, _buf, _buf_len) == false) {  // TODO encr ???
		responce_screen_in_queue--;
	}
	else {
		need_start_screenflow = false;
	}

};

void NET_SERVER_SESSION::SEND_SCREEN_FROM_SERVER_TO_CLIENT_8bit_second(MASTER_AGENT_PACKET_HEADER *w_buf, MASTER_AGENT_PACKET_HEADER *r_buf, ENCODED_SCREEN_8bit_header *scr_head_buf) {

	if (screen_one_byte_ == nullptr) screen_one_byte_ = new SCREEN_LIGHT_one_byte();


	//screen_one_byte->emulate_red();

	if (jj < 3) {
		screen_one_byte_->emulate_red();
		jj++;
	}
	else {
		if (app_attributes.is_desktop == true) {

			init_encode_color_matrix_all();

			if (get_screenshot(&multidisplay, screen_one_byte_, nullptr) == false) {

				screen_one_byte_->emulate_red();
			}
		}
		else {
			if (service->interaction_with_agent_GET_SCREEN_8bit(w_buf, r_buf, scr_head_buf, screen_one_byte_) == false) {
				screen_one_byte_->emulate_red();
			}
		};
	};
	screen_one_byte_->screen_id++;

	if (screen_encoded_8bit_second == NULL) screen_encoded_8bit_second = new SCREEN_LIGHT_encoded_8bit_second();

	screen_encoded_8bit_second->encode_screen_ONE_BYTE_second(screen_one_byte_, last_set_mouse_x, last_set_mouse_y);

	//char ss[300];

	//sprintf_ s(ss, 290, "send_ screen %d %d %d", screen_one_byte_->header.w, screen_one_byte_->header.h, screen_one_byte_->header.mouse_cursor_type_id );
	//send_udp(ss);

	/*

	if (responce_screen_in_queue > 0) { return; };

	if (screen_for_client == NULL) return;

	//send_udp("get screen");

	if (screen_for_client->get_raw_screen_from_session() == false) {
		return;
	}

	//send_udp("encode");

	if (screen_for_client->encode() == false) {
		fatal_error("screen_for_client->encode() error");
		return;
	}
	*/
	unsigned char *buf;
	unsigned int buf_len, *sz, *crc, *sol, *type, zz;


	buf = screen_encoded_8bit_second->encoded_buffer;
	buf_len = screen_encoded_8bit_second->encoded_buffer_len + sizeof(ENCODED_SCREEN_8bit_header);

	zz = buf_len / 16;
	zz *= 16;
	if (zz < buf_len) zz += 16;
	buf_len = zz;

	sz = (unsigned int *)&(buf[0]);
	crc = (unsigned int *)&(buf[4]);
	type = (unsigned int *)&(buf[8]);
	sol = (unsigned int *)&(buf[12]);
	ENCODED_SCREEN_8bit_header *hhh;

	*sz = buf_len;
	*crc = 0;
	*type = PACKET_TYPE_responce_screen_ver33;
	*sol = get_sol();
	hhh = (ENCODED_SCREEN_8bit_header *)screen_encoded_8bit_second->encoded_buffer;

	unsigned int sscr_id;
	sscr_id = hhh->screen_id;
	//send_udp( "SCREEN prepared " , hhh->screen_id );

	//sprintf_ s(ss, 290, "encoded = %d", hhh->mouse_cursor_type_id);
	//send_udp(ss);

	responce_screen_in_queue++;

	//send_udp("encrypt");

	aes_partner.encrypt_stream(buf, buf_len);

	//c3 = clock();

	//wchar_t cc[ 250 ];
	//wsprintf(cc, L"encode  %d  %d ", c2 - c1, c3-c2 );
	//set_status(cc);

	//send_udp("add SCREEN into queue");
					   // 100003
	sudp("SERVER send responce screen (8bit_second)");
	if (out_queue_command.add_element_(PACKET_TYPE_responce_screen_ver33, sscr_id, buf, buf_len) == false) {  // TODO encr ???
		responce_screen_in_queue--;
	}
	else {
		need_start_screenflow = false;
	}

}

void NET_SERVER_SESSION::TRANSFER_from_SRV_to_CLI_CANCELED() {
	CloseHandle(last_filefolder);
	last_filefolder = 0;
	last_filefolder_start_from = 0;
	sudp("CloseHandle(last_filefolder)");
}

void NET_SERVER_SESSION::request_filefolder_stat(unsigned char *in_buf) {



	unsigned char *out_buf;

	unsigned int *sz, *crc, *type, *file_ID_, file_ID, *is_folder__;

	sz = (unsigned int *)&(in_buf[0]);
	crc = (unsigned int *)&(in_buf[4]);
	type = (unsigned int *)&(in_buf[8]);
	file_ID_ = (unsigned int *)&(in_buf[16]);

	file_ID = *file_ID_;

	if (last_filefolder != 0) {
		fatal_error( "request_filefolder_stat() last_filefolder != 0");
		sudp("ss_need_disconnect = true (6)");
		ss_need_disconnect = true;
		return;
	}
	if (last_filefolder_start_from != 0) {
		fatal_error( "request_filefolder_stat() last_filefolder_start_from != 0");
		sudp("ss_need_disconnect = true (7)");
		ss_need_disconnect = true;
		return;
	}


	wchar_t name[5100];
	zero_wchar_t(name, 5100);
	int i, j, k;

	my_strcpy(name, L"C:\\VISIATOR_FILES\\");

	j = (*sz - 20) / 2;

	wchar_t *w;
	w = (wchar_t *)&(in_buf[20]);
	i = 0;
	k = 18;
	while (k < 5000 && i < j && w[i] != 0) {
		name[k++] = w[i++];

	}
	name[k] = 0;



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~ TODO определяем реквизиты файла
	unsigned long long f_size_, f_date_, *ll;
	unsigned int is_folder_; // 1-folder; 2-file; 3-not found; 4-access deny

	f_size_ = 0;
	f_date_ = 0;

	//sudp("QQQQ ", name);

	if (my_DirectoryExists(name)) {
		is_folder_ = 1;
		//sudp(L"is_folder_ = 1", name);
	}
	else {
		if (my_FileExists(name)) {

			is_folder_ = 2;
			//send_udp2(L"is_folder_ = 2", name);
			GetFileAttributes_(name, &f_size_, &f_date_);
			filefolder_size = f_size_;
			last_filefolder = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
			last_filefolder_start_from = 0;
			//send_udp2(L"last_filefolder = CreateFile()", name);
			if (last_filefolder == INVALID_HANDLE_VALUE) {
				is_folder_ = 4;
				last_filefolder = 0;

				//send_udp2(L"is_folder_ = 4 ~~~~~~~~~~~~~~~~~~~ last_filefolder=0", name);
			}
			else {
				//send_udp2(L"last_filefolder = CreateFile( READ )", name);
			}
		}
		else {
			is_folder_ = 3;
			//send_udp2(L"is_folder_ = 3", name);
		}
	}

	//send_udp2(L"name=", name, is_folder_);





	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	out_buf = new unsigned char[64];

	zero_unsigned_char(out_buf, 64);

	sz = (unsigned int *)&(out_buf[0]);
	crc = (unsigned int *)&(out_buf[4]);
	type = (unsigned int *)&(out_buf[8]);
	file_ID_ = (unsigned int *)&(out_buf[16]);
	is_folder__ = (unsigned int *)&(out_buf[20]);
	ll = (unsigned long long *)&(out_buf[24]);

	*sz = 64;
	*type = PACKET_TYPE_responce_filefolder_stat;
	*file_ID_ = file_ID;

	*is_folder__ = is_folder_;
	*ll = f_size_;
	ll++;
	*ll = f_date_;

	aes_partner.encrypt_stream(out_buf, 64);

	if (out_queue_command.add_element_low_level(PACKET_TYPE_responce_filefolder_stat, 0, out_buf, 64) == false) {

	}
	else {

	}

}

void NET_SERVER_SESSION::DELETE_FILE_LIST_cancel() {
	need_delete_files_list_CANCEL = true;
}

void NET_SERVER_SESSION::DELETE_FILE_LIST(wchar_t *w, int sz, unsigned int file_ID) {

	if (need_delete_files_list != false) { // мы еще не закончили удалять файлы по старому списку, а нам уже прислали новый
		return;
	};

	if (files_list_for_delete != nullptr) {
		delete[] files_list_for_delete;
		files_list_for_delete = nullptr;
	};
	files_list_for_delete_size = sz + 16;
	files_list_for_delete = new wchar_t[files_list_for_delete_size];


	files_list_for_delete[0] = 0;
	files_list_for_delete[1] = 0;
	int i;

	//send_udp2(L"DELETE_FILE_LIST() w = ", w);

	for (i = 0; i < sz; i++) {
		files_list_for_delete[i] = w[i];
	};
	//send_udp2("DELETE_FILE_LIST() i = ", i);
	//send_udp2(L"_files_list_for_delete = ", files_list_for_delete);
	files_list_for_delete[i++] = 0;
	files_list_for_delete[i] = 0;

	files_list_for_delete_file_ID = file_ID;

	need_delete_files_list = true;


}

void NET_SERVER_SESSION::TRANSFER_FILE_ROUND_cancel_transfer() {
	if (hFILE != 0) {
		//send_udp2(L"TRANSFER_FILE_ROUND_cancel_transfer()", target_file_full_name);
		CloseHandle(hFILE);
		hFILE = 0;
		int i = 0;
		while (i < 5 && my_FileExists(target_file_full_name)) {
			if (i > 0) ::Sleep(500);
			DeleteFile(target_file_full_name);
			i++;
		};
	};

	target_file_full_name[0] = 0;

}

bool NET_SERVER_SESSION::TRANSFER_FILE_ROUND_1(unsigned char *buf_) {

	wchar_t *w;
	unsigned long long *iii, FileSize, FileDate;
	unsigned int *ii, file_ID, ss, *allow_overwrite_;
	int i, j;

	ii = (unsigned int *)&buf_[0];
	ss = *ii;

	ii = (unsigned int *)&buf_[16];
	file_ID = *ii;

	iii = (unsigned long long *)&buf_[20];
	FileSize = *iii;
	iii = (unsigned long long *)&buf_[28];
	FileDate = *iii;

	allow_overwrite_ = (unsigned int *)&buf_[36];

	if (ss < 48) {
		fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_1 ss < 48");
		//send_udp2("ss_need_disconnect = true (8)");
		ss_need_disconnect = true;

		return false;
	}

	w = (wchar_t *)&buf_[40];


	if (target_file_full_name == nullptr) target_file_full_name = new wchar_t[5100];

	my_strcpy(target_file_full_name, L"C:\\VISIATOR_FILES");

	if (my_DirectoryExists(target_file_full_name) == false) {
		CreateDirectory(target_file_full_name, nullptr);
	}

	wchar_t dir[5100];
	zero_wchar_t(dir, 5100);

	i = 0;
	j = 0;
	while (i < 5000) {
		if (i * 2 > ss - 40) {
			fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_1 i * 2 > ss - 36 !");// , i, ss);
			//send_udp2("ss_need_disconnect = true (9)");
			ss_need_disconnect = true;
			return false;
		}

		//target_file_full_name[i + 18] = w[i];
		if (w[i] == '\\' || w[i] == 0) {
			if ((dir[0] == '.' && dir[1] == 0) ||
				(dir[0] == '.' && dir[1] == '.')) {
				fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_1 . || ..");
				//send_udp2("ss_need_disconnect = true (10)");
				ss_need_disconnect = true;
				return false;
			}



			my_strcat_s( target_file_full_name, 5000, L"\\");
			my_strcat_s(target_file_full_name, 5000, dir);

			//send_udp2(dir);

			dir[0] = 0;
			j = 0;
			if (w[i] == 0) break;
		}
		else {
			if (is_bad_symbol_folder_name(w[i]) == true) { // TEST
				fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_1 is_bad_symbol_folder_name ");// , (int)w[i]);
				//send_udp2("ss_need_disconnect = true (11)");
				ss_need_disconnect = true;
				return false;
			}
			dir[j++] = w[i];
			dir[j] = 0;
		};
		i++;
	}


	//send_udp2(target_file_full_name);


	bool r = true;

	if (hFILE != 0) { // нас просят начать получение нового файла, а мы еще со старым не закончили. 
		fatal_error("hFILE != 0");
		//send_udp2("ss_need_disconnect = true (12)");
		ss_need_disconnect = true;
		return false;
	}

	unsigned long long _fs, _dt;
	int _result_ = 0;
	_fs = -1;
	_dt = -1;

	if (_result_ == 0) {
		if (*allow_overwrite_ == 0 && my_FileExists(target_file_full_name)) {
			_result_ = ROUND_1_RESULT_ERR_target_file_already_exists; // файл, который нас просят создат, уже существует. нужно запросить пользователя SKIP/OVERWRITE/CANCEL
			_fs = -1;
			_dt = -1;
			GetFileAttributes_(target_file_full_name, &_fs, &_dt);

		};
	}

	if (_result_ == 0) { // создаем файл

		if (*allow_overwrite_ == 1) { // Разрешено заменять файл при его наличии
			if (my_FileExists(target_file_full_name)) {
				DeleteFile(target_file_full_name);
			}
		}
		//send_udp2(L"CreateFile ", target_file_full_name);
		hFILE = CreateFile(target_file_full_name, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFILE == INVALID_HANDLE_VALUE) {
			_result_ = ROUND_1_RESULT_ERR_fatal;
			hFILE_time = 0;
		}
		else {
			hFILE_time = GetTickCount();

			hFILE_writed_size = 0;
			target_file_size = FileSize;
			target_file_date = FileDate;

			if (target_file_size == 0) {
				FinishFile();
				_result_ = ROUND_1_RESULT_OK_target_file_finish_ZEROSIZE;
				//send_udp2(L"ROUND_1_RESULT_OK_target_file_finish_ZEROSIZE");
			}
			else {
				_result_ = ROUND_1_RESULT_OK_target_file_created;
				//send_udp2(L"ROUND_1_RESULT_OK_target_file_created");
			}

		};
	};
	//need_disconnect = true;

	// r = TODO ()
	// тут вся работа по созданию временного файла

	//my_FileExists()

	unsigned int *sz, *crc, *sol, *type, *GID, *result;
	unsigned long long *vvv;

	int buf_size = 48;
	unsigned char *sndd;

	sndd = new unsigned char[48];
	zero_unsigned_char(sndd, 48);

	sz = (unsigned int *)&(sndd[0]);
	crc = (unsigned int *)&(sndd[4]);
	type = (unsigned int *)&(sndd[8]);
	sol = (unsigned int *)&(sndd[12]);
	GID = (unsigned int *)&(sndd[16]);
	result = (unsigned int *)&(sndd[20]);
	vvv = (unsigned long long *)&(sndd[24]);

	*sz = 48;
	*crc = 0;
	*type = PACKET_TYPE_transfer_file_ROUND_1_commit;
	*sol = get_sol();
	*GID = file_ID;
	*result = _result_;
	*vvv = _fs;
	vvv++;
	*vvv = _dt;

	aes_partner.encrypt_stream(sndd, 48);

	// 100003
	if (out_queue_command.add_element_low_level(PACKET_TYPE_transfer_file_ROUND_1_commit, 0, sndd, 48) == false) {
		//send_udp2("+1-------");
	}
	else {
		//send_udp2("-1++++++++");
	}

	return true;
}

bool NET_SERVER_SESSION::TRANSFER_FILE_ROUND_2(unsigned char *buf_) {

	unsigned int *sz, *crc, *sol, *type, *GID, *kusok_size, file_ID, _result_;

	sz = (unsigned int *)&(buf_[0]);
	crc = (unsigned int *)&(buf_[4]);
	type = (unsigned int *)&(buf_[8]);
	sol = (unsigned int *)&(buf_[12]);
	GID = (unsigned int *)&(buf_[16]);
	kusok_size = (unsigned int *)&(buf_[20]);

	file_ID = *GID;

	char sss[500];
	sprintf__s_ui_ui(sss, 450, "===>>> %d ROUND_2 %d ", *sz, *kusok_size);

	//send_udp2(sss);

	if (*sz < 48) {
		fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_2 *sz < 48");
		return false;
	};
	if (*kusok_size > *sz - 32) {
		fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_2 *kusok_size > *sz - 32");
		return false;
	}
	if (*kusok_size < *sz - 32 - 16) {
		fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_2 *kusok_size < *sz - 32 - 16");
		return false;
	};

	if (hFILE == 0) {
		fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_2 hFILE == 0");
		return false;
	}

	if (hFILE_writed_size + *kusok_size > target_file_size) {
		fatal_error("NET_SERVER_SESSION::TRANSFER_FILE_ROUND_2 tmp_hFILE == 0");
		return false;
	}

	BOOL b;
	DWORD ww;
	ww = 0;
	b = WriteFile(hFILE, buf_ + 32, *kusok_size, &ww, 0);

	if (b == FALSE) {
		//send_udp2("b == FALSE");
		return false;
	};
	if (ww != *kusok_size) {
		//send_udp2("ww != *kusok_size ", ww, *kusok_size);
		return false;
	};

	hFILE_writed_size += ww;

	if (hFILE_writed_size == target_file_size) {
		FinishFile();
		_result_ = 2;
		//send_udp2("ROUND2 FinishFile()");
	}
	else {
		_result_ = 1;
		//send_udp2("ROUND2...");
	}

	//**************************************************************************************
	//**

	unsigned int *result;

	int buf_size = 32;
	unsigned char *sndd;

	sndd = new unsigned char[32];
	zero_unsigned_char(sndd, 32);

	sz = (unsigned int *)&(sndd[0]);
	crc = (unsigned int *)&(sndd[4]);
	type = (unsigned int *)&(sndd[8]);
	sol = (unsigned int *)&(sndd[12]);
	GID = (unsigned int *)&(sndd[16]);
	result = (unsigned int *)&(sndd[20]);

	*sz = 32;
	*crc = 0;
	*type = PACKET_TYPE_transfer_file_ROUND_2_commit;
	*sol = get_sol();
	*GID = file_ID;
	*result = _result_;

	aes_partner.encrypt_stream(sndd, 32);

	// 100003
	if (out_queue_command.add_element_low_level(PACKET_TYPE_transfer_file_ROUND_2_commit, 0, sndd, 32) == false) {
		//send_udp2("+2-------");
	}
	else {
		//send_udp2("-2++++++++", _result_);
	}


	return true;
};

void NET_SERVER_SESSION::request_filefolder_part(unsigned char *in_buf) {
	unsigned char *out_buf;

	unsigned int *sz, *crc, *type, *file_ID_, file_ID, *start_from_, start_from;

	sz = (unsigned int *)&(in_buf[0]);
	crc = (unsigned int *)&(in_buf[4]);
	type = (unsigned int *)&(in_buf[8]);
	file_ID_ = (unsigned int *)&(in_buf[16]);
	start_from_ = (unsigned int *)&(in_buf[20]);

	file_ID = *file_ID_;
	start_from = *start_from_;

	if (start_from != last_filefolder_start_from) {
		fatal_error("request_filefolder_part()- start_from != last_filefolder_start_from -");// , (int)start_from, (int)last_filefolder_start_from);
		//send_udp2("ss_need_disconnect = true (3)");
		ss_need_disconnect = true;
		return;
	}

	if (last_filefolder == 0) {
		fatal_error("request_filefolder_part() last_filefolder == 0");
		//send_udp2("ss_need_disconnect = true (4)");
		ss_need_disconnect = true;
		return;
	}

	if (filefolder_part_buf == nullptr) {
		filefolder_part_buf = new unsigned char[filefolder_part_buf_max_size];
		zero_unsigned_char(filefolder_part_buf, filefolder_part_buf_max_size);
	}

	unsigned int j, zz, *sol, *GID, *part_size, *result;

	BOOL r;
	DWORD readed;

	readed = 0;

	r = ReadFile(last_filefolder, filefolder_part_buf, filefolder_part_buf_max_size, &readed, NULL);
	if (r == FALSE) { // ошибка при чтении файла
		CloseHandle(last_filefolder);
		last_filefolder = 0;
		last_filefolder_start_from = 0;
		//send_udp2("~~~~~~~~~~~~~~~~ last_filefolder=0");
		last_filefolder_start_from = 0;

		j = 32;
		zz = j / 16;
		zz *= 16;
		if (zz < j) zz += 16;

		out_buf = new unsigned char[zz + 32];

		sz = (unsigned int *)&(out_buf[0]);
		crc = (unsigned int *)&(out_buf[4]);
		type = (unsigned int *)&(out_buf[8]);
		sol = (unsigned int *)&(out_buf[12]);
		GID = (unsigned int *)&(out_buf[16]);
		part_size = (unsigned int *)&(out_buf[20]);
		result = (unsigned int *)&(out_buf[24]);

		*sz = zz;
		*crc = 0;
		*type = PACKET_TYPE_responce_filefolder_part;
		*sol = get_sol();
		*GID = file_ID;
		*part_size = 0;
		*result = 2; // ошибка

		aes_partner.encrypt_stream(out_buf, zz);

		if (out_queue_command.add_element_low_level(PACKET_TYPE_responce_filefolder_part, 0, out_buf, zz) == false) {

		}
		else {

		}
	}
	else {
		if (readed > filefolder_part_buf_max_size) {
			fatal_error("request_filefolder_part() readed > filefolder_part_buf_max_size");
			//send_udp2("ss_need_disconnect = true (5)");
			ss_need_disconnect = true;
			return;
		};




		last_filefolder_start_from += readed;

		//send_udp2("~~~~~~~~~~~~~~~~~~~ ", (int)last_filefolder_start_from, (int)readed);

		if (last_filefolder_start_from == filefolder_size) {
			//send_udp2("CloseHandle(last_filefolder)");

			CloseHandle(last_filefolder);
			last_filefolder = 0;
			last_filefolder_start_from = 0;
			//send_udp2("~~~~~~~~~~~~~~~~~~ last_filefolder=0");


		}

		j = readed + 32;
		zz = j / 16;
		zz *= 16;
		if (zz < j) zz += 16;


		out_buf = new unsigned char[zz + 32];

		sz = (unsigned int *)&(out_buf[0]);
		crc = (unsigned int *)&(out_buf[4]);
		type = (unsigned int *)&(out_buf[8]);
		sol = (unsigned int *)&(out_buf[12]);
		GID = (unsigned int *)&(out_buf[16]);
		part_size = (unsigned int *)&(out_buf[20]);
		result = (unsigned int *)&(out_buf[24]);

		*sz = zz;
		*crc = 0;
		*type = PACKET_TYPE_responce_filefolder_part;
		*sol = get_sol();
		*GID = file_ID;
		*part_size = readed;
		*result = 1;

		unsigned char *q;

		q = &out_buf[28];
		unsigned int i = 0;
		while (i < readed) {
			*q++ = filefolder_part_buf[i++];
		}

		aes_partner.encrypt_stream(out_buf, zz);

		if (out_queue_command.add_element_low_level(PACKET_TYPE_responce_filefolder_part, 0, out_buf, zz) == false) {

		}
		else {

		}
	}
};
void NET_SERVER_SESSION::FinishFile() {
	FILETIME *f, t;
	f = (FILETIME *)&target_file_date;
	t = *f;
	SetFileTime(hFILE, &t, &t, &t);

	CloseHandle(hFILE);

	hFILE = 0;
	target_file_full_name[0] = 0;

	sudp("FinishFile+");
}

bool nnnn(wchar_t *buf, int sz) {

	unsigned int i, j, k;
	wchar_t path[5100];
	wchar_t name[5100];
	wchar_t ww[5100];
	//wchar_t *w;

	path[0] = 0;
	name[0] = 0;

	if (buf[0] == 0) return true;

	my_strcpy(path, L"C:\\VISIATOR_FILES");


	if (my_CreateDirectory(path) == false) return false;

	i = my_strlen(path);

	path[i++] = '\\'; path[i] = 0;


	ww[0] = 0;
	j = 0;
	k = 0;
	while (i < 5000 && buf[j] != 0) {
		if (buf[j] == '\\') {
			if (ww[0] == 0) return false;
			if (ww[0] == '.' && ww[1] == 0) return false;
			if (ww[0] == '.' && ww[1] == '.') return false;

			if (my_CreateDirectory(path) == false) return false;
			ww[0] = 0;
			k = 0;
		}
		else {
			ww[k++] = buf[j]; ww[k] = 0;
		}
		path[i++] = buf[j]; path[i] = 0;

		j++;
	}

	if (ww[0] != 0) {
		if (ww[0] == '.' && ww[1] == 0) return false;
		if (ww[0] == '.' && ww[1] == '.') return false;

		if (my_CreateDirectory(path) == false) return false;
	}


	//***************************************************************************
	//** проверить путь path и при необходимости создать недостающие папки


	//***************************************************************************
	// проверить и при необходимости создать папку name



	//send_udp2(path);
	//send_udp2(name);

	return true;
}


bool NET_SERVER_SESSION::NEED_CREATE_FOLDER(unsigned char *name) {

	unsigned int *ii, i, file_ID, ss;
	wchar_t *w;

	ii = (unsigned int *)&name[0];
	ss = *ii;
	ii = (unsigned int *)&name[16];
	file_ID = *ii;

	bool r;

	if (ss < 32) {
		fatal_error("NET_SERVER_SESSION::NEED_CREATE_FOLDER ss < 32");
		GLOBAL_STOP = true;
		return false;
	}

	w = (wchar_t *)&name[20];
	i = 20;
	while (*w != 0) {
		i += 2;
		if (i > ss) {
			fatal_error("NET_SERVER_SESSION::NEED_CREATE_FOLDER i > ss");
			GLOBAL_STOP = true;
			return false;
		}
		if (*w < 32) {
			fatal_error("NET_SERVER_SESSION::NEED_CREATE_FOLDER *w < 32");
			GLOBAL_STOP = true;
			return false;
		}
		w++;
	}

	r = nnnn((wchar_t*)&name[20], ss); // ТУТ вся работа по созданию папки

	unsigned int *sz, *crc, *sol, *type, *GID, *result;

	int buf_size = 32;
	unsigned char *buf;

	buf = new unsigned char[32];
	zero_unsigned_char(buf, 32);

	sz = (unsigned int *)&(buf[0]);
	crc = (unsigned int *)&(buf[4]);
	type = (unsigned int *)&(buf[8]);
	sol = (unsigned int *)&(buf[12]);
	GID = (unsigned int *)&(buf[16]);
	result = (unsigned int *)&(buf[20]);

	*sz = 32;
	*crc = 0;
	*type = PACKET_TYPE_need_create_folder_commit;
	*sol = get_sol();
	*GID = file_ID;
	if (r) *result = 1; else *result = 2;

	aes_partner.encrypt_stream(buf, 32);

	// 100003
	if (out_queue_command.add_element_low_level(PACKET_TYPE_need_create_folder_commit, 0, buf, 32) == false) {
		//send_udp2("+--------");
	}
	else {
		//send_udp2("-+++++++++");
	}

	return true;
}

void NET_SERVER_SESSION::SEND_file_list_FROM_SERVER_to_CLIENT_1(wchar_t *need_request_files_list_DIR) {



	//send_udp2("SEND_file_list_FROM_SERVER_to_CLIENT");
	//send_udp2(need_request_files_list_DIR);


	FL_1.clean();
	FL_1.LOAD(need_request_files_list_DIR);

	unsigned int *sz, *crc, *sol, *type, zz, buf_size;

	unsigned char *buf;

	buf_size = FL_1.size1 + 16;

	zz = buf_size / 16;
	zz *= 16;
	if (zz < buf_size) zz += 16;
	buf_size = zz;


	//send_udp2("buf_size===", buf_size);

	buf = new unsigned char[buf_size + 64];

	FL_1.get(buf + 16, buf_size - 16);





	sz = (unsigned int *)&(buf[0]);
	crc = (unsigned int *)&(buf[4]);
	type = (unsigned int *)&(buf[8]);
	sol = (unsigned int *)&(buf[12]);

	//send_udp2("buf_size = ", buf_size);

	*sz = buf_size;
	*crc = 0;
	*type = PACKET_TYPE_responce_file_list_1;
	*sol = get_sol();





	aes_partner.encrypt_stream(buf, buf_size);



	if (out_queue_command.add_element_low_level(PACKET_TYPE_responce_file_list_1, 0, buf, buf_size) == false) {  // TODO encr ???

	}
	else {

	}
	//send_udp2("[[555]]");



}

void NET_SERVER_SESSION::SEND_file_list_FROM_SERVER_to_CLIENT_2(wchar_t *need_request_files_list_DIR) {

	wchar_t dd[5100];

	zero_wchar_t(dd, 5100);

	my_strcpy_s(dd, 5000, L"C:\\VISIATOR_FILES\\");
	my_strcat_s(dd, 5000, need_request_files_list_DIR);

	//send_udp2(L"SEND_file_list_FROM_SERVER_to_CLIENT_2", dd);


	FL_2.clean();
	FL_2.LOAD(dd);

	unsigned int *sz, *crc, *sol, *type, zz, buf_size;

	unsigned char *buf;

	buf_size = FL_2.size1 + 16;

	zz = buf_size / 16;
	zz *= 16;
	if (zz < buf_size) zz += 16;
	buf_size = zz;


	//send_udp2("buf_size===", buf_size);

	buf = new unsigned char[buf_size + 64];

	FL_2.get(buf + 16, buf_size - 16);





	sz = (unsigned int *)&(buf[0]);
	crc = (unsigned int *)&(buf[4]);
	type = (unsigned int *)&(buf[8]);
	sol = (unsigned int *)&(buf[12]);

	//send_udp2("buf_size = ", buf_size);

	*sz = buf_size;
	*crc = 0;
	*type = PACKET_TYPE_responce_file_list_2;
	*sol = get_sol();





	aes_partner.encrypt_stream(buf, buf_size);



	if (out_queue_command.add_element_low_level(PACKET_TYPE_responce_file_list_2, 0, buf, buf_size) == false) {  // TODO encr ???

	}
	else {

	}
	//send_udp2("[[555]]");



}
