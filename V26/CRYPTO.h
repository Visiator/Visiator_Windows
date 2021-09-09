
#pragma once
#define WIN32_LEAN_AND_MEAN
#define WIN32_WINNT 0x0501

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_


#include "cryptopp/rsa.h"
#include "cryptopp/integer.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/osrng.h"
#include "cryptopp/sha.h"
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/crc.h"
#include "cryptopp/sha3.h"



#if !defined(AFX_CRYPTO_H_INCLUDED_)
#define AFX_CRYPTO_H_INCLUDED_

using namespace CryptoPP;

///////////////////////////////////////////////////////////////////////////////////////////////////////

class MY_CRC
{
public:
	CryptoPP::CRC32 crc32;
	unsigned int calc(unsigned char *src, int len);
	MY_CRC(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

class MY_SHA3
{
public:
	//CryptoPP::
	SHA3_256 sha3;
	void hash_32_byte(unsigned char *src_32_byte, unsigned char *dst_32_byte);
	MY_SHA3(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

class MY_RSA
{
public:
	RSA::PrivateKey priv_Key;
	RSA::PublicKey  publ_Key;
	ModularArithmetic modn;
	Integer r, rInv;
	Integer re_base;
	Integer re;

	Integer _decode(Integer &x);
	void decode_128_byte(unsigned char *src_128_byte, unsigned char *dest_128_byte);

	void encode_128_byte(unsigned char *src_128_byte, unsigned char *dest_128_byte);

	void init(void);

	MY_RSA(void);
};

class MY_AES
{
public:
	ECB_Mode<AES>::Decryption d;
	ECB_Mode<AES>::Encryption e;

	void set_key_16_byte(unsigned char *key_16_byte);
	void encrypt_16_byte(unsigned char *buf_16_byte);

	void encrypt_stream(unsigned char *buf, int buf_len);

	void decrypt_16_byte(unsigned char *buf_16_byte);
	void decrypt_128_byte(unsigned char *buf_16_byte);
	void decrypt_1120_byte(unsigned char *buf_16_byte);

	MY_AES(void);
};

#endif