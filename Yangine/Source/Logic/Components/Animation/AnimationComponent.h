#pragma once
/** \file AnimationComponent.h */
/** Animation Component class */

#include "..\IComponent.h"
#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>
#include <Utils/Math.h>
#include <Utils/Rectangle.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class LuaManager;
    class Sprite;
/** \class AnimationComponent */
/** Class that contains data for animation process */
class AnimationComponent
	: public IComponent
{
public:
    // TODO: think how to properly transition between animation sequences

    ///\struct Frame
    /// Contains data related to a single frame
    struct Frame
    {
        //IRect m_frameRect = { 0,0,0,0 };                       ///< Rectangle from the texture to draw
        //std::optional<std::string> m_texturePath = {};         ///< If texture path is different from the default one, this will be initialized and used instead of the default one
        std::shared_ptr<Sprite> m_pSprite = nullptr;                     ///< Sprite for that frame
        float m_duration = 0;                                  ///< Frame duration
    };

    ///\ struct AnimationSequence
    /// Contains data related to a single animation sequence
    struct AnimationSequence
    {
        size_t m_frameCount = 0;                               ///< Number of frames in the sequence
        std::string m_name;                                    ///< Name of a sequence
        bool m_isLooping = false;                              ///< Does the animation loop?
        float m_framerate = 0;                                 ///< Sequence framerate (how fast frames are changing)
        //std::optional<std::string> m_texturePath;              ///< Path to a texture to use for this animation sequence (initialized only if it is different from the default path
        size_t m_currentFrameIndex = 0;                        ///< Index of the current frame
        std::vector<Frame> m_frameData;                        ///< Array of frame related data. \see Frame.

        // TODO: maybe add getters to expose them to Lua?
    };

	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/// Constructor
    /// \param pOwner - actor that owns this component
	AnimationComponent(Actor* pOwner);

	/** Default Destructor */
	~AnimationComponent();

    /// Initializes the AnimationComponent from XML data
    /// \param pData - XMLElement to construct component from
    /// \return true if initialized successfully
    virtual bool Init(tinyxml2::XMLElement* pData) override final;

    /// The name of this component
    /// \return "AnimationComponent"
    static constexpr const char* GetName() { return "AnimationComponent"; }

    /// Registers this component's functions to Lua environment
    /// \param manager - lua manager reference
    static void RegisterToLua(const LuaManager& manager);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

    size_t m_sequenceCount;                                          ///< Number of animation sequences
    //std::string m_defaultTexturePath;                                ///< Path to a default texture. If all frames are from the same texture, better to specify only this.
    float m_defaultFrameRate;                                        ///< Default framerate. If all animation sequences have the same framerate - specify only this
    std::unordered_map<std::string, AnimationSequence> m_sequences;  ///< Map of sequences. Key is a sequence name
    AnimationSequence* m_pActiveSequence;                            ///< Pointer to a current active sequence. Pointer to an element in the map, should be safe to use, because data inside the map is not changing once initialized.

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Getter for current active sequence
    /// \return pointer to current active sequence
    AnimationSequence* GetActiveSequence() const;

    /// Setter for current active sequence
    /// \param name - name of the new active sequence.
    void SetActiveSequence(const std::string& name);

    /// Getter for the path to a texture that is active now
    /// \return path to a texture file that should be drawn
    //const std::string& GetCurrentTexturePath() const;

};
}