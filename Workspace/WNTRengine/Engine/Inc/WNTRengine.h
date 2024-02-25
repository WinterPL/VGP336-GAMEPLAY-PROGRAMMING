#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "TypeIds.h"
#include "GameWorld.h"
#include "GameObjectHandle.h"

#include "SaveUtil.h"
//Components
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "ModelComponent.h"
#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "SoundEffectComponent.h"
#include "SoundBankComponent.h"

//Service
#include "Service.h"
#include "CameraService.h"
#include "UpdateService.h"
#include "RenderService.h"



namespace WNTRengine
{
	App& MainApp();
}