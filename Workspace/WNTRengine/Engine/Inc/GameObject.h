#pragma once

#include "Component.h"

namespace WNTRengine
{
	class GameObject final
	{
	public:
		GameObject() = default;

		void Initialize();
		void Terminate();
		void DebugUI();

		void SetName(const std::string& name) {mName = std::move(name);}
		const std::string& GetName() const { return mName; }
		uint32_t GetUniqueId() const { return mUniqueId; }

		template<class ComponentType>
		ComponentType* AddComponent()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject : ComponentType must be the type of Component");
			
			ASSERT(!mInitialized, "GameObject : cannot add components once initialized");
			ASSERT(!HasA<ComponentType>(), "GameObject : already has component");
			auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
			newComponent->mOwner = this;
			return static_cast<ComponentType*>(newComponent.get());
		}
		template<class ComponentType>
		bool HasA()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject : ComponentType must be the type of Component");

			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == ComponentType::StaticGetTypeId())
				{
					return true;
				}
			}

			return false;
		}

	private:
		std::string mName = "EMPTY";
		bool mInitialized = false;
		uint32_t mUniqueId = 0;

		using Components = std::vector<std::unique_ptr<Component>>;
		Components mComponents;
	};

}