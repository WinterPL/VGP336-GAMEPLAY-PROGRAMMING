#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"

using namespace WNTRengine;

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
	return nullptr;
}
void GameWorld::DestroyGameObject(const GameObjectHandle& handle)
{

}


bool GameWorld::IsValid(const GameObjectHandle& handle)
{
	return false;
}
void GameWorld::ProcessDestroyList()
{

}