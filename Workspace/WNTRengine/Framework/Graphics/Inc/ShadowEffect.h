#pragma once
#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightType.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace WNTRengine::Graphics
{
	class RenderObject;

	class ShadowEffect
	{
	public: 
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetDirectionalLight(const DirectionalLight& directionalLight) { mDirectionalLight = &directionalLight; }
		void SetFocus(const WNTRmath::Vector3& focusPosition) { mFocusPosition = focusPosition; }
		void SetSize(float size) { mSize = size; }
		const Camera& GetLightCamera() const { return mLightCamera; }
		const Texture& GetDepthMap() const { return mDepthMapRenderTarget; }

	private:

		void UpdateLIgthCamera();

		struct TransformData
		{
			WNTRmath::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = ConstantBuffer;

		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransform;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const DirectionalLight* mDirectionalLight = nullptr;

		Camera mLightCamera;
		RenderTarget mDepthMapRenderTarget;

		WNTRmath::Vector3 mFocusPosition = WNTRmath::Vector3::Zero;
		float mSize = 100.0f;
	
	};
}