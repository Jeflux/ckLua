#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <Windows.h>

class InputListener {
public:
	InputListener() {}
	InputListener(void(*keyDown)(int, int), void(*keyUp)(int, int));
	~InputListener();

	void hook();
	void unhook();

	void interceptMessage(MSG& msg);

private:
	bool hooked;
	bool keyPressed[256];
};

#endif