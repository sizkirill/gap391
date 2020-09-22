#pragma once
#include "../Timers/TimedProcess.h"

namespace yang
{
class TransformComponent;

class RotationProcess : public TimedProcess<void(RotationProcess::*)(float), RotationProcess*>
{
public:
    RotationProcess(yang::Actor* pOwner, float executionTime);
    virtual bool Init() override final;
    void Rotate(float deltaTime);
private:
    TransformComponent* m_pTransform;
};
}