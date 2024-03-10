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
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponent: UpdateService is not Available");
	updateService->Register(this);
}

void ControllerComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponent: UpdateService is not Available");
	updateService->Unregister(this);
}

void ControllerComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(mTransformComponent->GetMatrix4());
}

void ControllerComponent::Update(float deltaTime)
{

	auto input = InputSystem::Get();

	if (input->IsKeyDown(KeyCode::W)) {
		//mTransformComponent->position.x += 1*deltaTime;
	}
	else if (input->IsKeyDown(KeyCode::S)) {
		//mTransformComponent->position.x -= 1*deltaTime;
	}

	if (input->IsKeyDown(KeyCode::D)) {
		
	}
	else if (input->IsKeyDown(KeyCode::A)) {
		
	}

	if (input->IsKeyDown(KeyCode::E)) {
		
	}
	else if (input->IsKeyDown(KeyCode::Q)) {
		
	}

	if (input->IsMouseDown(MouseButton::RBUTTON)) {
		
	}
	if (mRigidBodyComponent != nullptr)
	{
		mRigidBodyComponent->SetPosition(mTransformComponent->position);
	}
}


void ControllerComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	/*rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Position", mTransform.position, doc, componentValue);
	SaveUtil::SaveQuaternion("Rotation", mTransform.rotation, doc, componentValue);
	SaveUtil::SaveVector3("Scale", mTransform.scale, doc, componentValue);
	value.AddMember("TransformComponent", componentValue, doc.GetAllocator());*/
}

void ControllerComponent::DeSerialize(const rapidjson::Value& value)
{
	/*if (value.HasMember("Position"))
	{
		const auto& pos = value["Position"].GetArray();
		mTransform.position.x = pos[0].GetFloat();
		mTransform.position.y = pos[1].GetFloat();
		mTransform.position.z = pos[2].GetFloat();
	}
	if (value.HasMember("Rotation"))
	{
		const auto& rot = value["Rotation"].GetArray();
		mTransform.rotation.x = rot[0].GetFloat();
		mTransform.rotation.y = rot[1].GetFloat();
		mTransform.rotation.z = rot[2].GetFloat();
		mTransform.rotation.w = rot[3].GetFloat();
	}
	if (value.HasMember("Scale"))
	{
		const auto& s = value["Scale"].GetArray();
		mTransform.scale.x = s[0].GetFloat();
		mTransform.scale.y = s[1].GetFloat();
		mTransform.scale.z = s[2].GetFloat();
	}*/
}