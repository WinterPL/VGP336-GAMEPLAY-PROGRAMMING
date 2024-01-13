#pragma once

#include "Inc/AppState.h"
#include "Inc/WNTRengine.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Physics;

class GameState : public AppState
{
public:
	void Initialize();
	void Terminate();
	void Render();
	void Update(float deltaTime);
	void DebugUI();

protected:

	Camera mCamera;
	DirectionalLight mDirectionalLight;
	StandardEffect mStandardEffect;

	RenderObject mBall;
	CollisionShape mBallShape;
	RigidBody mBallRigidBody;

	RenderObject mGround;
	CollisionShape mGroundShape;
	RigidBody mGroundRigidBody;
};


