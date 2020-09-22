#pragma once
/// \file LuaState.h
/// LuaState class description

#include <assert.h>
#include <numeric>
#include <type_traits>
#include <string>
#include <Utils/Logger.h>
#include <Lua/lua.hpp>
//#include <Logic/Scripting/LuaCallback.h>
#include <Utils/TypeTraits.h>

//! namespace yang Contains all Yangine code
namespace yang
{
/// \class LuaState
/// Wrapper class for lua_State. Owns the lua_State and interacts with it. Singleton class
class LuaState
{
public:
	~LuaState();

	/// Gets value of specified type from the top of the lua stack. Calls lua_pop on it.
    /// Note: We need to return std::decay_t of Type to handle references
	/// \tparam Type of the variable to get from lua stack
    /// \return std::decay of type that we got from lua stack
	template <class Type>
	std::decay_t<Type> Get() const;

	/// Pushes value of specified type to the top of the lua stack.
    /// \tparam Type - Type of a variable that we want to push to lua stack
    /// \param value - the instance of Type to push to lua stack
    /// \return 1
	template <class Type>
	int Push(Type value) const;

    /// Template specialization for pushing Vector2 to the top of the lua stack
    /// \tparam Type - template parameter of Vector2 that we are pushing
    /// \param value - the instance of Vector2<Type> to push
    /// \return 2
	template <class Type>
	int Push(Vector2<Type> value) const;

    /// Get LuaState singleton instance
	static LuaState* GetInstance();

    /// Call lua function by lua function using reference (see luaL_ref in Lua Reference Manual)
    /// \tparam ReturnType - expected type of the return value. Don't know what happens if we're trying to call it with the wrong type
    /// \tparam Args - Types of arguments that are going to be passed into the lua function
    /// \param luaRef - reference to a lua function
    /// \param args - arguments that are going to be passed into the lua function
    /// \return instance of ReturnType returned from Lua function
    template <class ReturnType, class... Args>
    ReturnType CallLuaFunction(size_t luaRef, Args&&... args) const;

    /// Call lua function using name of the function in the Lua environment
    /// \tparam ReturnType - expected type of the return value. Don't know what happens if we're trying to call it with the wrong type
    /// \tparam Args - Types of arguments that are going to be passed into the lua function
    /// \param functionName - name of the function in the lua world
    /// \param args - arguments that are going to be passed into the lua function
    /// \return instance of ReturnType returned from Lua function
    template <class ReturnType, class... Args>
    ReturnType CallLuaFunction(const char* functionName, Args&&... args) const;
private:
    /// Private default constructor
	LuaState();

	lua_State* m_pState;    ///< actual lua state object

    /// Internal helper function. Calls lua function from top of the stack
    /// \tparam ReturnType - expected type of the return value. Don't know what happens if we're trying to call it with the wrong type
    /// \tparam Args - Types of arguments that are going to be passed into the lua function
    /// \param args - arguments that are going to be passed into the lua function
    /// \return instance of ReturnType returned from Lua function
    template <class ReturnType, class... Args>
    ReturnType InternalCallLuaFunction(Args&& ... args) const;
public:
	/// raw lua_State* Getter (for any reason that it might be needed)
	lua_State* GetRaw() const { return m_pState; }
};
}

template<class Type>
inline std::decay_t<Type> yang::LuaState::Get() const
{
	using DecayedType = std::decay_t<Type>;
	// What happens if we use nullptr inside Lua C API?
    assert(m_pState);

	// TODO: count args to pop from stack later
	//size_t argsToPop = 0;

	// For boolean, probably need to do it first, otherwise it will fall into integral category?
	if constexpr (std::is_same_v<DecayedType, bool>)
	{
		Type retVal = lua_toboolean(m_pState, -1);
        lua_pop(m_pState, 1);
        return retVal;
	}
	// For integral types
	else if constexpr (std::is_integral_v<DecayedType>)
	{
		Type retVal = static_cast<Type>(lua_tointeger(m_pState, -1));
        lua_pop(m_pState, 1);
        return retVal;
	}
	// Floating point types
	else if constexpr (std::is_floating_point_v<DecayedType>)
	{
		Type retVal = static_cast<Type>(lua_tonumber(m_pState, -1));
        lua_pop(m_pState, 1);
        return retVal;
	}
	// strings
	else if constexpr (std::is_same_v<DecayedType, const char*>/* || std::is_same_v<Type, std::string>*/)
	{
		Type retVal = lua_tostring(m_pState, -1);
        lua_pop(m_pState, 1);
        return retVal;
	}
	else if constexpr (std::is_same_v<DecayedType, std::string>)
	{
		Type retVal = std::string(lua_tostring(m_pState, -1));
        lua_pop(m_pState, 1);
        return retVal;
	}
    // IGameLayer has to be the different from other user data types (for now)
	else if constexpr (std::is_same_v<DecayedType, class IGameLayer*>)
	{
		lua_getglobal(m_pState, "GameLayer");
		Type retVal = static_cast<Type>(lua_touserdata(m_pState, -1));
        lua_pop(m_pState, 1);
        return retVal;
	}

    else if constexpr (std::is_same_v<DecayedType, class LuaCallback>)
    {
        return Type(luaL_ref(m_pState, LUA_REGISTRYINDEX));
    }

	// user data
	else if constexpr (std::is_pointer_v<DecayedType>)
	{
		Type retVal = static_cast<Type>(lua_touserdata(m_pState, -1));
        lua_pop(m_pState, 1);
        return retVal;
	}
    // Vector2
	else if constexpr (yang::is_vec2_v<DecayedType>)
	{
		typename Type::ValueType x = static_cast<typename Type::ValueType>(lua_tonumber(m_pState, -2));
		typename Type::ValueType y = static_cast<typename Type::ValueType>(lua_tonumber(m_pState, -1));
		Type retVal = Type(x, y);
		// Temporary thing to pop extra value
		lua_pop(m_pState, 2);
        return retVal;
	}
    else if constexpr (yang::is_optional_v<DecayedType>)
    {
        if (lua_isnil(m_pState, -1))
        {
            return {};
        }
        else
        {
            return Get<Type::value_type>();
        }
    }

	// Okay, what if it didn't fit into any of the above categories.... Compile time error?
	else
	{
		static_assert(false, "Type is not implemented. If you want user data, use a Type* instead of Type");
	}
}

