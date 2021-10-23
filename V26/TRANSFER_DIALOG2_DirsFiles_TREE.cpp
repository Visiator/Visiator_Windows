#include "stdafx.h"

#include "VIEWER.h"
#include "TRANSFER_DIALOG2_DirsFiles_TREE.h"
#include "TRANSFER_DIALOG2_DirsFiles_TREE_element.h"
#include "TRANSFER_DIALOG2_DirsFiles.h"
#include "TRANSFER_DIALOG2_DirsFiles_Toolbar.h"

#include "GUI.h"


TRANSFER_DIALOG2_DirsFiles_TREE::TRANSFER_DIALOG2_DirsFiles_TREE(TRANSFER_DIALOG2_DirsFiles *parent_) {
	
	parent = parent_;
	if (SelectRectangle == nullptr) SelectRectangle = new SELECT_RECTANGLE(this);

	LOAD_INDICATOR = new LOAD_INDICATOR_tree();

	zero_wchar_t(need_ASYNC_load_name, 5100);
}
TRANSFER_DIALOG2_DirsFiles_TREE::~TRANSFER_DIALOG2_DirsFiles_TREE() {

}

void TRANSFER_DIALOG2_DirsFiles_TREE::START(GUI_low_level *low_level, int LOCAL_or_PARTNER_) {
	
	DWORD d, m;
	LOCAL_or_PARTNER = LOCAL_or_PARTNER_;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *q, *q2;

	if (top != nullptr) {
		//TODO
		// WTF?
	
		return;
	}

	top = new TRANSFER_DIALOG2_DirsFiles_TREE_element(L"ROOT", true, 0, 0, nullptr, nullptr);

	if (LOCAL_or_PARTNER == LOCAL_FILES) {

		/*
		q = add_child(top, L"A:", true, 0, 0);
		q = add_next(q, L"B:", true, 0, 0);
		q = add_next(q, L"C:", true, 0, 0);
		q = add_next(q, L"D:", true, 0, 0);
		*/
		
		d = GetLogicalDrives();

		wchar_t cc[10];
		cc[0] = 0;
		cc[1] = ':';
		cc[2] = 0;
		cc[3] = 0;

		int i = 0, j;
		m = 1;
		
		q = top;
		j = 0;
		while (i < 26)
		{
			if ((d & m) == m) {
				cc[0] = 'A' + i;
				
				if (j == 0) {
					q = add_child(top, cc, true, 0, 0);
				}
				else {
					q = add_next(q, cc, true, 0, 0);
				}
				j++;

				
			}
			i++;
			m = (m << 1);
		}

	}
	if (LOCAL_or_PARTNER == PARTNER_FILES) {
		q = add_child(top, L"C:\\VISIATOR_FILES", true, 0, 0);
		set_current_active_element(q);
		parent->refresh(low_level);
	};
	//need_LOAD_fileslist(q2);

	auto_arrange(low_level);

}

void TRANSFER_DIALOG2_DirsFiles_TREE::RECALC_polzunok_size(GUI_low_level *low_level) {
	
	polzunok_size_in_percent = (float)visible_line_count / (float)all_line_count * (float)100;
	
	if (all_line_count > visible_line_count) {
		polzunok_size_in_pixel = (int)((float)((float)h - (float)_V_Scroll_w * (float)2) / (float)100 * polzunok_size_in_percent);

		// polzunok_position_in_pixel = 

	}
	else {
		polzunok_size_in_pixel = h - _V_Scroll_w*2;
		polzunok_position_in_pixel = 0;
	}

	

	generate_info();
	low_level->invalidate();
}

