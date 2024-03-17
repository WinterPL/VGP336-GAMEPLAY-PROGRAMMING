#include "ControllerComponent.h"

#include "../Engine/Inc/SaveUtil.h"
#include "../Engine/Inc/GameWorld.h"
#include "../Engine/Inc/UpdateService.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void ControllerComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mRigidBodyComponent = GetOwner().GetComponent<RigidBodyComponent>();
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "ControllerComponent: UpdateService is not Available");
	updateService->Register(this);
}

void ControllerComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "ControllerComponent: UpdateService is not Available");
	updateService->Unregister(this);
}

void ControllerComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(mTransformComponent->GetMatrix4());
}

void ControllerComponent::Update(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = mMoveSpeed * deltaTime;
	const float turnSpeed = mTurnSpeed * deltaTime;
	if (input->IsKeyDown(KeyCode::W)) {
		Walk(-moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S)) {
		Walk(moveSpeed);
	}

	if (input->IsKeyDown(KeyCode::D)) {
		Strafe(-moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A)) {
		Strafe(moveSpeed);
	}

	if (input->IsKeyDown(KeyCode::E)) {
		Yaw(turnSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q)) {
		Yaw(-turnSpeed);
	}

	Camera& camera = mCameraComponent->GetCamera();
	/*float x =,y = ,z = ,
	const WNTRmath::Vector3 vec = {x,y,z};*/
	const WNTRmath::Vector3 r = WNTRmath::Normalize(WNTRmath::Cross(WNTRmath::Vector3::YAxis, mDirection));
	const WNTRmath::Vector3 u = WNTRmath::Normalize(WNTRmath::Cross(mDirection, r));
	const WNTRengine::WNTRmath::Vector3 pos = mTransformComponent->position + mDirection * 1.0f + r * -0.6f + u * 1.5f;
	camera.SetPosition(pos);
	camera.SetLookAt(pos - mDirection);
}


void ControllerComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveFloat("TurnSpeed", mTurnSpeed, doc, componentValue);
	SaveUtil::SaveFloat("MoveSpeed", mMoveSpeed, doc, componentValue);
	value.AddMember("ControllerComponent", componentValue, doc.GetAllocator());
}

void ControllerComponent::DeSerialize(const rapidjson::Value& value)
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

void ControllerComponent::Walk(float distance)
{
	Vector3 velocity = mDirection * distance;
	mRigidBodyComponent->SetVelocity(velocity);
}

void ControllerComponent::Strafe(float distance)
{
	const WNTRmath::Vector3 r = WNTRmath::Normalize(WNTRmath::Cross(WNTRmath::Vector3::YAxis, mDirection));
	Vector3 velocity = r * distance;
	mRigidBodyComponent->SetVelocity(velocity);
}

void ControllerComponent::Yaw(float radian)
{
	WNTRmath::Matrix4 matRotate = WNTRmath::Matrix4::RotationY(radian);
	mDirection = WNTRmath::TransformNormal(mDirection, matRotate);
	if (mRigidBodyComponent != nullptr)
	{
		mRigidBodyComponent->SetLookDirection(mDirection);
	}
}

