#include "stdafx.h"

#include "tools.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "NET_CLIENT_SESSION.h"
#include "NET_SERVER_SESSION.h"
#include "CRYPTO.h"
#include "VIEWER.h"
#include "MODAL_DIALOG.h"
#include "TRANSFER_DIALOG2_DirsFiles_Label.h"
#include "TRANSFER_DIALOG2_DirsFiles_Button.h"

#include "tools.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;
extern VIEWER  *viewer;

void PERFOMANCE_COUNTER::add(int val) {
	DWORD d;
	d = GetTickCount() / 100;
	if (values[0][0] == d) {
		values[1][0] += val;
		return;
	}
	int i;
	i = max_count - 1;
	while (i > 0) {
		values[0][i] = values[0][i - 1];
		values[1][i] = values[1][i - 1];
		i--;
	}
	values[0][0] = d;
	values[1][0] = val;
}

int PERFOMANCE_COUNTER::get_per_sec() {
	int v, i, dd;
	dd = GetTickCount() / 100 - 30;
	i = 0;
	v = 0;
	while (i < max_count) {
		if (values[0][i] < dd) {
			return v / 3;
		}
		v += values[1][i];
		i++;
	}

	return 0;
}

void PERFOMANCE_COUNTER::clean() {
	for (int i = 0; i < max_count; i++) {
		values[0][i] = 0;
		values[1][i] = 0;
	}
}

PERFOMANCE_COUNTER::PERFOMANCE_COUNTER() {
	clean();
}

PERFOMANCE_COUNTER::~PERFOMANCE_COUNTER() {

}

//***********************************************************************


NET_CLIENT_SESSION::NET_CLIENT_SESSION() {
	
}

NET_CLIENT_SESSION::~NET_CLIENT_SESSION() {

}

void NET_CLIENT_SESSION::mouse_left_button_down(GUI_low_level *low_level, int mx, int my) {
	out_event_queue.addd(0, WM_LBUTTONDOWN__, (uint16_t)mx, (uint16_t)my, "5");
}

void NET_CLIENT_SESSION::mouse_left_button_up(GUI_low_level *low_level, int mx, int my) {
	out_event_queue.addd(0, WM_LBUTTONUP__, (uint16_t)mx, (uint16_t)my, "5");
}

void NET_CLIENT_SESSION::mouse_right_button_down(GUI_low_level *low_level, int mx, int my) {
	out_event_queue.addd(0, WM_RBUTTONDOWN__, (uint16_t)mx, (uint16_t)my, "5");
}

void NET_CLIENT_SESSION::mouse_right_button_up(GUI_low_level *low_level, int mx, int my) {
	out_event_queue.addd(0, WM_RBUTTONUP__, (uint16_t)mx, (uint16_t)my, "5");
}
void NET_CLIENT_SESSION::get_statistic(char *ss, int ss_max_size_) {

	sprintf__s_ull_ull(ss, ss_max_size_, "rcv:%lld snd:%lld ", recv__counter, send__counter);

}
void NET_CLIENT_SESSION::mouse_move(GUI_low_level *low_level, int mx, int my) {
	//out_event_queue.add(0, 5555, mx, my, "5");
	need_set_mouse_x = mx;
	need_set_mouse_y = my;
}
void NET_CLIENT_SESSION::mouse_whell(GUI_low_level *low_level, int msg, int wp, int lp) {
	out_event_queue.addd(0, (uint16_t)msg, (uint16_t)wp, (uint16_t)lp, "6");
}

void NET_CLIENT_SESSION::char_keydown(GUI_low_level *low_level, int msg, int wp, int lp) {
	out_event_queue.addd(0, (uint16_t)msg, (uint16_t)wp, (uint16_t)lp, "5");
}

void NET_CLIENT_SESSION::char_keyup(GUI_low_level *low_level, int msg, int wp, int lp) {
	out_event_queue.addd(0, (uint16_t)msg, (uint16_t)wp, (uint16_t)lp, "5");
}


void NET_CLIENT_SESSION::EXECUTE() {

	bool local_stop = false;
	int connect_try_count = 0;
	unsigned int proxy_ip = 0;
	int res;

	boost::posix_time::milliseconds SleepTime(100);

	EXECUTE_is_run = true;

	set__status(L"waiting for the partner_id");

	while (GLOBAL_STOP == false && local_stop == false) {

		if (partner_pass_and_id_is_set == true) {

			connect_try_count++;

			/////////////////////////////

			//net_client_session->


			/////////////////////////////

			if (my_priv_id == 0) {

				set__status(L"Get local id... ");

				Load_private_id_and_public_id_from_USER_registry(&my_pub_id, &my_priv_id);

				if (my_pub_id == 0) {

					set__status(L"Register... ");

					Register_new_partner_on_proxy();
				}
			}

			if (my_priv_id != 0) {


				set__status(L"Preparing to connect...");
				sudp("get_IP_for_server_location()...");
				proxy_ip = get_IP_for_server_location(partner_id, my_pub_id, my_priv_id);

				if (proxy_ip < 100) {
					sudp("NCS IP not determine...");
					set__status(L"Partner not found");

					my_Slip(3000);
				}
				else {
					sudp("NCS IP determine Ok");

					set__status(L"Connect... ");

					res = Connect_to_server( partner_id, my_pub_id, my_priv_id, partner_pass_encripted, proxy_ip);

					if (res == -1) { // pass incorrect
						//local_stop = true;
					}
					my_Slip(3000);
				}
			}
			else {

				set__status(L"Connect error");

				my_Slip(3000);
			}

		} 


		boost::this_thread::sleep(SleepTime);
	}
	set__status(L"stoped");
	EXECUTE_is_run = false;

}


void NET_CLIENT_SESSION::set__status(wchar_t *v) {
	status = v;

	if (parent_low_level != nullptr) parent_low_level->invalidate();
}

