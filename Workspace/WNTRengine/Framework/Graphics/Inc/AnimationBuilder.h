#pragma once

#include "Animation.h"

namespace WNTRengine::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& AddPositionKey(const WNTRmath::Vector3& position, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddRotationKey(const WNTRmath::Quaternion& position, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddScaleKey(const WNTRmath::Vector3& position, float time, EaseType easeType = EaseType::Linear);

		[[nodiscard]]Animation Build();

	private:
		Animation mWorkingCopy;
	};
}
