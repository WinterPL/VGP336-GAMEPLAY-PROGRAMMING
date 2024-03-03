#include "Precompiled.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "WNTRengine.h"

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

		if (ImGui::Button(("Save##" + mName).c_str()))
		{
			mWorld->SaveTemplate(mTemplatePath, mHandle);
		}
	}
}

void GameObject::EditorUI()
{
	if (ImGui::CollapsingHeader(mName.c_str())) {
		for (auto& Component : mComponents)
		{
			Component->EditorUI();
		}

		if (ImGui::Button(("Edit##" + mName).c_str()))
		{
			//global data here to load the template
			GameWorld::SetEditObject(mName);
			MainApp().ChangeState("EditTemplateState");
		}
	}
}

void GameObject::Serialize(rapidjson::Document& doc)
{
	rapidjson::Value components(rapidjson::kObjectType);
	for (auto& component : mComponents)
	{
		component->Serialize(doc, components);
	}
	doc.SetObject();
	doc.AddMember("Components", components, doc.GetAllocator());
}