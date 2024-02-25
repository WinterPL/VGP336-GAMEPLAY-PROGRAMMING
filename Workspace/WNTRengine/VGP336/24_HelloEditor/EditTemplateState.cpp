#include "EditTemplateState.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

namespace
{
    bool CustomComponentMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
    {
        if (strcmp(componentName, "NewComponent") == 0)
        {
            //NewComponent* newComponent = gameObject.AddComponent<NewComponent>();
            //newComponent->Deserialize(value);

            return true;
        }
        return false;
    }

    bool CustomServiceMake(const char* componentName, const rapidjson::Value& value, GameWorld& gameWorld)
    {
        if (strcmp(componentName, "NewService") == 0)
        {
            //NewComponent* newComponent = gameObject.AddComponent<NewComponent>();
            //newComponent->Deserialize(value);

            return true;
        }
        return false;
    }
}


void EditTemplateState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameWorld::SetCustomServiceMake(CustomServiceMake);
    mGameWorld.loadLevel("../../Assets/Templates/Levels/test_level.json");
}

void EditTemplateState::Terminate()
{
    mGameWorld.Terminate();
}

void EditTemplateState::Render()
{
    mGameWorld.Render();
}

void EditTemplateState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void EditTemplateState::DebugUI()
{
    mGameWorld.EditorUI();
    if (ImGui::Button("Exit##EditTemplate"))
    {
        MainApp().ChangeState("EditorState");
    }
}


