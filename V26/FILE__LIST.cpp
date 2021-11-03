#include "stdafx.h"

#include "TRANSFER_DIALOG2_DirsFiles_TREE_element.h"
#include "FILE__LIST.h"
#include "tools.h"

//##################################################################################

FILE__LIST_element::FILE__LIST_element() {

}

FILE__LIST_element::~FILE__LIST_element() {

}

void FILE__LIST_element::clean() {
	is_use = false;
	if (name != nullptr) name[0] = 0;
	is_folder = false;
	size = 0;
	date = 0;
}


FILE__LIST::FILE__LIST() {
	for (int i = 0; i < max_count; i++) {
		item[i] = nullptr;
	};
}

FILE__LIST::~FILE__LIST() {

}

void FILE__LIST::sort_by_name() {
	if (item[0] == nullptr || item[1] == nullptr) return;
	int i, a;
	bool flag;
	FILE__LIST_element *ee;
	a = 1;
	while (a == 1) {
		a = 0;
		i = 1;
		while (i < max_count && item[i] != nullptr && item[i]->is_use == true) {
			flag = false;

			if (item[i - 1]->is_folder == false && item[i]->is_folder == true) {
				flag = true;
			}
			else {
				if (my_strcmp(item[i - 1]->name, item[i]->name) > 0) {
					flag = true;
				};
			};

			if (flag == true) {
				ee = item[i - 1];
				item[i - 1] = item[i];
				item[i] = ee;
				a = 1;
			}
			i++;
		}
	}
}

void FILE__LIST::clean() {
	for (int i = 0; i < max_count; i++) {
		if (item[i] != nullptr) item[i]->clean();
	}
	size1 = 0;
}

void FILE__LIST::get(unsigned char *buf, unsigned int buf_size) {
	wchar_t *w;
	unsigned int *ii;

	ii = (unsigned int *)buf;
	w = (wchar_t *)&(buf[4]);

	unsigned __int16 *i16;
	unsigned long long *i64;
	int i, j, k;
	j = 0;
	i = 0;
	while (i < max_count && item[i] != nullptr && item[i]->is_use == true) {

		k = 0;
		while (item[i]->name[k] != 0) {
			w[j++] = item[i]->name[k++];
		}
		w[j++] = 0;

		i16 = (unsigned __int16 *)&w[j];
		if (item[i]->is_folder) *i16 = 1; else *i16 = 0;

		i64 = (unsigned long long *)&w[j + 1];
		*i64 = item[i]->size;
		i64++;
		*i64 = item[i]->date;

		j += 1 + 4 + 4;
		i++;
	}
	*ii = i;
}

void FILE__LIST::set_from_buffer(unsigned char *buf, unsigned int sz) {
	wchar_t ww[5100];
	wchar_t *w;
	unsigned int *i32, ii;
	unsigned __int16 *i16, ii16;
	unsigned long long *i64, sz_, dt_;
	bool is_folder_;

	ww[0] = 0;

	i32 = (unsigned int *)buf;
	ii = *i32; // кол-во записей
	w = (wchar_t *)&(buf[4]);;

	sz /= 2;

	unsigned int i, j, k;

	i = 0;
	j = 0;
	k = 0;
	while (i < sz && k < ii) {
		j = 0;
		while (i < sz && w[i] != 0) {
			ww[j++] = w[i++];
		};
		ww[j] = 0;

		i16 = (unsigned __int16 *)&w[i + 1];
		ii16 = *i16;
		if (ii16 == 1) {
			is_folder_ = true;
		}
		else {
			is_folder_ = false;
		};


		i64 = (unsigned long long *)&w[i + 2];
		sz_ = *i64;
		i64++;
		dt_ = *i64;

		add(ww, is_folder_, sz_, dt_); k++;

		i += 1 + 1 + 4 + 4;
	}

}


