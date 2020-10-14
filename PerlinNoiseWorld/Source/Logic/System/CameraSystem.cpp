#include "CameraSystem.h"
#include <Application/Input/IMouse.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/ApplicationLayer.h>
#include <Application/ApplicationConstants.h>
#include <Logic/Components/CameraComponent.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Event/Input/KeyboardInputEvent.h>
#include <Logic/Event/Input/MouseWheelEvent.h>
#include <Logic/Event/EventDispatcher.h>

#include <cassert>

CameraSystem::CameraSystem(const yang::ApplicationLayer& appLayer)
    :m_mouse(*appLayer.GetMouse())      // TODO (ksizykh): When refactoring the engine, change that this to return a const reference initially. No need for raw pointers
    ,m_windowDimensions(appLayer.GetWindowDimensions())
    ,m_pGraphics(appLayer.GetGraphics())
    ,m_pActiveCamera(nullptr)
    ,m_cameraDirection(0.f, 0.f)
{
    yang::EventDispatcher::Get()->AddEventListener(yang::KeyboardInputEvent::kEventId, [this](yang::IEvent* pEvent)
    {
        if (!m_pActiveCamera)
            return;

        using namespace yang;

        assert(KeyboardInputEvent::kEventId == pEvent->GetEventId());

        KeyboardInputEvent* pResult = static_cast<KeyboardInputEvent*>(pEvent);

        if (pResult->GetEventType() == KeyboardInputEvent::EventType::kKeyPressed)
        {
            if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kS)
            {
                m_cameraDirection += {0, -1};
            }
            else if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kW)
            {
                m_cameraDirection += {0, 1};
            }
            else if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kD)
            {
                m_cameraDirection += {-1, 0};
            }
            else if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kA)
            {
                m_cameraDirection += {1, 0};
            }
        }
        else if (pResult->GetEventType() == KeyboardInputEvent::EventType::kKeyReleased)
        {
            if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kS)
            {
                m_cameraDirection += {0, 1};
            }
            else if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kW)
            {
                m_cameraDirection += {0, -1};
            }
            else if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kD)
            {
                m_cameraDirection += {1, 0};
            }
            else if (pResult->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kA)
            {
                m_cameraDirection += {-1, 0};
            }
        }
    });

    yang::EventDispatcher::Get()->AddEventListener(yang::MouseWheelEvent::kEventId, [this](yang::IEvent* pEvent)
    {
        if (!m_pActiveCamera)
            return;

        using namespace yang;

        assert(MouseWheelEvent::kEventId == pEvent->GetEventId());

        MouseWheelEvent* pResult = static_cast<MouseWheelEvent*>(pEvent);

        auto mousePos = yang::FVec2(m_mouse.GetMousePosition());
        auto transformPos = m_pActiveCamera->m_pCameraTransform->GetPosition();
        auto scaleFactors = m_pActiveCamera->GetCameraScaleFactors();

        //m_pActiveCamera->m_pCameraTransform->SetScalePoint(transformPos + mousePos);

        m_pActiveCamera->m_pCameraTransform->SetScalePoint({yang::kWindowWidth/2, yang::kWindowHeight/2});
        m_pActiveCamera->m_pCameraTransform->Scale(1 + (float)pResult->GetScrollAmount().y / 10.f);
    });
}

void CameraSystem::Update(float deltaSeconds)
{
    // All the safety code below is killed by the engine architecture =/

    if (!m_pActiveCamera)
    {
        for (auto pCamera : m_cameras)
        {
            if (pCamera)
            {
                m_pActiveCamera = pCamera;
                break;
            }
        }

        if (!m_pActiveCamera)
            return;
    }

    float delta = m_pActiveCamera->m_moveSpeed * deltaSeconds;

    m_pActiveCamera->MoveCamera(m_cameraDirection * delta);

    m_pGraphics->SetCameraTransform(m_pActiveCamera->GetCameraTransform());
    m_pGraphics->SetCameraScaleFactors(m_pActiveCamera->GetCameraScaleFactors());
}

void CameraSystem::RegisterCamera(CameraComponent* pCamera)
{
    if (!m_pActiveCamera)
    {
        m_pActiveCamera = pCamera;
    }
    m_cameras.emplace_back(pCamera);
}

void CameraSystem::RemoveCamera(CameraComponent* pCamera)
{
    for (size_t i = 0; i < m_cameras.size(); ++i)
    {
        if (m_cameras.at(i) == pCamera)
        {
            std::swap(m_cameras.begin() + i, m_cameras.end() - 1);
            m_cameras.pop_back();
            return;
        }
    }
}