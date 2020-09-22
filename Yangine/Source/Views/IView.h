#pragma once
/** \file IView.h */
/** IView interface description */
#include <memory>

//! \namespace yang Contains all Yangine code
namespace yang
{
    // Forward declarations
    class ApplicationLayer;
    class Actor;
/** \class IView */
/** Contains data for the actor view */
class IView
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	IView();

	/** Default Destructor */
	virtual ~IView();

    /// Initializes the view
    /// \param app - Application layer instance
    /// \return true if initialized successfully
    virtual bool Init(const ApplicationLayer& app) = 0;

    /// Updates the input for this view
    virtual void UpdateInput() = 0;

    /// Views the scene
    virtual void ViewScene() = 0; // TODO: Const correctness

    /// Sets the controlled actor for this view
    /// \param pActor - shared pointer to an actor to control
    virtual void SetControlledActor(std::shared_ptr<Actor> pActor);

    /// Detaches actor from the view
    void DetachActor();

protected:
    std::shared_ptr<Actor> m_pActor;    ///< Actor that this view controls
    size_t m_index;                     ///< Index in the vector that this view lives in
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    friend class IGameLayer;

    /// Setter for index. Should be called only from IGameLayer, thus made private
    void SetIndex(size_t index) { m_index = index; }

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
    
    /// Get the index
    size_t GetIndex() const { return m_index; }
};
}