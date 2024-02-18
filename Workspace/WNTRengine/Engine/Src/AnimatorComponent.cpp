#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "GameWorld.h"
#include "ModelComponent.h"
#include "UpdateService.h"

using namespace WNTRengine;

void AnimatorComponent::Initialize()
{
	ModelComponent* modelComponent = GetOwner().GetComponent<ModelComponent>();
	mAnimator.Initialize(modelComponent->GetModelId());
	
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	updateService->Register(this);
}
void AnimatorComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	updateService->Unregister(this);
}
void AnimatorComponent::Update(float deltaTime)
{
	mAnimator.update(deltaTime);
}
void AnimatorComponent::DebugUI()
{
	std::string buttonTag = "PlayAnim##" + GetOwner().GetName();
	uint32_t animationCount = mAnimator.GetAnimationCount();
	for (uint32_t i = 0; i<animationCount;++i)
	{
		std::string buttonName = buttonTag + std::to_string(i);
		if (ImGui::Button(buttonName.c_str()))
		{
			Play(i, true);
		}
	}
}
bool AnimatorComponent::Play(int index, bool looping)
{
	mAnimator.PlayAnimation(index, looping);
	return true;
}