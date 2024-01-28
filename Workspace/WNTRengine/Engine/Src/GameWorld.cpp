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