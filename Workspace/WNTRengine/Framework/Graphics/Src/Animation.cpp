#include "Precompiled.h"
#include "Animation.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;

namespace {
	float GetLerpTime(float startTime, float endTime, float time, EaseType easeType)
	{
		float t =  (time - startTime) / (endTime - startTime);
		switch (easeType) {
		case EaseType::Linear: break;
		case EaseType::EaseInQuad: t = t * t; break;
		case EaseType::EaseOutQuad: t = -t * (t -2.0f); break;
		case EaseType::EaseInOutQuad:
		{
			t *= 2.0f;
			if (t < 1.0f){t = 0.5f * t * t;}
			else {t -= 1.0f;t = -0.5 *((t*(t -2.f)) - 1.f);}
		}break;
		default: ASSERT(false,"AnimationGetLerpTime : EaseType not Supported"); break;
		}
		return t;
	}
}


Transform Animation::GetTransform(float time)const {
	Transform transform;
	transform.position = GetPosition(time);
	transform.rotation = GetRotation(time);
	transform.scale = GetScale(time);

	return transform;
}

float Animation::GetDuration() const {
	return mDuration;
}

WNTRmath::Vector3 Animation::GetPosition(float time)const 
{
		
	for (uint32_t i = 0; i < mPositionKeys.size(); ++i) 
	{
		if (time < mPositionKeys[i].time) 
		{
			if (i > 0)
			{
				float t = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time,mPositionKeys[i].easeType);
				return WNTRmath::Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, t);
			}
			return mPositionKeys[i].key;
		}
	}
	if (mPositionKeys.size() > 0)
	{
		return mPositionKeys[mPositionKeys.size() - 1].key;
	}
	return WNTRmath::Vector3::Zero;
}
WNTRmath::Quaternion Animation::GetRotation(float time)const 
{

	for (uint32_t i = 0; i < mRotationKeys.size(); ++i) 
	{
		if (time < mRotationKeys[i].time) 
		{
			if (i > 0)
			{
				float t = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time, mRotationKeys[i].easeType);
				return WNTRmath::Quaternion::SLerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, t);
			}
			return mRotationKeys[i].key;
		}
	}
	if (mScaleKeys.size() > 0)
	{
		return mRotationKeys[mRotationKeys.size() - 1].key;
	}
	return WNTRmath::Quaternion::Identity;
}
WNTRmath::Vector3 Animation::GetScale(float time)const 
{

	for (uint32_t i = 0; i < mScaleKeys.size(); ++i) 
	{
		if (time < mScaleKeys[i].time) 
		{
			if (i > 0)
			{
				float t = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time, mScaleKeys[i].easeType);
				return WNTRmath::Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, t);
			}
			return mScaleKeys[i].key;
		}
	}
	if (mScaleKeys.size() > 0)
	{
		return mScaleKeys[mScaleKeys.size() - 1].key;
	}
	return WNTRmath::Vector3::One;
}