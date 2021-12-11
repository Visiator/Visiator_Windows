
#include "stdafx.h"

#include "GUI_low_level.h"
#include "GUI.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "DESKTOP.h"
#include "VIEWER.h"
#include "SERVICE.h"
#include "KERNEL.h"
#include "AGENT.h"
#include "INDICATOR.h"
#include "TOTAL_CONTROL.h"
#include "CLIPBOARD.h"

#include <boost/lambda/lambda.hpp>

#pragma comment(lib,"vfw32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "cryptlib.lib")
#pragma comment(lib, "zlibstat.lib")


FONT *font[10] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
bool GLOBAL_STOP = false;
APPLICATION_ATTRIBUTES app_attributes;
DESKTOP  *desktop = nullptr;
VIEWER *viewer = nullptr;
SERVICE *service = nullptr;
KERNEL kernel;
CMDLINE cmd_line;
AGENT *agent = nullptr;
TOTAL_CONTROL *total_control = nullptr;
INDICATOR *indicator = NULL;
CLIPBOARD clipboard;

int SIGABRT_id = 0;
void test_bbb();
void test_b12();

void WinMain_finish();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	unsigned char id[32], ps[32], pse[32];// , *ps_encr;
	//using namespace boost::lambda;

	init_crit_section();
	
	if (init_net() == false) { return -1; }

	//init_encode_color_matrix_all();

	app_attributes.get_all_parametrs(hInstance);
	
	sudp("start");

	cmd_line.decode((char *)lpCmdLine);
	
	load_fonts();

	

	//test_b12();

	total_control = new TOTAL_CONTROL();
	total_control->start_EXECUTE();

	test_bbb();

	/*
	thread_list = new ALL_THREAD_LIST();

	view_visiator_com = new char[50];
	zero_char(view_visiator_com, 50);
	my_strcpy((unsigned char *)view_visiator_com, (unsigned char *)"view.visiator.com");

	

	cmd_line.decode((char *)lpCmdLine);
	*/

	/***
	unsigned char partner_id[32];
	unsigned char pass_encripted[32];
	unsigned char pass_no_encripted[32];

	zero_unsigned_char(pass_encripted, 32);
	zero_unsigned_char(pass_no_encripted, 32);

	my_strcpy(partner_id, (unsigned char *)"239231241");                               
	my_strcpy(pass_no_encripted, (unsigned char *)"Bara*}Oke");

	viewer = new VIEWER();
	viewer->RUN_VIEWER(partner_id, pass_encripted, pass_no_encripted);
	***/

	if (cmd_line.count == 1 && cmd_line.compare_param_by_no(0, "indicator")) {

		indicator = new INDICATOR();

		indicator->RUN(hInstance);

		WinMain_finish();

		return 0;
	}


	

	if ((cmd_line.count == 1 && cmd_line.first_param_is_ID() == true)) {
		sudp("run");
		
		zero_unsigned_char(id, 32);
		zero_unsigned_char(ps, 32);
		zero_unsigned_char(pse, 32);
		
		my_strcpy(id, (unsigned char *)cmd_line.get_param_by_no(0));

		viewer = new VIEWER();
		viewer->RUN_VIEWER(id, app_attributes.startup_parametr_PASS_ENCR, ps);

		return 0;
	};
	if ((cmd_line.count == 2 && cmd_line.first_param_is_ID() == true)) {
		sudp("run");

		zero_unsigned_char(id, 32);
		zero_unsigned_char(ps, 32);
		zero_unsigned_char(pse, 32);

		my_strcpy(id, (unsigned char *)cmd_line.get_param_by_no(0));
		my_strcpy(ps, (unsigned char *)cmd_line.get_param_by_no(1));

		viewer = new VIEWER();
		viewer->RUN_VIEWER(id, pse, ps);

		return 0;
	};
	if ((cmd_line.count == 3 && cmd_line.compare_param_by_no(0, "viewer") == true)) {

		
		zero_unsigned_char(id, 32);
		zero_unsigned_char(ps, 32);
		zero_unsigned_char(pse, 32);

		my_strcpy(id, (unsigned char *)cmd_line.get_param_by_no(1));
		my_strcpy(ps, (unsigned char *)cmd_line.get_param_by_no(2));

		viewer = new VIEWER();
		viewer->RUN_VIEWER(id, pse, ps);
		

		return 0;
	}


	if (cmd_line.compare_param_by_no(0, "reinstall")) {

		ServiceUnInstallLocal();
		ServiceInstallLocal();

		return 0;
	}

	//--------------------------------------------------------------------------------------------------------
	// AGENT

	if (cmd_line.count == 1 && cmd_line.compare_param_by_no(0, "agent")) {

		/*if (service == NULL) { service = neww SERVICE(); }
		service->RUN();*/

		if (agent == NULL) { agent = new AGENT(); };

		agent->RUN();

		WinMain_finish();

		sudp("AGENT finish");

		return 0;
	}

	// ==========================================================================================================
	// SERVICE

	if (app_attributes.is_service) {
		//send_udp("is_service");
		RUN_SERVICE();
		//send_udp("service is stoped");

		WinMain_finish();

		sudp("SERVICE finish");

		return 0;
	};

	
	if ( cmd_line.count == 1 && ( cmd_line.compare_param_by_no(0, "servicedeinstall") || cmd_line.compare_param_by_no(0, "uninstall") )
		) 
	{
		SERVICE_DEINSTALL();
		return 0;
	};

	//==========================================================================================================

	if (desktop == NULL) desktop = new DESKTOP();
	desktop->RUN();
	
	sudp("DESKTOP finish");

	WinMain_finish();
	return 0;
};

