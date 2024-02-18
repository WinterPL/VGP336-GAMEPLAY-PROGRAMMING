#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameObject.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "ModelComponent.h"
#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "SoundEffectComponent.h"
#include "SoundBankComponent.h"
#include "AnimatorComponent.h"

using namespace WNTRengine;

namespace rj = rapidjson;

namespace {
	CustomMake TryMake;
}

void GameObjectFactory::SetCustomMake(CustomMake customMake) 
{
	TryMake = customMake;

}


void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, templatePath.u8string().c_str(), "r");
	ASSERT(err == 0,"GameObjectFactory : failed to open file %s", templatePath.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rj::Document doc;
	doc.ParseStream(readStream);
	auto components = doc["Components"].GetObj();
	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();
		if (TryMake(componentName, component.value, gameObject))
		{
			// it is custom and we handle on the project
		}
		else if (strcmp(componentName, "TrasnformComponent") == 0)
		{
			TransformComponent* transformComponent = gameObject.AddComponent<TransformComponent>();
			transformComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "CameraComponent") == 0)
		{
			CameraComponent* cameraComponent = gameObject.AddComponent<CameraComponent>();
			cameraComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "FPSCameraComponent") == 0)
		{
			FPSCameraComponent* fpsCameraComponent = gameObject.AddComponent<FPSCameraComponent>();
			fpsCameraComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			ModelComponent* modelComponent = gameObject.AddComponent<ModelComponent>();
			modelComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "MeshComponent") == 0)
		{
			MeshComponent* meshComponent = gameObject.AddComponent<MeshComponent>();
			meshComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "RigidBodyComponent") == 0)
		{
			RigidBodyComponent* rigidBodyComponent = gameObject.AddComponent<RigidBodyComponent>();
			rigidBodyComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			ColliderComponent* colliderComponent = gameObject.AddComponent<ColliderComponent>();
			colliderComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "SoundEffectComponent") == 0)
		{
			SoundEffectComponent* soundEffectComponent = gameObject.AddComponent<SoundEffectComponent>();
			soundEffectComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "SoundBankComponent") == 0)
		{
			SoundBankComponent* soundBankComponent = gameObject.AddComponent<SoundBankComponent>();
			soundBankComponent->DeSerialize(component.value);
		}
		else if (strcmp(componentName, "AnimatorComponent") == 0)
		{
			AnimatorComponent* animatorComponent = gameObject.AddComponent<AnimatorComponent>();
			animatorComponent->DeSerialize(component.value);
		}
		else 
		{
			ASSERT(false, "GameObjectFactory: %s was not defined", componentName);
		}
	}

}