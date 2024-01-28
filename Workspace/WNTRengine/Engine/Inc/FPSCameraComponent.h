#pragma once

#include "Component.h"

namespace WNTRengine
{
	class CameraComponent;

	class FPSCameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FPSCamera);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DeSerialize(rapidjson::Value& value) override;

	private:
		CameraComponent* mCameraComponent = nullptr;
		float mMoveSpeed = 1.0f;
		float mTurnSpeed = 0.1f;

	};
}