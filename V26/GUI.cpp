
#include "GUI.h"

GUI::GUI() {

}

void GUI::Paint(GUI_low_level *low_level) {
	//const_iterator it, end;
	std::list <GUI_Element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		// do nothing, for loop terminates if "it" points to "myElem"
		  // or if we don't find your element.
		(*it).Paint(low_level);
	}

}

GUI_Element *GUI::add_element(int type_, int x_, int y_, int w_, int h_, int BMP_id) {

	elements.emplace_back(type_, x_, y_, w_, h_, BMP_id);
	return &(elements.back());
}