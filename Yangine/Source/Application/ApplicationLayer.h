#pragma once

#include <Application/OS/IOpSys.h>
#include <Application/Window/IWindow.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Fonts/IFontLoader.h>
#include <Application/Audio/IAudio.h>
#include <Logic/IGameLayer.h>
#include <Logic/Physics/IPhysicsSimulation.h>
#include <Utils/Logger.h>
#include <Utils/Vector2.h>

/** \file ApplicationLayer.h */
/** Abstraction layer to communicate between the operating system and the game */

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class ApplicationLayer */
/** Abstraction layer to communicate between the operating system and the game */
class ApplicationLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	ApplicationLayer();

	/** Default Destructor */
	~ApplicationLayer();

    /// \brief Run the game until the exit event happens
	void Run();
	
    /// \brief Initializes the application
    /// \return true if all systems initialized successfully
	bool Init();

    /// \brief Cleans up the memory used by application
	void Cleanup();

    /// \brief Create game layer to be used by the application
    /// \return unique pointer to a base game layer
	virtual std::unique_ptr<IGameLayer> CreateGameLayer() = 0;

    /// \brief Get window dimensions
    /// \return IVec2 that contains window dimensions
    IVec2 GetWindowDimensions() const;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	std::unique_ptr<IOpSys> m_pSystem;                  ///< Operating system abstraction
	std::unique_ptr<IGameLayer> m_pGameLayer;           ///< Game layer
	std::unique_ptr<IWindow> m_pWindow;                 ///< Associated window
	std::unique_ptr<IGraphics> m_pGraphics;             ///< Graphics subsystem
    std::unique_ptr<IAudio> m_pAudio;                   ///< Audio subsystem
    std::unique_ptr<IPhysicsSimulation> m_pPhysics;     ///< Physics subsystem
    std::unique_ptr<IFontLoader> m_pFontLoader;         ///< Font loader subsystem

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// \brief Get the graphics subsystem
    /// \return raw pointer to graphics system
    IGraphics* GetGraphics() const { return m_pGraphics.get(); }

    /// \brief Get the associated mouse object
    /// \return raw pointer to associated mouse object
    IMouse* GetMouse() const { return m_pWindow->GetMouse(); }

    /// \brief Get the associated keyboard object
    /// \return raw pointer to associated keyboard object
    IKeyboard* GetKeyboard() const { return m_pWindow->GetKeyboard(); }

    /// \brief Get the audio subsystem
    /// \return raw pointer to audio system
    IAudio* GetAudio() const { return m_pAudio.get(); }

    /// \brief Get the associated game layer
    /// \return raw pointer to the game layer
    IGameLayer* GetGameLayer() const { return m_pGameLayer.get(); }

    /// \brief Get the physics subsystem
    /// \return raw pointer to physics system
    IPhysicsSimulation* GetPhysics() const { return m_pPhysics.get(); }

    /// \brief Get the font loader subsystem
    /// \return raw pointer to font loader system
    IFontLoader* GetFontLoader() const { return m_pFontLoader.get(); }

};
}