void TRANSFER_DIALOG2_DirsFiles_TREE::auto_arrange(GUI_low_level *low_level) {
	int line_idx, lvl;
	line_idx = 0;
	lvl = 0;
	TRANSFER_DIALOG2_DirsFiles_TREE_element *q;

	q = nullptr;
	if (top != nullptr) q = top->child;

	while (q != nullptr) {
		line_idx += q->set_line_idx(line_idx, lvl);
		//line_idx += q->calc_line_count();

		q = q->next;
	}

	all_line_count = line_idx;

	RECALC_polzunok_size(low_level);
	
}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE::add_child(TRANSFER_DIALOG2_DirsFiles_TREE_element *element, wchar_t *name_, bool is_folder_, unsigned long long size_, unsigned long long date_) {
	if (element == nullptr) {
		// TODO
		// WTF?
		return nullptr;
	}
	if (element->child != nullptr) {
		// TODO
		// WTF?
		return nullptr;
	}
	element->child = new TRANSFER_DIALOG2_DirsFiles_TREE_element(name_, is_folder_, size_, date_, element, nullptr);

	return element->child;
}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE::add_next(TRANSFER_DIALOG2_DirsFiles_TREE_element *element, wchar_t *name_, bool is_folder_, unsigned long long size_, unsigned long long date_) {
	if (element == nullptr) {
		// TODO
		// WTF?
		return nullptr;
	}
	if (element->next != nullptr) {
		// TODO
		// WTF?
		return nullptr;
	}
	element->next = new TRANSFER_DIALOG2_DirsFiles_TREE_element(name_, is_folder_, size_, date_, element->parent, element);

	return element->next;
}


void LOAD_INDICATOR_tree::Paint(GUI_low_level *low_level, int xx, int yy)
{

	unsigned int col;
	/*
	border_col = 0xeeeeee;

	low_level->line_h(xx , yy , LOAD_Indicator_ww, border_col);
	low_level->line_h(xx, yy + LOAD_Indicator_hh, LOAD_Indicator_ww, border_col);

	low_level->line_v(xx, yy, LOAD_Indicator_hh, border_col);
	low_level->line_v(xx+ LOAD_Indicator_ww, yy, LOAD_Indicator_hh, border_col);
	*/
	int zz;
	zz = LOAD_Indicator_ww / 2 - (20 + 19 * 4) / 2;

	for (int i = 0; i < 20; i++) {
		col = line[i] | line[i]<<8 | line[i]<<16;
		low_level->line_v(xx + zz + i*5, yy+10, 20, col);

	}
	RECT rr;
	rr.left = xx;
	rr.top = yy+30;
	rr.right = xx + LOAD_Indicator_ww;
	rr.bottom = yy + 50;

	wchar_t pp[500];
	pp[0] = 0;
	if (progress > 0) {
		wsprintf(pp, L"Load %d", progress);
	}

	//2021+ low_level->paint_text(4, 0,0,rr, pp, 0x999999,1);
}

void LOAD_INDICATOR_tree::turn() {
	int min, min_idx, i;
	min = 55555;
	min_idx = -1;
	for (i = 0; i < 20; i++) {
		if (line[i] < min) {
			min = line[i];
			min_idx = i;
		}
		if (line[i] < 220) line[i] += 12;
	}
	min_idx++;
	if (min_idx >= 20) min_idx = 0;
	line[min_idx] = 100;
}

LOAD_INDICATOR_tree::LOAD_INDICATOR_tree() {

	for (int i = 0; i < 20; i++) line[i] = 240;
	line[0] = 100;
}

void LOAD_INDICATOR_tree::ASYNC(GUI_low_level *low_level) {
	
	if (last_turn + 50 < GetTickCount()) {
		last_turn = GetTickCount();
		turn();
	}
};



void TRANSFER_DIALOG2_DirsFiles_TREE::Paint(GUI_low_level *low_level) {

	

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;

	int xx, yy, ww, hh;
	xx = 0 + parent->get_global_x();
	yy = 0 + parent->get_global_y() + _Toolbar_h;
	ww = parent->w - _V_Scroll_w;
	hh = parent->h - _Toolbar_h;

	

	if (need_ASYNC_load != nullptr) {

		LOAD_INDICATOR->Paint(low_level, xx + ww / 2 - LOAD_Indicator_ww / 2, yy + hh / 2 - LOAD_Indicator_hh/2);
		//LOAD_INDICATOR->turn();

		return;
	}

	lock();

	

	

	//low_level->fill_rectangle(xx+10, yy+10, ww-20, hh-20, 0xaaa0ff, 255);

	e = nullptr;
	if (top != nullptr) e = top->child;

	//e = top->child->get_element_by_line_idx(top_visible_line);
	if (e == nullptr) {
		unlock();
		return;
	};

	int line_idx;

	line_idx = 0;

	while (e != nullptr) {

		e->paint_element_and_all_opened_sub_elements(low_level, top_visible_line, xx,yy,ww,hh);

		

		e = e->next;
	}

	//SelectRectangle->SET_begin_selection_range(10, 10);
	//SelectRectangle->SET_end_selection_range(400, 200);

	SelectRectangle->Paint(low_level);

	unlock();
}

