#include "Precompiled.h"
#include "GameObject.h"
#include "TransformComponent.h"

using namespace WNTRengine;

static uint32_t gUinqueId = 0;

void GameObject::Initialize()
{
	ASSERT(!mInitialized, "GameObject: is Already initialized");

	for (auto& Component : mComponents)
	{
		Component->Initialize();
	}

	mUniqueId = ++gUinqueId;
	mInitialized = true;

	ASSERT(HasA<TransformComponent>(),"GameObject: must have a transform");
}

void GameObject::Terminate()
{
	for (auto& Component : mComponents)
	{
		Component->Terminate();
	}
}

void GameObject::DebugUI()
{
	if (ImGui::CollapsingHeader(mName.c_str())) {
		for (auto& Component : mComponents)
		{
			Component->DebugUI();
		}
	}
}