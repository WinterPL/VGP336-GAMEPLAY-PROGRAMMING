#include "GameState.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 2.0f,-4.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f,0.0f });

    mDirectionalLight.direction = WNTRmath::Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f,1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mCharacterId = ModelManager::Get()->LoadModel("../../Assets/Models/Character/Character.model");
    ModelManager::Get()->AddAnimation(mCharacterId,"../../Assets/Models/Character/Running.animset");
    ModelManager::Get()->AddAnimation(mCharacterId,"../../Assets/Models/Character/walking.animset");
    ModelManager::Get()->AddAnimation(mCharacterId,"../../Assets/Models/Character/walkback.animset");
    ModelManager::Get()->AddAnimation(mCharacterId,"../../Assets/Models/Character/ForwardFlip.animset");
    mCharacter = CreateRenderGroup(mCharacterId, &mCharacterAnimator);
    mCharacterAnimator.Initialize(mCharacterId);

    Mesh groundMesh = MeshBuilder::CreateGroupPlane(20, 20, (int)1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"misc/concrete.jpg");
    mGround.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mGround.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mGround.material.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    mGround.material.power = 20.f;
}

void GameState::Terminate()
{
    CleanupRenderGroup(mCharacter);
    mGround.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Render()
{
    mStandardEffect.Begin();
    if (mDrawSkeleton)
    {
        AnimationUtil::BoneTransforms boneTransform;
        AnimationUtil::ComputeBoneTransform(mCharacterId, boneTransform, &mCharacterAnimator);
        AnimationUtil::DrawSkeleton(mCharacterId, boneTransform);
    }
    else
    {
        DrawRenderGroup(mStandardEffect, mCharacter);
    }
        mStandardEffect.Render(mGround);
    mStandardEffect.End();
}

void GameState::Update(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 0.1f);
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W)) {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S)) {
        mCamera.Walk(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::D)) {
        mCamera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A)) {
        mCamera.Strafe(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::E)) {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q)) {
        mCamera.Rise(-moveSpeed);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON)) {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }

    mCharacterAnimator.update(deltaTime);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Lighting##", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Directional##Light", &mDirectionalLight.direction.x, 0.01f, -0.01f, 1.0f)){
                mDirectionalLight.direction = WNTRmath::Normalize(mDirectionalLight.direction);
            }
            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }
        ImGui::Checkbox("DrawSkeleton##", &mDrawSkeleton);

        if (ImGui::Checkbox("Loop", &mLooping)||ImGui::Checkbox("Blending", &mLoopAll)|| ImGui::DragInt("PlayAnimation##", &mAnimationIndex, 1, -1, mCharacterAnimator.GetAnimationCount() - 1))
        {

            mCharacterAnimator.PlayAnimation(mAnimationIndex, mLooping, mLoopAll);
        }
       
        mStandardEffect.DebugUI();
        SimpleDraw::Render(mCamera);
    ImGui::End();
}


