#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameWorld.h"
#include "CameraService.h"
#include "SaveUtil.h"

using namespace WNTRengine;

void CameraComponent::Initialize()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	ASSERT(cameraService!=nullptr,"CameraComponent: CameraService is not Available");
	cameraService->Register(this);

}
void CameraComponent::Terminate()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	ASSERT(cameraService != nullptr, "CameraComponent: CameraService is not Available");
	cameraService->Unregister(this);
}

void CameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Position", mStartingPosition, doc, componentValue);
	SaveUtil::SaveVector3("LookAt", mStartingLookAt, doc, componentValue);
	value.AddMember("CameraComponent", componentValue, doc.GetAllocator());
}

void CameraComponent::DeSerialize(const rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		const auto& pos = value["Position"].GetArray();
		float x = pos[0].GetFloat();
		float y = pos[1].GetFloat();
		float z = pos[2].GetFloat();
		mStartingPosition = { x,y,z };
		mCamera.SetPosition({ x,y,z });
	}
	if (value.HasMember("LookAt"))
	{
		const auto& pos = value["LookAt"].GetArray();
		float x = pos[0].GetFloat();
		float y = pos[1].GetFloat();
		float z = pos[2].GetFloat();
		mStartingLookAt = { x,y,z };
		mCamera.SetLookAt({ x,y,z });
	}
}