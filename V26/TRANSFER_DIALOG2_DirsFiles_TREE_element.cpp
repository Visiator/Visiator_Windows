#include "stdafx.h"

#include "GUI.h"
//#include "MyGUI_low_level.h"
#include "TRANSFER_DIALOG2_DirsFiles_TREE_element.h"
#include "tools.h"



TEXTURA *folder_ico2 = nullptr;
TEXTURA *file_ico2 = nullptr;


TRANSFER_DIALOG2_DirsFiles_TREE_element::TRANSFER_DIALOG2_DirsFiles_TREE_element(wchar_t *name_, bool is_folder_, unsigned long long size_, unsigned long long date_, TRANSFER_DIALOG2_DirsFiles_TREE_element *parent_, TRANSFER_DIALOG2_DirsFiles_TREE_element *prev_) {
	
	

	name = new wchar_t[my_strlen(name_)+1];
	my_strcpy(name, name_);

	size = size_;
	date = date_;

	parent = parent_;
	prev = prev_;


	is_open = false;
	is_folder = is_folder_;
}
TRANSFER_DIALOG2_DirsFiles_TREE_element::~TRANSFER_DIALOG2_DirsFiles_TREE_element() {
	
}

void TRANSFER_DIALOG2_DirsFiles_TREE_element::clean() {

}

int TRANSFER_DIALOG2_DirsFiles_TREE_element::set_line_idx(int idx_, int lvl_) {
	
	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	int i;

	line_idx = idx_;
	lvl = lvl_;
	i = 1;
	if (is_open == true) {
		e = child;
		
		while (e != nullptr) {

			i += e->set_line_idx(idx_ + i, lvl_ + 1);

			e = e->next;
		}

	}

	return i;
}
/*
int TRANSFER_DIALOG2_DirsFiles_TREE_element::calc_line_count() {
	if(is_open == false) return 1;
	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	int i;
	i = 0;
	e = child;
	while (e != nullptr) {
		i += e->calc_line_count();
		e = e->next;
	}

	return i;
}
*/
void TRANSFER_DIALOG2_DirsFiles_TREE_element::paint_element_and_all_opened_sub_elements(GUI_low_level *low_level, int top_element, int xx, int yy, int ww, int hh) {

	int y;
 	y = yy + (line_idx - top_element) * _line_h + 1;

	if (y >= yy && y + _line_h <= yy + hh +1) {
		paint_one_element(low_level, xx, y, ww, hh);
	}
	else {

	}

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	
	if (is_open == true && child != nullptr) {
		
		e = child;

		while (e != nullptr) {
			e->paint_element_and_all_opened_sub_elements(low_level , top_element, xx, yy, ww, hh);
			e = e->next;
		}

	};


	
		

};

void TRANSFER_DIALOG2_DirsFiles_TREE_element::paint_one_element(GUI_low_level *low_level, int x, int y, int w, int h) {
	
	/* 2021 09
	unsigned int clr1 = 0x777777;

	FONT *fnt;

	fnt = fontt[4];

	if (fnt == nullptr) return;

	int ww, name_x1, name_x2, size_x1, date_x1;

	if (folder_ico2 == nullptr) folder_ico2 = new TEXTURA(1060);
	if (file_ico2 == nullptr) file_ico2 = new TEXTURA(1061);

	unsigned int selected_bg_color = 0xffaa00;

	if (is_selected) {
		if (is_open) selected_bg_color = 0xbbbbbb;
		low_level->fill_rectangle(x + 15 * (lvl + 1) + 15 + 8, y, w - 15 * (lvl + 0) - 15 - 8, 17, selected_bg_color, 30);
	};
	
	//low_level->fill_rectangle(x + 15 * (lvl + 1), y, w - 15*(lvl+1)-1, 17, 0xff00ff, 255);
	
	if (is_folder) folder_ico2->Paint(low_level, x + 15 * (lvl + 1)-1+4, y + 0);
	else   	       file_ico2->Paint(low_level, x + 15 * (lvl + 1)-1+4, y + 0);
	
	unsigned int name_color = 0;
	
	if (is_current_active_element) name_color = 255;

	name_x1 = x + 15 * (lvl + 1) + 15 + 8;

	name_x2 = name_x1 + fnt->paint_text_in_width(low_level, name_x1, y, w - 15 * (lvl + 0) - 15 - 8,name, name_color);

	wchar_t sz[500];
	wchar_t dt[500];
	//************************************************************************************************
	//low_level->fill_rectangle(x + w - 130, y, 130, 17, 0xffffff, 255);

	// покажем размер файла
	if (is_folder == false) {
		
		
		format_sz(sz, size);
		ww = fnt->text_width(sz);

		size_x1 = x + w - ww - 10 - 70;

		if (size_x1 < name_x2) {
			//font->paint_text_in_width(size_x1, y, ww + 1, L"-", clr1);
		}
		else {

			fnt->paint_text_in_width(low_level, size_x1, y, ww + 1, sz, clr1);
		};
	};
	//*************************************************************************************************
	// покажем дату
	SYSTEMTIME st;
	if (date != 0) {
		//x1 = x + w - 100;
		//w1 = 95;

		//low_level->fill_rectangle(x1, y, w1, 17, 0xeeffee, 255);
		FILETIME ddt, *dd;
		dd = (FILETIME *)&date;
		ddt = *dd;
		FileTimeToLocalFileTime(&ddt, &ddt);
		FileTimeToSystemTime(&ddt, &st);
		
		//wsprintf(dt, L"%04d.%02d.%02d  %02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

		wsprintf(dt, L"%04d.%02d.%02d", st.wYear, st.wMonth, st.wDay);
		
		ww = fnt->text_width(dt);

		date_x1 = x + w - ww - 5;
		if (date_x1 > name_x2) {
			fnt->paint_text_in_width(low_level, date_x1, y, ww + 1, dt, clr1);
		};
	};
	//-------------------------------------------------------------------------------------------

	unsigned int clr;

	clr = 0xaaaaaa;
	
	if (15 * lvl < w) {

		if (is_folder) {

			if (is_open) { low_level->paint_tri_1_open(x + 15 * lvl, y, clr); }
			else { low_level->paint_tri_1_close(x + 15 * lvl, y, clr); }
		};
	};	
	*/
}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE_element::get_element_by_fullname(wchar_t * val) {
	TRANSFER_DIALOG2_DirsFiles_TREE_element *e, *q;

	wchar_t fn[5100];

	e = this;
	while (e != nullptr) {

		zero_wchar_t(fn, 5100);
		e->get_folder_full_name(fn);

		if (my_strcmp(fn, val) == 0) {
			return e;
		};

		if (e->is_open == true && e->child != nullptr) {
			q = e->child->get_element_by_fullname(val);
			if (q != nullptr) return q;
		}

		e = e->next;
	};

	return nullptr;
}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE_element::get_element_by_line_idx(int val) {

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e, *q;

	e = this;
	while (e != nullptr) {

		if (e->line_idx == val) return e;

		if (e->is_open == true && e->child != nullptr) {
			q = e->child->get_element_by_line_idx(val);
			if (q != nullptr) return q;
		}

		e = e->next;
	};

	return nullptr;
}

