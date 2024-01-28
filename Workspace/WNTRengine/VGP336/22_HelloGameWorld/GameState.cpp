#include "GameState.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void GameState::Initialize()
{
    mGameWorld.AddService<CameraService>();
    mGameWorld.AddService<UpdateService>();
    mGameWorld.Initialize(1000);
    mGameWorld.CreateGameObject("../../Assets/Templates/test_Object.json");
    mGameWorld.CreateGameObject("../../Assets/Templates/test_Camera.json");
    mGameWorld.CreateGameObject("../../Assets/Templates/test_fps_Camera.json");

}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Render()
{

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


