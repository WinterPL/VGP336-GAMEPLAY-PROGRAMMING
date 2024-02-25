#include <../Engine/Inc/WNTRengine.h>
#include "GameState.h"
#include "EditorState.h"
#include "EditTemplateState.h"

using namespace WNTRengine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"HelloEditor";

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.AddState<GameState>("EditorState");
	myApp.AddState<GameState>("EditTemplateState");
	myApp.Run(config);

	return (0);
}