#include "LuaCallback.h"
#include <Logic/Scripting/LuaState.h>

using yang::LuaCallback;

yang::LuaCallback::LuaCallback(size_t luaRef)
    :m_luaRef(luaRef)
{
}
