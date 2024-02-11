#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"
#include "CameraService.h"
#include "UpdateService.h"
#include "RenderService.h"
#include "PhysicsService.h"

#include "TransformComponent.h"
#include "RigidBodyComponent.h"

using namespace WNTRengine;

namespace {
	CustomService TryServiceMake;
}

void GameWorld::SetCustomServiceMake(CustomService customService)
{
	TryServiceMake = customService;
}


void GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(mInitialized, "GameWorld: is alreadt Initialized");
	for (auto& service : mServices) {
		service->Initialize();
	}

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);

	mInitialized = true;
}
void GameWorld::Terminate()
{
	ASSERT(!mUpdating, "GameWorld: can't terminate while update");
	if (!mInitialized)
	{
		return;
	}
	for (auto& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->Terminate();
			slot.gameObject.reset();
		}
	}
	for (auto& service : mServices) {
		service->Terminate();
	}
	mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
	for (auto& service : mServices) {
		service->Update(deltaTime);
	}
}
void GameWorld::Render()
{
	for (auto& service : mServices) {
		service->Render();
	}
}
void GameWorld::DebugUI()
{
	for (auto& service : mServices) {
		service->DebugUI();
	}

	for (auto& slot : mGameObjectSlots) {
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->DebugUI();
		}
	}

}

GameObject* GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
	ASSERT(mInitialized, "GameWorld: is not initialized");
	if (mFreeSlots.empty())
	{
		ASSERT(false, "GameWorld: no free slots available");
		return nullptr;
	}

	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	Slot& slot = mGameObjectSlots[freeSlot];
	std::unique_ptr<GameObject>& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	GameObjectFactory::Make(templateFile, *newObject);

	newObject->mWorld = this;
	newObject->mHandle.mIndex = freeSlot;
	newObject->mHandle.mGeneration = slot.generation;
	newObject->Initialize();
	return newObject.get();
}


GameObject* GameWorld::GetGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	{
		return nullptr;
	}
	return mGameObjectSlots[handle.mIndex].gameObject.get();
}
void GameWorld::DestroyGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	{
		return;
	}

	Slot& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++;
	mToBeDestroyed.push_back(handle.mIndex);
}

void GameWorld::loadLevel(const std::filesystem::path& levelFile)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameWorld: failed to load level %s", levelFile.u8string().c_str());
	
	char readBuffer[65536];
	rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rapidjson::Document doc;
	doc.ParseStream(readStream);

	auto services = doc["Services"].GetObj();
	for (auto& service : services)
	{
		const char* serviceName = service.name.GetString();

		if (TryServiceMake(serviceName, service.value, *this))
		{
			//Custom Service, Project will handle
		}
		else if (strcmp(serviceName, "CameraService") == 0)
		{
			CameraService* cameraService = AddService<CameraService>();
			cameraService->DeSerialize(service.value);
		}
		else if (strcmp(serviceName, "UpdateService") == 0)
		{
			UpdateService* updateService = AddService<UpdateService>();
			updateService->DeSerialize(service.value);
		}
		else if (strcmp(serviceName, "RenderService") == 0)
		{
			RenderService* renderService = AddService<RenderService>();
			renderService->DeSerialize(service.value);
		}
		else if (strcmp(serviceName, "PhysicsService") == 0)
		{
			PhysicsService* physicsService = AddService<PhysicsService>();
			physicsService->DeSerialize(service.value);
		}
		else
		{
			ASSERT(false, "GameWorld: service %s is not defined", serviceName);
		}
	}
	uint32_t capacity = static_cast<uint32_t>(doc["Capacity"].GetInt());
	Initialize(capacity);

	auto gameObjects = doc["GameObjects"].GetObj();
	for (auto& gameObject : gameObjects)
	{
		const char* templateFile = gameObject.value["Template"].GetString();
		GameObject* obj = CreateGameObject(templateFile);
		if (obj != nullptr)
		{
			const char* name = gameObject.name.GetString();
			obj->SetName(name);

			if (gameObject.value.HasMember("Position"))
			{
				const auto& pos = gameObject.value["Position"].GetArray();
				const float x = pos[0].GetFloat();
				const float y = pos[1].GetFloat();
				const float z = pos[2].GetFloat();

				TransformComponent* transformComponent = obj->GetComponent<TransformComponent>();
				transformComponent->position = { x,y,z };

				RigidBodyComponent* rigidBodyComponent = obj->GetComponent<RigidBodyComponent>();
				if (rigidBodyComponent != nullptr)
				{
					rigidBodyComponent->SetPosition(transformComponent->position);
				}
			}
		}
	}

}


bool GameWorld::IsValid(const GameObjectHandle& handle)
{
	if (handle.mIndex < 0 || handle.mIndex >= mGameObjectSlots.size())
	{
		return false;
	}
	if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
	{
		return false;
	}
	return true;
}
void GameWorld::ProcessDestroyList()
{
	ASSERT(!mUpdating, "GameWorld: can't destroy while updating");
	for (uint32_t index : mToBeDestroyed)
	{
		Slot& slot = mGameObjectSlots[index];

		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GamWordl:object is still Alive");
		
		gameObject->Terminate();
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}
	mToBeDestroyed.clear();
}