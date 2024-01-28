#pragma once

#include "Service.h"

namespace WNTRengine
{
	class Component;

	class UpdateService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Update);

		void Update(float deltaTime) override;
		void Register(Component* component);
		void Unregister(Component* component);

	private:
		void RegisterPendingUpdates();

		using UpdateComponents = std::vector<Component*>;
		UpdateComponents mUpdateComponents;
		UpdateComponents mPendingUpdateComponents;
	
		bool mIsUpdating = false;
	};
}