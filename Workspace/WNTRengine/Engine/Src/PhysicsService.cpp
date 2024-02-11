#include "Precompiled.h"
#include "PhysicsService.h"

#include "RigidBodyComponent.h"

using namespace WNTRengine;
using namespace WNTRengine::Physics;

void  PhysicsService::Update(float deltaTime)
{
	PhysicsWorld::Get()->Update(deltaTime);
}
void  PhysicsService::DebugUI()
{
	PhysicsWorld::Get()->DebugUI();
}

void  PhysicsService::DeSerialize(rapidjson::Value& value)
{
	if (value.HasMember("Gravity"))
	{
		const auto& gravity = value["Gravity"].GetArray();
		const float x = gravity[0].GetFloat();
		const float y = gravity[1].GetFloat();
		const float z = gravity[2].GetFloat();
		PhysicsWorld::Get()->SetGravity({x,y,z});
	}
	if (value.HasMember("SimStep"))
	{
		const uint32_t simSteps = value["SimStep"].GetInt();
		PhysicsWorld::Get()->SetSimulationSteps(simSteps);
	}
	if (value.HasMember("FixedTimeStep"))
	{
		const float fixedTimeStep = value["FixedTimeStep"].GetFloat();
		PhysicsWorld::Get()->SetFixedTimeStep(fixedTimeStep);
	}
}

void  PhysicsService::Register(RigidBodyComponent& rigidBodyComponent)
{
	PhysicsWorld::Get()->Register(&rigidBodyComponent.mRigidBody);
}
void  PhysicsService::Unregister(RigidBodyComponent& rigidBodyComponent)
{
	PhysicsWorld::Get()->Unregister(&rigidBodyComponent.mRigidBody);
}