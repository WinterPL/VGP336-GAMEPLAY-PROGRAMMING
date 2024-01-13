#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float t, EaseType easeType)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= t, "AnimationBuilder: cannot add keyframe back in ");
		keyframes.emplace_back(value, t, easeType);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const WNTRmath::Vector3& position, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mPositionKeys,position, time, easeType);
	mWorkingCopy.mDuration = WNTRmath::Max(mWorkingCopy.mDuration, time);
	return  *this;
}
AnimationBuilder& AnimationBuilder::AddRotationKey(const WNTRmath::Quaternion& position, float time, EaseType easeType) 
{
	PushKey(mWorkingCopy.mRotationKeys, position, time, easeType);
	mWorkingCopy.mDuration = WNTRmath::Max(mWorkingCopy.mDuration, time);
	return  *this;
}
AnimationBuilder& AnimationBuilder::AddScaleKey(const WNTRmath::Vector3& position, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mScaleKeys, position, time, easeType);
	mWorkingCopy.mDuration = WNTRmath::Max(mWorkingCopy.mDuration, time);
	return  *this;
}

Animation AnimationBuilder::Build() 
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty() || !mWorkingCopy.mRotationKeys.empty() || !mWorkingCopy.mScaleKeys.empty(), "AnimationBuilder: No animation keys ");
	return std::move(mWorkingCopy);
}