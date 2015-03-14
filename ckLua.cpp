#include "ckLua.h"
#include <ctime>

#include <iostream>
#include <Windows.h>

#include "LibCorsairRGB\LibCorsairRGB.h"
#include "keymapParser.h"

// Wrap callback pointers to right object
ckLua* context = NULL;
void _keyDown(int key, int flags) {
	if (context != NULL)
		context->keyDownCallback(key, flags);
}
void _keyUp(int key, int flags) {
	if (context != NULL)
		context->keyUpCallback(key, flags);
}

// Wrap lua functions
int _setLed(lua_State* L)	{ if (context != NULL) return context->setLed(L); return 0; }
int _setUPS(lua_State* L)	{ if (context != NULL) return context->setUPS(L); return 0; }
int _setScript(lua_State* L){ if (context != NULL) return context->setScript(L); return 0; }
int _setKeymap(lua_State* L){ if (context != NULL) return context->setKeymap(L); return 0; }
int _luaPanic(lua_State* L) { if (context != NULL) return context->luaPanic(L); return 0; }

// Use cout (redirected) instead of whatever lualib is using
int _luaPrint(lua_State* L) {
	int nargs = lua_gettop(L);

	for (int i = 1; i <= nargs; i++) {
		if (lua_isstring(L, i)) {
			std::cout << lua_tostring(L, i);

			if (i < nargs + 1)
				std::cout << '\t';
		}
	}

	std::cout << std::endl;
	return 0;
}


jmp_buf jumpBuf;
ckLua::ckLua(): 
	deltaTime(0),
	colorChange(false),
	targetFPS(60),
	terminate(false),
	luaError(false),
	altgrdown(false),
	altgrup(false) {
	context = this;

	L = lua_open();
	luaL_openlibs(L);

	// Register global callbacks
	lua_register(L, "setLed", _setLed);
	lua_register(L, "setUPS", _setUPS);
	lua_register(L, "loadKeymap", _setKeymap);
	lua_register(L, "loadScript", _setScript);
	lua_register(L, "print", _luaPrint);
	lua_atpanic(L, _luaPanic);

	lastUpdateTime = std::clock();

	// Create listener
	inputListener = InputListener(&_keyDown, &_keyUp);

	int status = lcrgb_initialise();
	if (status != 0) {
		printf("Failed to intialise LibCorsairRGB\n");
		return;
	}
	atexit(lcrgb_deinitialise);
}

ckLua::~ckLua() {
	lua_close(L);
}

void ckLua::kill() {
	terminate = true;
}

