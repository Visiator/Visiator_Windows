

#include "tools.h"

#pragma once

#if !defined(AFX_HIGHLEVEL_COMMAND_QUEUE_H_)
#define AFX_HIGHLEVEL_COMMAND_QUEUE_H_

class HIGHLEVEL_COMMAND_QUEUE_element
{
private:
	//wchar_t status[250];
public:
	int command_type;
	int command_info;
	unsigned char *command_data;
	unsigned int command_data_size;


	HIGHLEVEL_COMMAND_QUEUE_element(void);
};


class HIGHLEVEL_COMMAND_QUEUE
{
private:
	//wchar_t status[250];
	int queue_max_size = 0;
	bool is_locked_ = false;

	void lock(void);

	void unlock(void);

	//int get_len(void);
	int len;
public:

	//void set_status(wchar_t *s);
	//void set_status(wchar_t *s, int p2);
	//void set_status(wchar_t *s, int p2, int p3);

	//TMemo *memo;
	wchar_t *name;
	HIGHLEVEL_COMMAND_QUEUE_element *elements;

	int counter; // обще количество команд, которые были в очереди

	void free(void);

	bool add_element_(unsigned int type, int inf, unsigned char *command_data, unsigned int command_data_size);// , int tmp);

	bool add_element_low_level(unsigned int type, int info, unsigned char *command_data_, unsigned int command_data_size_);

	bool get_element_low_level(int &type, int &info, unsigned char * &command_data, unsigned int &command_data_size);

	int get_len(void) { return len; };

	void clean(void);

	HIGHLEVEL_COMMAND_QUEUE(void);

	~HIGHLEVEL_COMMAND_QUEUE();


};

#endif