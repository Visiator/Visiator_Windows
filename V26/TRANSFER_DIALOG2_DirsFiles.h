#pragma once

class TRANSFER_DIALOG2;
class GUI_low_level;
class TRANSFER_DIALOG2_DirsFiles_Toolbar;
class TRANSFER_DIALOG2_DirsFiles_VScroll;
class TRANSFER_DIALOG2_DirsFiles_TREE;

class TRANSFER_DIALOG2_DirsFiles
{
public:
	TRANSFER_DIALOG2 *parent = nullptr;
	int x = 0, y = 0, w = 0, h = 0;
	bool its_me(int local_mx, int local_my);

	void refresh(GUI_low_level *low_level);

	void START(GUI_low_level *low_level, int LOCAL_or_PARTNER);

	void set_size(GUI_low_level *low_level, int x_, int y_, int w_, int h_);

	int get_global_x();
	int get_global_y();
	int get_global_w();
	int get_global_h();

	TRANSFER_DIALOG2_DirsFiles_Toolbar *Toolbar = nullptr;
	TRANSFER_DIALOG2_DirsFiles_VScroll *VScroll = nullptr;
	TRANSFER_DIALOG2_DirsFiles_TREE    *Tree = nullptr;


	TRANSFER_DIALOG2_DirsFiles(int local_or_dest_, TRANSFER_DIALOG2 *parent_);
	~TRANSFER_DIALOG2_DirsFiles();

};

