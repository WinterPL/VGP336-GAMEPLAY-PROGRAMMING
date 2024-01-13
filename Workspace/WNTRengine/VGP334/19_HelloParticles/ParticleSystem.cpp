
#include "ParticleSystem.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Physics;
using namespace WNTRengine::WNTRmath;


void ParticleSystem::Initialize(const ParticleSystemInfo& info)
{
	mInfo = info;
	mNextAvailableParticleIndex = 0;
	mNextSpawn = info.spawnDelay;
	mLifeTime = info.lifeTime;
	mParticles.resize(info.maxParticles);
	for (auto& p : mParticles)
	{
		p.Initialize();
	}
}
void ParticleSystem::Terminate()
{
	for (auto& p : mParticles)
	{
		p.Terminate();
	}
}
void ParticleSystem::Update(float deltaTime)
{
	if (mLifeTime > 0.0f)
	{
		mNextSpawn -= deltaTime;
		mLifeTime -= deltaTime;
		if (mNextSpawn <= 0.0f)
		{
			SpawnParticles();
		}
		for (auto& p : mParticles)
		{
			p.Update(deltaTime);
		}
	}
}

void ParticleSystem::DebugUI()
{
	if (ImGui::CollapsingHeader("Particle##", ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::DragFloat3("SpawnPosition", &mInfo.spawnPosition.x);
		if (ImGui::DragFloat3("SpawnDirection", &mInfo.spawnDirection.x))
		{
			mInfo.spawnDirection = WNTRmath::Normalize(mInfo.spawnDirection);
		}
		ImGui::DragInt("MinParticles", &mInfo.minParticlePerEmit);
		ImGui::DragInt("MaxParticles", &mInfo.maxParticlePerEmit,1,mInfo.minParticlePerEmit);
		ImGui::DragFloat("MinTime",&mInfo.minTimeBetweenParticles,0.1f);
		ImGui::DragFloat("MaxTime", &mInfo.maxTimeBetweenParticles, 0.1f, mInfo.minTimeBetweenParticles);
		ImGui::DragFloat("MinAngle", &mInfo.minSpawnAngle, 0.01f);
		ImGui::DragFloat("MaxAngle", &mInfo.maxSpawnAngle, 0.01f, mInfo.minSpawnAngle);
		ImGui::DragFloat("MinSpeed", &mInfo.minSpeed, 0.1f);
		ImGui::DragFloat("MaxSpeed", &mInfo.maxSpeed, 0.1f, mInfo.minSpeed);
		//Particle info
		ImGui::DragFloat("LifeTime", &mInfo.particleInfo.lifeTime);
		ImGui::ColorEdit4("StartColor",&mInfo.particleInfo.colorStart.r);
		ImGui::ColorEdit4("EndColor", &mInfo.particleInfo.colorEnd.r);
		ImGui::DragFloat3("StartScale", &mInfo.particleInfo.scaleStart.x);
		ImGui::DragFloat3("StartEnd", &mInfo.particleInfo.scaleEnd.x);
	}
}


void ParticleSystem::SpawnParticles()
{
	int numParticles = mInfo.minParticlePerEmit + (rand() % (mInfo.maxParticlePerEmit - mInfo.minParticlePerEmit));
	for (int i = 0; i < numParticles; ++i)
	{
		SpawnParticle();
	}

	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	mNextSpawn = mInfo.minTimeBetweenParticles +((mInfo.maxTimeBetweenParticles - mInfo.minTimeBetweenParticles)* randFloat);
}														   
void ParticleSystem::SpawnParticle()
{
	Particle& p = mParticles[mNextAvailableParticleIndex];
	mNextAvailableParticleIndex = (mNextAvailableParticleIndex + 1) % mParticles.size();

	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float randAngle = mInfo.minSpawnAngle + ((mInfo.maxSpawnAngle - mInfo.minSpawnAngle) * randFloat);

	Vector3 rotAxis = (Dot(mInfo.spawnDirection, Vector3::YAxis) >= 0.99f) ? Vector3::XAxis : Vector3::ZAxis;
	rotAxis = Normalize(Cross(rotAxis, mInfo.spawnDirection));
	Matrix4 matRot = Matrix4::RotationAxis(rotAxis, randAngle);
	Vector3 spawnDirection = TransformNormal(mInfo.spawnDirection, matRot);

	randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float speed = mInfo.minSpeed + ((mInfo.minSpeed - mInfo.minSpeed) * randFloat);
	p.Activate(mInfo.particleInfo, mInfo.spawnPosition, spawnDirection * speed);

}