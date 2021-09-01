#include "GUI_low_level.h"

#include "tools.h"

extern bool GLOBAL_STOP;

GUI_low_level::GUI_low_level(HWND hw_) {
	hw = hw_;
}

void GUI_low_level::change_size(HWND hw, int w_, int h_) {
	if (w_ < 50) w_ = 50;
	if (h_ < 50) h_ = 50;

	//window_x = x;
	//window_y = y;


	if ((w_ * h_) > buf_size) {

		lock_FULL();

		if (buf != NULL) delete[] buf;

		buf = (unsigned int *)(new char[w_ * h_ * 4]); //neew(w_ * h_ * 4, sscr_buf, "MyGUI_low_level::set_window_size()");
		
		/*** 2021
		buf2 = (unsigned int *)(new char[w_ * h_ * 4]); //neew(w_ * h_ * 4, sscr_buf, "MyGUI_low_level::set_window_size()");
		***/

		buf_size = w_ * h_;
		window_w = w_;
		window_h = h_;

		unlock_FULL();

	}

	fill_color(0x1d364d);
	//for (int i = 0; i < 150; i++) buf[i] = 0xff0000;
	window_w = w_;
	window_h = h_;



	if (bmInfo != NULL) delete bmInfo;

	bmInfo = new BITMAPINFO(); //neew(sizeof(BITMAPINFO), bmInfo, "BITMAPINFO()");
	bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo->bmiHeader.biBitCount = 32;
	bmInfo->bmiHeader.biClrUsed = 0;
	bmInfo->bmiHeader.biClrImportant = 0;
	bmInfo->bmiHeader.biPlanes = 1;
	bmInfo->bmiHeader.biCompression = BI_RGB;
	bmInfo->bmiHeader.biSizeImage = 0;
	bmInfo->bmiHeader.biHeight = window_h * -1;
	bmInfo->bmiHeader.biWidth = window_w;


}

void GUI_low_level::fill_color(unsigned int col) {
	int i;
	i = 0;
	while (i < buf_size) {
		buf[i] = col;
		i++;
	}
}


void GUI_low_level::lock_FULL() {
	while (GLOBAL_STOP == false) {

		if (is_lock_FULL == false && is_lock_READ == false && is_lock_WRITE == false) {
			enter_crit(7);
			if (is_lock_FULL == false && is_lock_READ == false && is_lock_WRITE == false) {
				is_lock_FULL = true;

				leave_crit(7);
				return;
			}
			leave_crit(7);
		}
		else {
			Sleep(0);
		}
	}
}
void GUI_low_level::unlock_FULL() {
	is_lock_FULL = false;

}

void GUI_low_level::lock_READ() {
	while (GLOBAL_STOP == false) {

		if (is_lock_FULL == false && is_lock_READ == false) {
			enter_crit(7);
			if (is_lock_FULL == false && is_lock_READ == false) {
				is_lock_READ = true;

				leave_crit(7);
				return;
			}
			leave_crit(7);
		}
		else {
			Sleep(0);
		}
	}
}
void GUI_low_level::unlock_READ() {
	is_lock_READ = false;

}


void GUI_low_level::Paint() {

	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hw, &ps);

	lock_READ();

	int liness;

	unsigned int *q2, *q, r, g, b;

	int w2 = window_w * 2;

	/*** 2021
	q2 = &(buf2[(window_h - 1)* window_w]);
	q = buf;
	for (int y = 0; y < window_h; y++)
	{
		for (int x = 0; x < window_w; x++)
		{

			r = (*q) & 0xff0000; r = r >> 16;
			g = (*q) & 0x00ff00;
			b = (*q) & 0x0000ff; b = b << 16;

			*q2 = r | g | b;

			//*q2 = *q;

			q++;
			q2++;
		};
		q2 -= w2;
	}
	***/


	liness = SetDIBitsToDevice(hdc, 0, 0
		, window_w
		, window_h
		, 0, 0, 0
		, window_h
		, buf //, buf2   - 2021
		, bmInfo
		, DIB_RGB_COLORS);


	unlock_READ();

	EndPaint(hw, &ps);


}
