#include "EditorState.h"
#include "CustomFactory.h"
using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void EditorState::Initialize()
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

void EditorState::Terminate()
{
    PhysicsService* ps = mGameWorld.GetService<PhysicsService>();
    if (ps != nullptr)
    {
        ps->SetEnabled(true);
    }
    mGameWorld.Terminate();
}

void EditorState::Render()
{
    mGameWorld.Render();
}

void EditorState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void EditorState::DebugUI()
{
    ImGui::Begin("EditorState", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.EditorUI();


    if (ImGui::Button("Save##EditorState"))
    {
        mGameWorld.SaveLevel(mGameWorld.GetLevelFile());
    }
    if (ImGui::Button("Exit##EditorState"))
    {
        MainApp().ChangeState("GameState");
    }

    ImGui::End();
}


