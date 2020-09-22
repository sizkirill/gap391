#pragma once
/** \file LuaCallback.h */
/** LuaCallback class description */
#include <Logic/Scripting/LuaState.h>
#include <optional>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class LuaCallback */
/** Stores reference to a function in the Lua environment and responsible for calling it */
class LuaCallback
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	LuaCallback() = default;

    /// Constructor
    /// \param luaRef - reference to a lua function, which is an index in the lua table (see luaL_ref in Lua Reference Manual)
    LuaCallback(size_t luaRef);

	/** Default Destructor */
	~LuaCallback() = default;

    // TODO: Think how to make it type-safer?
    // TODO: Think what we should return if the m_luaRef is 0 and we need something to return... or require to check if callback is valid before calling it?

    /// Calls the stored callback
    /// \tparam ReturnType - expected return type
    /// \tparam Args - expected function arguments
    /// \param args - arguments to call the function with
    template <class ReturnType, class... Args>
    ReturnType Call(Args&&... args) const;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    size_t m_luaRef = 0;    ///< reference to a lua function, which is an index in the lua table (see luaL_ref in Lua Reference Manual)

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
    
    /// Get the underlying lua function reference
    size_t GetRef() const { return m_luaRef; }
    /// Set new function to a callback
    void SetRef(size_t luaRef) { m_luaRef = luaRef; }
    /// Reset the callback
    void ResetRef() { m_luaRef = 0; }

};
template<class ReturnType, class... Args>
inline ReturnType LuaCallback::Call(Args&&... args) const
{
    if (m_luaRef != 0)
    {
        return LuaState::GetInstance()->CallLuaFunction<ReturnType>(m_luaRef, std::forward<Args>(args)...);
    }
    else
    {
        return ReturnType();
    }
}
}