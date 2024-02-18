#include "Precompiled.h"
#include "ModelComponent.h"

#include "GameWorld.h"
#include "RenderService.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;

void ModelComponent::Initialize()
{
	ModelManager* mm = ModelManager::Get();
	mModelId = mm->GetModelId(mFileName);
	if (mm->GetModel(mModelId)==nullptr)
	{
		mModelId = mm->LoadModel(mFileName);
		for (const std::string& fileName : mAnimationFileNames)
		{
			mm->AddAnimation(mModelId, fileName);
		}
	}

	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Register(this);

}
void ModelComponent::Terminate()
{
	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Unregister(this);
}

void ModelComponent::DeSerialize(const rapidjson::Value& value)
{
	if (value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
	if (value.HasMember("Animations"))
	{
		auto animations = value["Animations"].GetArray();
		for (auto& animation : animations)
		{
			mAnimationFileNames.push_back(animation.GetString());
		}
	}
}