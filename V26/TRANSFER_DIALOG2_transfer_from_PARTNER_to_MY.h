#include "tools.h"

#pragma once

class TRANSFER_DIALOG2;
class FILE__LIST;

class TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY
{
private:
	
public:
	TRANSFER_DIALOG2 *parent = nullptr;

	HANDLE hFile = 0;
	wchar_t *partner_filefolder_name = nullptr;
	wchar_t *my_filefolder_name = nullptr;

	
	int folder_content_RESPONCE_result = 0;
	FILE__LIST *RESPONCE_2_fl = nullptr;

	void clean();

	//****************************************************
	//**

	unsigned int request_filefolder_stat_(wchar_t *partner_name_, unsigned int *_is_file_folder, unsigned long long *_size, unsigned long long *_date, unsigned int file_ID, int *modal_result);

	bool request_file_part_(unsigned int file_ID, unsigned char *buf, unsigned int buf_size, unsigned int *buf_size_readed, unsigned int start_from, int *modal_result);

	bool request_folder_content_(unsigned int file_ID, wchar_t *partner_folder_name_, FILE__LIST *fl, int *modal_result);
	void request_folder_content_RESPONCE(unsigned char *buf, int buf_size);


	//**
	//*****************************************************

	bool RUN_TRANSFER_from_PARTNER_to_MY(wchar_t *partner_name, wchar_t *my_name, int *modal_result);
	void FINISH_TRANSFER_filefolder_from_Partner_to_My();
	bool try_delete_file(wchar_t *file_name);

	TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY(TRANSFER_DIALOG2 *parent_);
	~TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY();
};

