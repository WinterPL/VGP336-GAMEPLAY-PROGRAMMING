#pragma once

#include "../Engine/Inc/Component.h"
#include "../Engine/Inc/TransformComponent.h"
#include "../Engine/Inc/RigidBodyComponent.h"
#include "../Engine/Inc/CameraComponent.h"
#include "TypeIds.h"

using namespace WNTRengine;

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

		//movement
		void Walk(float distance);
		void Strafe(float distance);
		void Yaw(float radian);


	private:
		TransformComponent* mTransformComponent = nullptr;
		RigidBodyComponent* mRigidBodyComponent = nullptr;
		CameraComponent* mCameraComponent = nullptr;
		float mTurnSpeed = 5.0f;
		float mMoveSpeed = 35.0f;
		WNTRmath::Vector3 mDirection = WNTRmath::Vector3::ZAxis;
	};