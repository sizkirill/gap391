#include "LuaState.h"
#include <Utils/Logger.h>

using yang::LuaState;

void yang::LuaState::DoFile(const char* pFileName) const
{
	luaL_dofile(m_pState, pFileName);
}

LuaState::LuaState()
	:m_pState(luaL_newstate())
{
	if (!m_pState)
	{
		LOG(Error, "Lua failed to init"); 
	}
	luaL_openlibs(m_pState);
}

LuaState::~LuaState()
{
	lua_close(m_pState);
}

LuaState* LuaState::GetInstance()
{
	static LuaState s_instance;
	return &s_instance;
}
