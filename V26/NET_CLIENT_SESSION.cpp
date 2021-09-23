#include "APPLICATION_ATTRIBUTES.h"
#include "NET_CLIENT_SESSION.h"
#include "NET_SERVER_SESSION.h"
#include "CRYPTO.h"
#include "VIEWER.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;
extern VIEWER  *viewer;

NET_CLIENT_SESSION::NET_CLIENT_SESSION() {
	
}

NET_CLIENT_SESSION::~NET_CLIENT_SESSION() {

}

void NET_CLIENT_SESSION::mouse_left_button_down(int mx, int my) {
	out_event_queue.addd(0, WM_LBUTTONDOWN__, (uint16_t)mx, (uint16_t)my, "5");
}

void NET_CLIENT_SESSION::mouse_left_button_up(int mx, int my) {
	out_event_queue.addd(0, WM_LBUTTONUP__, (uint16_t)mx, (uint16_t)my, "5");
}

void NET_CLIENT_SESSION::mouse_right_button_down(int mx, int my) {
	out_event_queue.addd(0, WM_RBUTTONDOWN__, (uint16_t)mx, (uint16_t)my, "5");
}

void NET_CLIENT_SESSION::mouse_right_button_up(int mx, int my) {
	out_event_queue.addd(0, WM_RBUTTONUP__, (uint16_t)mx, (uint16_t)my, "5");
}
void NET_CLIENT_SESSION::get_statistic(char *ss, int ss_max_size_) {

	sprintf__s_ull_ull(ss, ss_max_size_, "rcv:%lld snd:%lld ", recv__counter, send__countern);

}
void NET_CLIENT_SESSION::mouse_move(int mx, int my) {
	//out_event_queue.add(0, 5555, mx, my, "5");
	need_set_mouse_x = mx;
	need_set_mouse_y = my;
}
void NET_CLIENT_SESSION::mouse_whell(int msg, int wp, int lp) {
	out_event_queue.addd(0, (uint16_t)msg, (uint16_t)wp, (uint16_t)lp, "6");
}

void NET_CLIENT_SESSION::char_keydown(int msg, int wp, int lp) {
	out_event_queue.addd(0, (uint16_t)msg, (uint16_t)wp, (uint16_t)lp, "5");
}

void NET_CLIENT_SESSION::char_keyup(int msg, int wp, int lp) {
	out_event_queue.addd(0, (uint16_t)msg, (uint16_t)wp, (uint16_t)lp, "5");
}


void NET_CLIENT_SESSION::EXECUTE() {

	bool local_stop = false;
	int connect_try_count = 0;
	unsigned int proxy_ip = 0;
	int res;

	boost::posix_time::milliseconds SleepTime(100);

	EXECUTE_is_run = true;

	set_status(L"waiting for the partner_id");

	while (GLOBAL_STOP == false && local_stop == false) {

		if (partner_pass_and_id_is_set == true) {

			connect_try_count++;

			/////////////////////////////

			//net_client_session->


			/////////////////////////////

			if (my_priv_id == 0) {

				set_status(L"Get local id... ");

				Load_private_id_and_public_id_from_USER_registry(&my_pub_id, &my_priv_id);

				if (my_pub_id == 0) {

					set_status(L"Register... ");

					Register_new_partner_on_proxy();
				}
			}

			if (my_priv_id != 0) {


				set_status(L"Preparing to connect... ");
				sudp("get_IP_for_server_location()...");
				proxy_ip = get_IP_for_server_location(partner_id, my_pub_id, my_priv_id);

				if (proxy_ip < 100) {
					sudp("NCS IP not determine...");
					set_status(L"IP not determine... ");

					my_Slip(3000);
				}
				else {
					sudp("NCS IP determine Ok");

					set_status(L"Connect... ");

					res = Connect_to_server( partner_id, my_pub_id, my_priv_id, partner_pass_encripted, proxy_ip);

					if (res == -1) { // pass incorrect
						//local_stop = true;
					}
					my_Slip(3000);
				}
			}
			else {

				set_status(L"Connect error");

				my_Slip(3000);
			}

		} 


		boost::this_thread::sleep(SleepTime);
	}
	set_status(L"stoped");
	EXECUTE_is_run = false;

}


void NET_CLIENT_SESSION::set_status(wchar_t *v) {
	status = v;

	if (parent_low_level != nullptr) parent_low_level->invalidate();
}

