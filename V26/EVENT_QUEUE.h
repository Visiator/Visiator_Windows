#pragma once

#include <cstdint>

#include "tools.h"



#define event_empty 0
#define event_mouse_move 1

#define event_mouse_press_left_button 2
#define event_mouse_press_right_button 3

#define event_mouse_unpress_left_button 4
#define event_mouse_unpress_right_button 5

#define event_mouse_click_left_button 6
#define event_mouse_click_right_button 7

#define event_mouse_doubleclick_left_button 8
#define event_mouse_doubleclick_right_button 9

#define event_key_press 10
#define event_key_unpress 11
#define event_key_click 12

#define this_is_left_mouse_button 13
#define this_is_right_mouse_button 14
#define this_is_whell_v 20
#define event_ctrl_alt_del 15


class EVENT_QUEUE_element {
public:
	uint16_t global_type = 0;
	uint16_t type = 0; // 0 - значит элемент пустой
	// 1 - mouse move. (пока не отслеживаем)
	// 2 - mouse press
	// 3 - mouse unpress
	// 4 - mouse click
	// 5 - mouse doubleclick
	// 6 - key press
	// 7 - key unpress
	// 8 - key click
	uint16_t data_1 = 0, data_2 = 0;
	void set(uint16_t global_type, uint16_t type_, uint16_t data_1_, uint16_t data_2_);
	void get(uint16_t &global_type_, uint16_t &type_, uint16_t &data_1_, uint16_t &data_2_);
	void clean();
	EVENT_QUEUE_element(void);

};


class EVENT_QUEUE
{
public:

	EVENT_QUEUE_element element[50];
	int element_max_count = 50;

	void addd(uint16_t global_type_, uint16_t type_, uint16_t data_1_, uint16_t data_2_, const char *info);
	bool get(uint16_t &global_type_, uint16_t &type_, uint16_t &data_1_, uint16_t &data_2_);

	void clean();


	EVENT_QUEUE();
	~EVENT_QUEUE();
};

