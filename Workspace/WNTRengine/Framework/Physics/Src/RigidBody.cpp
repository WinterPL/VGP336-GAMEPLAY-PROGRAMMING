#include "Precompiled.h"
#include "RigidBody.h"
#include "PhysicsWorld.h"
#include "CollisionShape.h"

#include <Graphics/Inc/Transform.h>

using namespace WNTRengine;
using namespace WNTRengine::Physics;
using namespace WNTRengine::Graphics;


RigidBody::~RigidBody()
{
	ASSERT(mRigidBody == nullptr && mMotionState == nullptr, "RigidBody: terminate must be called first");
}

void RigidBody::Initialize(WNTRengine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass)
{
	mGraphicsTransform = &graphicsTransform;
	mMass = mass;

	mMotionState = new btDefaultMotionState(ConvertTobtTransform(graphicsTransform));
	mRigidBody = new btRigidBody(mMass, mMotionState, shape.GetCollisionShape());
	
	//PhysicsWorld::Get()->Register(this);
}
void RigidBody::Terminate()
{
	//PhysicsWorld::Get()->Unregister(this);
	SafeDelete(mMotionState);
	SafeDelete(mRigidBody);
	mGraphicsTransform = nullptr;
}

void RigidBody::SetPosition(const WNTRengine::WNTRmath::Vector3& position)
{
	if (IsDynamic())
	{
		mRigidBody->activate();
	}
	mGraphicsTransform->position = position;
	mRigidBody->setWorldTransform(ConvertTobtTransform(*mGraphicsTransform));

}
void RigidBody::SetLookDirection(const WNTRengine::WNTRmath::Vector3& lookDirection)
{
	const WNTRmath::Vector3 l = lookDirection;
	const WNTRmath::Vector3 r = WNTRmath::Normalize(WNTRmath::Cross(WNTRmath::Vector3::YAxis, l));
	const WNTRmath::Vector3 u = WNTRmath::Normalize(WNTRmath::Cross(l, r));
	btMatrix3x3 rbBasis =
	{
		r.x, u.x, l.x,
		r.y, u.y, l.y,
		r.z, u.z, l.z
	};
	mRigidBody->getWorldTransform().setBasis(rbBasis);
}
void RigidBody::SetVelocity(const WNTRengine::WNTRmath::Vector3& velocity)
{
	mRigidBody->activate();
	mRigidBody->setLinearVelocity(velocity);
}

bool RigidBody::IsDynamic() const
{
	return mMass > 0.0f;
}

void RigidBody::Update()
{
	btTransform& worldTransform = mRigidBody->getWorldTransform();
	mGraphicsTransform->position = mRigidBody->getWorldTransform().getOrigin();
	mGraphicsTransform->rotation = mRigidBody->getWorldTransform().getRotation();
}