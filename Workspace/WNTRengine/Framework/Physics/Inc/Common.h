#pragma once

//framwork headers
#include <Math/Inc/WNTRmath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Transform.h>
#include <Graphics/Inc/Colors.h>

//bullet files
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <Bullet/BulletSoftBody/btSoftBodyHelpers.h>
#include <Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <Bullet/BulletSoftBody/btSoftBodySolvers.h>


// helper functions
template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

inline btTransform ConvertTobtTransform(const WNTRengine::Graphics::Transform& trans)
{
	return btTransform(trans.rotation, trans.position);
}