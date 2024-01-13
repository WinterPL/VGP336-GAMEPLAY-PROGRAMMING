#pragma once

#include "Inc/AppState.h"
#include "Inc/WNTRengine.h"

#include "ParticleSystem.h"

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

	RenderObject mGround;
	CollisionShape mGroundShape;
	RigidBody mGroundRigidBody;

	ParticleEffect mParticleEffect;
	ParticleSystem mParticleSystem;
};


