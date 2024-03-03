#include "Precompiled.h"
#include "ShadowEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"
#include "AnimationUtil.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;

static constexpr size_t MaxBoneCount = 256;

void ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Shadow.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();
	mBoneTransform.Initialize(MaxBoneCount * sizeof(WNTRengine::WNTRmath::Matrix4));

	constexpr uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mBoneTransform.Terminate();
}

void ShadowEffect::Begin()
{
	UpdateLIgthCamera();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);
	mBoneTransform.BindVS(1);

	mDepthMapRenderTarget.BeginRender();
}

void ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();
}

void ShadowEffect::Render(const RenderObject& renderObject)
{
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mLightCamera.GetViewMatrix();
	const auto& matproj = mLightCamera.GetProjectionMatrix();

	TransformData data;
	data.wvp = WNTRmath::Transpose(matWorld * matView * matproj);
	mTransformBuffer.Update(data);

	if (renderObject.skeleton != nullptr)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransform(renderObject.modelId, boneTransforms,renderObject.animator);
		AnimationUtil::ApplyBoneOffsets(renderObject.modelId, boneTransforms);
		for (WNTRmath::Matrix4& transform : boneTransforms)
		{
			transform = WNTRmath::Transpose(transform);
		}
		boneTransforms.resize(MaxBoneCount);
		mBoneTransform.Update(boneTransforms.data());
	}

	renderObject.meshBuffer.Render();


}								   

void ShadowEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("ShadowEffect"))
	{
		ImGui::Text("DepthMap");
		ImGui::Image(
			mDepthMapRenderTarget.GetRawData(),
			{144,144},
			{ 0, 0 },
			{ 1,1 },
			{ 1,1,1,1 },
			{ 1,1,1,1 });
		ImGui::DragFloat("size##",&mSize,0.1f,1.0f,1000.0f);
	}

}

void ShadowEffect::UpdateLIgthCamera()
{
	ASSERT(mDirectionalLight != nullptr, "ShadowEffect: no light set!");

	const WNTRmath::Vector3& direction = mDirectionalLight->direction;
	mLightCamera.SetDirection(direction);
	mLightCamera.SetPosition(mFocusPosition - (direction * 1000.0f));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(2000.0f);
	mLightCamera.SetSize(mSize, mSize);
}