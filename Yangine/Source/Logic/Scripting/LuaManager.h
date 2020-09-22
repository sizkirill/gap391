#pragma once
#include <string>
#include <numeric>

#include <Logic/Scripting/LuaState.h>
#include <Utils/Logger.h>

/** \file LuaManager.h */
/** LuaManager class description */

//! \namespace yang Contains all Yangine code
namespace yang
{
// TODO: Change class name for something more appropriate

/** \class LuaManager */
/** Handles registering C++ function in Lua environment */
class LuaManager
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	LuaManager();

	/** Default Destructor */
	~LuaManager();

    /// Registers C++ function in the Lua environment
    /// Works for member functions, non-member functions and non-capturing lambdas
    /// \tparam Func - Function type to register
    /// \param luaName - Name of the function in Lua world
    /// \param func - Function to register
	template <class Func>
	void ExposeToLua(const char* luaName, Func&& func) const;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    /// The actual function that is being registered in Lua environment
    /// \tparam Function - function type that is registered
    /// \param pState - lua_State that is responsible for the lua environment
    /// \return number of returned arguments
	template <class Function>
	static int CallBoundFunction(lua_State* pState);

    /// Helper function that builds arguments from the lua stack
    /// \tparam OwnerType - Owner class of the registered function
    /// \tparam Args - Argument types that C++ function requires
    /// \return tuple of OwnerType and all Args that contain function arguments and a pointer to an instance of the function owner class
	template <class OwnerType, class... Args>
	static auto BuildArgs();

    /// Helper function to register class member function to the Lua environment
    /// \tparam ReturnType - return type of the C++ function
    /// \tparam OwnerType - class that owns the function
    /// \tparam Args - argument types to call that function with
    /// \param pFunction - the function pointer that is being registered to Lua
    /// \return number of returned arguments
	template <class ReturnType, class OwnerType, class... Args>
	static int Call(ReturnType(OwnerType::*pFunction)(Args...));

    /// Helper function to register const class member function to the Lua environment. Effectively calls reinterpret_cast to non const
    /// \tparam ReturnType - return type of the C++ function
    /// \tparam OwnerType - class that owns the function
    /// \tparam Args - argument types to call that function with
    /// \param pFunction - the function pointer that is being registered to Lua
    /// \return number of returned arguments
	template <class ReturnType, class OwnerType, class... Args>
	static int Call(ReturnType(OwnerType::* pFunction)(Args...) const);

    /// Helper function to register non-member function to the Lua environment.
    /// \tparam ReturnType - return type of the C++ function
    /// \tparam Args - argument types to call that function with
    /// \param pFunction - the function pointer that is being registered to Lua
    /// \return number of returned arguments
    template <class ReturnType, class... Args>
    static int Call(ReturnType(*pFunction)(Args...));
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

};

template<class Func>
inline void LuaManager::ExposeToLua(const char* luaName, Func&& func) const
{
	lua_State* pState = LuaState::GetInstance()->GetRaw();

	void* pFunction = lua_newuserdata(pState, sizeof(func));
	memcpy(pFunction, &func, sizeof(func));

	lua_pushcclosure(pState, &LuaManager::CallBoundFunction<Func>, 1);
	lua_setglobal(pState, luaName);
}

template<class Function>
inline int LuaManager::CallBoundFunction(lua_State* pState)
{
	void* pFuncBuffer = lua_touserdata(pState, lua_upvalueindex(1));
	Function* pFunction = reinterpret_cast<Function*>(pFuncBuffer);

	return Call(*pFunction);
}

/// To make doxygen happy (doesn't change the doxygen output in any ways)
template<class ReturnType, class OwnerType, class ...Args>
inline int LuaManager::Call(ReturnType(OwnerType::* pFunction)(Args ...) const)
{
	return Call(reinterpret_cast<ReturnType(OwnerType::*)(Args...)>(pFunction));
}

template <class ReturnType, class... Args>
inline int LuaManager::Call(ReturnType(*pFunction)(Args...))
{
    auto invokeArgs = std::make_tuple(LuaState::GetInstance()->Get<Args>()...);

    if constexpr (std::is_void_v<ReturnType>)
    {
        std::apply(pFunction, invokeArgs);
        return 0;
    }
    else
    {
        auto retVal = std::apply(pFunction, invokeArgs);
        return LuaState::GetInstance()->Push(retVal);
    }
}

template<class ReturnType, class OwnerType, class ...Args>
inline int LuaManager::Call(ReturnType(OwnerType::*pFunction)(Args ...))
{
	auto invokeArgs = BuildArgs<OwnerType, Args...>();

	if (std::get<0>(invokeArgs) == nullptr)
	{
		// Can this actually happen? If the object is not there, probably something else is on top of the stack, which will be reinterpret casted to this object, which won't result in nullptr
		LOG(Error, "Object was nullptr in function: %s", __FUNCTION__);
		return 0;
	}

	if constexpr (std::is_void_v<ReturnType>)
	{
		std::apply(pFunction, invokeArgs);
		return 0;
	}
	else
	{
		auto retVal = std::apply(pFunction, invokeArgs);
		return LuaState::GetInstance()->Push(retVal);
	}
}

template<class OwnerType, class ...Args>
inline auto LuaManager::BuildArgs()
{
	LuaState* pState = LuaState::GetInstance();
	return std::make_tuple(pState->Get<std::add_pointer_t<OwnerType>>(), pState->Get<Args>()...);
}

}