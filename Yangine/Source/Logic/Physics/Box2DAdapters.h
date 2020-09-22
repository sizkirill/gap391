#pragma once
/// \file Box2DAdapters.h
/// Contains adapter functions from Yangine units to Box2D units

#include <Utils/Math.h>
#include <Utils/Vector2.h>
#include <Utils/Typedefs.h>
#include <Box2D/Common/b2Math.h>

/// \enum ContactState
/// Describes the contact state
enum class ContactState
{
    kBegin, ///< Begin of contact
    kEnd    ///< End of contact
};

// TODO: Make it assignable at Physics creation. Maybe we want to make it open to designer.

/// Number of iterations the Box2D takes to approximate resulting velocity
static constexpr int    kVelocityIterations = 8;

/// Number of iterations the Box2D takes to approximate resulting position
static constexpr int    kPositionIterations = 3;

/// How often the physics simulation updates (in seconds)
static constexpr float  kPhysicsStep = 1.f / 60.f;

/// Pixels per meter constant
static constexpr int    kPixelsPerMeter = 32;

/// Convert yang::FVec to b2Vec2
inline b2Vec2 ConvertFVec(yang::FVec2 vec) { return b2Vec2(vec.x / kPixelsPerMeter, vec.y / kPixelsPerMeter); }

/// Convert b2Vec2 to yang::FVec2
inline yang::FVec2 ConvertB2Vec(b2Vec2 vec) { return yang::FVec2(vec.x * kPixelsPerMeter, vec.y * kPixelsPerMeter); };