#pragma once

#include "ModelManager.h"
#include "Bone.h"

namespace WNTRengine::Graphics
{
	class Animator
	{
	public:
		void Initialize(ModelId id);
		void PlayAnimation(int clipIndex, bool looping, bool blend = false);
		void update(float deltaTime);


		bool IsFinished() const;
		size_t GetAnimationCount() const;
		WNTRmath::Matrix4 GetToParentTransform(const Bone* bone) const;

		void blending(int lastClip, int newClip,float mDuration);
	private:
		ModelId mModelId = 0;
		float mClipIndex = -1;
		float mAnimationTick = 0.0f;
		float mNextAnimationTick = 0.0f;
		bool mIsLooping = false;
		bool mIsBlending = false;
		float blendingDuration = 3;
		float blendTime = 0;
	};

}