void TRANSFER_DIALOG2_DirsFiles_TREE::set_size(GUI_low_level *low_level, int x_, int y_, int w_, int h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;

	visible_line_count = h / _line_h;

	RECALC_polzunok_size(low_level);

	polzunok_position_in_pixel = (int)(((float)h - (float)_V_Scroll_w * (float)2 - polzunok_size_in_pixel) / (float)100 * (float)polzunok_position_in_percent);

	

}

bool TRANSFER_DIALOG2_DirsFiles_TREE::its_me(int m_x, int m_y) {
	if (m_x >= x && m_x < x + w && m_y >= y && m_y < y + h) {
		return true;
	}
	return false;
}

void TRANSFER_DIALOG2_DirsFiles_TREE::set_current_active_element(TRANSFER_DIALOG2_DirsFiles_TREE_element *val) {
	if (current_active_element != nullptr) current_active_element->is_current_active_element = false;
	current_active_element = val;
	if(current_active_element != nullptr)	current_active_element->is_current_active_element = true;
}

void TRANSFER_DIALOG2_DirsFiles_TREE::mouse_left_button_down(GUI_low_level *low_level, int mouse_x, int mouse_y) {

	if (need_ASYNC_load != nullptr) return;

	int local_local_mx, local_local_my, line_no;
	local_local_mx = mouse_x - x;
	local_local_my = mouse_y - y;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;

	line_no = local_local_my / _line_h;

	e = get_element_by_line_idx(top_visible_line + line_no);
	if (e != nullptr) {

		if (local_local_mx >= 0 && local_local_mx >= 15 * e->lvl && local_local_mx < 15 * (e->lvl + 1)) {
			
			set_current_active_element(e);
			if (e->is_open == false) {
				// открываем
				if (parent != nullptr && parent->Tree != nullptr) {
					parent->Tree->need_LOAD_fileslist(low_level, e);
				}
			}
			else {
				// закрываем
				
				parent->Tree->need_CLOSE(low_level, e);
			}
			return;
		};

		
	}

	SHORT control_state;

	control_state = GetKeyState(VK_CONTROL) & 0x8000;

	SelectRectangle->SET_begin_selection_range(low_level, local_local_mx, local_local_my, control_state);

}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE::get_element_by_line_idx(int val) {


	TRANSFER_DIALOG2_DirsFiles_TREE_element *e, *q;

	if (top == nullptr) return nullptr;

	e = top->child;
	
	while (e != nullptr) {

		if (e->line_idx == val) {
			return e;
		};

		if (e->is_open) {
			q = e->get_element_by_line_idx(val);
			if (q != nullptr) return q;
		}

		e = e->next;
	}

	return nullptr;
}

void TRANSFER_DIALOG2_DirsFiles_TREE::lock() {
	
	do
	{
		if (is_locked == false) {
			enter_crit(53);
			if (is_locked == false) {
				is_locked = true;
				leave_crit(53);
				return;
			}
			leave_crit(53);
		}
		else {
			::Sleep(0);
		}
	}
	while (true);
	
}

void TRANSFER_DIALOG2_DirsFiles_TREE::unlock() {
	if (is_locked == true) {
		is_locked = false;
	}
}

TRANSFER_DIALOG2_DirsFiles_TREE_element *TRANSFER_DIALOG2_DirsFiles_TREE::find_element_by_fullname(wchar_t *v) {
	if (v == nullptr || v[0] == 0) return nullptr;
	if (top == nullptr || top->child == nullptr) return nullptr;
	return top->child->get_element_by_fullname(v);
}

void TRANSFER_DIALOG2_DirsFiles_TREE::ASYNC_finish(GUI_low_level *low_level) {
	need_ASYNC_load->is_open = true;
	auto_arrange(low_level);
	need_ASYNC_load = nullptr;
	need_ASYNC_load_q = nullptr;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;

	if (need_ASYNC_load_name != nullptr && need_ASYNC_load_name[0] != 0) {
		e = find_element_by_fullname(need_ASYNC_load_name);
		zero_wchar_t(need_ASYNC_load_name, 5100);
		set_current_active_element(e);
		need_ASYNC_load_name[0] = 0;
	};
}

