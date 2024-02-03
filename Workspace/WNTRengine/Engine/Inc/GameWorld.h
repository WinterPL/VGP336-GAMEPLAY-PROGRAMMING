#pragma once

#include "Service.h"
#include "GameObject.h"

namespace WNTRengine
{
	class GameWorld final
	{
	public:
		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		GameObject* CreateGameObject(const std::filesystem::path& templateFile);
		GameObject* GetGameObject(const GameObjectHandle& handle);
		void DestroyGameObject(const GameObjectHandle& handle);

		void loadLevel(const std::filesystem::path& levelFile);

		template<class ServiceType>
		ServiceType* AddService() 
		{
			static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld: Service must be type service");
			ASSERT(!mInitialized, "GameWorld: can't add service after world is initialized");
			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template<class ServiceType>
		ServiceType* GetService()
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == ServiceType::StaticGetTypeId())
				{
					return static_cast<ServiceType*>(service.get());
				}
			}
			return nullptr;
		}
		template<class ServiceType>
		const ServiceType* GetService() const
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == ServiceType::StaticGetTypeId())
				{
					return static_cast<ServiceType*>(service.get());
				}
				return nullptr;
			}
		}
	

	private:
		bool IsValid(const GameObjectHandle& handle);
		void ProcessDestroyList();

		struct Slot {
			std::unique_ptr<GameObject> gameObject;
			uint32_t generation = 0;
		};

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectSlots = std::vector<Slot>;

		Services mServices;

		GameObjectSlots mGameObjectSlots;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		bool mInitialized = false;
		bool mUpdating = false;
	};

}