int  NET_CLIENT_SESSION::Connect_to_server( unsigned long long partner_id, unsigned long long public_id, unsigned long long private_id, unsigned char pass_hash16[16], unsigned int proxy_ip) {
	if (public_id == 0 || private_id == 0 || proxy_ip < 100 || partner_id == 0) {
		return 0;
	};

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
				sos = proxy_list->try_connect(proxy_ip); // ������� ������������ ����� ������ +
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
			snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__countern);//ok
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

			if (s1005->result == 2) { // ������� ������
				//ALOG("key_for_commit_connection = {%X}", s1005->key);

				sudp("NCS partner founded");

				key_for_commit_connection = s1005->key;

				//********************************************
				zero_unsigned_char(bb, 128);

				PACKET_LEVEL1_1006_request *r1006;
				r1006 = (PACKET_LEVEL1_1006_request *)bb;

				r1006->key = key_for_commit_connection;
				r1006->sol = get_sol();

				aes_proxy.encrypt_16_byte(bb);
				//********************************************

				snd = my_send(sos, (unsigned char *)bb, 16, 0, "", &send__countern);//ok

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
					// ������ ������������, ��� �� ���������� ���������� � ���������
					sudp("Connected");

					set_status(L"Connected");

					connection_to_partner_established = true;
					need_start_screenflow_from_server = true;

					if (parent_func__connect != NULL) parent_func__connect();

					//send_udp("ClientConnect");

					res = Client_Main_Loop(sos);

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
				// ������� �� ������
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
	partner_id = partner_id_;
	set_status(L"partner_id received");
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
	send__countern = 0;

	int res, snd, z, zz;
	MY_CRC crc;

	local_stop = false;

	while (GLOBAL_STOP == false && local_stop == false) {

		//send_udp("loop");

		res = my_recv(sos, bb, 500000, &recv__counter);
		if (res > 0) { // arrived


			//recv__counter__time = GetTickCount();

			READ(bb, res);

		}
		if (res < 0) { // disconnect 
			local_stop = true;
		}
		if (res == 0) {
			::Sleep(1);
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
				snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__countern);
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
				snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__countern);
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
				snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__countern);
				if (snd > 0) {
					ss += snd;
					//send__counter += snd; 
					send__counter__time = GetTickCount();
				};
				if (snd == -1) break;
				if (snd == 0) Sleep(1);
			};

		}

		if (need_start_screenflow_from_server == true) {
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
			*type = PACKET_TYPE_request_start_screenflow;
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

			snd = my_send(sos, (unsigned char *)bb, 32, 0, "", &send__countern);
			if (snd > 0) {
				//send__counter += snd;
				send__counter__time = GetTickCount();
			};
			//DT_request_screen = GetTickCount();
			//DT_delay_responce_screen = DT_request_screen - DT_responce_screen;

		};

		if (need_start_sendCLIPBOARD_to_server == true) {
			need_start_sendCLIPBOARD_to_server = false;

			/* 2021 09
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
					snd = my_send(sos, (cmd + ss), zz - ss, 0, "", &send__countern);
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
			2021 09 ***/
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

			snd = my_send(sos, (unsigned char *)bb, 32, 0, "", &send__countern);
			if (snd > 0) {
				//send__counter += snd;
				send__counter__time = GetTickCount();
			}


		};

		if (transfer_file_buffer_need_send == true) { // ����� �������� �������� ����� �����

			//send_udp("transfer_file_buffer_need_send == true ", transfer_file_buffer_len);

			int ss = 0;
			while (ss < transfer_file_buffer_len && GLOBAL_STOP == false) {
				snd = my_send(sos, (transfer_file_buffer + ss), transfer_file_buffer_len - ss, 0, "", &send__countern);
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

		if (buf_tdn_need_send == true) { // ����� �������� �������� ������� �� �������� �����


			unsigned int ss = 0;
			while (ss < buf_tdn_len && GLOBAL_STOP == false) {
				snd = my_send(sos, (buf_tdn + ss), buf_tdn_len - ss, 0, "", &send__countern);
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

				snd = my_big_send(sos, buffer, ss, &send__countern);

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

		set_status(L"Password incorrect");

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

	if (*type == PACKET_TYPE_responce_screen) {

		recv_responce_screen = GetTickCount();

		need_start_screenflow_from_server = true;

		header = (ENCODED_SCREEN_8bit_header *)&(buf[0]);

		if (header->mouse_cursor_type_id != 0) {
			//char ss[500];
			//sprintf_ s(ss, 490, "mouse_cursor_type_id = %d ", header->mouse_cursor_type_id);
			//send_udp(ss);
		}

		if (parent_func__arrived_screen != NULL) parent_func__arrived_screen(buf, *sz);

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
		responce_from_partner_result = *ii; // ������ ������ ���� ��������� �������

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
		responce_from_partner_result = *ii; // ������ ������ ���� ��������� �������

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
		responce_from_partner_result = *ii; // ������ ������ ���� ��������� �������

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
		responce_from_partner_result = *ii; // ������ ������ ���� ��������� �������

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


		/* 2021 09
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
	if (*type == PACKET_TYPE_responce_screen) {
		//send_udp("PACKET_TYPE_responce_screen");
		/* 2019 +
		header = (ENCODED_SCREEN_8bit_header *)&(buf[0]);

		ALOG("PACKET_TYPE_responce_screen пришел скрин %d ", header->screen_id);

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


