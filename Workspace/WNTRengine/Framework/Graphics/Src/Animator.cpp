#include "Precompiled.h"
#include "Animator.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;



void Animator::Initialize(ModelId id) 
{
	mModelId = id;
	mIsLooping = false;
	mAnimationTick = 0.0f;
	mClipIndex = -1;
}
void Animator::PlayAnimation(int clipIndex, bool looping, bool loopAll)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mIsLoopAll = loopAll;
	mAnimationTick = 0.0f;
}
void Animator::update(float deltaTime)
{
	if(mClipIndex < 0)
	{
		return;
	}

	auto model = ModelManager::Get()->GetModel(mModelId);
	const auto& animClip = model->animationClips[mClipIndex];
	mAnimationTick += animClip.ticksPerSecond * deltaTime;
	if (mAnimationTick > animClip.tickDuration)
	{
		if (mIsLooping)
		{
			while (mAnimationTick >= animClip.tickDuration)
			{
				mAnimationTick -= animClip.tickDuration;
			}
		}
		else
		{
			mAnimationTick = animClip.tickDuration;
			if (mIsLoopAll)
			{
				mClipIndex++;
				if (mClipIndex >= model->animationClips.size())
				{
					mClipIndex = 1;
				}
				PlayAnimation(mClipIndex,mIsLooping, mIsLoopAll);
			}
		}
	}
}

bool Animator::IsFinished() const
{
	if (mIsLooping || mClipIndex < 0)
	{
		return false;
	}
	auto model = ModelManager::Get()->GetModel(mModelId);
	const auto& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.tickDuration;
}
size_t Animator::GetAnimationCount() const
{
	auto model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}
WNTRmath::Matrix4 Animator::GetToParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
	{
		return bone->toParentTransform;
	}
	auto model = ModelManager::Get()->GetModel(mModelId);
	const auto& animClip = model->animationClips[mClipIndex];
	const auto& animation = animClip.boneAnimations[bone->index];
	if (animation == nullptr)
	{
		return WNTRmath::Matrix4::Identity;
	}

	Transform transform = animation->GetTransform(mAnimationTick);
	return transform.GetMatrix4();
}
