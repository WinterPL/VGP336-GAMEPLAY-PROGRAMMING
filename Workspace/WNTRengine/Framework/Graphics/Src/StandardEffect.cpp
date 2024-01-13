#include "Precompiled.h"
#include "StandardEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "TextureManager.h"
#include "AnimationUtil.h"

using namespace WNTRengine;
using namespace WNTRengine::WNTRmath;
using namespace WNTRengine::Graphics;

static constexpr size_t MaxBoneCount = 256;

void StandardEffect::Initialize(const std::filesystem::path& filepath)
{
	mTransformBuffer.Initialize();
	mLightingBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingBuffer.Initialize();
	mBoneTransformBuffer.Initialize(MaxBoneCount * sizeof(WNTRmath::Matrix4));
	mVertexShader.Initialize<Vertex>(filepath);
	mPixelShader.Initialize(filepath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

}
void StandardEffect::Terminate() 
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mBoneTransformBuffer.Terminate();
	mSettingBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightingBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "StandardEffect: no camera set!");
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mLightingBuffer.BindVS(1);
	mLightingBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mSettingBuffer.BindVS(3);
	mSettingBuffer.BindPS(3);

	mBoneTransformBuffer.BindVS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

}
void StandardEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mCamera->GetViewMatrix();
	const auto& matProj = mCamera->GetProjectionMatrix();

	SettingData settingData;
	settingData.useDiffuseMap = mSettingData.useDiffuseMap > 0 && renderObject.diffuseMapId != 0;
	settingData.useNormalMap = mSettingData.useNormalMap > 0 && renderObject.normalMapId != 0;
	settingData.useDiffuseMap = mSettingData.useDiffuseMap > 0 && renderObject.diffuseMapId != 0;
	settingData.useBumpMap = mSettingData.useBumpMap > 0 && renderObject.bumpMapId != 0;
	settingData.useSpecMap = mSettingData.useSpecMap > 0 && renderObject.specMapId != 0;
	settingData.useCelShading = mSettingData.useCelShading;
	settingData.useShadowMap = mSettingData.useShadowMap > 0 && mShadowMap != nullptr;
	settingData.bumpWeight = mSettingData.bumpWeight;
	settingData.depthBias = mSettingData.depthBias;
	settingData.useSkinning = mSettingData.useSkinning > 0 && renderObject.skeleton != nullptr;
	mSettingBuffer.Update(mSettingData);
	
	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera->GetPosition();
	if (settingData.useShadowMap)
	{
		const auto& matLightView = mLightCamera->GetViewMatrix();
		const auto& matLightProj = mLightCamera->GetProjectionMatrix();
		transformData.lwvp = Transpose(matWorld * matLightView * matLightProj);

		mShadowMap->BindPS(4);
	}
	mTransformBuffer.Update(transformData);

	if (settingData.useSkinning)
	{
		AnimationUtil::BoneTransforms boneTransform;
		AnimationUtil::ComputeBoneTransform(renderObject.modelId, boneTransform, renderObject.animator);
		AnimationUtil::ApplyBoneOffsets(renderObject.modelId, boneTransform);

		for (auto& transform : boneTransform)
		{
			transform = Transpose(transform);
		}
		boneTransform.resize(MaxBoneCount);
		mBoneTransformBuffer.Update(boneTransform.data());
	}

	mLightingBuffer.Update(*mDirectionalLight);

	mMaterialBuffer.Update(renderObject.material);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.normalMapId, 1);
	tm->BindVS(renderObject.bumpMapId, 2);
	tm->BindPS(renderObject.specMapId, 3);

	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera) 
{
	mCamera = &camera;
}

void StandardEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}
void StandardEffect::SetShadowMap(const Texture& shadowMap) {
	mShadowMap = &shadowMap;
}


void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect##", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuseMap = mSettingData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuseMap##", &useDiffuseMap))
		{
			mSettingData.useDiffuseMap = (useDiffuseMap) ? 1 : 0;
		}
		bool useNormalMap = mSettingData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormalMap##", &useNormalMap))
		{
			mSettingData.useNormalMap = (useNormalMap) ? 1 : 0;
		}
		bool useBumpMap = mSettingData.useBumpMap > 0;
		if (ImGui::Checkbox("UseBumpMap##", &useBumpMap))
		{
			mSettingData.useBumpMap = (useBumpMap) ? 1 : 0;
		}
		if (useBumpMap == true) {
			ImGui::DragFloat("BumpWeight##", &mSettingData.bumpWeight, 0.1f, 0.0f, 2.0f);
		}
		bool useSpecMap = mSettingData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpecMap##", &useSpecMap))
		{
			mSettingData.useSpecMap = (useSpecMap) ? 1 : 0;
		}
		bool useCelShading = mSettingData.useCelShading > 0;
		if (ImGui::Checkbox("UseCelShading##", &useCelShading))
		{
			mSettingData.useCelShading = (useCelShading) ? 1 : 0;
		}
		bool useShadowMap = mSettingData.useShadowMap > 0;
		if (ImGui::Checkbox("UseShadowMap##", &useShadowMap))
		{
			mSettingData.useShadowMap = (useShadowMap) ? 1 : 0;
		}
		bool useSkinning = mSettingData.useSkinning > 0;
		if (ImGui::Checkbox("UseSkinning##", &useSkinning))
		{
			mSettingData.useSkinning = (useSkinning) ? 1 : 0;
		}
		ImGui::DragFloat("DepthBias##", &mSettingData.depthBias, 0.0000001f, 0.0f, 1.0f , "%.6f");
	}
}