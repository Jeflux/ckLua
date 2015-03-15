#include "ckLua.h"

#include <iostream>
#include <sstream>

#include <Windows.h>
#include "MainForm.h"


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
	
	RAWINPUTDEVICE device;
	device.usUsagePage = 0x01;
	device.usUsage = 0x06;
	device.dwFlags = RIDEV_NOLEGACY|RIDEV_INPUTSINK;
	device.hwndTarget = GetActiveWindow();
	if (RegisterRawInputDevices(&device, 1, sizeof(device)) == FALSE){
		std::cout << GetLastError() << std::endl;
	}

	lua.run();
	//Application::Run();

	return 0;
}