#pragma once

#include "tools.h"

struct PACKET_LEVEL0
{
	uint32_t zero;
	uint32_t crc32;
	uint32_t sol;
	uint32_t packet_type;
	uint32_t sol2;
	unsigned char body[108];
};
struct PACKET_LEVEL1_1002_request // get_proxy_ip
{
	uint64_t sub_type; // 101 - просим ip сервера для регистрации нового партнера
						   // 102 - просим ip сервера для подключения сервера
						   // 103 - просим ip сервера на котором зареган сервер требуемого партнера
	uint64_t im_public_id;
	uint64_t im_private_id;
	unsigned char  AES_pass[32];
	unsigned char  body[52];
};
struct PACKET_LEVEL1_1002_responce // get_proxy_ip
{
	uint32_t  sub_type;
	uint32_t  ip4;
	unsigned char   body[100];
};
struct PACKET_LEVEL1_1003_request // register neww partner
{
	unsigned char AES_pass[32];
	unsigned char body[76];
};
struct PACKET_LEVEL1_1003_responce // register neww partner
{
	uint64_t public_id;
	uint64_t private_id;
	unsigned char body[92];
};


class NET_SERVER_SESSION
{
public:
	NET_SERVER_SESSION();
	~NET_SERVER_SESSION();
};

