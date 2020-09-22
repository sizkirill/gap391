#pragma once
#include <Logic/Shapes/IShape.h>
/** \file ColliderComponent.h */
/** TODO: File Purpose */

#include <Logic/Components/IComponent.h>
#include <Logic/Collisions/ICollisionCalback.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class CollisionSystem;
	class TransformComponent;
/** \class ColliderComponent */
/** TODO: Class Purpose */
class ColliderComponent
	: public yang::IComponent
{
public:
    enum class Type
    {
        kCollider,
        kSensor,
        kMaxTypes
    };
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	ColliderComponent(yang::Actor* pOwner, CollisionSystem* pCollisionSystem);

	/// The name of this component
	/// \return "ColliderComponent"
	static constexpr const char* GetName() {return "ColliderComponent"; }

	/// Initializes ColliderComponent from XMLElement
	/// \param pData - pointer to XMLElement to initialize ColliderComponent from.
	/// \return true if initialized successfully
	virtual bool Init(tinyxml2::XMLElement* pData) override;

	virtual bool PostInit() override final;

	bool Collide(ColliderComponent* pOther);

	virtual bool Render(IGraphics* pGraphics) override;
	
	virtual void Update(float deltaSeconds) override;

	virtual void OnCollisionStart(ColliderComponent* pOther);
	virtual void OnCollisionEnd(ColliderComponent* pOther);
	virtual void UpdateCollision(ColliderComponent* pOther, float deltaSeconds);
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	std::unique_ptr<IShape> m_pColliderShape;
    std::unique_ptr<ICollisionCallback> m_pCollisionCallback;
	TransformComponent* m_pTransform;
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
protected:
    ColliderComponent(yang::Actor* pOwner, CollisionSystem* pCollisionSystem, Type colliderType, const char* pName);
    Type m_type;
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
    void SetCollisionCallback(std::unique_ptr<ICollisionCallback>&& pCallback) { m_pCollisionCallback = std::move(pCallback); }
    void RemoveCollisionCallback() { m_pCollisionCallback = nullptr; }
    ICollisionCallback* GetCollisionCallback() const { return m_pCollisionCallback.get(); }
    
    IShape* GetShape() const { return m_pColliderShape.get(); }

    Type GetType() const { return m_type; }
};
}