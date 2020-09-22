#pragma once
/** \file MouseInputListener.h */
/** MouseInput component description */

#include ".\IComponent.h"
#include <Logic/Scripting/LuaCallback.h>
#include <Utils/Math.h>
#include <functional>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class TransformComponent;
    class IEvent;
    class LuaManager;
/** \class MouseInputListener */
/** Actor component that handle mouse input to control the actor */
class MouseInputListener
	: public IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/// Constructor
    /// \param pOwner - actor, that owns this component
	MouseInputListener(Actor* pOwner);

	/** Default Destructor */
	~MouseInputListener();

    /// Initialize the component from an XML element
    /// \param pData - XML Element that contains component data
    /// \return true if initialized successfully
    virtual bool Init(tinyxml2::XMLElement* pData) override final;

    /// Obtains reference to actor's transform component
    /// \return true if actor has transform component
    virtual bool PostInit() override final;

    /// Name of this component, used for hashing
    /// \return "MouseInputListener"
    static constexpr const char* GetName() { return "MouseInputListener"; }

    /// Registers member functions to Lua environment
    /// Not intended for use outside of IGameLayer::Init
    /// \param manager - the Lua environment manager \see yang::LuaManager
    static void RegisterToLua(const LuaManager& manager);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    
    // TODO: consider just grabbing SpriteComponent? 
    TransformComponent* m_pTransform;                                   ///< Owner actor's transform component
    IVec2 m_colliderDimensions;                                         ///< Dimensions of the actor's collider

    std::function<void()> m_onMouseOver;                                ///< Callback that is called when mouse cursor is on the actor
    std::function<void()> m_onMouseOut;                                 ///< Callback that is called when mouse cursor leaves the actor
    std::function<void(const IVec2& scrollAmount)> m_onClick;           ///< Callback that is called when left mouse button is clicked
    std::function<void(const IVec2& scrollAmount)> m_onRightClick;      ///< Callback that is called when right mouse button is clicked
    std::function<void(const IVec2& scrollAmount)> m_onScrollWheel;     ///< Callback that is called when mouse wheel is scrolled while the cursor is on the actor

    LuaCallback m_onMouseOverLua  ;                                     ///< Reference to a Lua function, that is called when mouse cursor is on the actor \see yang::LuaCallback
    LuaCallback m_onMouseOutLua   ;                                     ///< Reference to a Lua function, that is called when mouse cursor leaves the actor \see yang::LuaCallback
    LuaCallback m_onClickLua      ;                                     ///< Reference to a Lua function, that is called when left mouse button is clicked \see yang::LuaCallback
    LuaCallback m_onRightClickLua ;                                     ///< Reference to a Lua function, that is called when right mouse button is clicked \see yang::LuaCallback
    LuaCallback m_onScrollWheelLua;                                     ///< Reference to a Lua function, that is called when mouse wheel is scrolled while the cursor is on the actor \see yang::LuaCallback

    // TODO: implement this
    std::function<void()> m_onDoubleClick;                              ///< Callback that is called when left mouse button is double clicked. Not implemented yet
    
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    size_t m_mouseMotionListenerIndex;                                  ///< Index of the event listener, associated with the mouse motion event
    void HandleMouseMotion(IEvent* pEvent);                             ///< Callback for event system that handles the mouse motion event

    size_t m_clickListenerIndex;                                        ///< Index of the event listener, associated with the mouse click event
    void HandleClick(IEvent* pEvent);                                   ///< Callback for event system that handles the mouse click event

    size_t m_wheelListenerIndex;                                        ///< Index of the event listener, associated with the mouse wheel event
    void HandleWheelScroll(IEvent* pEvent);                             ///< Callback for event system that handles the mouse wheel event

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Set the OnMouseOver callback      
    void SetOnMouseOver(const std::function<void()>& onMouseOver) { m_onMouseOver = onMouseOver; }
    /// Set the OnMouseOut callback      
    void SetOnMouseOut(const std::function<void()>& onMouseOut) { m_onMouseOut = onMouseOut; }
    /// Set the OnClick callback      
    void SetOnClick(const std::function<void(const IVec2& position)>& onClick) { m_onClick = onClick; }
    /// Set the OnRightClick callback      
    void SetOnRightClick(const std::function<void(const IVec2& position)>& onRightClick) { m_onRightClick = onRightClick; }
    /// Set the OnDoubleClick callback      
    void SetOnDoubleClick(const std::function<void()>& onDoubleClick) { m_onDoubleClick = onDoubleClick; }
    /// Set the OnScrollWheel callback      
    void SetOnScrollWheel(const std::function<void(const IVec2& scrollAmount)>& onScrollWheel) { m_onScrollWheel = onScrollWheel; }
    /// Set the OnMouseOver Lua callback      
    void SetOnMouseOverLua(LuaCallback luaRef)   { m_onMouseOverLua = luaRef; }
    /// Set the OnMouseOut Lua callback
    void SetOnMouseOutLua(LuaCallback luaRef)    { m_onMouseOutLua = luaRef; }
    /// Set the OnClick Lua callback
    void SetOnClickLua(LuaCallback luaRef)       { m_onClickLua = luaRef; }
    /// Set the OnRightClick Lua callback 
    void SetOnRightClickLua(LuaCallback luaRef)  { m_onRightClickLua = luaRef; }
    /// Set the OnScrollWheel Lua callback 
    void SetOnScrollWheelLua(LuaCallback luaRef) { m_onScrollWheelLua = luaRef; }

    
};                               
}                         