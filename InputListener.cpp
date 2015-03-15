#include "InputListener.h"

#include <string>

#include <Windows.h>
#include <iostream>
#pragma comment(lib,"user32.lib")

HHOOK _hook = NULL;

void(*keyDownCallback)(int, int);
void(*keyUpCallback)(int, int);

// Not used anymore. Keeping for reference (used by low level hook)
/*LRESULT CALLBACK hookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;

	// Check which kind of event was received
	switch (wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (keyDownCallback != NULL)
				keyDownCallback(kbdStruct->vkCode, kbdStruct->flags);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (keyUpCallback != NULL)
				keyUpCallback(kbdStruct->vkCode, kbdStruct->flags);
			break;
	}

	return  CallNextHookEx(_hook, nCode, wParam, lParam);
}*/

InputListener::InputListener(void(*keyDown)(int, int), void(*keyUp)(int, int)) :
	hooked(false){
	keyDownCallback = keyDown;
	keyUpCallback = keyUp;

	for (int i = 0; i < sizeof(keyPressed); i++)
		keyPressed[i] = false;
	int a = 0;
}

InputListener::~InputListener() {
	unhook();
}

void InputListener::hook() {
	if (hooked) return;
	hooked = true;
	//_hook = SetWindowsHookEx(WH_KEYBOARD_LL, hookCallback, NULL, NULL); // Keeping for reference
}

void InputListener::unhook() {
	if (!hooked) return;
	hooked = false;
	
	//UnhookWindowsHookEx(_hook); // Keeping for reference

	keyDownCallback = NULL;
	keyUpCallback = NULL;
}

// Magic message interception here!
void InputListener::interceptMessage(MSG& msg) {
	// Check which type of message
	switch (msg.message) {
	case WM_INPUT:
		UINT dwSize;

		// Try to get raw data
		GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL, &dwSize,
			sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL) {
			break;
		}

		if (GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		// Raw data here
		RAWINPUT* raw = (RAWINPUT*)lpb;

		// Try to get device name. We only want to listen to corsair keyboards
		UINT nBufferSize;
		bool del = false;
		WCHAR* wcDeviceName = L"\\\\?\\HID#Vid_1B1C&Pid_1B13&MI_01"; // Assume it's good if we can't get it
		if (GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &nBufferSize) == 0) {
			del = true;
			wcDeviceName = new WCHAR[nBufferSize + 1];
			GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, wcDeviceName, &nBufferSize);
		}

		// Map virtual keys to scan codes
		int key = MapVirtualKey(raw->data.keyboard.VKey, 0);
		
		// If we have any flags describing if key is right version, use "the other side of the spectrum"
		if (((raw->data.keyboard.Flags & 0x6) >> 1) == 1) {
			key = 0xFC - key;
		}
		// Null key
		if (key == 0xFC)
			key = 0xFF;

		// Pause break is exception here
		if (key == 0x00 && ((raw->data.keyboard.Flags & 0x6) >> 1) == 2) {
			key = 0xFE;
		}
		// Special case for right shift (must check make code)
		else if (key == 0x2A && raw->data.keyboard.MakeCode == 0x36)
			key = 0xFC;
		

		// Check if corsair keyboard and keys are valid
		if (std::wcsncmp(wcDeviceName, L"\\\\?\\HID#Vid_1B1C&Pid_1B13&MI_01", 31) == 0 && key != 0x00 && key != 0xFF) {

			// Key down. Call function if key is pressed and not held
			if (!(raw->data.keyboard.Flags & RI_KEY_BREAK) && !keyPressed[key]) {
				keyDownCallback(key, raw->data.keyboard.Flags);
				keyPressed[key] = true;
			}

			// Key up
			else if ((raw->data.keyboard.Flags & RI_KEY_BREAK)) {
				keyUpCallback(key, raw->data.keyboard.Flags);
				keyPressed[key] = false;
			}
		
		}

		// Clean up
		if (del)
			delete[] wcDeviceName;

		break;
	}
}