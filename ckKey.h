#ifndef CKKEY_H
#define CKKEY_H

#include <string>

// Class for representing a key.
// A key has a scan code, key code, position and width
class ckKey {
public:
	ckKey(){}
	ckKey(int scancode, int keycode, float x, float y, float width, std::string name) : 
		scancode(scancode), 
		keycode(keycode), 
		x(x), 
		y(y),
		width(width),
		name(name){}

	int scancode, keycode;
	float x, y;
	float width;
	std::string name;
};

#endif