#include "stdafx.h"

#include "TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY.h"
#include "TRANSFER_DIALOG2.h"
#include "VIEWER.h"
#include "NET_SERVER_SESSION.h"
#include "MODAL_DIALOG.h"
#include "TRANSFER_DIALOG2_DirsFiles_Label.h"
#include "TRANSFER_DIALOG2_DirsFiles_Button.h"
#include "FILE__LIST.h"
#include "tools.h"

extern bool GLOBAL_STOP;

TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY(TRANSFER_DIALOG2 *parent_) {
	parent = parent_;
	clean();
}

TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::~TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY() {

}

void TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::clean() {

}

unsigned int TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::request_filefolder_stat_(wchar_t *partner_name_, unsigned int *_is_file_folder, unsigned long long *_size, unsigned long long *_date, unsigned int file_ID, int *modal_result) { //запросим статистику по файлу / папке с именем partner_name
	
	return parent->viewer->net_client_session->request_filefolder_stat(partner_name_, _is_file_folder, _size, _date, file_ID, modal_result);
}

bool TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::request_file_part_(unsigned int file_ID, unsigned char *buf, unsigned int buf_size, unsigned int *buf_size_readed, unsigned int start_from, int *modal_result) {

	bool r;

	r = parent->viewer->net_client_session->request_file_part(file_ID, buf, buf_size, buf_size_readed, start_from, modal_result);


	return r;
}


