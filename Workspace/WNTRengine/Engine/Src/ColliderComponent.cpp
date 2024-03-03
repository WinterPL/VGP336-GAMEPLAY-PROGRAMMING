#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameWorld.h"
#include "SaveUtil.h"

using namespace WNTRengine;

void ColliderComponent::Initialize()
{
	//
}
void ColliderComponent::Terminate()
{
	mCollisionShape.Terminate();
}

void ColliderComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	rapidjson::Value shapeValue(rapidjson::kObjectType);
	SaveUtil::SaveString("Type", mLoadingData.shapeType.c_str(), doc, shapeValue);
	if (mLoadingData.shapeType == "Empty")
	{
		//nothing
	}
	else if (mLoadingData.shapeType == "Box")
	{
		SaveUtil::SaveVector3("HalfExtents", mLoadingData.param0, doc, shapeValue);
	}
	else if (mLoadingData.shapeType == "sphere")
	{
		SaveUtil::SaveFloat("Radius", mLoadingData.param0.x, doc, shapeValue);
	}
	else if (mLoadingData.shapeType == "Hull")
	{
		SaveUtil::SaveVector3("HalfExtents", mLoadingData.param0, doc, shapeValue);
		SaveUtil::SaveVector3("Origin", mLoadingData.param1, doc, shapeValue);
	}
	componentValue.AddMember("Shape", shapeValue, doc.GetAllocator());
	value.AddMember("ColliderComponent", componentValue, doc.GetAllocator());
}

void ColliderComponent::DeSerialize(const rapidjson::Value& value)
{
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			mLoadingData.shapeType = shapeData["Type"].GetString();
			if (mLoadingData.shapeType == "Empty")
			{
				mCollisionShape.InitializeEmpty();
			}
			else if (mLoadingData.shapeType == "Box")
			{
				const auto& he= shapeData["HalfExtents"].GetArray();
				const float x = he[0].GetFloat();
				const float y = he[1].GetFloat();
				const float z = he[2].GetFloat();
				mLoadingData.param0 = { x,y,z };
				mCollisionShape.InitializeBox(mLoadingData.param0);
			}
			else if (mLoadingData.shapeType == "sphere")
			{
				mLoadingData.param0.x = shapeData["Radius"].GetFloat();
				mCollisionShape.InitializeSphere(mLoadingData.param0.x);
			}
			else if (mLoadingData.shapeType == "Hull")
			{
				const auto& he = shapeData["HalfExtents"].GetArray();
				const float heX = he[0].GetFloat();
				const float heY = he[1].GetFloat();
				const float heZ = he[2].GetFloat();
				const auto& o = shapeData["Origin"].GetArray();
				const float oX = o[0].GetFloat();
				const float oY = o[1].GetFloat();
				const float oZ = o[2].GetFloat();
				mLoadingData.param0 = { heX,heY,heZ };
				mLoadingData.param1 = { oX, oY,oZ };
				mCollisionShape.InitializeHull(mLoadingData.param0, mLoadingData.param1);
			}
		}
		else
		{
			ASSERT(false, "ColliderComponent: Collider must have shape type");
		}
	
	}
	else
	{
		ASSERT(false, "ColliderComponent: Collider must have shape data");
	}
}