#pragma once

#include "Component.h"

namespace WNTRengine
{
	class CameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Camera);

		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void DeSerialize(const rapidjson::Value& value) override;

		Graphics::Camera& GetCamera() { return mCamera; }
		const Graphics::Camera& GetCamera() const { return mCamera; }

	private:
		Graphics::Camera mCamera;
		WNTRmath::Vector3 mStartingPosition = WNTRmath::Vector3::Zero;
		WNTRmath::Vector3 mStartingLookAt = WNTRmath::Vector3::Zero;

	};

}