int  NET_CLIENT_SESSION::Connect_to_server( unsigned long long partner_id, unsigned long long public_id, unsigned long long private_id, unsigned char pass_hash16[16], unsigned int proxy_ip) {
	if (public_id == 0 || private_id == 0 || proxy_ip < 100 || partner_id == 0) {
		return 0;
	};

	SERVER_VER = 0;

	sudp("NCS Connect_to_server");

	int return_result;

	return_result = 0;

	unsigned int crc32, key_for_commit_connection;
	unsigned char xx[500], bb[500];
	PACKET_LEVEL0 *p0, *r0;
	PACKET_LEVEL1_1005_request *p1;

	key_for_commit_connection = 1;

	SOCKET sos;
	sockaddr_in dest_addr;
	int res, snd;

	MY_AES aes_proxy;
	MY_CRC crc;
	MY_RSA rsaa;
	rsaa.init();

	aes_partner.set_key_16_byte(pass_hash16, "NCS Connect_to_server 1 aes_partner");

	if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
	{ //3
		//status = "create socket error";
		//send_udp("Connect_to_server() err 2");
		return return_result;
	} //3
	else
	{ // 3


		/*app_attributes.proxy_ip = 0x4b00a8c0;
		app_attributes.proxy_port = 3128;
		app_attributes.proxy_use = true;*/


		//*******************************************************



		//*******************************************************



		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(SERVER_PORT);
		dest_addr.sin_addr.S_un.S_addr = proxy_ip;// proxy_address );

		PROXY_LIST *proxy_list; // app_attributes+



		//status = "connect...";
		if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+!
		{//4
			closesocket(sos);
			sos = 0;
			proxy_list = app_attributes.proxy_list;
			if (proxy_list != NULL) {
				sos = proxy_list->try_connect(proxy_ip); // пробуем подключиться через прокси +
			};
			if (sos == 0) {
				closesocket(sos);
				return return_result;
			};
		}//4



		setnonblocking(sos);

		zero_unsigned_char(bb, 128);

		p0 = (PACKET_LEVEL0 *)bb;
		p0->packet_type = 1005;
		p1 = (PACKET_LEVEL1_1005_request *)p0->body;

		p1->client_public_id = public_id;
		p1->client_private_id = private_id;
		p1->client_ver = app_attributes.VER;
		p1->partner_public_id = partner_id;//generate_id(461, 50, 164);

		my_random(p1->AES_passs, 16);

		aes_proxy.set_key_16_byte(p1->AES_passs, "NCS Connect_to_server 2 aes_proxy");

		p0->crc32 = crc.calc(&bb[8], 120);

		rsaa.encode_128_byte(bb, bb);

		snd = 0;
		do
		{
			snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__counter);//ok
		} while (snd != 128);


		do
		{
			res = my_recv(sos, xx, 128, &recv__counter);
		} while (res != 128 && res != -1 && GLOBAL_STOP == false);
		aes_proxy.decrypt_128_byte(xx);


		crc32 = crc.calc(&xx[8], 120);


		r0 = (PACKET_LEVEL0 *)xx;
		if (r0->crc32 == crc32)
		{
			PACKET_LEVEL1_1005_responce *s1005;
			s1005 = (PACKET_LEVEL1_1005_responce *)r0->body;

			if (s1005->result == 2) { // партнер найден
				//ALOG("key_for_commit_connection = {%X}", s1005->key);

				sudp("NCS partner founded");

				SERVER_VER = s1005->srv_ver;

				if (viewer != nullptr) {
					viewer->set_SERVER_VER(SERVER_VER);
				}

				key_for_commit_connection = s1005->key;

				//********************************************
				zero_unsigned_char(bb, 128);

				PACKET_LEVEL1_1006_request *r1006;
				r1006 = (PACKET_LEVEL1_1006_request *)bb;

				r1006->key = key_for_commit_connection;
				r1006->sol = get_sol();

				aes_proxy.encrypt_16_byte(bb);
				//********************************************

				snd = my_send(sos, (unsigned char *)bb, 16, 0, "", &send__counter);//ok

				if (snd != 16) {

				}

				zero_unsigned_char(bb, 128);

				do
				{
					res = my_recv(sos, xx, 16, &recv__counter);
				} while (res != 16 && res != -1 && GLOBAL_STOP == false);
				aes_proxy.decrypt_16_byte(xx);

				sudp("1");

				PACKET_LEVEL1_1006_responce *s1006;
				s1006 = (PACKET_LEVEL1_1006_responce *)xx;



				if (s1006->result == 1022) {
					// прокси подтверждает, что мы установили соединение с партнером
					sudp("Connected");

					set__status(L"Connected");

					connection_to_partner_established = true;
					if (SERVER_VER > 4000) {
						need_start_screenflow_from_server_FORMAT_VER = PACKET_TYPE_request_start_screenflow_ver22;
					}
					else {
						need_start_screenflow_from_server_FORMAT_VER = PACKET_TYPE_request_start_screenflow_ver11;
					}

					if (parent_func__connect != NULL) parent_func__connect();

					need_start_screenflow_from_server = true;

					//send_udp("ClientConnect");

					res = Client_Main_Loop(sos);

					//set__status(L"Disconnected");

					//send_udp("ClientDisconnect");

					if (parent_func__disconnect != NULL) parent_func__disconnect();

					//enter_crit();
					//status += " Disconnected ";
					//leave_crit();
					//if (gui != NULL) gui->invaidate();

					//if (viewer != NULL) viewer->net_client_disconnected();

					connection_to_partner_established = false;
					if (res == -1) { // pass incorrect
						return_result = -1;
					}

				}

			}
			else
			{
				// партнер не найден
			}

		}
		else
		{
			// crc error
		}
		struct linger lng = { 1, 0 };
		setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

		int ee;
		ee = shutdown(sos, SD_BOTH);
		closesocket(sos);



	} //3

	return return_result;
}

void NET_CLIENT_SESSION::set_partner_pass_and_id(uint64_t partner_id_, uint8_t *partner_pass_encripted_len32) {
	
	for (int i = 0; i < 32; i++) {
		partner_pass_encripted[i] = partner_pass_encripted_len32[i];
	}

	char ss[100];

	sprintf_s(ss, 90, "set_partner_pass_and_id %02X-%02X-%02X ", partner_pass_encripted[0], partner_pass_encripted[1], partner_pass_encripted[2] );

	sudp(ss);

	partner_id = partner_id_;
	set__status(L"partner_id received");
	partner_pass_and_id_is_set = true;
}

