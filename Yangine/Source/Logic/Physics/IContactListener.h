#pragma once
/** \file IContactListener.h */
/** Contact listener interface description */

#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Logic/Physics/Box2DAdapters.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class IGameLayer;
/** \class IContactListener */
/** TODO: Class Purpose */
class IContactListener : public b2ContactListener
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/// Constructor
    /// \param pGameLayer - Game layer to initialize the contact listener
	IContactListener(IGameLayer* pGameLayer);

	/** Default Destructor */
	virtual ~IContactListener();

    /// Callback that Box2D calls when contact is about to begin
    /// \param pContact - b2Contact that is about to happen
    virtual void BeginContact(b2Contact* pContact) override;

    /// Callback that Box2D calls when contact is about to end
    /// \param pContact - b2Contact that is about to end
    virtual void EndContact(b2Contact* pContact) override;


    //virtual void PreSolve(b2Contact* pContact, const b2Manifold* pOldManifold) override;
    //virtual void PostSolve(b2Contact* pContact, const b2ContactImpulse* pImpulse) override;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    IGameLayer* m_pGameLayer;   ///< The game layer. Needed to notify the contacts

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    /// Resolves the contact depending on it's type
    /// \param pContact - contact to solve
    /// \param type - contact state \see yang::IContactListener::ContactState
    void HandleContact(b2Contact* pContact, ContactState type);

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Sets the game layer
    void SetGameLayer(IGameLayer* pGameLayer) { m_pGameLayer = pGameLayer; }
};
}