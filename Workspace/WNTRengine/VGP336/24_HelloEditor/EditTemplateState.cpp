#include "EditTemplateState.h"
#include "CustomFactory.h"
using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void EditTemplateState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponents::CustomComponentMake);
    GameWorld::SetCustomServiceMake(CustomComponents::CustomServiceMake);
    mGameWorld.loadLevel("../../Assets/Templates/Levels/test_level.json");

    PhysicsService* ps = mGameWorld.GetService<PhysicsService>();
    if (ps != nullptr)
    {
        ps->SetEnabled(false);
    }
}

void EditTemplateState::Terminate()
{

    mGameWorld.Terminate();
}

void EditTemplateState::Render()
{
    mGameWorld.Render();
}

void EditTemplateState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void EditTemplateState::DebugUI()
{
    ImGui::Begin("EditTemplateState", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.EditorUI();

    if (ImGui::Button("Save##EditTemplate"))
    {
        GameObject* go = mGameWorld.GetGameObject(GameWorld::GetEditObject());
        mGameWorld.SaveTemplate(go->GetTemplatePath(), go->GetHandle());
    }
    ImGui::SameLine();
    if (ImGui::Button("Reload##EditTemplate"))
    {
        MainApp().ChangeState("EditTemplateState");
    }
    ImGui::SameLine();
    if (ImGui::Button("Exit##EditTemplate"))
    {
        GameWorld::SetEditObject("");
        MainApp().ChangeState("EditorState");
    }

    ImGui::End();
}


