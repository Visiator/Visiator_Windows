#include "stdafx.h"

#include "tools.h"

#include "CRYPTO.h"

using namespace CryptoPP;

/////////////////////////////////////////////////////////////////////////////

MY_CRC::MY_CRC(void) {

}

unsigned int MY_CRC::calc(unsigned char *src, int len) {

	unsigned int result;
	result = 0;
	crc32.CalculateDigest((unsigned char *)&result, src, len);
	return result;
};

/////////////////////////////////////////////////////////////////////////////

MY_SHA3::MY_SHA3(void) {
	
}
void MY_SHA3::hash_32_byte(unsigned char *src_32_byte, unsigned char *dst_32_byte) {
	
	

	sha3.CalculateDigest( (unsigned char *)dst_32_byte, src_32_byte, 32);
	
}
//////////////////////////////////////////////////////////////////////////////

Integer  
  key_d(  "0x1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111")
, key_e("0x10001")
, key_n("0xB9DEDE47E176C24136BF9D12E4FC258834E3FCC09C62D647FF9D2D1A50AEF8A532E589D3055DD4307A8A3C53F0DC421873A19F85E39E7E33C3DC54DEC138DFEE86D3BE3395E03671783B93666163651AFE59073414A5C60263C99E1996B144D75FAB5ADEDCC7488C149AF856956B94E7E77910DB642FB69E80FFF4CFAD06BA63");


MY_RSA::MY_RSA(void) {

}

Integer MY_RSA::_decode(Integer &x) {
	re = modn.Multiply(re_base, x);			// blind

// here we follow the notation of PKCS #1 and let u=q inverse mod p
// but in ModRoot, u=p inverse mod q, so we reverse the order of p and q
	Integer y = ModularRoot(re, priv_Key.m_dq, priv_Key.m_dp, priv_Key.m_q, priv_Key.m_p, priv_Key.m_u);

	y = modn.Multiply(y, rInv);
	return y;
}

void MY_RSA::decode_128_byte(unsigned char *src_128, unsigned char *dest_128) {
	Integer c, r;
	//unsigned char dd[128];

	c.Decode(src_128, 128);
	r = _decode(c);
	r.Encode(dest_128, 128);
	//for (int i = 0; i < 128; i++) dest_128[i] = dd[127 - i];
}

void MY_RSA::encode_128_byte(unsigned char *src_128_byte, unsigned char *dest_128_byte) {
	Integer r, c, m;
	//unsigned char dd[128];
	m.Decode(src_128_byte, 128);
	r = publ_Key.ApplyFunction(m);
	r.Encode(dest_128_byte, 128);
	//for (int i = 0; i < 128; i++) dest_128_byte[i] = dd[i];
}

void MY_RSA::init(void) {
	
	publ_Key.Initialize(key_n, key_e);

}

////////////////////////////////////////////////////////////////////////////////////

MY_AES::MY_AES(void) {

}

void MY_AES::set_key_16_byte(unsigned char *key, char *info) {

	char ss[500];
	sprintf_s(ss, 400, "MY_AES::set_key_16_byte( %02X.%02X.%02X.%02X %02X.%02X.%02X.%02X %02X.%02X.%02X.%02X %02X.%02X.%02X.%02X ) %s"
		, key[0], key[1], key[2], key[3]
		, key[4], key[5], key[6], key[7]
		, key[8], key[9], key[10], key[11]
		, key[12], key[13], key[14], key[15]
		, info
	);
	sudp(ss);


	e.SetKey(key, 16);
	d.SetKey(key, 16);
}

void MY_AES::encrypt_16_byte(unsigned char *buf) {
	e.ProcessData(buf, buf, 16);
}

void MY_AES::encrypt_stream(unsigned char *buf, int buf_len) {

	e.ProcessData(buf, buf, buf_len);
	/*
	int i;
	i = 0;
	while (i + 16 <= buf_len) {
		
		e.ProcessData(buf+i, buf+i, 16);
		i += 16;
	}*/
}

void MY_AES::decrypt_16_byte(unsigned char *buf) {
	d.ProcessData(buf, buf, 16);
}

void MY_AES::decrypt_128_byte(unsigned char *buf) {
	d.ProcessData(buf, buf, 16);
	d.ProcessData(buf + 16, buf + 16, 16);
	d.ProcessData(buf + 16 * 2, buf + 16 * 2, 16);
	d.ProcessData(buf + 16 * 3, buf + 16 * 3, 16);
	d.ProcessData(buf + 16 * 4, buf + 16 * 4, 16);
	d.ProcessData(buf + 16 * 5, buf + 16 * 5, 16);
	d.ProcessData(buf + 16 * 6, buf + 16 * 6, 16);
	d.ProcessData(buf + 16 * 7, buf + 16 * 7, 16);
	

}
void MY_AES::decrypt_1120_byte(unsigned char *buf) {
	for (int i = 0; i < 70; i++) {
		d.ProcessData(buf + 16*i, buf + 16*i, 16);
	};
};