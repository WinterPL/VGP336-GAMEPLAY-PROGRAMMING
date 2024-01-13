#pragma once

namespace WNTRengine::Physics
{
	class CollisionShape
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void InitializeSphere(float radius);
		void InitializeBox(const WNTRengine::WNTRmath::Vector3& halfExtents);
		void InitializeHull(const WNTRengine::WNTRmath::Vector3& halfExtents, const WNTRengine::WNTRmath::Vector3& origin);
		void InitializeEmpty();
		void Terminate();
	private:
		btCollisionShape* mCollisionShape = nullptr;
		friend class RigidBody;
		btCollisionShape* GetCollisionShape() const { return mCollisionShape; }
	};

}