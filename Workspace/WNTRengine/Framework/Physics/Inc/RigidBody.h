#pragma once

#include "PhysicsObject.h"


namespace WNTRengine::Graphics
{
	struct Transform;
}

namespace WNTRengine::Physics
{
	class CollisionShape;
	class RigidBody final : public PhysicsObject
	{
	public:
		RigidBody() = default;
		~RigidBody() override;

		void Initialize(WNTRengine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f);
		void Terminate();

		void SetPosition(const WNTRengine::WNTRmath::Vector3& position);
		void SetVelocity(const WNTRengine::WNTRmath::Vector3& velocity);

		bool IsDynamic() const;

		void Update() override;
	private:
		btRigidBody* GetRigidBody() override{ return mRigidBody; }

		btRigidBody* mRigidBody = nullptr;
		btDefaultMotionState* mMotionState = nullptr;
		float mMass = 0.0f;

		WNTRengine::Graphics::Transform* mGraphicsTransform = nullptr;
	
	};
}