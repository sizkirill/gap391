#pragma once
#include <Utils/Vector2.h>
#include <vector>
#include <memory>

namespace yang
{
    class ApplicationLayer;
    class IMouse;
    class IGraphics;
    class IEvent;
}

class CameraComponent;

/// \class CameraSystem
/// \brief Handles camera management
/// TODO(ksizykh): consider storing array indices inside camera components to easy access for the cameras. And deletion from camera list / setting active camera, etc.
class CameraSystem
{
public:
    /// \brief Constructor that takes in a const reference to application layer
    /// \param appLayer - const reference to application layer
    CameraSystem(const yang::ApplicationLayer& appLayer);

    /// \brief default destructor
    ~CameraSystem() = default;

    /// \brief Updates the active camera by delta seconds
    /// \param deltaSeconds - number of seconds passed since last frame
    void Update(float deltaSeconds);

    /// \brief Registers a camera to the camera system
    /// \param pCamera - camera to register
    /// \see CameraComponent constructor. Engine architecture doesn't allow to use shared ptrs on components. TODO(ksizykh) : refactor the components to be able to handle shared/weak ptrs
    void RegisterCamera(CameraComponent* pCamera);

    /// \brief Removes camera from the cameras list
    /// \param pCamera - camera to remove
    /// Effectively O(n) time complexity, but not a big deal, I don't expect a lot of cameras there.
    void RemoveCamera(CameraComponent* pCamera);

    // TODO(ksizykh): something to set active camera. For now one camera is enough.
private:
    CameraComponent* m_pActiveCamera;           ///< Shared pointer to active camera
    std::vector<CameraComponent*> m_cameras;    ///< List of all cameras
    const yang::IMouse& m_mouse;                ///< reference to a mouse (to get mouse position)
    yang::IGraphics* m_pGraphics;               ///< pointer to graphics system
    yang::FVec2 m_cameraDirection;
    /// \brief Window dimensions. 
    /// TODO (ksizykh): this is not const because in future I'd like that to be changeable in case of window resizing. Right now engine doesn't support that.
    yang::IVec2 m_windowDimensions;
public:

    /// \brief Get the pointer to active camera
    /// TODO (ksizykh): consider returning a shared pointer after engine refactor
    /// \return pointer to an active camera component
    CameraComponent* GetActiveCamera() const {return m_pActiveCamera;}
    
};