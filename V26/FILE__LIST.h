#pragma once

class GUI_low_level;
class TRANSFER_DIALOG2_DirsFiles_TREE_element;

class FILE__LIST_element
{
public:
	bool is_use = false;
	wchar_t *name = nullptr;
	int name_size = 0;
	bool is_folder = false;
	unsigned long long size = 0;
	unsigned long long date = 0;
	void clean();

	FILE__LIST_element();
	~FILE__LIST_element();
};

class FILE__LIST
{
public:
	FILE__LIST_element *item[5000];
	int max_count = 5000;
	int size1 = 0;

	void sort_by_name();

	void get(unsigned char *buf, int unsigned buf_size);
	void set_from_buffer(unsigned char *buf, unsigned int sz);

	void add(wchar_t *name, bool is_folder_, unsigned long long size_, unsigned long long date_);

	void LOAD(wchar_t *folder);

	void to_tree(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *need_ASYNC_load, TRANSFER_DIALOG2_DirsFiles_TREE_element **&need_ASYNC_load_q);

	void clean();

	bool check_allowed_path(wchar_t *pt);

	FILE__LIST();
	~FILE__LIST();
};

