
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef WIN32

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



#endif


#include "TEXTURA.h"
#include "GUI_low_level.h"

TEXTURA::TEXTURA(int resource_id) {
	if (resource_id != 0) load_from_resource(resource_id);
}

void TEXTURA::load_from_resource(int resource_id) {

	HMODULE hMod = GetModuleHandle(NULL);

	HRSRC hRes_;
	HGLOBAL hGlob_;
	BYTE *lpbArray_;
	DWORD sz_;

	hRes_ = FindResource(hMod, MAKEINTRESOURCE(resource_id), L"BINARY");
	hGlob_ = LoadResource(hMod, hRes_);
	lpbArray_ = (BYTE*)hGlob_;// LockResource(hGlob_);

	sz_ = SizeofResource(hMod, hRes_);

	load_textura_from_BMP_buffer(lpbArray_);


	FreeResource(hGlob_);

}

void TEXTURA::load_textura_from_BMP_buffer(BYTE *buf) {
	int inverse = 0;

	if (buf == nullptr) return;
	if (bitmap != nullptr) delete[] bitmap;

	bitmap = nullptr;
	w = 0;
	h = 0;


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
	unsigned int *q;
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

			w = *biWidth;
			h = *biHeight;

			bitmap = new unsigned int[w * h]; //neew(w * h * 4, t->bitmap, "load_textura_from_BMP_buffer()");
			w = *biWidth;
			h = *biHeight;

			q = (unsigned int *)bitmap;
			char cc[4];
			unsigned int *qq;
			qq = (unsigned int *)cc;

			i = delta;
			for (y = h - 1; y >= 0; y--) {
				j = 0;
				for (x = 0; x < w; x++) {

					if (inverse == 0) {

						cc[3] = 0;
						cc[0] = buf[i];
						cc[1] = buf[i + 1];
						cc[2] = buf[i + 2];
					}
					else {
						cc[3] = 0;
						cc[2] = buf[i];
						cc[1] = buf[i + 1];
						cc[0] = buf[i + 2];
					}
					q[y*w + x] = *qq;
					i += 3;
					j += 3;
				}
				j = w % 4;

				/*
				if (w == 705) j = 1;
				if (w == 235) j = 3;
				if (w == 185) j = 1;
				if (w == 141) j = 1;
				if (w == 87) j = 3;
				if (w == 13) j = 1;
				if (w == 15) j = 3;
				if (w == 19) j = 3;
				if (w == 27) j = 3;
				if (w == 27) j = 3;
				*/

				i += j;
			};
			return;
		}
		return;
	}
};

void TEXTURA::Paint(GUI_low_level *low_level, int x, int y) {
	if (x + this->w > low_level->window_w) return;
	if (y + this->h > low_level->window_h) return;
	if (x < 0 || y < 0) return;
	unsigned int *b;
	
	int i, j, k;
	i = 0;
	k = 0;
	while (i < this->h) {
		b = low_level->buf + x + (i+y)*low_level->window_w;
		j = 0;
		while (j < this->w) {
			if (bitmap[k] != 0x01ff01) {
				*b++ = bitmap[k++];
			}
			else {
				b++;
				k++;
			}
			j++;
		}
		i++;
	}
}





