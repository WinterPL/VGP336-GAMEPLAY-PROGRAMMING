#pragma once

#include "Common.h"

namespace WNTRengine::Graphics
{
	struct Bone
	{
		std::string name;
		int index = -1;

		Bone* parent = nullptr;
		int parentIndex = 0;

		std::vector<Bone*>children;
		std::vector<int>childrenIndices;


		WNTRmath::Matrix4 toParentTransform;
		WNTRmath::Matrix4 offsetTransform;
	};


}