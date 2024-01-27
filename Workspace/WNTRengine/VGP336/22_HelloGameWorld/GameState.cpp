#include "GameState.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void GameState::Initialize()
{
    CameraComponent* cameraComponent = mCameraGameObject1.AddComponent<CameraComponent>();
    cameraComponent->GetCamera().SetPosition({ 0.0f, 1.5f,-4.0f });
    cameraComponent->GetCamera().SetLookAt({ 0.0f, 0.0f,0.0f });
    mCameraService.Register(cameraComponent);

    cameraComponent = mCameraGameObject2.AddComponent<CameraComponent>();
    cameraComponent->GetCamera().SetPosition({ 0.0f, 1.5f,-4.0f });
    cameraComponent->GetCamera().SetLookAt({ 0.0f, 0.0f,0.0f });
    mCameraService.Register(cameraComponent);


    GameObjectFactory::Make("../../Assets/Templates/test_Object.json", mGameObject);
    mGameObject.Initialize();
}

void GameState::Terminate()
{
    mGameObject.Terminate();
}

void GameState::Render()
{

}

void GameState::Update(float deltaTime)
{
    CameraComponent* cameraComponent = mCameraGameObject1.GetComponent<CameraComponent>();
    Camera& camera = cameraComponent->GetCamera();
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 0.1f);
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W)) {
        camera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S)) {
        camera.Walk(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::D)) {
        camera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A)) {
        camera.Strafe(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::E)) {
        camera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q)) {
        camera.Rise(-moveSpeed);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON)) {
        camera.Yaw(input->GetMouseMoveX() * turnSpeed);
        camera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }

    if (input->IsKeyPressed(KeyCode::ONE))
    {
        mCameraService.SetMainCamera(0);
    }
    else if (input->IsKeyPressed(KeyCode::TWO))
    {
        mCameraService.SetMainCamera(1);
    }

}

void GameState::DebugUI()
{
    mGameObject.DebugUI();
    SimpleDraw::Render(mCameraService.GetMain());
    
}


