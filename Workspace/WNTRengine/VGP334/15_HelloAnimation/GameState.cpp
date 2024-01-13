#include "GameState.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 7.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f, 3.0f,0.0f });

    mDirectionalLight.direction = WNTRmath::Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f,1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    Mesh groundMesh = MeshBuilder::CreateGroupPlane(20, 20, (int)1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"misc/concrete.jpg");
    mGround.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mGround.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mGround.material.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    mGround.material.power = 20.f;

    Mesh ball = MeshBuilder::CreateSphere(60, 60, 0.5f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureManager::Get()->LoadTexture(L"misc/basketball.jpg");
    mBall.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mBall.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mBall.material.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    mBall.material.power = 20.f;

    constexpr auto rad = WNTRmath::Constants::DegToRad;
    mAnimationTime = 0.0f;
    mBallAnimation = AnimationBuilder()
        .AddPositionKey(Vector3( 8.0f, 5.0f, 0.0f), 0.f)
        .AddPositionKey(Vector3( 6.0f, 4.0f, 0.0f), 1.f)
        .AddPositionKey(Vector3( 4.0f, 3.0f, 0.0f), 2.f)
        .AddPositionKey(Vector3( 2.0f, 2.0f, 0.0f), 3.f)
        .AddPositionKey(Vector3( 0.0f, 1.0f, 0.0f), 4.f)
        .AddPositionKey(Vector3(-2.0f, 2.0f, 0.0f), 5.f)
        .AddPositionKey(Vector3(-4.0f, 3.0f, 0.0f), 6.f)
        .AddPositionKey(Vector3(-6.0f, 4.0f, 0.0f), 7.f)
        .AddPositionKey(Vector3(-8.0f, 5.0f, 0.0f), 8.f)
        .AddPositionKey(Vector3(-6.0f, 4.0f, 0.0f), 9.f)
        .AddPositionKey(Vector3(-4.0f, 3.0f, 0.0f), 10.f)
        .AddPositionKey(Vector3(-2.0f, 2.0f, 0.0f), 11.f)
        .AddPositionKey(Vector3( 0.0f, 1.0f, 0.0f), 12.f)
        .AddPositionKey(Vector3( 2.0f, 2.0f, 0.0f), 13.f)
        .AddPositionKey(Vector3( 4.0f, 3.0f, 0.0f), 14.f)
        .AddPositionKey(Vector3( 6.0f, 4.0f, 0.0f), 15.f)
        .AddPositionKey(Vector3( 8.0f, 5.0f, 0.0f), 16.f)
        .AddScaleKey(Vector3::One, 0.0f)
        .AddScaleKey(Vector3::One, 3.9f)
        .AddScaleKey(Vector3(1.0f,0.8f,1.0f), 4.0f)
        .AddScaleKey(Vector3::One, 5.0f)
        .AddScaleKey(Vector3::One, 11.9f)
        .AddScaleKey(Vector3(1.0f, 0.8f, 1.0f), 12.0f)
        .AddScaleKey(Vector3::One, 13.0f)
        .AddScaleKey(Vector3::One, 16.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(0 * rad, rad, rad), 0.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(90 * rad,rad, rad), 2.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(180 * rad,rad, rad), 4.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(270 * rad, rad, rad), 6.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(360 * rad, rad, rad), 8.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(270 * rad, rad, rad), 10.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(180 * rad, rad, rad), 12.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll(90 * rad, rad, rad), 14.0f)
        .AddRotationKey(Quaternion::CreateFromYawPitchRoll( 0 * rad, rad, rad), 16.0f)
        .AddScaleKey(Vector3::One, 13.0f)
        .Build();
}

void GameState::Terminate()
{
    mBall.Terminate();
    mGround.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Render()
{
    mStandardEffect.Begin();
       mStandardEffect.Render(mBall);
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

    mAnimationTime += deltaTime;
    const float animDuration = mBallAnimation.GetDuration();
    while (mAnimationTime > animDuration)
    {
        mAnimationTime -= animDuration;
    }
    mBall.transform = mBallAnimation.GetTransform(mAnimationTime);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        /*if (ImGui::CollapsingHeader("Lighting##", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Directional##Light", &mDirectionalLight.direction.x, 0.01f, -0.01f, 1.0f)){
                mDirectionalLight.direction = WNTRmath::Normalize(mDirectionalLight.direction);
            }
            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }*/
        bool updateCharacter = false;
        if (ImGui::CollapsingHeader("Position##", ImGuiTreeNodeFlags_DefaultOpen))
        {
         
        }
        //mStandardEffect.DebugUI();
    ImGui::End();
    
}