void TRANSFER_DIALOG2_DirsFiles_TREE_element::mouse_left_button_down(int mouse_x, int mouse_y) {

}

void TRANSFER_DIALOG2_DirsFiles_TREE_element::clean_child() {
	if (child == nullptr) return;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;

	e = child;
	while (e != nullptr) {
		if (e->child != nullptr) {
			e->clean_child();
		}
		e = e->next;
	}
	delete child;
	child = nullptr;

}

void TRANSFER_DIALOG2_DirsFiles_TREE_element::get_name(wchar_t *buf) {
	int i;

	i = 0;
	while (name[i] != 0 && i < 5000 - 10) {
		buf[i] = name[i];
		i++;
	}
	buf[i] = 0;
}

void TRANSFER_DIALOG2_DirsFiles_TREE_element::get_folder_full_path(wchar_t *dir) {

	
	if (parent == nullptr) return;

	parent->get_folder_full_name(dir);

	/*
	i = 0;
	while (i < 5000 - 10 && dir[i] != 0) i++;

	if (is_folder) {
		if (i > 0 && i < 5000 - 10) {
			dir[i] = '\\';
			i++;
		};
	
		j = 0;
		while (name[j] != 0 && i < 5000 - 10) {
			dir[i++] = name[j++];
		}
		dir[i] = 0;
	};
	*/
}


void TRANSFER_DIALOG2_DirsFiles_TREE_element::get_folder_full_name(wchar_t *dir) {

	int i, j;
	if (parent == nullptr) return;
	
	parent->get_folder_full_name(dir);

	i = 0;
	while (i < 5000 - 10 && dir[i] != 0) i++;

	if (i > 0 && i < 5000 - 10) {
		dir[i] = '\\'; 
		i++;
	};
	
	j = 0;
	while (name[j] != 0 && i < 5000 - 10) {
		dir[i++] = name[j++];
	}
	dir[i] = 0;
	
} 

void TRANSFER_DIALOG2_DirsFiles_TREE_element::UNSELECT_all_elements() {
	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;

	e = this;

	while (e != nullptr) {

		e->is_selected = false;

		if (e->child != nullptr) e->child->UNSELECT_all_elements();

		e = e->next;
	}
}

void TRANSFER_DIALOG2_DirsFiles_TREE_element::SELECT_element_from_to(int i1, int i2, int control_state) {

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;

	e = this;

	while (e != nullptr) {

		if (e->line_idx >= i1 && e->line_idx <= i2) {
			e->is_selected = true;
		}
		else {
			if(control_state == 0) e->is_selected = false;
		}

		if (e->child != nullptr) e->child->SELECT_element_from_to(i1, i2, control_state);

		e = e->next;
	}

}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE_element::get_first_selected_element() {
	
	TRANSFER_DIALOG2_DirsFiles_TREE_element *e, *q;

	e = this;
	while (e != nullptr) {
		if (e->is_selected && e->is_open == false) return e;
		if (e->child != nullptr) {
			q = e->child->get_first_selected_element();
			if (q != nullptr) return q;
		}
		e = e->next;
	}
	
	return nullptr;
}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE_element::get_next_selected_element() {

	

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	e = get_next_element();

	while (e != nullptr) {

		if (e->is_selected) {
			return e;
		};

		e = e->get_next_element();
	}

	return nullptr;
}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE_element::get_next_element() {
	if (child != nullptr && is_open) return child->get_next_element();
	if (next != nullptr) return next;
	if (parent != nullptr && parent->next != nullptr) return parent->next;
	return nullptr;
}