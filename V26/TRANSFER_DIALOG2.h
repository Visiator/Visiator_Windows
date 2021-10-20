#pragma once

class VIEWER;
class GUI_low_level;

class TRANSFER_DIALOG2
{
public:

	void set_visible(GUI_low_level *low_level, bool val);

	 TRANSFER_DIALOG2(VIEWER *viewer_);
	~TRANSFER_DIALOG2();
};

