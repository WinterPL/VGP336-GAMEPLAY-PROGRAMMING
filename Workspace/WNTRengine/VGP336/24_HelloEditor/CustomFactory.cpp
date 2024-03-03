#include "CustomFactory.h"
#include "TypeIds.h"


bool CustomComponents::CustomComponentMake(const char* componentName, const rapidjson::Value& value, WNTRengine::GameObject& gameObject)
{
    if (strcmp(componentName, "NewComponent") == 0)
    {
        //NewComponent* newComponent = gameObject.AddComponent<NewComponent>();
        //newComponent->Deserialize(value);

        return true;
    }
    return false;
}

bool CustomComponents::CustomServiceMake(const char* componentName, const rapidjson::Value& value, WNTRengine::GameWorld& gameWorld)
{
        if (strcmp(componentName, "NewService") == 0)
        {
            //NewComponent* newComponent = gameObject.AddComponent<NewComponent>();
            //newComponent->Deserialize(value);

            return true;
        }
        return false;
 }