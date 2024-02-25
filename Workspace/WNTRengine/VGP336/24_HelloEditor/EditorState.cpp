#include "EditorState.h"

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


void EditorState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameWorld::SetCustomServiceMake(CustomServiceMake);
    mGameWorld.loadLevel("../../Assets/Templates/Levels/test_level.json");
}

void EditorState::Terminate()
{
    mGameWorld.Terminate();
}

void EditorState::Render()
{
    mGameWorld.Render();
}

void EditorState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void EditorState::DebugUI()
{
    mGameWorld.EditorUI();
}


