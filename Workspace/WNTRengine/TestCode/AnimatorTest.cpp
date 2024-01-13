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
void Animator::PlayAnimation(int clipIndex, bool looping, bool blend)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mIsBlending = blend;
	mAnimationTick = 0.0f;
	mNextAnimationTick = 0.0f;
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
	if (mIsBlending)
	{
		int nextClip = mClipIndex + 1;
		if (nextClip >= model->animationClips.size())
		{
			nextClip = 1;
		}
		const auto& newAnim = model->animationClips[nextClip];
		mNextAnimationTick += newAnim.ticksPerSecond * deltaTime;
		if (mNextAnimationTick > newAnim.tickDuration)
		{
			if (mIsLooping)
			{
				while (mNextAnimationTick >= newAnim.tickDuration)
				{
					mNextAnimationTick -= newAnim.tickDuration;
				}
			}
		}
	}
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
		}
	}
	if (mIsBlending)
	{
		blendTime += deltaTime;
		if (blendTime >= blendingDuration)
		{
			mClipIndex++;
			if (mClipIndex >= model->animationClips.size())
			{
				mClipIndex = 1;
			}
			blendTime = 0;
			PlayAnimation(mClipIndex, mIsLooping, mIsBlending);
		}
	}
	else
	{
		blendTime = 0.0f;
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
	int nextClipIndex = mClipIndex + 1;
	if (nextClipIndex >= model->animationClips.size())
	{
		nextClipIndex = 1;
	}
	const auto& nextAnimClip = model->animationClips[nextClipIndex];
	const auto& animation = animClip.boneAnimations[bone->index];
	const auto& nextAnimation = nextAnimClip.boneAnimations[bone->index];
	if (animation == nullptr|| nextAnimation == nullptr)
	{
		if (animation != nullptr)
		{
			Transform transform = animation->GetTransform(mAnimationTick);
			return transform.GetMatrix4();
		}
		if (nextAnimation != nullptr)
		{
			Transform nextTransform = nextAnimation->GetTransform(mNextAnimationTick);
			return nextTransform.GetMatrix4();
		}
		return WNTRmath::Matrix4::Identity;
	}

	Transform transform = animation->GetTransform(mAnimationTick);
	Transform nextTransform = nextAnimation->GetTransform(mNextAnimationTick);

	float T = std::clamp(blendTime/blendingDuration,0.0f,1.0f);
	Transform finalTransform;
	finalTransform.position = WNTRmath::Lerp(transform.position,nextTransform.position,T);
	finalTransform.rotation = WNTRmath::Quaternion::SLerp(transform.rotation,nextTransform.rotation,T);
	finalTransform.scale = WNTRmath::Lerp(transform.scale,nextTransform.scale,T);

	return finalTransform.GetMatrix4();
}



