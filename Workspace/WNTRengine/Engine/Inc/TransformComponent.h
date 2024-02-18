#pragma once

#include "Component.h"

namespace WNTRengine
{
	class TransformComponent final : public Component, public Graphics::Transform
	{
	public:
		SET_TYPE_ID(ComponentId::Transform);

		void DebugUI() override;
		void DeSerialize(const rapidjson::Value& value) override;
	};

}