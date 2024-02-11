#pragma once

#include "Service.h"

namespace WNTRengine
{
	class RigidBodyComponent;

	class PhysicsService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Physics);

		void Update(float deltaTime) override;
		void DebugUI()override;
		
		void DeSerialize(rapidjson::Value& value) override;

		//Register
		void Register(RigidBodyComponent& rigidBodyComponent);
		void Unregister(RigidBodyComponent& rigidBodyComponent);

	private:
		
	};
}