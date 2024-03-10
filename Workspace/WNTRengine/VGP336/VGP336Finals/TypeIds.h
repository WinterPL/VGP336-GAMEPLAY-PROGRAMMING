#pragma once
#include <../Engine/Inc/typeIds.h>

enum class CustomComponentId
{
	Controller = static_cast<uint32_t>(WNTRengine::ComponentId::Count)
};

enum class CustomServiceId
{
	NewService = static_cast<uint32_t>(WNTRengine::ServiceId::Count)
};