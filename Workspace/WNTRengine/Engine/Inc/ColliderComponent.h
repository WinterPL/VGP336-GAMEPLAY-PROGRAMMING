#pragma once

#include "Component.h"

namespace WNTRengine
{
	class ColliderComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Collider);

		void Initialize() override;
		void Terminate() override;

		void DeSerialize(const rapidjson::Value& value) override;

	private:
		friend class RigidBodyComponent;
		Physics::CollisionShape mCollisionShape;
	};
}