bool TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::RUN_TRANSFER_from_PARTNER_to_MY(wchar_t *partner_name, wchar_t *my_name, int *modal_result) {

	BOOL b;
	bool r;

	wchar_t partner_name__[5100];
	zero_wchar_t(partner_name__, 5100);

	wchar_t my_name__[5100];
	zero_wchar_t(my_name__, 5100);
	

	

	// Копируем из partner_name в my_name
	// 1) запросим статистику по файлу/папке с именем partner_name
	unsigned int _is_file_folder; unsigned long long _size, _date;
	_is_file_folder = 0;
	_size = 0;
	_date = 0;

	unsigned int file_ID, result;

	file_ID = get_sol();

	parent->modal_dialog_progress->label1->set_label(nullptr, partner_name);

	//Sleep(3333);


	result = request_filefolder_stat_(partner_name, &_is_file_folder, &_size, &_date, file_ID, modal_result);
	if(result == 4) { // access deny
		
		// TODO
		parent->modal_dialog_skip->modal__result = 0;

		parent->modal_dialog_skip->label_warning->set_label(nullptr, L"label_warning");
		parent->modal_dialog_skip->label_info->set_label(nullptr, L"Access denied");
		parent->modal_dialog_skip->label_file_name->set_label(nullptr, L"label_file_name");

		parent->modal_dialog_skip->set_visible(nullptr, true);

		while (parent->modal_dialog_skip->modal__result == 0) {
			::Sleep(10);
		}
		parent->modal_dialog_skip->set_visible(nullptr, false);
		
		
		FINISH_TRANSFER_filefolder_from_Partner_to_My();
		return false;
	};

	if (_is_file_folder == 1) { // это папка
		// надо запросить у партнера список вложенных папок/файлов которые содержит эта папка

		my_CreateDirectory(my_name);

		FILE__LIST FL;

		FL.clean();

		r = request_folder_content_(file_ID, partner_name, &FL, modal_result);

		if (r == false) return false;

		int i;
		i = 0;
		while (i < FL.max_count && FL.item[i] != nullptr && FL.item[i]->is_use == true) {
			if (FL.item[i]->is_folder) { // это папка, нужно скопировать ее содержимое
	
				my_strcpy_s(partner_name__, 5000, partner_name);
				my_strcat_s(partner_name__, 5000, L"\\");
				my_strcat_s(partner_name__, 5000, FL.item[i]->name);

				my_strcpy_s(my_name__, 5000, my_name);
				my_strcat_s(my_name__, 5000, L"\\");
				my_strcat_s(my_name__, 5000, FL.item[i]->name);

				//send_udp2( L"FOLDER - ", partner_name__, my_name__);

				RUN_TRANSFER_from_PARTNER_to_MY(partner_name__, my_name__, modal_result);

			}
			else { // это файл, нужно скопировать

				my_strcpy_s(partner_name__, 5000, partner_name);
				my_strcat_s(partner_name__, 5000, L"\\");
				my_strcat_s(partner_name__, 5000, FL.item[i]->name);

				my_strcpy_s(my_name__, 5000, my_name);
				my_strcat_s(my_name__, 5000, L"\\");
				my_strcat_s(my_name__, 5000, FL.item[i]->name);

				//send_udp2(L"FILE - ", partner_name__, my_name__);

				RUN_TRANSFER_from_PARTNER_to_MY(partner_name__, my_name__, modal_result);

			}
			
			i++;
		}
		FINISH_TRANSFER_filefolder_from_Partner_to_My();
		return true;
	}

	if (my_FileExists(my_name)) {

		unsigned long long _sz = 0, _dt = 0;

		GetFileAttributes_(my_name, &_sz, &_dt);

		wchar_t txt[500];
		SYSTEMTIME st;

		


		parent->modal_dialog_confirm_overwrite->modal__result = 0;

		
		parent->modal_dialog_confirm_overwrite->label_file_name->set_label(nullptr, my_name);

		FileTimeToSystemTime((FILETIME *)&_dt, &st); // dest_full_name
		swprintf_s(txt, 490, L"sz=%lld %04d.%02d.%02d %02d:%02d:%02d", _sz, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		parent->modal_dialog_confirm_overwrite->label_old_file_info->set_label(nullptr, txt);

		FileTimeToSystemTime((FILETIME *)&_date, &st); // dest_full_name
		swprintf_s(txt, 490, L"sz=%lld %04d.%02d.%02d %02d:%02d:%02d", _size, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		parent->modal_dialog_confirm_overwrite->label_new_file_info->set_label(nullptr, txt);
		


		parent->modal_dialog_confirm_overwrite->set_visible(nullptr, true);

		while (parent->modal_dialog_confirm_overwrite->modal__result == 0) {
			::Sleep(10);
		}
		parent->modal_dialog_confirm_overwrite->set_visible(nullptr, false);

		if (parent->modal_dialog_confirm_overwrite->chbox_REMEMBER->is_pressed) {

		}

		if (parent->modal_dialog_confirm_overwrite->modal__result == MR_OVERWRITE) {
			if (try_delete_file(my_name) == false) {
				FINISH_TRANSFER_filefolder_from_Partner_to_My();
				return false;
			} 

		}
		if (parent->modal_dialog_confirm_overwrite->modal__result == MR_CONFIRM_CANCEL ||
			parent->modal_dialog_confirm_overwrite->modal__result == MR_CANCEL
			) {
			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;
		}
		if (parent->modal_dialog_confirm_overwrite->modal__result == MR_CONFIRM_SKIP) {
			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return true;
		}

		

		

	}
	else {

		if (my_DirectoryExists(my_name)) { // мы хотим создать файл а уже есть такая папка!

			parent->modal_dialog_skip->modal__result = 0;

			parent->modal_dialog_skip->label_warning->set_label(nullptr, L"label_warning");
			parent->modal_dialog_skip->label_info->set_label(nullptr, L"Can't create file");
			parent->modal_dialog_skip->label_file_name->set_label(nullptr, L"label_file_name");

			parent->modal_dialog_skip->set_visible(nullptr, true);

			while (parent->modal_dialog_skip->modal__result == 0) {
				::Sleep(10);
			}
			parent->modal_dialog_skip->set_visible(nullptr, false);

			if (parent->modal_dialog_skip->chbox_REMEMBER->is_pressed) {

			}

			if (parent->modal_dialog_skip->modal__result == MR_SKIP_SKIP) {
				FINISH_TRANSFER_filefolder_from_Partner_to_My();
				return true;
			}
			if (parent->modal_dialog_skip->modal__result == MR_SKIP_CANCEL ||
				parent->modal_dialog_skip->modal__result == MR_CANCEL
				) {
				FINISH_TRANSFER_filefolder_from_Partner_to_My();
				return false;
			}

			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;
		}

	};

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~ содаем локальный файл
	HANDLE hFILE;

	hFILE = CreateFile(my_name, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFILE == INVALID_HANDLE_VALUE) {	

		// TODO

		parent->modal_dialog_skip->modal__result = 0;

		parent->modal_dialog_skip->label_warning->set_label(nullptr, L"label_warning");
		parent->modal_dialog_skip->label_info->set_label(nullptr, L"Can't create file");
		parent->modal_dialog_skip->label_file_name->set_label(nullptr, my_name);

		parent->modal_dialog_skip->set_visible(nullptr, true);

		while (parent->modal_dialog_skip->modal__result == 0) {
			::Sleep(10);
		}
		parent->modal_dialog_skip->set_visible(nullptr, false);
		FINISH_TRANSFER_filefolder_from_Partner_to_My();
		return false;
	}

	unsigned char *buf;
	unsigned int buf_size, buf_size_readed;
	unsigned long long writed_size = 0;

	buf_size = filefolder_part_buf_max_size;
	buf = new unsigned char[buf_size];
	buf_size_readed = 0;

	
	DWORD writed;
	writed_size = 0;

	while (writed_size < _size) {

		if (parent->get_visible() == false) {
			CloseHandle(hFILE);
			delete[] buf;

			my_deletefile(my_name);

			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;
		}

		if (*modal_result != 0) {

			CloseHandle(hFILE);
			delete[] buf;

			my_deletefile(my_name);

			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;

		}

		r = request_file_part_(file_ID, buf, buf_size, &buf_size_readed, writed_size, modal_result);
		if (r == false) {
			// link error

			parent->modal_dialog_info->modal__result = 0;
			parent->modal_dialog_info->label_info->set_label(nullptr, L"Link error");
			parent->modal_dialog_info->set_visible(nullptr, true);
			while (parent->modal_dialog_info->modal__result == 0) { // ждем пока пользователь нажмет OK
				
				if (parent->get_visible() == false) {
					CloseHandle(hFILE);
					delete[] buf;

					my_deletefile(my_name);

					FINISH_TRANSFER_filefolder_from_Partner_to_My();
					return false;
				}
				
				::Sleep(100);
			}

			CloseHandle(hFILE);
			delete[] buf;

			my_deletefile(my_name);

			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;
		}
		if (buf_size_readed > buf_size) {
			// WTF ?

			parent->modal_dialog_info->modal__result = 0;
			parent->modal_dialog_info->label_info->set_label(nullptr, L"WTF?");
			parent->modal_dialog_info->set_visible(nullptr, true);
			while (parent->modal_dialog_info->modal__result == 0) { // ждем пока пользователь нажмет OK
				::Sleep(100);
			}

			CloseHandle(hFILE);
			delete[] buf;

			my_deletefile(my_name);

			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;
		}

		writed = 0;
		b = WriteFile(hFILE, buf, buf_size_readed, &writed, nullptr);
		if (b == FALSE) {
			parent->modal_dialog_info->modal__result = 0;
			parent->modal_dialog_info->label_info->set_label(nullptr, L"Write error1");
			parent->modal_dialog_info->set_visible(nullptr, true);
			while (parent->modal_dialog_info->modal__result == 0) { // ждем пока пользователь нажмет OK
				::Sleep(100);
			}

			CloseHandle(hFILE);
			delete[] buf;

			my_deletefile(my_name);

			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;
		}
		if (writed < buf_size_readed) {
			parent->modal_dialog_info->modal__result = 0;
			parent->modal_dialog_info->label_info->set_label(nullptr, L"Write error1");
			parent->modal_dialog_info->set_visible(nullptr, true);
			while (parent->modal_dialog_info->modal__result == 0) { // ждем пока пользователь нажмет OK
				::Sleep(100);
			}

			my_deletefile(my_name);

			CloseHandle(hFILE);
			delete[] buf;

			FINISH_TRANSFER_filefolder_from_Partner_to_My();
			return false;
		}
		
		writed_size += writed;
		wchar_t ss[500];
		
		float pp;

		pp = (float)writed_size / (float)_size * (float)100;

		//sprintf__s_wc_i( ss, 490, L"%s %d ", L"sz", (int)pp);

		try
		{
			swprintf_s(ss, 490, L"%.1f %s", pp, L"%");
		}
		catch (...)
		{
			ss[0] = '?';
			ss[1] = 0;
		};

		parent->modal_dialog_progress->label3->set_label(nullptr, ss);

	}

	

	CloseHandle(hFILE);
	delete[] buf;
	
	FINISH_TRANSFER_filefolder_from_Partner_to_My();
	return true;
}

bool TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::request_folder_content_(unsigned int file_ID, wchar_t *partner_folder_name_, FILE__LIST *fl, int *modal_result) {

	bool b;

	RESPONCE_2_fl = fl;

	folder_content_RESPONCE_result = 0;

	b = parent->viewer->net_client_session->request_folder_content(file_ID, partner_folder_name_, modal_result);
	
	if (b == false) {
		RESPONCE_2_fl = nullptr;
		return false;
	};

	while (GLOBAL_STOP == false && folder_content_RESPONCE_result == 0) { // ждем пока придет ответ от партнера

		if (*modal_result != 0) { // пользователь нажал CANCEL
			RESPONCE_2_fl = nullptr;
			return false;
		}

		::Sleep(0);
	}


	RESPONCE_2_fl = nullptr;

	return true;

}

void TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::request_folder_content_RESPONCE(unsigned char *buf, int buf_size) {
	if (RESPONCE_2_fl == nullptr) return;
	RESPONCE_2_fl->clean();
	RESPONCE_2_fl->set_from_buffer(buf, buf_size);

	folder_content_RESPONCE_result = 1;
}

bool TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::try_delete_file(wchar_t *file_name) {
	
	if (my_deletefile(file_name) == false) {

		parent->modal_dialog_info->modal__result = 0;

		parent->modal_dialog_info->label_info->set_label(nullptr, L"Can't delete file");

		parent->modal_dialog_info->set_visible(nullptr, true);

		while (parent->modal_dialog_info->modal__result == 0) {
			::Sleep(110);
		}

		return false;
	}

	return true;
}

void TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY::FINISH_TRANSFER_filefolder_from_Partner_to_My() {

	parent->viewer->net_client_session->need_send_TRANSFER_CANCELED = true;
	while (GLOBAL_STOP == false && parent->viewer->net_client_session->need_send_TRANSFER_CANCELED == true) {
		::Sleep(111);
	}
}