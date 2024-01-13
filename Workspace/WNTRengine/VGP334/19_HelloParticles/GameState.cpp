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

    mGroundShape.InitializeHull({ 10.0f, 0.5f, 10.0f }, {0.0f,-0.5f,0.0f});
    mGroundRigidBody.Initialize(mGround.transform, mGroundShape, 0.0f);

    //Particle
    mParticleEffect.Initialize();
    mParticleEffect.SetCamera(mCamera);
    //ParticleSystem
    ParticleSystemInfo info;
    info.lifeTime = FLT_MAX;
    info.maxParticles = 100;
    info.spawnPosition = {0.0f,1.0f,0.0f};
    info.spawnDirection = {0.0f,1.0f,0.0f};
    info.spawnDelay = 0.0f;
    info.minParticlePerEmit = 1;
    info.maxParticlePerEmit = 3;
    info.minTimeBetweenParticles = 0.1f;
    info.maxTimeBetweenParticles = 0.3f;
    info.minSpawnAngle = WNTRmath::Constants::HalfPi * 0.0f;
    info.maxSpawnAngle = WNTRmath::Constants::HalfPi * 4.0f;
    info.minSpeed = 1.0f;
    info.maxSpeed = 3.0f;
    //Particle info
    info.particleInfo.lifeTime = 2.0f;
    info.particleInfo.colorStart = WNTRengine::Colors::OrangeRed;
    info.particleInfo.colorEnd = WNTRengine::Colors::LightYellow;
    info.particleInfo.colorEnd.a = 0.1f;
    info.particleInfo.scaleStart = WNTRengine::WNTRmath::Vector3::One;
    info.particleInfo.scaleEnd = {0.1f,0.1f,0.1f};
    mParticleSystem.Initialize(info);
}

void GameState::Terminate()
{
    mParticleSystem.Terminate();
    mParticleEffect.Terminate();
    mGroundRigidBody.Terminate();
    mGroundRigidBody.Terminate();
    mGround.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Render()
{
    mStandardEffect.Begin();
       mStandardEffect.Render(mGround);
    mStandardEffect.End();

    mParticleEffect.Begin();
        mParticleSystem.Render(mParticleEffect);
    mParticleEffect.End();


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

    mParticleSystem.Update(deltaTime);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mParticleSystem.DebugUI();
        mStandardEffect.DebugUI();
        PhysicsWorld::Get()->DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCamera);
}