template<class Type>
inline int yang::LuaState::Push(Type value) const
{
	// What happens if we use nullptr inside Lua C API?
	assert(m_pState);

	// For boolean
	if constexpr (std::is_same_v<Type, bool>)
	{
		lua_pushboolean(m_pState, value);
	}
	
	// cclosure? wtf is that?
	// TODO: lua_pushcfunction
	// lua_push_globaltable ??

	// integral types
	else if constexpr (std::is_integral_v<Type>)
	{
		lua_pushinteger(m_pState, value);
	}

	// lua_pushlstring ?? Why do we even need it ??

	// Floating point types
	else if constexpr (std::is_floating_point_v<Type>)
	{
		lua_pushnumber(m_pState, value);  //< convert to lua_Number?
	}
	// const char*
	else if constexpr (std::is_same_v<Type, const char*>)
	{
		lua_pushstring(m_pState, value);
	}

	///////////////////////////////////////////////////////
	// For the below code:
	// Consider doing something like
	//	if constexpr (std::is_invocable_r<const char*, Type::c_str>) ? Or how should it look like??
	///////////////////////////////////////////////////////

	// std::string
	else if constexpr (std::is_same_v<Type, std::string>)
	{
		lua_pushstring(m_pState, value.c_str());
	}

	// std::shared_ptr
	else if constexpr (yang::is_shared_ptr_v<Type>)
	{
		return Push(value.get());
	}

	// std::optional
	else if constexpr (yang::is_optional_v<Type>)
	{
		if (value.has_value())
		{
			// Yep, naming is not good
			return Push(value.value());
		}
		else
		{
			lua_pushnil(m_pState);
		}
	}

	// lua_pushthread.. Probably no, at least not now

	// lua_pushvalue.. Probably another function

	// TODO: lua_pushfstring. Requires variadic templates. Later!

	// lua_pushvfstring.. Who uses va_list? Use C++and variadic templates!

	// lua_pushliteral.. Well, why do we care about it at the first place? Somebody explain me, lol

	// Pointer types.. I guess we already excluded const char*, so any other pointer thing will be a user data?
	else if constexpr (std::is_pointer_v<Type>)
	{
		lua_pushlightuserdata(m_pState, value);
	}

	// I guess if nothing was matched - push nil ?
	else
	{
		lua_pushnil(m_pState);
	}

	return 1;
}

// Specialization for Vector2
template<class Type>
inline int yang::LuaState::Push(Vector2<Type> value) const
{
	lua_pushnumber(m_pState, value.x);
	lua_pushnumber(m_pState, value.y);
	return 2;
}

template<class ReturnType, class... Args>
inline ReturnType yang::LuaState::CallLuaFunction(size_t luaRef, Args&&... args) const
{
    lua_rawgeti(m_pState, LUA_REGISTRYINDEX, luaRef);
    return InternalCallLuaFunction<ReturnType>(std::forward<Args>(args)...);
}

template<class ReturnType, class... Args>
inline ReturnType yang::LuaState::CallLuaFunction(const char* functionName, Args&&... args) const
{
    lua_getglobal(m_pState, functionName);
    return InternalCallLuaFunction<ReturnType>(std::forward<Args>(args)...);
}

template<class ReturnType, class ...Args>
inline ReturnType yang::LuaState::InternalCallLuaFunction(Args&& ...args) const
{
    auto argsList = { 0, Push(std::forward<Args>(args))... };
    int numArgs = std::accumulate(argsList.begin(), argsList.end(), 0);

    int numResults;
    if constexpr (yang::is_vec2_v<ReturnType>)
    {
        numResults = 2;
    }
    else if constexpr (std::is_void_v<ReturnType>)
    {
        numResults = 0;
    }
    else
    {
        numResults = 1;
    }

    int errorCode = lua_pcall(m_pState, numArgs, numResults, 0);

    if (errorCode == LUA_ERRRUN)
    {
        LOG(Error, "Lua returned runtime error");
    }
    else if (errorCode == LUA_ERRMEM)
    {
        LOG(Error, "Lua returned memory allocation error");
    }
    else if (errorCode == LUA_ERRERR)
    {
        LOG(Error, "Lua returned error running message handler");
    }
    else if (errorCode == LUA_ERRGCMM)
    {
        LOG(Error, "Lua error while running __gc metamethod");
    }
    else if (errorCode != LUA_OK)
    {
        LOG(Error, "Lua returned unknown error");
    }

    if constexpr (std::is_void_v<ReturnType>)
    {
        return;
    }
    else
    {
        return Get<ReturnType>();
    }
}
