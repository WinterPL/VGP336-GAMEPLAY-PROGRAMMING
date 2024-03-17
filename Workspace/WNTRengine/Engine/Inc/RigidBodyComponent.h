#pragma once

#include "Component.h"

namespace WNTRengine
{
	class RigidBodyComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::RigidBody);

		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void DeSerialize(const rapidjson::Value& value) override;

		void SetPosition(const WNTRengine::WNTRmath::Vector3& position);
		void SetLookDirection(const WNTRengine::WNTRmath::Vector3& lookDirection);
		void SetVelocity(const WNTRengine::WNTRmath::Vector3& velocity);

	private:
		friend class PhysicsService;
		Physics::RigidBody mRigidBody;
		float mMass = 1.0f;
	};
}