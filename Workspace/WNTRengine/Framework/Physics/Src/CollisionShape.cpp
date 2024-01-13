#include "Precompiled.h"
#include "CollisionShape.h"

using namespace WNTRengine;
using namespace WNTRengine::Physics;
CollisionShape::~CollisionShape()
{
	ASSERT(mCollisionShape == nullptr, "CollisoionShape: must call terminate first");
}

void CollisionShape::InitializeSphere(float radius)
{
	ASSERT(mCollisionShape == nullptr, "CollisoionShape: Already Initialized");
	mCollisionShape = new btSphereShape(radius);
}
void CollisionShape::InitializeBox(const WNTRengine::WNTRmath::Vector3& halfExtents)
{
	ASSERT(mCollisionShape == nullptr, "CollisoionShape: Already Initialized");
	mCollisionShape = new btBoxShape(halfExtents);
}
void CollisionShape::InitializeHull(const WNTRengine::WNTRmath::Vector3& halfExtents, const WNTRengine::WNTRmath::Vector3& origin)
{
	ASSERT(mCollisionShape == nullptr, "CollisoionShape: Already Initialized");
	btConvexHullShape* hullShape = new btConvexHullShape();
	std::vector<WNTRengine::WNTRmath::Vector3> points =
	{
		{-halfExtents.x, -halfExtents.y,-halfExtents.z},
		{-halfExtents.x,  halfExtents.y,-halfExtents.z},
		{ halfExtents.x,  halfExtents.y,-halfExtents.z},
		{ halfExtents.x, -halfExtents.y,-halfExtents.z},
		{-halfExtents.x, -halfExtents.y, halfExtents.z},
		{-halfExtents.x,  halfExtents.y, halfExtents.z},
		{ halfExtents.x,  halfExtents.y, halfExtents.z},
		{ halfExtents.x, -halfExtents.y, halfExtents.z},
	};
	for (auto& point : points)
	{
		hullShape->addPoint(point + origin, false);
	}
	hullShape->recalcLocalAabb();
	mCollisionShape = hullShape;

}

void CollisionShape::InitializeEmpty()
{
	ASSERT(mCollisionShape == nullptr, "CollisoionShape: Already Initialized");
	mCollisionShape = new btEmptyShape();
}


void CollisionShape::Terminate()
{
	SafeDelete(mCollisionShape);
}