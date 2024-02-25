#pragma once

#include <Inc/WNTRengine.h>

using namespace WNTRengine;
using namespace WNTRengine::Graphics;

class EditorState : public AppState
{
public:
	void Initialize();
	void Terminate();
	void Render();
	void Update(float deltaTime);
	void DebugUI();

protected:
	GameWorld mGameWorld;
};