void FILE__LIST::add(wchar_t *name, bool is_folder_, unsigned long long size_, unsigned long long date_) {

	//SYSTEMTIME st;
	//FileTimeToSystemTime((FILETIME *)&date_, &st);
	//wchar_t xxx[1500];
	//swprintf_s(xxx, 1500, L"%s - %d.%d.%d | %d:%d:%d ", name, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	//send_udp2(xxx);


	int i, j;
	i = 0;
	while (i < max_count) {
		if (item[i] == nullptr || item[i]->is_use == false) {
			if (item[i] == nullptr) item[i] = new FILE__LIST_element();

			j = my_strlen(name);
			if (size1 == 0) size1 += 4;
			size1 += (j + 1) * 2 + 2 + 8 + 8;

			if (item[i]->name_size < j + 1) {
				if (item[i]->name != nullptr) delete[] item[i]->name;
				item[i]->name = new wchar_t[j + 1];
				item[i]->name_size = j + 1;
			};

			my_strcpy(item[i]->name, name);

			item[i]->is_folder = is_folder_;
			item[i]->size = size_;
			item[i]->date = date_;
			item[i]->is_use = true;

			return;
		}
		i++;
	}
}


bool FILE__LIST::check_allowed_path(wchar_t *pt) {

	wchar_t p0[] = { L"C:\\VISIATOR_FILES\\" };


	if (pt == nullptr) { return false; };
	if (pt[0] == 0) { return false; };
	if (pt[1] == 0) { return false; };
	if (pt[2] == 0) { return false; };

	int i;
	i = 0;
	while (i < 5000 && p0[i] != 0 && pt[i] != 0) {
		if (p0[i] != pt[i]) { return false; };
		i++;
	}

	wchar_t ww[5100];
	zero_wchar_t(ww, 5100);
	int j;
	j = 0;

	while (i < 5000 && pt[i] != 0) {

		if (pt[i] == '\\') {
			if (ww[0] == '.' || ww[1] == '.') { return false; };
			zero_wchar_t(ww, 5100);
			j = 0;
		}
		else {
			ww[j++] = pt[i];
			if (j >= 5000) { return false; };
			if (is_bad_symbol_file_name(pt[i])) {
				{ return false; };
			}

		};
		i++;
	}








	return true;
}

void FILE__LIST::to_tree(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *need_ASYNC_load, TRANSFER_DIALOG2_DirsFiles_TREE_element **&need_ASYNC_load_q) {

	int i;
	i = 0;
	while (i < max_count && item[i] != nullptr && item[i]->is_use == true) {

		*need_ASYNC_load_q = new TRANSFER_DIALOG2_DirsFiles_TREE_element(item[i]->name, item[i]->is_folder, item[i]->size, item[i]->date, need_ASYNC_load, nullptr);

		need_ASYNC_load_q = &((*need_ASYNC_load_q)->next);
		i++;
	};

	//*need_ASYNC_load_q = new TRANSFER_DIALOG2_DirsFiles_TREE_element(low_level, Name, bb, sz, dt, need_ASYNC_load, nullptr);
	//need_ASYNC_load_q = &((*need_ASYNC_load_q)->next);
}

void FILE__LIST::LOAD(wchar_t *folder) {
	//send_udp("FILE__LIST::LOAD() begin ");
	//send_udp(folder);

	clean();

	//add(L"1111", false, 123, 0);
	//add(L"2222", false, 223, 1);
	//add(L"3333", false, 323, 2);

	WIN32_FIND_DATA fd;

	HANDLE h;
	if (my_strlen(folder) > 5000 - 50) return;


	wchar_t ff[5000];
	ff[0] = 0;



	if (check_allowed_path(folder) == false) {
		return;
	}

	wsprintf(ff, L"%s\\*.*", folder);

	//send_udp(L"FILE__LIST::LOAD() ", ff);

	h = FindFirstFile(ff, &fd);
	if (h != INVALID_HANDLE_VALUE) {
		do {
			bool is_folder;
			unsigned long long sz, dt;
			sz = fd.nFileSizeLow;

			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
				sz = -1;
				is_folder = true;
			}
			else {
				is_folder = false;
			}

			dt = (unsigned long long)fd.ftLastWriteTime.dwHighDateTime << 32 | (unsigned long long)fd.ftLastWriteTime.dwLowDateTime;

			//FileTimeToLocalFileTime((FILETIME *)&dt, (FILETIME *)&dt); //FILE__LIST::LOAD

			if ((fd.cFileName[0] == '.' && fd.cFileName[1] == 0) ||
				(fd.cFileName[0] == '.' && fd.cFileName[1] == '.' && fd.cFileName[2] == 0))
			{
			}
			else {
				add(fd.cFileName, is_folder, sz, dt);
			};
		} while (FindNextFile(h, &fd));

		FindClose(h);
	}

	//sudp("FILE__LIST::LOAD() end ", size1);
}