void ckLua::run(bool lowLevelHook) {
	// Only hook if low level
	if (lowLevelHook)
		inputListener.hook();


	while (!terminate) {
		if (beginFrame()) {

			setjmp(jumpBuf); // Set jump buffer if we fail
			if (luaError) continue;

			// Call update function in lua
			lua_getglobal(L, "update");
			lua_pushnumber(L, deltaTime);
			lua_call(L, 1, 0);


			// Only flush colors if colors has changed
			if (colorChange) {
				lcrgb_flush_light_buffer();
				colorChange = false;
			}
		}


		// Message loop: needed to forward message to currently running application.
		// Without this, only this program would receive keyboard input
		MSG msg;
		if (MsgWaitForMultipleObjects(0, NULL, FALSE, 17, QS_ALLINPUT) == WAIT_OBJECT_0)
		{
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				// Only intercept if not low level
				if (!lowLevelHook)
					inputListener.interceptMessage(msg);

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	// Cleanly unhook listener
	if (lowLevelHook)
		inputListener.unhook();
}

// Check if time to update
bool ckLua::beginFrame() {
	std::clock_t now = std::clock();

	if ((deltaTime = (now - lastUpdateTime) / 1000.0f) >= 1.0f / targetFPS) {
		lastUpdateTime	= std::clock();
		return true;
	}
	
	return false;
}

// Callback from hook
void ckLua::keyDownCallback(int key, int flags) {
	setjmp(jumpBuf); // Set jump buffer if we fail
	if (luaError) return;

	// Setup call to lua
	lua_getglobal(L, "keyDown");
	lua_pushinteger(L, key);
	lua_pushnumber(L, keymap[key].x);
	lua_pushnumber(L, keymap[key].y);
	lua_pushstring(L, keymap[key].name.c_str());
	lua_call(L, 4, 0);
}

// Callback from hook
void ckLua::keyUpCallback(int key, int flags) {
	setjmp(jumpBuf); // Set jump buffer if we fail
	if (luaError) return;

	// Set up call to lua
	lua_getglobal(L, "keyUp");
	lua_pushnumber(L, keymap[key].x);
	lua_pushnumber(L, keymap[key].y);
	lua_pushstring(L, keymap[key].name.c_str());
	lua_call(L, 3, 0);
}

void ckLua::changeScript(const std::string& fileName) {
	luaError = false;

	setjmp(jumpBuf);
	if (luaError) return;

	std::cout << "Loading script: " << fileName << std::endl;
	luaL_dofile(L, fileName.c_str());
	lua_getglobal(L, "create");
	lua_call(L, 0, 0);
}

void ckLua::changeKeyConfig(const std::string& fileName) {
	std::cout << "Loading keymap: " << fileName << std::endl;
	keymap = KeymapParser::parseFile(fileName);
}



//
// Lua callbacks down here
//

int ckLua::setLed(lua_State* L) {
	int n = lua_gettop(L);
	
	int x, y, r, g, b;
	std::string key;

	// Determine which call was made
	switch (n) {
		// Set color based on position
		case 5:
			// Get values from stack
			x = (int) lua_tonumber(L, 1);
			y = (int) lua_tonumber(L, 2);
				
			r = (int) lua_tonumber(L, 3);
			g = (int) lua_tonumber(L, 4);
			b = (int) lua_tonumber(L, 5);
		
			// Remove values from stack
			lua_pop(L, 5);

			lcrgb_set_position(x, y, r, g, b);
			colorChange = true;
			break;

		// Set color based on key code
		case 4:
			// Get values from stack
			key = lua_tostring(L, 1);

			r = (int) lua_tonumber(L, 2);
			g = (int) lua_tonumber(L, 3);
			b = (int) lua_tonumber(L, 4);

			// Remove values from stack
			lua_pop(L, 4);

			int keycode = 0xFF;
			for each (std::pair<int, ckKey> k in keymap) {
				if (k.second.name == key) {
					lcrgb_set_position((int) k.second.x, (int) k.second.y, r, g, b);
					colorChange = true;
					break;
				}
			}

			break;
	}

	return 0;
}

int ckLua::setUPS(lua_State* L) {
	int n = lua_gettop(L);
	if (n > 1) return 0;

	targetFPS = (int) lua_tonumber(L, 1);
	lua_pop(L, 1);

	// Clamp values
	if (targetFPS > 60)
		targetFPS = 60;
	else if (targetFPS <= 0)
		targetFPS = 1;

	return 0;
}

int ckLua::setScript(lua_State* L){
	int n = lua_gettop(L);
	if (n > 1) return 0;

	changeScript(lua_tostring(L, 1));
	lua_pop(L, 1);
	return 0;
}

int ckLua::setKeymap(lua_State* L) {
	int n = lua_gettop(L);
	if (n > 1) return 0;

	keymap = KeymapParser::parseFile(lua_tostring(L, 1));
	lua_pop(L, 1);
	return 0;
}

int ckLua::luaPanic(lua_State* L) {
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);

	std::cout << std::endl;
	std::cout << "Lua error: \n\t" << err << std::endl;
	std::cout << std::endl;
	luaError = true;

	longjmp(jumpBuf, 1);

	return 0;
}