#include "Precompiled.h"
#include "PhysicsDebugDrawer.h"

#include <Graphics/Inc/Colors.h>
#include <Graphics/Inc/SimpleDraw.h>

using namespace WNTRengine;
using namespace WNTRengine::Physics;
using namespace WNTRengine::Graphics;

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	SimpleDraw::AddLine(from, to, Color(color.getX(), color.getY(), color.getZ(), 1.0f));
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	//Nothing here
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("[PhysicsWarning] %s", warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	//Nothing here
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int  PhysicsDebugDrawer::getDebugMode() const
{
	return mDebugMode;
}