int NET_CLIENT_SESSION::Client_Main_Loop(SOCKET sos) {

	unsigned char *cmd;
	unsigned int *sz, *crc32, *type, *sol, *old_screen_id, *session_no, *reserv3, *reserv4, *ii;

	unsigned char *buffer;

	buffer = new unsigned char[500000]; //neew(50000, buffer);

	bool local_stop;
	unsigned char *bb;
	bb = new unsigned char[500000]; //neew(50000, bb);

	recv__counter = 0;
	send__counter = 0;

	int res, res2, snd, z, zz;
	MY_CRC crc;

	local_stop = false;

	boost::posix_time::milliseconds SleepTime(10);
		
	char ss[100];

	while (GLOBAL_STOP == false && local_stop == false) {

		//send_udp("loop");
		
		//

		res = my_recv(sos, bb, 500000, &recv__counter);
		if (res > 0) { // arrived

			 

			//recv__counter__time = GetTickCount();

			
			ss[0] = 0;
			sprintf_s(ss, 90, "READ %d", res);
			sudp(ss);

			READ(bb, res);
			if(parent_low_level != nullptr) parent_low_level->invalidate();
		}
		if (res < 0) { // disconnect 
			local_stop = true;
		}
		if (res == 0) {
			boost::this_thread::sleep(SleepTime);
		};

		if (need_send_ALL_KEY_UP_event == true) {
			need_send_ALL_KEY_UP_event = false;
			out_event_queue.addd(0, 5556, 0, 0, "5");
		};

		if ( need_send_TRANSFER_CANCELED == true ) {

			cmd = (unsigned char *)bb;

			sz = (unsigned int *)&(cmd[0]);
			crc32 = (unsigned int *)&(cmd[4]);
			type = (unsigned int *)&(cmd[8]);
			sol = (unsigned int *)&(cmd[12]);

			zz = 32;

			*sz = zz;
			*crc32 = 0;
			*type = PACKET_TYPE_TRANSFER_from_SRV_to_CLI_CANCELED;
			*sol = get_sol();

			aes_partner.encrypt_stream(&(bb[0]), zz);


			int ss = 0;
			while (ss < zz && GLOBAL_STOP == false) {
				snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__counter);
				if (snd > 0) {
					ss += snd;
					//send__counter += snd; 
					send__counter__time = GetTickCount();
				};
				if (snd == -1) break;
				if (snd == 0) Sleep(1);
			};

			need_send_TRANSFER_CANCELED = false;
		}

		if (need_send_delete_cancel == true) {

			cmd = (unsigned char *)bb;

			sz = (unsigned int *)&(cmd[0]);
			crc32 = (unsigned int *)&(cmd[4]);
			type = (unsigned int *)&(cmd[8]);
			sol = (unsigned int *)&(cmd[12]);

			zz = 32;

			*sz = zz;
			*crc32 = 0;
			*type = PACKET_TYPE_need_delete_files_list_CANCEL;
			*sol = get_sol();

			aes_partner.encrypt_stream(&(bb[0]), zz);


			int ss = 0;
			while (ss < zz && GLOBAL_STOP == false) {
				snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__counter);
				if (snd > 0) {
					ss += snd;
					//send__counter += snd; 
					send__counter__time = GetTickCount();
				};
				if (snd == -1) break;
				if (snd == 0) Sleep(1);
			};

			need_send_delete_cancel = false;
		}

		if (need_request_files_list == true) {  // request FILE LIST
			need_request_files_list = false;


			cmd = (unsigned char *)bb;
			sz = (unsigned int *)&(cmd[0]);
			crc32 = (unsigned int *)&(cmd[4]);
			type = (unsigned int *)&(cmd[8]);
			sol = (unsigned int *)&(cmd[12]);

			//*sz = ;
			*crc32 = 0;
			*type = PACKET_TYPE_request_file_list_1;
			*sol = get_sol();

			wchar_t *ww;

			ww = (wchar_t *)&(cmd[16]);

			int i, zz = 0;
			i = 0;
			while (i < 5000 && need_request_files_list_DIR[i] != 0) {
				ww[i] = need_request_files_list_DIR[i];
				i++;
			}
			ww[i++] = 0;

			//send_udp2(ww);

			zz = ((i) * 2) / 16;
			zz *= 16;
			if (zz < (i * 2)) zz += 16;


			*crc32 = crc.calc(&(bb[16]), zz);
			zz += 16;
			*sz = zz;
			aes_partner.encrypt_stream(&(bb[0]), zz);


			int ss = 0;
			while (ss < zz && GLOBAL_STOP == false) {
				snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__counter);
				if (snd > 0) {
					ss += snd;
					//send__counter += snd; 
					send__counter__time = GetTickCount();
				};
				if (snd == -1) break;
				if (snd == 0) Sleep(1);
			};

		}

		if ( need_start_screenflow_from_server == true) {
			need_start_screenflow_from_server = false;
			


			cmd = (unsigned char *)bb;
			sz = (unsigned int *)&(cmd[0]);
			crc32 = (unsigned int *)&(cmd[4]);
			type = (unsigned int *)&(cmd[8]);
			sol = (unsigned int *)&(cmd[12]);
			old_screen_id = (unsigned int *)&(cmd[16]);
			session_no = (unsigned int *)&(cmd[20]);
			reserv3 = (unsigned int *)&(cmd[24]);
			reserv4 = (unsigned int *)&(cmd[28]);

			*sz = 32;
			*crc32 = 0;
			*type = need_start_screenflow_from_server_FORMAT_VER;
			*sol = get_sol();
			*old_screen_id = 0;
			*session_no = 0;
			*reserv3 = last_commit_scr_id;
			if (viewer == NULL) {
				*reserv4 = 0;
			}
			else {
				if (viewer->is_sync_clipboards) {
					*reserv4 = 1;
				}
				else {
					*reserv4 = 2;
				}
			}

			*crc32 = crc.calc(&(cmd[8]), (8 + 16));

			aes_partner.encrypt_stream(cmd, 32);

			sudp("CLIENT send recuest screen...");

			snd = my_send(sos, (unsigned char *)bb, 32, 0, "", &send__counter);
			if (snd > 0) {
				//send__counter += snd;
				send__counter__time = GetTickCount();
			};
			//DT_request_screen = GetTickCount();
			//DT_delay_responce_screen = DT_request_screen - DT_responce_screen;

		};

		if (need_start_sendCLIPBOARD_to_server == true) {
			need_start_sendCLIPBOARD_to_server = false;

			/* 2021 09 clipboard
			//send_udp("need_start_sendCLIPBOARD_to_server == true");

			clipboard.clean();
			clipboard._get();

			if (clipboard.buf_size > 0) {

				//send_udp("clipboard.buf_size = ", clipboard.buf_size);


				z = clipboard.buf_size + 32;

				zz = z / 16;
				zz *= 16;
				if (zz < z) zz += 16;

				cmd = new unsigned char[zz + 32];
				zero_unsigned_char(cmd + 32, zz);
				for (unsigned int i = 0; i < clipboard.buf_size; i++) {
					cmd[i + 32] = clipboard.buf[i];
				}
				sz = (unsigned int *)&(cmd[0]);
				//crc = (unsigned int *)&(cmd[4]);
				type = (unsigned int *)&(cmd[8]);
				sol = (unsigned int *)&(cmd[12]);
				ii = (unsigned int *)&(cmd[16]);

				*sz = zz;
				//*crc = 0;
				*type = PACKET_TYPE_responce_userCLIPBOARD_list;
				*sol = get_sol();
				*ii = clipboard.buf_size;

				aes_partner.encrypt_stream(cmd, zz);

				int ss = 0;
				while (ss < zz && GLOBAL_STOP == false) {
					snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__counter);
					if (snd > 0) {
						ss += snd;
						//send__counter += snd; 
						send__counter__time = GetTickCount();
					};

					if (snd == -1) break;
					if (snd == 0) Sleep(1);
				};

				ss = 1;
				delete[] cmd;


			};
			clipboard 2021 09 ***/
		};

		if (need_start_requestCLIPBOARD_from_server == true) {
			need_start_requestCLIPBOARD_from_server = false;

			//send_udp("need_start_requestCLIPBOARD_from_server == true");

			unsigned char *cmd;
			//cmd = neww unsigned char[32];
			cmd = (unsigned char *)bb;
			unsigned int *sz, *crc32, *type, *sol, *old_screen_id, *session_no, *reserv3, *reserv4;
			sz = (unsigned int *)&(cmd[0]);
			crc32 = (unsigned int *)&(cmd[4]);
			type = (unsigned int *)&(cmd[8]);
			sol = (unsigned int *)&(cmd[12]);
			old_screen_id = (unsigned int *)&(cmd[16]);
			session_no = (unsigned int *)&(cmd[20]);
			reserv3 = (unsigned int *)&(cmd[24]);
			reserv4 = (unsigned int *)&(cmd[28]);

			*sz = 32;
			*crc32 = 0;
			*type = PACKET_TYPE_request_userCLIPBOARD_list;
			*sol = get_sol();
			*old_screen_id = 0;
			*session_no = 0;
			*reserv3 = last_commit_scr_id;
			*reserv4 = 4;

			*crc32 = crc.calc(&(cmd[8]), (8 + 16));

			aes_partner.encrypt_stream(cmd, 32);

			snd = my_send(sos, (unsigned char *)bb, 32, 0, "", &send__counter);
			if (snd > 0) {
				//send__counter += snd;
				send__counter__time = GetTickCount();
			}


		};

		if (transfer_file_buffer_need_send == true) { // нужно передать партнеру кусок файла

			//send_udp("transfer_file_buffer_need_send == true ", transfer_file_buffer_len);

			int ss = 0;
			while (ss < transfer_file_buffer_len && GLOBAL_STOP == false) {
				snd = my_send(sos, (transfer_file_buffer + ss), transfer_file_buffer_len - ss, 0, "", &send__counter);
				if (snd > 0) {
					ss += snd;
					//send__counter += snd; 
					send__counter__time = GetTickCount();
				};
				if (snd == -1) break;
				if (snd == 0) Sleep(1);
			};

			transfer_file_buffer_need_send = false;
		}

		if (buf_tdn_need_send == true) { // нужно передать партнеру команду на создание папки


			unsigned int ss = 0;
			while (ss < buf_tdn_len && GLOBAL_STOP == false) {
				snd = my_send(sos, (buf_tdn + ss), buf_tdn_len - ss, 0, "", &send__counter);
				if (snd > 0) {
					ss += snd;
					//send__counter += snd; 
					send__counter__time = GetTickCount();
				};
				if (snd == -1) break;
				if (snd == 0) Sleep(1);
			};

			buf_tdn_need_send = false;
		}

		if (1 == 1) { // 10
			//unsigned short event_type, event_data1, event_data2;
			//event_type = 0;
			//out_event_queue.get(event_type, event_data1, event_data2);


			uint16_t global_type, e_type, e_data1, e_data2, old_e_type;
			uint64_t *i64; int vv;
			vv = 0;


			i64 = (uint64_t *)&(buffer[32]);
			old_e_type = 0;

			if (out_event_queue.element[0].type == 0) {
				if (need_set_mouse_x != -1 && last_set_need_set_mouse + 250 < GetTickCount()) {
					out_event_queue.addd(0, 5555, (uint16_t)need_set_mouse_x, (uint16_t)need_set_mouse_y, "5");
					need_set_mouse_x = -1;
					need_set_mouse_y = -1;
					last_set_need_set_mouse = GetTickCount();
				};
			}

			while (vv < 20 && out_event_queue.get(global_type, e_type, e_data1, e_data2)) {
				//send_udp("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>> EVENT!! %d %d %d ", e_type, e_data1, e_data2);
				//net.send_event_to_partner( e_type, e_data1, e_data2 );
				if (vv > 0 && e_type == 5555 && old_e_type == 5555) {
					vv--;
					i64 -= 3;
				}
				//if (global_type != 0) 
					//send_udp("global_type = ", global_type);
				*i64 = ((uint64_t)global_type << (uint64_t)32) | (uint64_t)e_type;
				i64++;
				*i64 = e_data1;
				i64++;
				*i64 = e_data2;
				i64++;
				vv++;

				old_e_type = e_type;
			}
			if (vv > 0) {
				i64 = (uint64_t *) &(buffer[16]);
				*i64 = vv;
				unsigned int *sz, *crc, *type, *sol, ss; // buf_size, 

				ss = vv * 24;
				ss /= 16;
				ss *= 16;
				if (ss < (unsigned int)vv * 24) ss += 16;
				ss += 32;

				sz = (unsigned int *) &(buffer[0]);
				crc = (unsigned int *) &(buffer[4]);
				type = (unsigned int *) &(buffer[8]);
				sol = (unsigned int *) &(buffer[12]);
				*sz = ss;
				*crc = 0;
				*type = PACKET_TYPE_send_keyboard_event_list_from_client_to_server;
				*sol = 0;

				aes_partner.encrypt_stream((byte *)buffer, ss);

				snd = my_big_send(sos, buffer, ss, &send__counter);

				//send_udp("send_ events ", ss, vv, old_e_type );

				//ALOG("======== vv=%d ss=%d ", vv, ss );
				if (snd != ss) {
					local_stop = true;
				}
				//return ss;
			};

			  
		} // 10

	}

	out_event_queue.clean();

	delete[] bb;
	delete[] buffer;

	if (recv__counter == 0) {
		
		set__status(L"Password incorrect");
		
		if (parent_low_level != nullptr) parent_low_level->invalidate();
		return -1;
	}
	return 0;

}

