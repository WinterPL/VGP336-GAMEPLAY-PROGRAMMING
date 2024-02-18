#include "Precompiled.h"
#include "SoundEffectComponent.h"

#include "GameWorld.h"

using namespace WNTRengine;
using namespace WNTRengine::Audio;


void SoundEffectComponent::Initialize()
{
	ASSERT(!mFileName.empty(), "SoundEffectComponent: no sound file loaded");
	mSoundId = SoundEffectManager::Get()->Load(mFileName);
}
void SoundEffectComponent::Terminate()
{
	//should probably remove audio effect
}
void SoundEffectComponent::DeSerialize(const rapidjson::Value& value)
{
	if (value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
	if (value.HasMember("Looping"))
	{
		mLooping = value["Looping"].GetBool();
	}
}

void SoundEffectComponent::DebugUI()
{
	std::string buttonName = "PlaySound##" + GetOwner().GetName();
	if (ImGui::Button(buttonName.c_str()))
	{
		Play();
	}
}

void SoundEffectComponent::Play()
{
	SoundEffectManager::Get()->Play(mSoundId, mLooping);
}
void SoundEffectComponent::Stop()
{
	SoundEffectManager::Get()->Stop(mSoundId);
}