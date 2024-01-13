#pragma once

#include "Particle.h"

struct ParticleSystemInfo
{
	ParticleInfo particleInfo;
	WNTRengine::WNTRmath::Vector3 spawnPosition;
	WNTRengine::WNTRmath::Vector3 spawnDirection;
	float spawnDelay;
	int minParticlePerEmit = 0;
	int maxParticlePerEmit = 0;
	float minTimeBetweenParticles = 0.0f;
	float maxTimeBetweenParticles = 0.0f;
	float minSpawnAngle = 0.0f;
	float maxSpawnAngle = 0.0f;
	float minSpeed = 0.0f;
	float maxSpeed = 0.0f;
	float lifeTime = 0.0f;
	int maxParticles = 100;
};


class ParticleSystem
{
public:
	ParticleSystem() = default;
	~ParticleSystem() = default;


	void Initialize(const ParticleSystemInfo& info);
	void Terminate();
	void Update(float deltaTime);

	void DebugUI();

	template<class Effect>
	void Render(Effect& effect)
	{
		if (mLifeTime > 0)
		{
			for(auto& particle:mParticles)
			{ 
				particle.Render(effect);
			}
		}
	}

private:
	void SpawnParticles();
	void SpawnParticle();

	using Particles = std::vector<Particle>;
	Particles mParticles;

	int mNextAvailableParticleIndex;
	float mNextSpawn = 0.0f;
	float mLifeTime = 0.0f;

	ParticleSystemInfo mInfo;
};