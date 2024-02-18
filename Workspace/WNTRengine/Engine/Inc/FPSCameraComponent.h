#pragma once

#include "Component.h"

namespace WNTRengine
{
	class CameraComponent;
	class TransformComponent;

	class FPSCameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FPSCamera);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DeSerialize(const rapidjson::Value& value) override;

	private:
		TransformComponent* mTransformComponent = nullptr;
		CameraComponent* mCameraComponent = nullptr;
		float mMoveSpeed = 1.0f;
		float mTurnSpeed = 0.1f;

	};
}