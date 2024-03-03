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

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void DeSerialize(const rapidjson::Value& value) override;

	private:
		friend class RigidBodyComponent;
		Physics::CollisionShape mCollisionShape;

		struct LoadingData
		{
			std::string shapeType;
			WNTRmath::Vector3 param0;
			WNTRmath::Vector3 param1;
		};
		LoadingData mLoadingData;
	};
}