#pragma once

class GUI_low_level;

class TEXTURA
{
public:

	void load_textura_from_BMP_buffer(BYTE *buf);
	void load_from_resource(int resource_id);

	unsigned int *bitmap = nullptr;
	int w = 0, h = 0;

	

	void Paint(GUI_low_level *low_level, int x, int y);
	void paint_transparent_green(GUI_low_level *low_level, int dx, int dy);
	void paint_mono(GUI_low_level *low_level, int dx, int dy, unsigned int color);
	

	TEXTURA(int resource_id);
};

