#include "Precompiled.h"
#include "FPSCameraComponent.h"

#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "UpdateService.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void FPSCameraComponent::Initialize()
{
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponent: UpdateService is not Available");
	updateService->Register(this);
}

void FPSCameraComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponent: UpdateService is not Available");
	updateService->Unregister(this);
}

void FPSCameraComponent::Update(float deltaTime)
{
    Camera& camera = mCameraComponent->GetCamera();
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 0.1f);
    const float turnSpeed = mTurnSpeed * deltaTime;
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
    if (mTransformComponent != nullptr)
    {
        mTransformComponent->position = camera.GetPosition();
    }

}

void FPSCameraComponent::DeSerialize(const rapidjson::Value& value)
{
    if (value.HasMember("MoveSpeed"))
    {
        mMoveSpeed = value["MoveSpeed"].GetFloat();
    }
    if (value.HasMember("TurnSpeed"))
    {
        mTurnSpeed = value["TurnSpeed"].GetFloat();
    }
}