void TRANSFER_DIALOG2_DirsFiles_TREE::file_list_from_partner_RESPONCE(GUI_low_level *low_level, unsigned char *buf, int buf_size) {

	//send_udp("file_list_from_partner_RESPONCE()... begin");

	FL_.clean();
	FL_.set_from_buffer(buf, buf_size);
	FL_.sort_by_name();
	int i;
	i = 0;
	while (i < FL_.max_count && FL_.item[i] != nullptr && FL_.item[i]->is_use == true) {

		*need_ASYNC_load_q = new TRANSFER_DIALOG2_DirsFiles_TREE_element(FL_.item[i]->name, FL_.item[i]->is_folder, FL_.item[i]->size, FL_.item[i]->date, need_ASYNC_load, nullptr);

		need_ASYNC_load_q = &((*need_ASYNC_load_q)->next);
		i++;
	};

	need_ASYNC_load->is_open = true;
	
	set_current_active_element(need_ASYNC_load);

	need_ASYNC_load = nullptr;
	need_ASYNC_load_q = nullptr;

	request_FILE_LIST_from_partner_STATUS = 0;

	auto_arrange(low_level);

	//send_udp("file_list_from_partner_RESPONCE()... end");
}

int gg = 5;


void TRANSFER_DIALOG2_DirsFiles_TREE::ASYNC(GUI_low_level *low_level) {
	if (need_ASYNC_load != nullptr) {

		if (LOCAL_or_PARTNER == PARTNER_FILES) {

			// запросим первые 100 файлов у партнера

			if (request_FILE_LIST_from_partner_STATUS == 0) {
				request_FILE_LIST_from_partner_STATUS = 1; // запросили партнера, теперь нужно ждать ответа

				need_ASYNC_load_q = &(need_ASYNC_load->child);

				parent->parent->viewer->request_FILE_LIST_from_partner(need_ASYNC_load);
			};
			return;
		};

		if (LOCAL_or_PARTNER == LOCAL_FILES) {

			if (need_ASYNC_load_q == nullptr) {
				
				LL.clean();

				need_ASYNC_load_q = &(need_ASYNC_load->child);

				wchar_t folder[5000];

				for (int i = 0; i < 5000; i++) folder[i] = 0;

				need_ASYNC_load->get_folder_full_name(folder);

				if (my_strlen(folder) > 5000 - 10) { ASYNC_finish(low_level); 	return; };

				wchar_t ff[5100];
				ff[0] = 0;

				wsprintf(ff, L"%s\\*.*", folder);

				

				ASYNC_h = FindFirstFile(ff, &ASYNC_fd);
				if (ASYNC_h == INVALID_HANDLE_VALUE) { 
					ASYNC_finish(low_level);
					return; 
				};

				LOAD_INDICATOR->set_progress(0);
			}
			else {
				if (FindNextFile(ASYNC_h, &ASYNC_fd) == false) { 
					LL.sort_by_name();
					LL.to_tree(low_level, need_ASYNC_load, need_ASYNC_load_q);
					set_current_active_element(need_ASYNC_load);
					ASYNC_finish(low_level);
					return; 
				};
				LOAD_INDICATOR->increase_progress();
			};
			
			DWORD dd;
			dd = GetTickCount();

			do
			{

				//::Sleep(300);

				unsigned long long sz, dt;
				sz = ASYNC_fd.nFileSizeLow;

				if ((ASYNC_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
					sz = -1;
				}

				dt = (unsigned long long)ASYNC_fd.ftLastWriteTime.dwHighDateTime << 32 | (unsigned long long)ASYNC_fd.ftLastWriteTime.dwLowDateTime;

				//FileTimeToLocalFileTime((FILETIME *)&dt, (FILETIME *)&dt); //TRANSFER_DIALOG2_DirsFiles_TREE::ASYNC()

				if ((ASYNC_fd.cFileName[0] == '.' && ASYNC_fd.cFileName[1] == 0) ||
					(ASYNC_fd.cFileName[0] == '.' && ASYNC_fd.cFileName[1] == '.' && ASYNC_fd.cFileName[2] == 0))
				{
				}
				else {
					//add(fd.cFileName, sz, dt);

					bool bb;

					if ((ASYNC_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
						bb = true;
					}
					else {
						bb = false;
					}

					LL.add(ASYNC_fd.cFileName, bb, sz, dt);

					//*need_ASYNC_load_q = new TRANSFER_DIALOG2_DirsFiles_TREE_element(low_level, ASYNC_fd.cFileName, bb, sz, dt, need_ASYNC_load, nullptr);

					//need_ASYNC_load_q = &((*need_ASYNC_load_q)->next);
				};

				if (dd + 1000 < GetTickCount()) {
					return;
				}
				if (FindNextFile(ASYNC_h, &ASYNC_fd) == false) { 
					LL.sort_by_name();
					LL.to_tree(low_level, need_ASYNC_load, need_ASYNC_load_q);
					set_current_active_element(need_ASYNC_load);
					ASYNC_finish(low_level);
					return; 
				};
				LOAD_INDICATOR->ASYNC(low_level);
				LOAD_INDICATOR->increase_progress();

			} while (true);
			
			return;
		};

		//WTF??

		need_ASYNC_load = nullptr;

		/*
		----------------------

		
		wchar_t ww[500];
		bool dd_is_folder;
		dd_is_folder = false;
		unsigned long long  sz = 123, dt = 0;

		TRANSFER_DIALOG2_DirsFiles_TREE_element *q;

		wsprintf(ww, L"item_%d", gg);

		*need_ASYNC_load_q = new TRANSFER_DIALOG2_DirsFiles_TREE_element(low_level, ww, dd_is_folder, sz, dt, need_ASYNC_load, nullptr);
		need_ASYNC_load_q = &((*need_ASYNC_load_q)->next);

		gg++;
		if (gg > 10) {
			ASYNC_finish();
		}
		*/
	}
}

void TRANSFER_DIALOG2_DirsFiles_TREE::need_LOAD_fileslist(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *e) {
	if (e->is_open == true || e->child != nullptr) {
		return;
	}
	
	if (need_ASYNC_load != nullptr) return;

	need_ASYNC_load = e;
	
	low_level->invalidate();
	// ƒальнейша€ обработка происходит в процедуре ASYNC()

	
}

void TRANSFER_DIALOG2_DirsFiles_TREE::need_CLOSE(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *e) {
	
	lock();
	
	e->is_open = false;
	e->clean_child();
	auto_arrange(low_level);

	unlock();
}


void TRANSFER_DIALOG2_DirsFiles_TREE::mouse_over(GUI_low_level *low_level, int mouse_x, int mouse_y) {

	if (need_ASYNC_load != nullptr) return;

	int local_mouse_x, local_mouse_y, yy;

	local_mouse_x = mouse_x - x;
	local_mouse_y = mouse_y - y;

	int control_state;
	control_state = GetKeyState(VK_CONTROL) & 0x8000;

	if (SelectRectangle->x != -1) {
		SelectRectangle->SET_end_selection_range(low_level, local_mouse_x, local_mouse_y, control_state);
	
		if (SelectRectangle->y != -1) {
			if (local_mouse_y < _line_h) {
				yy = local_mouse_y + top_visible_line * _line_h;
				if (yy < SelectRectangle->y) {
					if (top_visible_line > 0) {
						top_visible_line--;
						RECALC_polzunok_position(low_level);
						generate_info();
						low_level->invalidate();
					};
				}
			}
			if (local_mouse_y > h - _line_h) {
				if (top_visible_line < all_line_count - visible_line_count) {
					top_visible_line++;
					RECALC_polzunok_position(low_level);
					generate_info();
					low_level->invalidate();
				}
			}
		}
		
		low_level->invalidate();
	}
}

void TRANSFER_DIALOG2_DirsFiles_TREE::mouse_left_button_up(GUI_low_level *low_level) {

	if (need_ASYNC_load != nullptr) return;

	SelectRectangle->x = -1;
	SelectRectangle->y = -1;
	SelectRectangle->w = 0;
	SelectRectangle->h = 0;
	
	//top->SELECT_element_from_to(-1, -1);
	low_level->invalidate();
}

void TRANSFER_DIALOG2_DirsFiles_TREE::RECALC_polzunok_position(GUI_low_level *low_level) {
	if (all_line_count != 0) {
		polzunok_position_in_percent = (float)top_visible_line / (float)(all_line_count - visible_line_count)* (float)100;
		polzunok_position_in_pixel = (int)((float)(h - _V_Scroll_w * 2 - polzunok_size_in_pixel) / (float)100 * (float)polzunok_position_in_percent);
	}
	else {
		polzunok_position_in_percent = 0;
		polzunok_position_in_pixel = 0;
	}
}

void TRANSFER_DIALOG2_DirsFiles_TREE::top_arrow_click(GUI_low_level *low_level) {

	if (need_ASYNC_load != nullptr) return;

	if (top_visible_line > 0) top_visible_line--;
	RECALC_polzunok_position(low_level);
	generate_info();
	low_level->invalidate();
}

void TRANSFER_DIALOG2_DirsFiles_TREE::bottom_arrow_click(GUI_low_level *low_level) {

	if (need_ASYNC_load != nullptr) return;

	top_visible_line++;
	if (top_visible_line > all_line_count - visible_line_count) top_visible_line = all_line_count - visible_line_count;
	if (top_visible_line < 0) top_visible_line = 0;
	/*
	if (all_line_count != 0) {
		polzunok_position_in_percent = (float)top_visible_line / (float)(all_line_count - visible_line_count) * (float)100;
		polzunok_position_in_pixel = (float)(h - _V_Scroll_w * 2 - polzunok_size_in_pixel) / (float)100 * (float)polzunok_position_in_percent;
	}
	else {
		polzunok_position_in_percent = 0;
		polzunok_position_in_pixel = 0;
	}
	*/
	RECALC_polzunok_position(low_level);
	generate_info();
	low_level->invalidate();
}

void TRANSFER_DIALOG2_DirsFiles_TREE::PgUp_polzunok_click(GUI_low_level *low_level) {
	
	if (need_ASYNC_load != nullptr) return;
	
	int v;
	
	if (top_visible_line > 0) {
		v = top_visible_line - (visible_line_count - 1);
		if (v < 0) v = 0;
		top_visible_line = v;
		RECALC_polzunok_position(low_level);
	};
};

void TRANSFER_DIALOG2_DirsFiles_TREE::PgDown_polzunok_click(GUI_low_level *low_level) {

	if (need_ASYNC_load != nullptr) return;

	int v;

	
	v = top_visible_line + (visible_line_count - 1);
	if (v > all_line_count - visible_line_count) v = all_line_count - visible_line_count;
	top_visible_line = v;
	RECALC_polzunok_position(low_level);
	
};

int ii0 = 0;

void TRANSFER_DIALOG2_DirsFiles_TREE::generate_info() {
	parent->Toolbar->lock();

	int xx;
	xx = -22;
	if (SelectRectangle->SOLO_selection != nullptr) {
		xx = SelectRectangle->SOLO_selection->line_idx;
	}

	sprintf__s_i_i_i_i_i(parent->Toolbar->ss_, 490, "%d top_visible_line=%d visible_line_count=%d all_line_count=%d xx=%d ", ii0++, top_visible_line, visible_line_count, all_line_count , xx);
	
	parent->Toolbar->ss2_[0] = 0;
	//sprintf__s(parent->Toolbar->ss2_, 490, "polz_size_in_pixel=%d | polz_size_in_percent=%.2f | polz_position_in_pixel=%d | polz_position_in_percent=%.2f", polzunok_size_in_pixel, polzunok_size_in_percent, polzunok_position_in_pixel, polzunok_position_in_percent );

	parent->Toolbar->unlock();
}

void TRANSFER_DIALOG2_DirsFiles_TREE::SET_polzunok_position_in_percent(GUI_low_level *low_level, float v) {
	polzunok_position_in_percent = v;

	if (all_line_count - visible_line_count <= 0) {
		top_visible_line = 0;
	}
	else {
		top_visible_line = (int)(((float)all_line_count - (float)visible_line_count) / (float)100 * (float)v);
		if (top_visible_line < 0) top_visible_line = 0;
	}

	low_level->invalidate();
}

int TRANSFER_DIALOG2_DirsFiles_TREE::get_global_x() { return x + parent->get_global_x(); };
int TRANSFER_DIALOG2_DirsFiles_TREE::get_global_y() { return y + parent->get_global_y(); };

void TRANSFER_DIALOG2_DirsFiles_TREE::char_keydown(GUI_low_level *low_level, int msg, int wp, int lp) {

	if (need_ASYNC_load != nullptr) return;

	SHORT shift_state;

	shift_state = GetKeyState(VK_SHIFT) & 0x8000;

	if (wp == 38) { // arr up
		SelectRectangle->moveUP_SOLO_selection(low_level, shift_state);
	}
	if (wp == 40) { // arr down
		SelectRectangle->moveDOWN_SOLO_selection(low_level, shift_state);
	}
	if (wp == 39) { //arr right
		if (SelectRectangle->SOLO_selection != nullptr) {
			parent->Tree->need_LOAD_fileslist(low_level, SelectRectangle->SOLO_selection);
		};
	};
	if (wp == 37) { // arr left
		if (SelectRectangle->SOLO_selection != nullptr) {
			parent->Tree->need_CLOSE(low_level, SelectRectangle->SOLO_selection);
		};
	};
	if (wp == 33) { // PgUp
		SelectRectangle->movePGUP_SOLO_selection(low_level);
	};
	if (wp == 34) { // PgDown
		SelectRectangle->movePGDOWN_SOLO_selection(low_level);
	};
}

//**********************************************************************************

void SELECT_RECTANGLE::Paint(GUI_low_level *low_level) {

	unsigned int clr1 = 0xffaa00;

	int x1, y1, w1, h1, ww, hh;

	x1 = x + w;
	w1 = w * -1;


	if (w > 0) {
		x1 = x;
		w1 = w;
	}
	else {
		if (w < 0)
		{
			x1 = x + w;
			w1 = w * -1;
		}
	}

	if (h > 0) {
		y1 = y;
		h1 = h;
	}
	else {
		y1 = y + h;
		h1 = h * -1;
	}

	y1 -= parent->top_visible_line * _line_h;
	

	if (w1 > 0 && h1 > 0) {

		if (x1 < 0) {
			w1 += x1;
			x1 = 0;
		}

		if (y1 < 0) {
			h1 += y1;
			y1 = 0;
		}

		ww = w1;
		if (x1 + ww + 1 > parent->w) {
			ww = parent->w - x1 - 1;
		}
		hh = h1;
		if (y1 + hh + 1 > parent->h) {
			hh = parent->h - y1 - 1;
		}

		low_level->fill_rectangle(x1 + parent->get_global_x(), y1 + parent->get_global_y(), ww, hh, 0xffaa00, 30);

		low_level->line_v(x1 + parent->get_global_x(), y1 + parent->get_global_y(), hh, clr1);
		low_level->line_v(x1 + parent->get_global_x() + ww-1, y1 + parent->get_global_y(), hh, clr1);

		low_level->line_h(x1 + parent->get_global_x(), y1 + parent->get_global_y(), ww, clr1);
		low_level->line_h(x1 + parent->get_global_x(), y1 + parent->get_global_y()+hh-1, ww, clr1);
	}
};

SELECT_RECTANGLE::SELECT_RECTANGLE( TRANSFER_DIALOG2_DirsFiles_TREE *parent_) {
	
	parent = parent_;
}
SELECT_RECTANGLE::~SELECT_RECTANGLE() {

}

void SELECT_RECTANGLE::set_SOLO_selection(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *v) {
	SOLO_selection = v;
	parent->set_current_active_element(v);
	parent->generate_info();
	low_level->invalidate();
}

void SELECT_RECTANGLE::SET_begin_selection_range(GUI_low_level *low_level, int xx, int yy, int control_state) {
	x = xx;
	y = yy + parent->top_visible_line * _line_h;
	w = 0;
	h = 0;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;

	

	parent->top->child->SELECT_element_from_to(y / _line_h , y / _line_h, control_state);

	e = parent->top->child->get_element_by_line_idx(y / _line_h);
	if (e != nullptr) {
		set_SOLO_selection(low_level, e);
	}

	low_level->invalidate();
}

void SELECT_RECTANGLE::SET_end_selection_range(GUI_low_level *low_level, int xx, int yy, int control_state) {

	yy += parent->top_visible_line * _line_h;

	w = xx - x;
	h = yy - y;

	int y1, y2, i1, i2;
	if (h > 0) {

		y1 = y;
		y2 = y1 + h - 1;

	}
	else {
		y1 = y + h;
		y2 = y1 + h * -1 - 1;
	}

	i1 = y1 / _line_h;
	i2 = y2 / _line_h;

	parent->top->child->SELECT_element_from_to(i1 , i2 , control_state);
	
	low_level->invalidate();
}

void SELECT_RECTANGLE::movePGUP_SOLO_selection(GUI_low_level *low_level) {
	if (SOLO_selection == nullptr) return;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	int i;

	if (SOLO_selection->line_idx > parent->top_visible_line) {
		e = parent->top->child->get_element_by_line_idx(parent->top_visible_line);
		if (e != nullptr) {
			parent->top->child->UNSELECT_all_elements();

			SOLO_selection = e;
			SOLO_selection->is_selected = true;

			parent->set_current_active_element(e);

			low_level->invalidate();
			return;
		}
	}

	if (SOLO_selection->line_idx == parent->top_visible_line) {
		if (parent->top_visible_line <= 0) return;

		i = parent->top_visible_line - (parent->visible_line_count - 1);
		if (i < 0) i = 0;
		

		e = parent->top->child->get_element_by_line_idx(i);
		if (e != nullptr) {
			parent->top->child->UNSELECT_all_elements();
			parent->top_visible_line = i;

			SOLO_selection = e;
			SOLO_selection->is_selected = true;

			parent->set_current_active_element(e);

			parent->RECALC_polzunok_position(low_level);

			low_level->invalidate();
			return;
		}

	};

};
void SELECT_RECTANGLE::movePGDOWN_SOLO_selection(GUI_low_level *low_level) {
	if (SOLO_selection == nullptr) return;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	int i, j;

	if (SOLO_selection->line_idx < parent->top_visible_line + parent->visible_line_count - 1) {

		i = parent->top_visible_line + parent->visible_line_count - 1;
		if (i > parent->all_line_count) i = parent->all_line_count;

		e = parent->top->child->get_element_by_line_idx(i);
		if (e != nullptr) {
			parent->top->child->UNSELECT_all_elements();

			SOLO_selection = e;
			SOLO_selection->is_selected = true;

			parent->set_current_active_element(e);

			low_level->invalidate();
			return;
		}
	}

	if (SOLO_selection->line_idx == parent->top_visible_line + parent->visible_line_count - 1) {
		

		i = SOLO_selection->line_idx + (parent->visible_line_count - 1);
		if (i >= parent->all_line_count) i = parent->all_line_count-1;

		e = parent->top->child->get_element_by_line_idx(i);
		if (e != nullptr) {
			j = i - (parent->visible_line_count - 1);
			if (j < 0) j = 0;
			parent->top_visible_line = j;
			
			parent->top->child->UNSELECT_all_elements();

			SOLO_selection = e;
			SOLO_selection->is_selected = true;

			parent->set_current_active_element(e);

			parent->RECALC_polzunok_position(low_level);

			low_level->invalidate();
			return;
		};

		/*
		i = parent->top_visible_line - (parent->visible_line_count - 1);
		if (i < 0) i = 0;


		e = parent->top->child->get_element_by_line_idx(i);
		if (e != nullptr) {
			parent->top->child->UNSELECT_all_elements();
			parent->top_visible_line = i;

			SOLO_selection = e;
			SOLO_selection->is_selected = true;

			parent->RECALC_polzunok_position();

			low_level->invalidate();
			return;
		}
		*/
	};
};

void SELECT_RECTANGLE::moveUP_SOLO_selection(GUI_low_level *low_level, SHORT shift_state) {
	if (SOLO_selection == nullptr) return;
	
	int i;

	i = SOLO_selection->line_idx;
	i--;
	if (i < 0) return;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	e = parent->top->child->get_element_by_line_idx(i);

	if (e != nullptr) {

		if(shift_state == 0) parent->top->child->UNSELECT_all_elements();
	
		SOLO_selection = e;
		SOLO_selection->is_selected = true;
		
		parent->set_current_active_element(e);

		if (i < parent->top_visible_line) {
			parent->top_visible_line = i;
			parent->RECALC_polzunok_position(low_level);
		}

		low_level->invalidate();
	}

}

void SELECT_RECTANGLE::moveDOWN_SOLO_selection(GUI_low_level *low_level, SHORT shift_state) {
	if (SOLO_selection == nullptr) return;

	int i;

	i = SOLO_selection->line_idx;
	i++;
	

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e;
	e = parent->top->child->get_element_by_line_idx(i);

	if (e != nullptr) {

		if(shift_state == 0) parent->top->child->UNSELECT_all_elements();

		SOLO_selection = e;
		SOLO_selection->is_selected = true;
		
		parent->set_current_active_element(e);

		if (i > parent->top_visible_line + parent->visible_line_count - 1) {
			parent->top_visible_line = i - parent->visible_line_count + 1;
			parent->RECALC_polzunok_position(low_level);
		}

		low_level->invalidate();
	}
}

