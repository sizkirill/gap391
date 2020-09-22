#pragma once

#include <Logic/Components/IComponent.h>
#include <Utils/Vector2.h>

namespace yang
{
    class TransformComponent;

class KinematicComponent : public yang::IComponent
{
public:
    KinematicComponent(yang::Actor* pOwner);
    void Accelerate(float deltaTime, yang::FVec2 linearAcceleration, float angularAcceleration);
    void Rotate(float deltaTime);
    void Stop();

    virtual bool Init(tinyxml2::XMLElement* pData) override final;
    virtual bool PostInit() override final;

    void SetOrientationFromVelocity();
    
    static constexpr const char* GetName() { return "KinematicComponent"; }
private:
    yang::FVec2 m_velocity;         ///< Current linear velocity vector
    float m_maxSpeed;               ///< Maximum linear speed value
    yang::FVec2 m_acceleration;     ///< Current linear acceleration vector
    float m_maxAcceleration;        ///< Maximum linear acceleration value


    float m_angularVelocity;        ///< Current angular velocity
    float m_maxAngularVelocity;     ///< Maximum angular velocity
    float m_angularAcceleration;    ///< Current angular acceleration
    float m_maxAngularAcceleration; ///< Maximum angular acceleration

    float m_targetSpeed;
    float m_targetAngularVelocity;

    float m_targetOrientation;

    yang::TransformComponent* m_pTransform; ///< Owner's transform component
public:
    yang::FVec2 GetVelocity() const         { return m_velocity; }                      ///< Get current linear velocity. \return current velocity vector
    float GetMaxSpeed() const               { return m_maxSpeed; }                      ///< Get maximum speed. \return maximum linear speed
    yang::FVec2 GetAcceleration() const     { return m_acceleration; }                  ///< Get current acceleration. \return current acceleration vector
    float GetMaxAcceleration() const        { return m_maxAcceleration; }               ///< Get maximum acceleration. \return maximum acceleration

    float GetAngularVelocity() const        { return m_angularVelocity; }               ///< Get current angular velocity. \return current rotation speed
    float GetMaxAngularVelocity() const     { return m_maxAngularVelocity; }            ///< Get maximum angular velocity. \return maximum rotation speed
    float GetAngularAcceleration() const    { return m_angularAcceleration; }           ///< Get current angular acceleration. \return current angular acceleration
    float GetMaxAngularAcceleration() const { return m_maxAngularAcceleration; }        ///< Get maximum angular acceleration. \return maximum angular acceleration

    float GetTargetSpeed() const { return m_targetSpeed; }
    float GetTargetAngularVelocity() const { return m_targetAngularVelocity; }

    float GetTargetOrientation() const { return m_targetOrientation; }

    void SetTargetSpeed(float speed) { m_targetSpeed = speed; }
    void SetTargetAngularVelocity(float angularVelocity) { m_targetAngularVelocity = angularVelocity; }

    void SetMaxAngularVelocity(float value)     { m_maxAngularVelocity = value; }       ///< Set maximum value of angular velocity. \param value - new maximum angular velocity
    void SetMaxAngularAcceleration(float value) { m_maxAngularAcceleration = value; }   ///< Set maximum value of angular acceleration. \param value - new maximum angular acceleration

    void SetMaxSpeed(float value)               { m_maxSpeed = value; }                 ///< Set maximum value of linear speed. \param value - new maximum speed
    void SetMaxAcceleration (float value)       { m_maxAcceleration = value; }          ///< Set maximum value of linear acceleration. \param value - new maximum acceleration

    void SetTargetOrientation(float value) { m_targetOrientation = value; }
};
}