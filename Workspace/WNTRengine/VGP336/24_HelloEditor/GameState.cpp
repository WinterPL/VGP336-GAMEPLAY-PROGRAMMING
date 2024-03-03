#include "GameState.h"
#include "CustomFactory.h"
using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;


void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponents::CustomComponentMake);
    GameWorld::SetCustomServiceMake(CustomComponents::CustomServiceMake);
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
    ImGui::Begin("GameState", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.EditorUI();

    if (ImGui::Button("Edit##GameWorld"))
    {
       MainApp().ChangeState("EditorState");
    }

    ImGui::End();
}


