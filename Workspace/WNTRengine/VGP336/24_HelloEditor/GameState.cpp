#include "GameState.h"

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


void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameWorld::SetCustomServiceMake(CustomServiceMake);
    mGameWorld.loadLevel("../../Assets/Templates/Levels/test_level.json");
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
   
    auto input = InputSystem::Get();
    if (input->IsKeyPressed(KeyCode::ONE))
    {
        CameraService* cameraService = mGameWorld.GetService<CameraService>();
        cameraService->SetMainCamera(0);
    }
    else if (input->IsKeyPressed(KeyCode::TWO))
    {
        CameraService* cameraService = mGameWorld.GetService<CameraService>();
        cameraService->SetMainCamera(1);
    }

}

void GameState::DebugUI()
{
    mGameWorld.DebugUI();

}


