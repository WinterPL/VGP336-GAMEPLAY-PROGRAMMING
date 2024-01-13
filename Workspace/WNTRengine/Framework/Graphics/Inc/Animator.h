#pragma once

#include "ModelManager.h"
#include "Bone.h"

namespace WNTRengine::Graphics
{
	class Animator
	{
	public:
		void Initialize(ModelId id);
		void PlayAnimation(int clipIndex, bool looping, bool loopAll = false);
		void update(float deltaTime);


		bool IsFinished() const;
		size_t GetAnimationCount() const;
		WNTRmath::Matrix4 GetToParentTransform(const Bone* bone) const;
	private:
		ModelId mModelId = 0;
		float mClipIndex = -1;
		float mAnimationTick = 0.0f;
		bool mIsLooping = false;
		bool mIsLoopAll = false;
	};

}