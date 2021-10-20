#include "stdafx.h"

#include "EVENT_QUEUE.h"


void EVENT_QUEUE_element::set(uint16_t global_type_, uint16_t type_, uint16_t data_1_, uint16_t data_2_) {
	global_type = global_type_;
	type = type_;
	data_1 = data_1_;
	data_2 = data_2_;
	//send_udp("EVENT_QUEUE_element::set() ", global_type);
}
void EVENT_QUEUE_element::get(uint16_t &global_type_, uint16_t &type_, uint16_t &data_1_, uint16_t &data_2_) {
	global_type_ = global_type;
	type_ = type;
	data_1_ = data_1;
	data_2_ = data_2;
}
void EVENT_QUEUE_element::clean() {
	global_type = 0;
	type = 0;
	data_1 = 0;
	data_2 = 0;
}
EVENT_QUEUE_element::EVENT_QUEUE_element(void) {
	clean();
}
///////////////////////////////////////////////////////////////////////


EVENT_QUEUE::EVENT_QUEUE()
{
}


EVENT_QUEUE::~EVENT_QUEUE()
{
}

void EVENT_QUEUE::clean() {
	for (int i = 0; i < element_max_count; i++) {
		element[i].clean();
	}
}

void EVENT_QUEUE::addd(uint16_t global_type_, uint16_t type_, uint16_t data_1_, uint16_t data_2_, const char *info) {
	if (type_ != 5555) {
		//send_udp("+");
	}
	//send_udp("global_type_", global_type_);
	//send_udp("type", type_);
	//send_udp("data_1_", data_1_);
	//send_udp("data_2_", data_2_);
	if (type_ == 0) return;
	if (type_ == event_key_click && data_1_ == 0 && data_2_ == 0) return;

	//ALOG( "EVENT_QUEUE::add(+ %s , %d , %d ) info={%s} ", decode_event_type(type_), data_1_, data_2_ , info);
	//send_udp("EVENT_QUEUE::add() ", global_type_);
	int i;
	i = 0;
	while (i < element_max_count && element[i].type != 0) i++;
	if (i >= element_max_count) {
		//ALOG("EVENT_QUEUE переполнена");
		return;
	}
	element[i].set(global_type_, type_, data_1_, data_2_);

}
bool EVENT_QUEUE::get(uint16_t &global_type_, uint16_t &type_, uint16_t &data_1_, uint16_t &data_2_) {
	if (element[0].type == 0) {
		global_type_ = 0;
		type_ = 0;
		data_1_ = 0;
		data_2_ = 0;
		return false;
	}
	//send_udp("get ");
	element[0].get(global_type_, type_, data_1_, data_2_);
	//send_udp("EVENT_QUEUE::get(...) ", global_type_);
	//ALOG("EVENT_QUEUE::get( %d , %d , %d )", type_ , data_1_, data_2_ );
	int i;
	i = 0;
	while (i < element_max_count - 1) {
		element[i].type = element[i + 1].type;
		element[i].data_1 = element[i + 1].data_1;
		element[i].data_2 = element[i + 1].data_2;
		i++;
	}
	element[element_max_count - 1].type = 0;
	element[element_max_count - 1].data_1 = 0;
	element[element_max_count - 1].data_2 = 0;
	return true;
}
