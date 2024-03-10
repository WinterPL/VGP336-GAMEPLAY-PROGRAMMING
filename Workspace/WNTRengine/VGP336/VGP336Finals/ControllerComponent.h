#pragma once

#include "../Engine/Inc/Component.h"
#include "../Engine/Inc/TransformComponent.h"
#include "../Engine/Inc/RigidBodyComponent.h"
#include "TypeIds.h"

class ControllerComponent final : public WNTRengine::Component
{
public:
		SET_TYPE_ID(CustomComponentId::Controller);

		void Initialize() override;
		void Terminate() override;
		void DebugUI() override;
		void Update(float deltaTime) override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void DeSerialize(const rapidjson::Value& value) override;

	private:
		WNTRengine::TransformComponent* mTransformComponent = nullptr;
		WNTRengine::RigidBodyComponent* mRigidBodyComponent = nullptr;
	};