int NET_CLIENT_SESSION::READ( byte *buffer, int buffer_size) {

	add_to_low_level_encoded_buffer((unsigned char *)buffer, buffer_size);

	while (detect_full_command_in_to_low_level_buffer() == true) //  <<<<==================
	{

	}

	if (parent_low_level != nullptr) parent_low_level->invalidate();

	return 0;
}

void NET_CLIENT_SESSION::add_to_low_level_encoded_buffer(unsigned char *buf, unsigned int size) {
	unsigned int i, j, jj;

	//ALOG("add_to_low_level_encoded_buffer size=%d ", size );

	if (low_level_encoded_buffer_read_idx + size + 5500 > llow_level_encoded_buffer_read_max_count) {
		increase_llow_level_encoded_buffer_read(size + 5500);
	}

	i = 0;
	while (i < size) {

		llow_level_encoded_buffer_read[low_level_encoded_buffer_read_idx++] = buf[i++];

		if (low_level_encoded_buffer_read_idx >= llow_level_encoded_buffer_read_max_count) {

			fatal_error("NET_SESSION::add_to_low_level_encoded_buffer fatal error");
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

void NET_CLIENT_SESSION::increase_llow_level_encoded_buffer_read(int add_size) {
	unsigned char *q;
	unsigned int ss, i;
	ss = llow_level_encoded_buffer_read_max_count + add_size;

	if (ss > 5000000) {
		//send_udp("WARNING NET_CLIENT_SESSION::increase_llow_level_encoded_buffer_read() ss > 5000000");
		//return;
	}
	if (ss > 25000000) {
		//send_udp("WARNING NET_CLIENT_SESSION::increase_llow_level_encoded_buffer_read() ss > 9000000");
		//return;
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

bool NET_CLIENT_SESSION::detect_full_command_in_to_low_level_buffer() {

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

	if (*sz > 3000000) {
		//ALOG("detect_full_command_in_to_low_level_buffer whf?? *sz > 3000000");
		//send_udp("WARNING detect_full_command_in_to_low_level_buffer whf?? *sz > 3000000");
		// 2019 10 need_disconnect = true;
		//return false;
	}

	if (*sz > low_level_buffer_read_idx) {
		//if (MODE == MODE_server) set_status(L"not complete command ");
		//ALOG("not complete command");
		//fatal_error("detect_full_command_in_to_low_level_buffer *sz > low_level_buffer_read_idx");
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

void NET_CLIENT_SESSION::analiz_command(unsigned char *buf) {


	if (viewer != nullptr) {
		if (viewer->view_mode == VIEW_MODE_NOCONNECT) {
			viewer->change_view_mode(VIEW_MODE_STRETCH);
		}
	}
	//2019+ ENCODED_SCREEN_8bit_header *header;


	unsigned long long *iii;
	unsigned int *sz, *crc, *type, *ii, i;// , *old_screen_id;

	sz = (unsigned int *)&(buf[0]);
	crc = (unsigned int *)&(buf[4]);
	type = (unsigned int *)&(buf[8]);

	//char ss[500];
	//sprintf_ s(ss, 450, "analiz_command() sz=%d crc=%d type=%d ", sz, crc, type);

	if (*crc != 0) {
		/*c1 = CRC32_short((void *)&(buf[8]), (8 + 16));
		if (c1 != *crc) {
			ALOG("analiz_command() fatal_error");
			fatal_error("analiz_command error 1 crc");
			return;
		}*/
	};

	//ALOG("type = %X (%d) ", *type, *type );

	ENCODED_SCREEN_8bit_header *header;

	if ( *type == PACKET_TYPE_responce_screen_ver11 ||
		 *type == PACKET_TYPE_responce_screen_ver22 ||
		 *type == PACKET_TYPE_responce_screen_ver33
		
		) {

		recv_responce_screen = GetTickCount();

		need_start_screenflow_from_server = true;

		header = (ENCODED_SCREEN_8bit_header *)&(buf[0]);

		if (header->mouse_cursor_type_id != 0) {
			//char ss[500];
			//sprintf_ s(ss, 490, "mouse_cursor_type_id = %d ", header->mouse_cursor_type_id);
			//send_udp(ss);
		}
		char ss[110];
		ss[0] = 0;
		sprintf_s( ss, 100, "sz=%d", *sz);
		sudp(ss);
		if (parent_func__arrived_screen != NULL) parent_func__arrived_screen(buf, *sz, *type);

		if (parent_low_level != nullptr) parent_low_level->invalidate();

		return;
	}


	if (*type == PACKET_TYPE_responce_filefolder_part) {

		//send_udp("PACKET_TYPE_responce_filefolder_part");

		ii = (unsigned int *)&buf[16];
		responce_from_partner_FILE_ID = *ii;

		ii = (unsigned int *)&buf[20];
		responce_from_partner_part_szie = *ii;


		if (responce_from_partner_part_szie > transfer_file_buffer_max_size) {
			//fatal_error(L"responce_from_partner_part_szie > transfer_file_buffer_max_size");
			GLOBAL_STOP = true;
			return;
		}

		if (transfer_file_buffer == nullptr) transfer_file_buffer = new unsigned char[transfer_file_buffer_max_size + 32];

		i = 0;
		while (i < responce_from_partner_part_szie) {
			transfer_file_buffer[i] = buf[i + 28];
			i++;
		}

		ii = (unsigned int *)&buf[24];
		responce_from_partner_result = *ii; // всегда должно быть последней строкой

		responce_from_partner_arrived = PACKET_TYPE_responce_filefolder_part;



	};

	if (*type == PACKET_TYPE_responce_filefolder_stat) {
		//send_udp("PACKET_TYPE_responce_filefolder_stat");

		ii = (unsigned int *)&buf[16];
		responce_from_partner_FILE_ID = *ii;



		iii = (unsigned long long *)&(buf[24]);

		responce_from_partner_FILE_size = *iii;
		iii++;
		responce_from_partner_FILE_date = *iii;

		ii = (unsigned int *)&buf[20];
		responce_from_partner_result = *ii; // всегда должно быть последней строкой

		responce_from_partner_arrived = PACKET_TYPE_responce_filefolder_stat;
	};


	if (*type == PACKET_TYPE_need_delete_files_list_intermediate) {
		//send_udp("PACKET_TYPE_need_delete_files_list_intermediate");

		ii = (unsigned int *)&buf[20];
		responce_from_partner_intermediate = *ii;


	};

	if (*type == PACKET_TYPE_need_delete_files_list_FINISH) {
		//send_udp("PACKET_TYPE_need_delete_files_list_FINISH");


		ii = (unsigned int *)&buf[16];
		responce_from_partner_FILE_ID = *ii;

		ii = (unsigned int *)&buf[20];
		responce_from_partner_result = *ii; // всегда должно быть последней строкой

		responce_from_partner_arrived = PACKET_TYPE_need_delete_files_list_FINISH;
	}

	if (*type == PACKET_TYPE_transfer_file_ROUND_2_commit) {
		//send_udp("PACKET_TYPE_transfer_file_ROUND_2_commit");
		transfer_file_buffer_commit_send_from_partner = true;
	}

	if (*type == PACKET_TYPE_transfer_file_ROUND_1_commit) {

		//send_udp("ANALIZ PACKET_TYPE_transfer_file_ROUND_1_commit");

		ii = (unsigned int *)&buf[16];
		responce_from_partner_FILE_ID = *ii;



		iii = (unsigned long long *)&buf[24];
		responce_from_partner_FILE_size = *iii; iii++;
		responce_from_partner_FILE_date = *iii;

		ii = (unsigned int *)&buf[20];
		responce_from_partner_result = *ii; // всегда должно быть последней строкой

		responce_from_partner_arrived = PACKET_TYPE_transfer_file_ROUND_1_commit;
	};

	if (*type == PACKET_TYPE_need_create_folder_commit) {

		//send_udp("ANALIZ PACKET_TYPE_need_create_folder_commit");

		ii = (unsigned int *)&buf[16];
		responce_from_partner_FILE_ID = *ii;

		ii = (unsigned int *)&buf[20];
		responce_from_partner_result = *ii;

		responce_from_partner_arrived = PACKET_TYPE_need_create_folder_commit;
	};

	if (*type == PACKET_TYPE_responce_file_list_1) {

		//send_udp("ANALIZ PACKET_TYPE_responce_file_list_1");
		if (viewer != nullptr) {
			viewer->request_FILE_LIST_from_partner_RESPONCE_1(buf + 16, (*sz) - 16);
		}
	}

	if (*type == PACKET_TYPE_responce_file_list_2) {

		//send_udp("ANALIZ PACKET_TYPE_responce_file_list_2");
		if (viewer != nullptr) {
			viewer->request_FILE_LIST_from_partner_RESPONCE_2(buf + 16, (*sz) - 16);
			responce_from_partner_result = 1;
		}
	}

	if (*type == PACKET_TYPE_responce_userlogin_list) {

		//send_udp("ANALIZ PACKET_TYPE_responce_userlogin_list");

	}

	if (*type == PACKET_TYPE_responce_userCLIPBOARD_list) {

		ii = (unsigned int *)&buf[16];

		//send_udp("ANALIZ PACKET_TYPE_responce_userCLIPBOARD_list = ", *sz, *ii);


		/* 2021 09 clipboard
		clipboard.clean();
		clipboard.paste_from_buffer(buf + 32, *ii);
		*/

		/* 2019 10
		clipboard.clean();
		clipboard.copy_from_buf(buf, *sz);
		clipboard.PASTE();
		*/

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
		header = (ENCODED_SCREEN_8bit_header *)&(buf[0]);

		ALOG("PACKET_TYPE_responce_screen РїСЂРёС€РµР» СЃРєСЂРёРЅ %d ", header->screen_id);

		screen_from_server.decode(buf, *sz, &server_screen);
		server_screen.scr_id = header->screen_id;
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
		//RECEIVE_KEYBOARD_EVENT_LIST_from_client_to_server(buf);
	}


};

void NET_CLIENT_SESSION::add_to_low_level_buffer(unsigned char *buf, int size) {
	//ALOG("add_to_low_level_buffer size=%d ", size );

	//char ss[500];
	//sprintf_ s(ss, 450, "add_to_low_level_buffer size=%d _%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X_ ! ", size, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9]);
	//send_udp(ss);

	if (low_level_buffer_read_idx + size + 5500 > llow_level_buffer_read_max_count) {
		increase_llow_level_buffer_read(size + 5500);
	}

	int i;
	i = 0;
	while (i < size) {
		llow_level_buffer_read[low_level_buffer_read_idx++] = buf[i++];
		if (low_level_buffer_read_idx >= llow_level_buffer_read_max_count) {
			fatal_error("NET_CLIENT_SESSION::add_to_low_level_buffer fatal error");
			return;
		};
	};

}

void NET_CLIENT_SESSION::increase_llow_level_buffer_read(int add_size) {
	unsigned char *q;
	unsigned int ss;
	unsigned int i;
	ss = llow_level_buffer_read_max_count + add_size;

	if (ss > 5000000) {
		//send_udp("WARNING NET_CLIENT_SESSION::increase_llow_level_buffer_read() ss > 5000000");

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
void NET_CLIENT_SESSION::send_CtrlAltDel() {
	out_event_queue.addd(0, event_ctrl_alt_del, 1, 1, "eq 25");
}
void NET_CLIENT_SESSION::send_Change_LNG() {
	out_event_queue.addd(0, WM_KEYDOWN, 0, 0x80, "eq 25");
}

unsigned int NET_CLIENT_SESSION::request_filefolder_stat(wchar_t *partner_name_, unsigned int *_is_file_folder, unsigned long long *_size, unsigned long long *_date, unsigned int file_ID, int *modal_result) {
	if (partner_name_ == nullptr) return false;
	if (partner_name_[0] == 0) return false;

	*_is_file_folder = 0;
	*_size = 0;
	*_date = 0;

	//unsigned char *buf;
	unsigned int i, j;

	i = my_strlen(partner_name_);
	j = (i + 1) * 2 + 32;

	if (i >= 5000) return false;

	//buf = new unsigned char[j + 100];

	unsigned int *sz, *crc, *sol, *type, zz, *GID;

	zz = j / 16;
	zz *= 16;
	if (zz < j) zz += 16;
	buf_tdn_len = zz;



	if (buf_tdn_max_size < buf_tdn_len + 20) {
		delete[] buf_tdn;
		buf_tdn_max_size = buf_tdn_len + 20;
		buf_tdn = new unsigned char[buf_tdn_max_size + 2];
	}

	sz = (unsigned int *)&(buf_tdn[0]);
	crc = (unsigned int *)&(buf_tdn[4]);
	type = (unsigned int *)&(buf_tdn[8]);
	sol = (unsigned int *)&(buf_tdn[12]);
	GID = (unsigned int *)&(buf_tdn[16]);

	*sz = buf_tdn_len;
	*crc = 0;
	*type = PACKET_TYPE_request_filefolder_stat;
	*sol = get_sol();
	*GID = file_ID;

	wchar_t *w;

	w = (wchar_t *)&buf_tdn[20];

	my_strcpy(w, partner_name_);

	aes_partner.encrypt_stream(buf_tdn, buf_tdn_len);

	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	buf_tdn_need_send = true;
	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && buf_tdn_need_send == true) { // ждем пока мы отправим запрос партнеру



		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {

			return false;
		}
	};



	timeout = GetTickCount();
	while (responce_from_partner_result == 0 && GLOBAL_STOP == false) { // ждем пока партнер нам ответит



		::Sleep(0);
		if (timeout + 15000 < GetTickCount()) {
			return false;
		}
	}

	if (responce_from_partner_result == 1) { // папка

	}
	if (responce_from_partner_result == 2) { // файл

	}
	if (responce_from_partner_result == 3) { // нет файла

	}
	if (responce_from_partner_result == 4) { // access deny

	}


	*_is_file_folder = responce_from_partner_result;
	*_size = responce_from_partner_FILE_size;
	*_date = responce_from_partner_FILE_date;


	return responce_from_partner_result;
}

bool NET_CLIENT_SESSION::request_file_part(unsigned int file_ID, unsigned char *buf, unsigned int buf_size, unsigned int *buf_size_readed, unsigned int start_from, int *modal_result) {

	unsigned int j;

	j = 32;


	unsigned int *sz, *crc, *sol, *type, zz, *GID, *ii;

	zz = j / 16;
	zz *= 16;
	if (zz < j) zz += 16;
	buf_tdn_len = zz;

	if (buf_tdn_max_size < buf_tdn_len + 20) {
		delete[] buf_tdn;
		buf_tdn_max_size = buf_tdn_len + 20;
		buf_tdn = new unsigned char[buf_tdn_max_size + 2];
	}

	sz = (unsigned int *)&(buf_tdn[0]);
	crc = (unsigned int *)&(buf_tdn[4]);
	type = (unsigned int *)&(buf_tdn[8]);
	sol = (unsigned int *)&(buf_tdn[12]);
	GID = (unsigned int *)&(buf_tdn[16]);
	ii = (unsigned int *)&(buf_tdn[20]);

	*sz = buf_tdn_len;
	*crc = 0;
	*type = PACKET_TYPE_request_filefolder_part;
	*sol = get_sol();
	*GID = file_ID;
	*ii = start_from;




	aes_partner.encrypt_stream(buf_tdn, buf_tdn_len);

	responce_from_partner_arrived = 0;

	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	buf_tdn_need_send = true;
	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && buf_tdn_need_send == true) { // ждем пока мы отправим запрос партнеру



		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {

			return false;
		}
	};



	timeout = GetTickCount();
	while (responce_from_partner_result == 0 && GLOBAL_STOP == false) { // ждем пока партнер нам ответит



		::Sleep(0);
		if (timeout + 15000 < GetTickCount()) {
			return false;
		}
	}

	if (buf_size < responce_from_partner_part_szie) return false;
	unsigned int i;
	i = 0;

	while (i < responce_from_partner_part_szie) {
		buf[i] = transfer_file_buffer[i];
		i++;
	}
	*buf_size_readed = responce_from_partner_part_szie;

	/*
	transfer_file_buffer

	responce_from_partner_FILE_ID
	responce_from_partner_part_szie

	responce_from_partner_result

	responce_from_partner_arrived
	*/
	//*_is_file_folder = responce_from_partner_r0esult;
	//*_size = responce_from_partner_FILE_size;
	//*_date = responce_from_partner_FILE_date;



	return true;
}

bool NET_CLIENT_SESSION::request_folder_content(unsigned int file_ID, wchar_t *partner_folder_name_, int *modal_result) {

	unsigned int j;

	j = (my_strlen(partner_folder_name_) + 1) * 2 + 32;


	unsigned int *sz, *crc, *sol, *type, zz, *GID, *ii;

	zz = j / 16;
	zz *= 16;
	if (zz < j) zz += 16;
	buf_tdn_len = zz;

	if (buf_tdn_max_size < buf_tdn_len + 20) {
		delete[] buf_tdn;
		buf_tdn_max_size = buf_tdn_len + 20;
		buf_tdn = new unsigned char[buf_tdn_max_size + 2];
	}

	zero_unsigned_char(buf_tdn, zz);

	sz = (unsigned int *)&(buf_tdn[0]);
	crc = (unsigned int *)&(buf_tdn[4]);
	type = (unsigned int *)&(buf_tdn[8]);
	sol = (unsigned int *)&(buf_tdn[12]);
	GID = (unsigned int *)&(buf_tdn[16]);
	ii = (unsigned int *)&(buf_tdn[20]);

	*sz = buf_tdn_len;
	*crc = 0;
	*type = PACKET_TYPE_request_file_list_2;
	*sol = get_sol();
	*GID = file_ID;

	wchar_t *w;

	w = (wchar_t*)&(buf_tdn[24]);

	my_strcpy(w, partner_folder_name_);


	aes_partner.encrypt_stream(buf_tdn, buf_tdn_len);

	responce_from_partner_arrived = 0;

	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	buf_tdn_need_send = true;
	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && buf_tdn_need_send == true) { // ждем пока мы отправим запрос партнеру

		if (*modal_result != 0) return false;

		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {

			return false;
		}
	};

	timeout = GetTickCount();
	while (responce_from_partner_result == 0 && GLOBAL_STOP == false) { // ждем пока партнер нам ответит

		if (*modal_result != 0) return false;

		::Sleep(0);
		if (timeout + 15000 < GetTickCount()) {
			return false;
		}
	}

	return true;
};

bool NET_CLIENT_SESSION::need_request_FilesList(wchar_t *folder_name) {
	if (need_request_files_list != false) return false;
	my_strcpy_s(need_request_files_list_DIR, 5000, folder_name);
	need_request_files_list = true;
	return true;
}

void NET_CLIENT_SESSION::delete_files_list_from_partner(unsigned char *buf, unsigned int buf_len) {

	if (buf_tdn_max_size < buf_len + 20) {
		delete[] buf_tdn;
		buf_tdn_max_size = buf_len + 20;
		buf_tdn = new unsigned char[buf_tdn_max_size + 2];
		zero_unsigned_char(buf_tdn, buf_tdn_max_size + 2);
	}

	unsigned int i;
	i = 0;
	while (i < buf_len) {
		buf_tdn[i] = buf[i];
		i++;
	}

	unsigned int *sz, *crc, *sol, *type, zz, *GID, file_ID;

	file_ID = get_sol();

	zz = buf_len / 16;
	zz *= 16;
	if (zz < buf_len) zz += 16;

	sz = (unsigned int *)&(buf_tdn[0]);
	crc = (unsigned int *)&(buf_tdn[4]);
	type = (unsigned int *)&(buf_tdn[8]);
	sol = (unsigned int *)&(buf_tdn[12]);
	GID = (unsigned int *)&(buf_tdn[16]);

	*sz = zz;
	*crc = 0;
	*type = PACKET_TYPE_need_delete_files_list;
	*sol = get_sol();
	*GID = file_ID;

	aes_partner.encrypt_stream(buf_tdn, zz);


	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	buf_tdn_len = zz;
	buf_tdn_need_send = true;
	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && buf_tdn_need_send == true) { // ждем пока мы отправим запрос партнеру

		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {
			return;
		}
	};

	responce_from_partner_intermediate = 0;
	timeout = GetTickCount();
	while (GLOBAL_STOP == false
		&& responce_from_partner_result == 0

		) {
		::Sleep(0);
		if (responce_from_partner_intermediate != 0) {

			wchar_t ss[500];
			swprintf_s(ss, 490, L" %X ", responce_from_partner_intermediate);
			viewer->file_transfer_dialog->modal_dialog_progress->label3->set_label(nullptr, ss);

			responce_from_partner_intermediate = 0;
			timeout = GetTickCount();
		}
		if (viewer->file_transfer_dialog->modal_dialog_progress->modal__result != 0) { // Пользователь асинхронно нажал CANCEL. Нужно послать партнеру команду на остановку поцесса удаления файлов/папок

			//send_udp2("MR=", viewer->file_transfer_dialog->modal_dialog_progress->modal_result);
			need_send_delete_cancel = true;

			while (GLOBAL_STOP == false && need_send_delete_cancel == true) { // ждем пока передадим партнеру
				::Sleep(1);
			}
			return;
		};


		if (timeout + 15000 < GetTickCount()) {
			return;
		}
	}


}

bool NET_CLIENT_SESSION::transfer_FILE_to_parnter(wchar_t *dest_target, wchar_t *local_path, wchar_t *name, bool overwrite_this_file) {

	unsigned int file_ID;
	BOOL r;
	HANDLE hFile;
	LARGE_INTEGER FileSize;

	wchar_t dest_full_name[5100];
	zero_wchar_t(dest_full_name, 5100);
	my_str_append(5000, dest_full_name, dest_target, name);

	file_ID = get_sol();

	wchar_t local_full_name[5100];
	zero_wchar_t(local_full_name, 5100);
	my_str_append(5000, local_full_name, local_path, name);

	if (viewer != nullptr && viewer->file_transfer_dialog != nullptr && viewer->file_transfer_dialog->modal_dialog_progress != nullptr) {
		viewer->file_transfer_dialog->modal_dialog_progress->label1->set_label(nullptr, local_full_name);
		viewer->file_transfer_dialog->modal_dialog_progress->label3->set_label(nullptr, L".");
		//send_udp2("...2");
	};

	hFile = CreateFile(local_full_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) return false;

	r = GetFileSizeEx(hFile, &FileSize);

	if (r == FALSE) {
		CloseHandle(hFile);
		return false;
	}

	FILETIME CreationTime, LastAccessTime, LastWriteTime;

	r = GetFileTime(hFile, &CreationTime, &LastAccessTime, &LastWriteTime);

	if (r == FALSE) {
		CloseHandle(hFile);
		return false;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	unsigned long long fs, dt, fs0, *z;
	fs = (unsigned long long)FileSize.LowPart | (((unsigned long long)FileSize.HighPart) << 32);

	z = (unsigned long long *)&CreationTime;

	dt = *z;

	unsigned int allow_overwrite;

	allow_overwrite = 0;
	if (overwrite_this_file) allow_overwrite = 1;

	int result;

	bool overwrite_one_file = false;

	do
	{
		allow_overwrite = 0;
		if (overwrite_one_file == true) allow_overwrite = 1;
		overwrite_one_file = false;

		result = transfer_FILE_to_parnter_RAUND_1(dest_full_name, fs, dt, file_ID, allow_overwrite);
		if (result == CLIENT_RAUND_1_result_OK_finish_ZEROSIZE) {
			CloseHandle(hFile);
			return true;
		}
		if (result == CLIENT_RAUND_1_result_OK) break;
		if (result == CLIENT_RAUND_1_result_SKIP) {
			CloseHandle(hFile);
			return true;
		}
		if (result == CLIENT_RAUND_1_result_CANCEL) {
			CloseHandle(hFile);
			return false;
		}
		if (result == CLIENT_RAUND_1_result_FATAL_ERROR) {
			CloseHandle(hFile);
			return false;
		}
		if (result == CLIENT_RAUND_1_result_OVERWRITE) {
			overwrite_one_file = true;
		}
	} while (result == CLIENT_RAUND_1_result_OVERWRITE);

	//if (transfer_FILE_to_parnter_RAUND_1(dest_full_name, fs, dt, file_ID, allow_overwrite) == false) {
	//	CloseHandle(hFile);
	//	return false;
	//}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (transfer_file_buffer == nullptr) transfer_file_buffer = new unsigned char[transfer_file_buffer_max_size + 32];

	DWORD readed;

	int kusok_idx;

	perf_counter.clean();

	kusok_idx = 0;
	fs0 = 0;
	do
	{
		readed = 0;
		for (int i = 0; i < 32; i++) transfer_file_buffer[i] = 0;
		r = ReadFile(hFile, transfer_file_buffer + 32, transfer_file_buffer_max_size, &readed, NULL);
		if (r == FALSE) {
			CloseHandle(hFile);
			return false;
		}
		if (readed > 0) {

			if (transfer_files_canceled == true) {
				CloseHandle(hFile);
				transfer_FILE_to_parnter_RAUND_cancel_transfer(file_ID);

				return false;
			};

			if (transfer_FILE_to_parnter_RAUND_2(transfer_file_buffer, readed, kusok_idx++, file_ID) == false) {
				CloseHandle(hFile);

				// TODO 2020 показать сообщение об ошибке передачи

				return false;
			}
			fs0 += readed;
			perf_counter.add(readed);
			wchar_t s[500], s1[500], s2[500], s3[500];
			unsigned long long vv;
			vv = perf_counter.get_per_sec();

			format_sz(s1, fs0);
			format_sz(s2, fs);
			format_sz_2(s3, vv);



			wsprintf(s, L"progress - %s/%s | %s/sec", s1, s2, s3);
			viewer->file_transfer_dialog->modal_dialog_progress->label3->set_label(nullptr, s);
			//send_udp2(s);
		};

	} while (readed > 0);

	CloseHandle(hFile);

	return true;
}

bool NET_CLIENT_SESSION::transfer_DIRECTORY_to_parnter(wchar_t *dest_target, wchar_t *local_path, wchar_t *name) {
	// нужно передать партнеру папку со всеми содержащимися файлами
	// 1 - передать название папки, что бы партнер ее создал у себя
	// 2 - передать файлы, если попадаются папки - рекурсия

	wchar_t full_name[5100];

	if (my_str_append(5000, full_name, dest_target, name) == false) return false;


	wchar_t dest_full_name[5100];
	zero_wchar_t(dest_full_name, 5100);
	my_str_append(5000, dest_full_name, dest_target, name);

	wchar_t local_full_name[5100];
	zero_wchar_t(local_full_name, 5100);
	my_str_append(5000, local_full_name, local_path, name);

	if (viewer != nullptr && viewer->file_transfer_dialog != nullptr && viewer->file_transfer_dialog->modal_dialog_progress != nullptr) {
		viewer->file_transfer_dialog->modal_dialog_progress->label1->set_label(nullptr, local_full_name);
		viewer->file_transfer_dialog->modal_dialog_progress->label3->set_label(nullptr, L".");
	};


	if (transfer_DIRECTORY_NAME(full_name, 123) == false) return false;
	//PACKET_TYPE_need_create_folder

	wchar_t ddir[5100];
	zero_wchar_t(ddir, 5100);

	my_str_append(5000, ddir, local_path, name);
	my_strcat_s(ddir, 5000, L"\\*.*");


	WIN32_FIND_DATA fd;

	HANDLE h;


	h = FindFirstFile(ddir, &fd);

	if (h != INVALID_HANDLE_VALUE) {
		do {

			if (transfer_files_canceled == true) return false;// CLIENT_RAUND_1_result_CANCEL; // где то асинхронно нажали CANCEL

			bool is_folder;
			unsigned long long sz, dt;
			sz = fd.nFileSizeLow;

			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
				sz = -1;
				is_folder = true;
			}
			else {
				is_folder = false;
			}

			dt = (unsigned long long)fd.ftLastWriteTime.dwHighDateTime << 32 | (unsigned long long)fd.ftLastWriteTime.dwLowDateTime;

			//FileTimeToLocalFileTime((FILETIME *)&dt, (FILETIME *)&dt); //transfer_DIRECTORY_to_parnter

			if ((fd.cFileName[0] == '.' && fd.cFileName[1] == 0) ||
				(fd.cFileName[0] == '.' && fd.cFileName[1] == '.'))
			{
			}
			else {
				//add(fd.cFileName, is_folder, sz, dt);
				if (is_folder == true) {
					if (transfer_DIRECTORY_to_parnter(dest_full_name, local_full_name, fd.cFileName) == false) {
						FindClose(h);
						return false;
					}
				}
				else {
					if (transfer_FILE_to_parnter(dest_full_name, local_full_name, fd.cFileName, false) == false) {
						FindClose(h);
						return false;
					}
				}
			};
		} while (FindNextFile(h, &fd));

		FindClose(h);
	}

	return true;
}

void NET_CLIENT_SESSION::transfer_FILE_to_parnter_RAUND_cancel_transfer(unsigned int file_ID) {

	unsigned int j;
	unsigned int *sz, *crc, *sol, *type, zz, *ii, *GID;



	j = 32;

	if (buf_tdn_max_size < j + 20) {
		delete[] buf_tdn;
		buf_tdn_max_size = j + 20;
		buf_tdn = new unsigned char[buf_tdn_max_size + 2];
	}

	zz = j / 16;
	zz *= 16;
	if (zz < j) zz += 16;
	buf_tdn_len = zz;

	sz = (unsigned int *)&(buf_tdn[0]);
	crc = (unsigned int *)&(buf_tdn[4]);
	type = (unsigned int *)&(buf_tdn[8]);
	sol = (unsigned int *)&(buf_tdn[12]);
	GID = (unsigned int *)&(buf_tdn[16]);
	ii = (unsigned int *)&(buf_tdn[20]);

	*sz = buf_tdn_len;
	*crc = 0;
	*type = PACKET_TYPE_transfer_file_ROUND_1_cancel_transfer;
	*sol = get_sol();
	*GID = file_ID;
	*ii = file_ID;

	aes_partner.encrypt_stream(buf_tdn, 32);

	//send_udp2("PACKET_TYPE_transfer_file_ROUND_1_cancel_transfer");

	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	buf_tdn_need_send = true;
	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && buf_tdn_need_send == true) { // ждем пока мы отправим запрос партнеру

		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {
			return;
		}
	};

}

bool NET_CLIENT_SESSION::transfer_FILE_to_parnter_RAUND_2(unsigned char *transfer_file_buffer, int len, int kusok_idx, unsigned int file_ID) {

	unsigned int j;
	unsigned int *sz, *crc, *sol, *type, zz, *GID, *kosok_size;

	j = len + 32;

	zz = j / 16;
	zz *= 16;
	if (zz < j) zz += 16;
	buf_tdn_len = zz;

	sz = (unsigned int *)&(transfer_file_buffer[0]);
	crc = (unsigned int *)&(transfer_file_buffer[4]);
	type = (unsigned int *)&(transfer_file_buffer[8]);
	sol = (unsigned int *)&(transfer_file_buffer[12]);
	GID = (unsigned int *)&(transfer_file_buffer[16]);
	kosok_size = (unsigned int *)&(transfer_file_buffer[20]);

	*sz = zz;
	*crc = 0;
	*type = PACKET_TYPE_transfer_file_ROUND_2;
	*sol = get_sol();
	*GID = file_ID;
	*kosok_size = len;

	aes_partner.encrypt_stream(transfer_file_buffer, zz);
	//send_udp2("PACKET_TYPE_transfer_file_ROUND_2");

	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	transfer_file_buffer_len = zz;
	transfer_file_buffer_need_send = true;
	transfer_file_buffer_commit_send_from_partner = 0;


	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && transfer_file_buffer_need_send == true) { // ждем пока мы отправим запрос партнеру
		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {
			return false;
		}
	};

	timeout = GetTickCount();
	while (GLOBAL_STOP == false && transfer_file_buffer_commit_send_from_partner == false) { // ждем пока мы придет подтверждение приема от партнера
		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {
			return false;
		}
	};



	return true;
}

int NET_CLIENT_SESSION::transfer_FILE_to_parnter_RAUND_1(wchar_t *dest_full_name, unsigned long long FileSize, unsigned long long LastWriteTime, unsigned int file_ID, unsigned int allow_overwrite) {
	// нужно передать партнеру имя файла, размер и дату изменения
	// партнер сделает временный файл
	// если у партнера нет столько места - выводим сообщение SKIP/CANSEL
	// если у партнера уже есть такой файл - выводим сообщение OVERWRITE/SKIP/CANCEL
	/*
	#define CLIENT_RAUND_1_result_OK 1
	#define CLIENT_RAUND_1_result_SKIP 2
	#define CLIENT_RAUND_1_result_OVERWRITE 3
	#define CLIENT_RAUND_1_result_FATAL_ERROR 4
	*/
	unsigned int j;
	unsigned int *sz, *crc, *sol, *type, zz, *GID, *allow_overwrite_;
	unsigned long long *iii;

	if (dest_full_name == nullptr) return false;

	j = 40 + my_strlen(dest_full_name) * 2 + 2;

	if (buf_tdn_max_size < j + 20) {
		delete[] buf_tdn;
		buf_tdn_max_size = j + 20;
		buf_tdn = new unsigned char[buf_tdn_max_size + 2];
	}

	zz = j / 16;
	zz *= 16;
	if (zz < j) zz += 16;
	buf_tdn_len = zz;

	sz = (unsigned int *)&(buf_tdn[0]);
	crc = (unsigned int *)&(buf_tdn[4]);
	type = (unsigned int *)&(buf_tdn[8]);
	sol = (unsigned int *)&(buf_tdn[12]);
	GID = (unsigned int *)&(buf_tdn[16]);
	iii = (unsigned long long *)&(buf_tdn[20]);

	*sz = buf_tdn_len;
	*crc = 0;
	*type = PACKET_TYPE_transfer_file_ROUND_1;
	*sol = get_sol();
	*GID = file_ID;

	*iii = FileSize;
	iii = (unsigned long long *)&(buf_tdn[28]);
	*iii = LastWriteTime;

	allow_overwrite_ = (unsigned int *)&(buf_tdn[36]);

	*allow_overwrite_ = allow_overwrite;

	wchar_t *w;

	w = (wchar_t *)&buf_tdn[40];

	my_strcpy(w, dest_full_name);

	aes_partner.encrypt_stream(buf_tdn, buf_tdn_len);

	//send_udp2("PACKET_TYPE_transfer_file_ROUND_1");

	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	buf_tdn_need_send = true;
	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && buf_tdn_need_send == true) { // ждем пока мы отправим запрос партнеру

		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {
			return CLIENT_RAUND_1_result_FATAL_ERROR;
		}
	};

	timeout = GetTickCount();
	while (responce_from_partner_result == 0 && GLOBAL_STOP == false) { // ждем пока партнер нам ответит

		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {
			return CLIENT_RAUND_1_result_FATAL_ERROR;
		}
	}
	/*
	responce_from_partner_result
	#define ROUND_1_RESULT_OK_tmp_file_created 1
	#define ROUND_1_RESULT_ERR_tmp_file_already_exists 2
	#define ROUND_1_RESULT_ERR_target_file_already_exists 3
	#define ROUND_1_RESULT_ERR_fatal 4
	*/

	if (responce_from_partner_result == ROUND_1_RESULT_OK_target_file_finish_ZEROSIZE) {
		return CLIENT_RAUND_1_result_OK_finish_ZEROSIZE;
	}
	if (responce_from_partner_result == ROUND_1_RESULT_OK_target_file_created) { // все ок
		return CLIENT_RAUND_1_result_OK;
	}
	if (responce_from_partner_result == ROUND_1_RESULT_ERR_target_file_already_exists) {

		if (viewer->file_transfer_dialog->OVERWRITE_ALL_FILES_mode == true) return CLIENT_RAUND_1_result_OVERWRITE;
		if (viewer->file_transfer_dialog->SKIP_ALL_FILES_mode == true) return CLIENT_RAUND_1_result_SKIP;

		viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->modal__result = 0;
		viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->label_file_name->set_label(nullptr, dest_full_name);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		wchar_t txt[500];
		SYSTEMTIME st;

		FileTimeToSystemTime((FILETIME *)&LastWriteTime, &st); // dest_full_name
		swprintf_s(txt, 490, L"sz=%lld %04d.%02d.%02d %02d:%02d:%02d", FileSize, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);


		//FileSize
		//LastWriteTime

		viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->label_new_file_info->set_label(nullptr, txt);



		FileTimeToSystemTime((FILETIME *)&responce_from_partner_FILE_date, &st);
		swprintf_s(txt, 490, L"sz=%lld %04d.%02d.%02d %02d:%02d:%02d", responce_from_partner_FILE_size, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		//swprintf_s(txt, 490, L"%lld %lld", responce_from_partner_FILE_size, responce_from_partner_FILE_date);
		viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->label_old_file_info->set_label(nullptr, txt);

		viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->set_visible(nullptr, true); // запрашиваем нашего пользователя OVERWRITE/SKIP/CANCEL

		while (viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->modal__result == 0 && GLOBAL_STOP == false) {
			::Sleep(1);
		}

		if (viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->modal__result == MR_OVERWRITE) { // наш пользователь выбрал OVERWRITE

			if (viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->chbox_REMEMBER->is_pressed) viewer->file_transfer_dialog->OVERWRITE_ALL_FILES_mode = true;
			return CLIENT_RAUND_1_result_OVERWRITE;
		}

		if (viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->modal__result == MR_CONFIRM_SKIP) { // наш пользователь выбрал SKIP
			if (viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->chbox_REMEMBER->is_pressed) viewer->file_transfer_dialog->SKIP_ALL_FILES_mode = true;
			return CLIENT_RAUND_1_result_SKIP;
		}
		if (viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->modal__result == MR_CONFIRM_CANCEL ||
			viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->modal__result == MR_CANCEL
			) { // наш пользователь выбрал CANCEL
			return CLIENT_RAUND_1_result_CANCEL;
		}
		return CLIENT_RAUND_1_result_FATAL_ERROR;
	}
	if (responce_from_partner_result == ROUND_1_RESULT_ERR_fatal) { // все плохо. прерываем
		return CLIENT_RAUND_1_result_FATAL_ERROR;
	}


	if (responce_from_partner_result == 2) { // не удалось создатать файл на клиенте.  либо нет прав, либо есть папка с таким именем

		viewer->file_transfer_dialog->modal_dialog_skip->modal__result = 0;

		viewer->file_transfer_dialog->modal_dialog_skip->set_visible(nullptr, true); // тут асинхронно вызывается модальный диалог SKIP/CANCEL

		while (viewer->file_transfer_dialog->modal_dialog_skip->modal__result == 0 && GLOBAL_STOP == false) {

			::Sleep(1);
		}

		if (viewer->file_transfer_dialog->modal_dialog_skip->modal__result == MR_SKIP_SKIP) {
			return CLIENT_RAUND_1_result_SKIP;
		}

		if (viewer->file_transfer_dialog->modal_dialog_skip->modal__result == MR_SKIP_CANCEL ||
			viewer->file_transfer_dialog->modal_dialog_skip->modal__result == MR_CANCEL
			) {
			return CLIENT_RAUND_1_result_CANCEL;
		}
	}


	return CLIENT_RAUND_1_result_FATAL_ERROR;
}

bool NET_CLIENT_SESSION::transfer_DIRECTORY_NAME(wchar_t *full_name, unsigned int file_ID) {


	unsigned int j;
	unsigned int *sz, *crc, *sol, *type, zz, *GID;

	if (full_name == nullptr) return false;

	j = my_strlen(full_name) * 2 + 20 + 2;

	if (buf_tdn_max_size < j + 20) {
		delete[] buf_tdn;
		buf_tdn_max_size = j + 20;
		buf_tdn = new unsigned char[buf_tdn_max_size + 1];
	}

	zz = j / 16;
	zz *= 16;
	if (zz < j) zz += 16;
	buf_tdn_len = zz;

	sz = (unsigned int *)&(buf_tdn[0]);
	crc = (unsigned int *)&(buf_tdn[4]);
	type = (unsigned int *)&(buf_tdn[8]);
	sol = (unsigned int *)&(buf_tdn[12]);
	GID = (unsigned int *)&(buf_tdn[16]);


	*sz = buf_tdn_len;
	*crc = 0;
	*type = PACKET_TYPE_need_create_folder;
	*sol = get_sol();
	*GID = file_ID;

	wchar_t *w;

	w = (wchar_t *)&buf_tdn[20];

	my_strcpy(w, full_name);

	aes_partner.encrypt_stream(buf_tdn, buf_tdn_len);

	//send_udp2("PACKET_TYPE_need_create_folder");

	responce_from_partner_result = 0;
	responce_from_partner_FILE_ID = 0;

	buf_tdn_need_send = true;
	DWORD timeout = GetTickCount();
	while (GLOBAL_STOP == false && buf_tdn_need_send == true) { // ждем пока мы отправим запрос партнеру



		::Sleep(0);
		if (timeout + 5000 < GetTickCount()) {
			return false;
		}
	};

	timeout = GetTickCount();
	while (responce_from_partner_result == 0 && GLOBAL_STOP == false) { // ждем пока партнер нам ответит



		::Sleep(0);
		if (timeout + 15000 < GetTickCount()) {
			return false;
		}
	}

	if (responce_from_partner_result == 2) { // не удалось создатать папку на клиенте.  либо нет прав, либо есть файл с таким именем

		viewer->file_transfer_dialog->modal_dialog_skip->modal__result = 0;
		viewer->file_transfer_dialog->modal_dialog_skip->set_visible(nullptr, true);

		while (viewer->file_transfer_dialog->modal_dialog_skip->modal__result == 0 && GLOBAL_STOP == false) {



			::Sleep(1);
		}

		if (viewer->file_transfer_dialog->modal_dialog_skip->modal__result == MR_SKIP_SKIP) {
			return true;
		}

		if (viewer->file_transfer_dialog->modal_dialog_skip->modal__result == MR_SKIP_CANCEL ||
			viewer->file_transfer_dialog->modal_dialog_skip->modal__result == MR_CANCEL
			) {
			return false;
		}
	}

	return true;
}
