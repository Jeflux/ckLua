#ifndef CKLUA_H
#define CKLUA_H

#include <string>
#include <ctime>
#include "InputListener.h"
#include "ckKey.h"
#include <unordered_map>
#include <setjmp.h>

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class ckLua {
public:
	ckLua();
	~ckLua();
	
	void run(bool lowLevelHook = false);

	void changeScript(const std::string& fileName);
	void changeKeyConfig(const std::string& fileName);


	void keyDownCallback(int key, int flag);
	void keyUpCallback(int key, int flag);

	// Lua callbacks
	int setLed(lua_State* L);
	int setUPS(lua_State* L);
	int setScript(lua_State* L);
	int setKeymap(lua_State* L);
	int luaPanic(lua_State* L);

	void kill();

private:
	bool beginFrame();

	std::clock_t lastUpdateTime;
	int targetFPS;

	lua_State* L;
	float deltaTime;

	InputListener inputListener;
	std::unordered_map<int, ckKey> keymap;
	
	bool colorChange;
	bool terminate;

	bool luaError;

	bool altgrdown;
	bool altgrup;
};


#endif