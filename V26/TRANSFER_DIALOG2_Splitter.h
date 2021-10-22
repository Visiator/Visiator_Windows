#pragma once
class GUI_low_level;
class TRANSFER_DIALOG2;


class TRANSFER_DIALOG2_Splitter
{
private:
	
	TRANSFER_DIALOG2 *parent = nullptr;
	int x = 0, y = 0, h = 0, w = 0;
public:
	void set_size(int x_, int y_, int w_, int h_);
	bool its_me(int local_mx, int local_my);
	void Paint(GUI_low_level *low_level);

	TRANSFER_DIALOG2_Splitter(TRANSFER_DIALOG2 *parent_);
	~TRANSFER_DIALOG2_Splitter();
};

