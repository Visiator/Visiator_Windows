#pragma once

#include <list>

#include "NET_SERVER_SESSION.h"

class NET_SERVER_SESSION_POOL
{
public:

	unsigned long long PUBLIC_ID = 0, PRIVATE_ID = 0;
	unsigned char PASS_ENCODED[32];

	std::list<NET_SERVER_SESSION> elements;

	NET_SERVER_SESSION *add_element();

	boost::thread* thread_EXECUTE = nullptr;
	bool EXECUTE_is_run = false;
	void EXECUTE();

	void RUN( unsigned long long PUBLIC_ID_, unsigned long long PRIVATE_ID_, unsigned char *PASS_ENCODED_ );

	NET_SERVER_SESSION_POOL();
	~NET_SERVER_SESSION_POOL();
};

