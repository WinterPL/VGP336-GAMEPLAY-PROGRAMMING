#pragma once

#include <Inc/WNTRengine.h>

using namespace WNTRengine;
using namespace WNTRengine::Graphics;

class GameState : public AppState
{
public:
	void Initialize();
	void Terminate();
	void Render();
	void Update(float deltaTime);
	void DebugUI();

protected:
	GameObject mGameObject;
	GameObject mCameraGameObject1;
	GameObject mCameraGameObject2;
	CameraService mCameraService;
};


