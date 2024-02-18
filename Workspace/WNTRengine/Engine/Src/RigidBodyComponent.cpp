#include "Precompiled.h"
#include "RigidBodyComponent.h"

#include "GameWorld.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "PhysicsService.h"

using namespace WNTRengine;


void RigidBodyComponent::Initialize()
{
	ColliderComponent* collider = GetOwner().GetComponent<ColliderComponent>();
	TransformComponent* transform = GetOwner().GetComponent<TransformComponent>();
	ASSERT(collider != nullptr, "RigidBodyComponent: game object requires collider");
	mRigidBody.Initialize(*transform,collider->mCollisionShape,mMass);

	PhysicsService* ps = GetOwner().GetWorld().GetService<PhysicsService>();
	ps->Register(*this);
}
void RigidBodyComponent::Terminate()
{
	PhysicsService* ps = GetOwner().GetWorld().GetService<PhysicsService>();
	ps->Unregister(*this);
	mRigidBody.Terminate();
}

void RigidBodyComponent::DeSerialize(const rapidjson::Value& value)
{
	if (value.HasMember("Mass"))
	{
		mMass = value["Mass"].GetFloat();
	}
}

void RigidBodyComponent::SetPosition(const WNTRengine::WNTRmath::Vector3& position)
{
	mRigidBody.SetPosition(position);
}