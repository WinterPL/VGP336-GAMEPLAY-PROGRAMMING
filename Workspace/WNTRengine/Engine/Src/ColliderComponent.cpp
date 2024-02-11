#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameWorld.h"

using namespace WNTRengine;

void ColliderComponent::Initialize()
{
	//
}
void ColliderComponent::Terminate()
{
	mCollisionShape.Terminate();
}


void ColliderComponent::DeSerialize(rapidjson::Value& value)
{
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			const std::string shapeType = shapeData["Type"].GetString();
			if (shapeType == "Empty")
			{
				mCollisionShape.InitializeEmpty();
			}
			else if (shapeType == "Box")
			{
				const auto& he= shapeData["HalfExtents"].GetArray();
				const float x = he[0].GetFloat();
				const float y = he[1].GetFloat();
				const float z = he[2].GetFloat();
				mCollisionShape.InitializeBox({ x,y,z });
			}
			else if (shapeType == "sphere")
			{
				const float radius = shapeData["Radius"].GetFloat();
				mCollisionShape.InitializeSphere(radius);
			}
			else if (shapeType == "Hull")
			{
				const auto& he = shapeData["HalfExtents"].GetArray();
				const float heX = he[0].GetFloat();
				const float heY = he[1].GetFloat();
				const float heZ = he[2].GetFloat();
				const auto& o = shapeData["Origin"].GetArray();
				const float oX = o[0].GetFloat();
				const float oY = o[1].GetFloat();
				const float oZ = o[2].GetFloat();
				mCollisionShape.InitializeHull({ heX,heY,heZ }, { oX,oY,oZ });
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