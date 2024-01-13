#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace WNTRengine;
using namespace WNTRengine::Physics;

namespace
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
	ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initialized");
	sPhysicsWorld = std::make_unique<PhysicsWorld>();
	sPhysicsWorld->Initialize(settings);
}
void PhysicsWorld::Staticterminate()
{
	if (sPhysicsWorld != nullptr)
	{
		sPhysicsWorld->Terminate();
		sPhysicsWorld.reset();
	}
}
PhysicsWorld* PhysicsWorld::Get()
{
	ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: is not initialized");
	return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mDynamicWorld == nullptr, "PhysicsWorld: Terminate must be called");

}

void PhysicsWorld::Initialize(const Settings& settings)
{
	mSettings = settings;
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mInterface = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
	
	mDynamicWorld = new btDiscreteDynamicsWorld(mDispatcher,mInterface,mSolver,mCollisionConfiguration);
	mDynamicWorld->setGravity(settings.gravity);
	mDynamicWorld->setDebugDrawer(&mDebugDrawer);

	mSoftBodyWorld = new btSoftRigidDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
	mSoftBodyWorld->setGravity(settings.gravity);
	mSoftBodyWorld->setDebugDrawer(&mDebugDrawer);

}
void PhysicsWorld::Terminate()
{
	SafeDelete(mDynamicWorld);
	SafeDelete(mSoftBodyWorld);
	SafeDelete(mSolver);
	SafeDelete(mInterface);
	SafeDelete(mDispatcher);
	SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
	mDynamicWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
	mSoftBodyWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);

	for (auto po : mPhysicsObjects)
	{
		po->Update();
	}
}

void PhysicsWorld::DebugUI()
{
	ImGui::Checkbox("RenderPhysics", &mRenderDebugUI);
	if (mRenderDebugUI)
	{
		int debugMode = mDebugDrawer.getDebugMode();
		bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe) > 0;
		if (ImGui::Checkbox("[DBG]DrawWireframe", &isEnabled))
		{
			debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
		}
		isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb) > 0;
		if (ImGui::Checkbox("[DBG]DrawAabb", &isEnabled))
		{
			debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
		}

		mDebugDrawer.setDebugMode(debugMode);
		mDynamicWorld->debugDrawWorld();
		mSoftBodyWorld->debugDrawWorld();
	}
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
	if (std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject) == mPhysicsObjects.end())
	{
		mPhysicsObjects.push_back(physicsObject);
		if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicWorld->addRigidBody(physicsObject->GetRigidBody());
		}
		if (physicsObject->GetSoftBody() != nullptr)
		{
			mSoftBodyWorld->addSoftBody(physicsObject->GetSoftBody());
		}
	}
}
void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter != mPhysicsObjects.end())
	{
		if (physicsObject->GetSoftBody() != nullptr)
		{
			mSoftBodyWorld->removeSoftBody(physicsObject->GetSoftBody());
		}
		if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicWorld->removeRigidBody(physicsObject->GetRigidBody());
		}
		mPhysicsObjects.erase(iter);
	}
}


btSoftBody* PhysicsWorld::CreateSoftBody(int nodeCount)
{
	btSoftBody* softBody = new btSoftBody(&mSoftBodyWorld->getWorldInfo(), nodeCount, nullptr, nullptr);
	return softBody;
}