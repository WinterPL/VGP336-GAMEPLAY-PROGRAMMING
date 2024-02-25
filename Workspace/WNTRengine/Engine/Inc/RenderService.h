#pragma once

#include "Service.h"

namespace WNTRengine
{
	class CameraService;
	class ModelComponent;
	class TransformComponent;
	class MeshComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime)override;
		void Render()override;
		void DebugUI()override;

		void DeSerialize(rapidjson::Value& value) override;

		//Register
		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);

		void Register(const MeshComponent* meshComponent);
		void Unregister(const MeshComponent* meshComponent);

	private:
		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::ShadowEffect mShadowEffect;

		float mFPS = 0.0f;

		struct Entry
		{
			const MeshComponent* meshComponent = nullptr;
			const ModelComponent* modelComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
			bool castShadow = true;
		};
		using RenderEntities = std::vector<Entry>;
		RenderEntities mRenderEntries;
	};

}