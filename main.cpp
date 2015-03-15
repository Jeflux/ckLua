#include "ckLua.h"

#include <iostream>
#include <sstream>

#include <Windows.h>
#include "MainForm.h"

[System::STAThread]
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	// Redirect stdout
	std::ostringstream out;
	std::cout.rdbuf(out.rdbuf());

	// Create model and controller
	ckLua lua = ckLua();
	MainForm^ controller = gcnew MainForm(&lua, &out);

	// Run "main loop"
	lua.run();

	return 0;
}