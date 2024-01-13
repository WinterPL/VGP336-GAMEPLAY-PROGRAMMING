#pragma once

#include "Keyframe.h"
#include "Transform.h"

namespace WNTRengine::Graphics
{
	class Animation {
	public: 

		Transform GetTransform(float time)const;
		float GetDuration() const;

	private:
		WNTRmath::Vector3 GetPosition(float time)const;
		WNTRmath::Quaternion GetRotation(float time)const;
		WNTRmath::Vector3 GetScale(float time)const;

		friend class AnimationBuilder;
		friend class AnimationIO;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration;
	};		

}