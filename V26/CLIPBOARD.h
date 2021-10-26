#pragma once
class CLIPBOARD
{
public:

	unsigned char *buf = NULL;
	unsigned int buf_size = 0;// , buf_max_size = 0;

	DWORD last_set_clipboard = 0;

	CLIPBOARD();
	~CLIPBOARD();
};

