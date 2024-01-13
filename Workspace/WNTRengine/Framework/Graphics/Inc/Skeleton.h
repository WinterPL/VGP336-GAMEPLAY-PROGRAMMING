#pragma once

#include "Bone.h"

namespace WNTRengine::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>>bones;
	};

}