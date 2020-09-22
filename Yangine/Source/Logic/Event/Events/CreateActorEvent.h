#pragma once
#include <string>
#include <optional>
#include <Utils/Vector2.h>
/** \file CreateActorEvent.h */
/** Create Actor Event description */

#include "..\IEvent.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class CreateActorEvent */
/** Event for actor creation */
    class CreateActorEvent
        : public IEvent
    {
    public:
        // --------------------------------------------------------------------- //
        // Public Member Variables
        // --------------------------------------------------------------------- //

        // {5ADD3A56-663D-45E5-9308-69225081E2A3}
        /// Event ID
        static constexpr EventId kEventId = 0x5add3a56;

        // --------------------------------------------------------------------- //
        // Public Member Functions
        // --------------------------------------------------------------------- //

        /// Constructor
        /// \param xmlFilepath - path to the XML file that contains actor description
        /// \param position - where to spawn an actor
        /// \param isEnemy - true if we're spawning an enemy
        CreateActorEvent(const char* xmlFilepath, std::optional<FVec2> position = {}, bool isEnemy = false); //TODO: it feels like this boolean is not what we want to do

        /** Default Destructor */
        ~CreateActorEvent();

        /// Get the Event ID
        EventId GetEventId() const override { return kEventId; }

        /// Get the name of the event
        /// \return "CreateActor"
        const char* GetName() const override { return "CreateActor"; }

    private:
        // --------------------------------------------------------------------- //
        // Private Member Variables
        // --------------------------------------------------------------------- //
        bool m_isEnemy;                         ///< True if the new actor is enemy
        std::optional<FVec2> m_positionToSpawn; ///< Where to spawn an actor. Overrides the transform component
        std::string m_xmlFilepath;              ///< Path to the XML Actor description

        // --------------------------------------------------------------------- //
        // Private Member Functions
        // --------------------------------------------------------------------- //


    public:
        // --------------------------------------------------------------------- //
        // Accessors & Mutators
        // --------------------------------------------------------------------- //

        /// Get the XML filepath
        const char* GetXmlFilepath() const { return m_xmlFilepath.c_str(); }
        /// Get if the actor to spawn is an enemy
        bool IsEnemy() const { return m_isEnemy; }

        std::optional<FVec2> GetSpawnPosition() const { return m_positionToSpawn; }
};
}