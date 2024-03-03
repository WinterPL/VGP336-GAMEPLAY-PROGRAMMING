#pragma once

#include <Inc/WNTRengine.h>

namespace CustomComponents
{
    bool CustomComponentMake(const char* componentName, const rapidjson::Value& value, WNTRengine::GameObject& gameObject);
    bool CustomServiceMake(const char* componentName, const rapidjson::Value& value, WNTRengine::GameWorld& gameWorld);
}