void WinMain_finish() {


}

unsigned short encode_rgb_to_12bit(unsigned int v)
{
	unsigned int r, r1, r2, r3;

	r1 = (v & 0xff)/16;
	
	r2 = (v & 0xff00);
	r2 = r2 >> 8;
	r2 = r2 / 16;
	r2 = r2 << 4;

	r3 = (v & 0xff0000);
	r3 = r3 >> 16;
	r3 = r3 / 16;
	r3 = r3 << 8;


	r = r1 | r2 | r3;

	return (r & 0xFFF);
};


unsigned short encode_rgb_to_12bit(unsigned int c0, unsigned int c1, unsigned int c2)
{
	unsigned int r;

	r = c0 | (c1 << 4) | (c2 << 8);

	return (r & 0xFFF);
};

//int bitmap_w = 0, bitmap_h = 0;
//unsigned int *bitmap = NULL;


void SCREEN_LIGHT_12bit::load_from_BMP_buffer(BYTE *buf, SCREEN_LIGHT_12bit *b12) {
	int inverse = 1;
	int bitmap_w, bitmap_h;

	if (buf == NULL) return;
	//if (bitmap != NULL) delete[] bitmap;

	//bitmap = NULL;

	//bitmap_w = 0;
	//bitmap_h = 0;


	unsigned short *bfType, *bfReserved1, *bfReserved2;
	unsigned int *bfSize, *bfOffBits;

	bfType = (unsigned short *)&(buf[0]);
	bfSize = (unsigned int *)&(buf[2]);
	bfReserved1 = (unsigned short *)&(buf[6]);
	bfReserved2 = (unsigned short *)&(buf[8]);
	bfOffBits = (unsigned int *)&(buf[10]);


	if (*bfType != 0x4d42) return;


	unsigned int *BITMAPINFO_ver, *biWidth, *biHeight, *biCompression, *biSizeImage, *biClrUsed, *biClrImportant;
	unsigned short *biBitCount;

	int i, j, delta, x, y;
	unsigned char *q;
	delta = *bfOffBits;

	BITMAPINFO_ver = (unsigned int *)&(buf[14]);

	if (*BITMAPINFO_ver == 40) {
		biWidth = (unsigned int *)&(buf[14 + 0x04]);
		biHeight = (unsigned int *)&(buf[14 + 0x08]);
		biBitCount = (unsigned short *)&(buf[14 + 0x0e]);
		biCompression = (unsigned int *)&(buf[14 + 0x10]);
		biSizeImage = (unsigned int *)&(buf[14 + 0x14]);
		biClrUsed = (unsigned int *)&(buf[14 + 0x20]);
		biClrImportant = (unsigned int *)&(buf[14 + 0x24]);
		if (*biCompression == 0 && *biBitCount == 24) {

			bitmap_w = *biWidth;
			bitmap_h = *biHeight;

			//bitmap = new unsigned int[bitmap_w * bitmap_h]; //neew(w * h * 4, t->bitmap, "load_textura_from_BMP_buffer()");

			b12->set_new_size_(bitmap_w, bitmap_h);

			q = b12->buf;

			char cc[4];
			unsigned int *qq;
			qq = (unsigned int *)cc;

			i = delta;
			for (y = bitmap_h - 1; y >= 0; y--) {
				j = 0;
				for (x = 0; x < bitmap_w; x++) {

					if (inverse == 0) {

						cc[3] = 0;
						cc[0] = buf[i];
						cc[1] = buf[i + 1];
						cc[2] = buf[i + 2];

						///q[y*bitmap_w + x] = encode_rgb_to_12bit( buf[i], buf[i+1], buf[i+2] );

					}
					else {
						cc[3] = 0;
						cc[2] = buf[i];
						cc[1] = buf[i + 1];
						cc[0] = buf[i + 2];
						
						///q[y*bitmap_w + x] = encode_rgb_to_12bit(buf[i+2], buf[i + 1], buf[i]);
					}
					
					//q[y*bitmap_w + x] = encode_rgb_to_12bit( *qq );

					i += 3;
					j += 3;
				}
				j = bitmap_w % 4;

				
				i += j;
			};
			return;
		}
		return;
	}
};

void test_b12() {

	//SCREEN_LIGHT_12bit *b12;

	//b12 = new SCREEN_LIGHT_12bit();

	////////////////////////////////////////////////////////////////////////////
	/*
	BYTE *b;
	b = new BYTE[35000000];
	FILE *f;
	fopen_s( &f, "C:\\1\\12.bmp", "rb");

	int i, j, k;

	j = fread(b, 1, 35000000, f);

	fclose(f);
	*/
	//b12->load_from_BMP_buffer(b, b12);



//	b12->encode();

	//delete[] b;

	////////////////////////////////////////////////////////////////////////////

	//b12->set_new_size_(bitmap_w, bitmap_h);



};


SCREEN_LIGHT_12bit *screen_12bit = nullptr;
SCREEN_LIGHT_encoded_12bit *screen_encoded_12bit = nullptr;

void test_bbb() {

	

	return;
	if (screen_12bit == nullptr) screen_12bit = new SCREEN_LIGHT_12bit();
	if (screen_encoded_12bit == nullptr) screen_encoded_12bit = new SCREEN_LIGHT_encoded_12bit();

	for (int i = 0; i < 500; i++) {

		screen_12bit->emulate_red();

		screen_encoded_12bit->encode_screen_12bit(screen_12bit, 10, 11);

	};

	// if (get_screenshot(nullptr, screen_12bit) == false) {

}

