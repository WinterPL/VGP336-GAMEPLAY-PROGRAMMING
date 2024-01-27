#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "TypeIds.h"
#include "GameWorld.h"
#include "GameObjectHandle.h"

//Components
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

//Service
#include "Service.h"
#include "CameraService.h"

namespace WNTRengine
{
	App& MainApp();
}