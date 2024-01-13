#pragma once

#include <Inc/WNTRengine.h>

struct ParticleInfo
{
	float lifeTime = 0.0f;
	WNTRengine::Color colorStart = WNTRengine::Colors::White;
	WNTRengine::Color colorEnd = WNTRengine::Colors::White;
	WNTRengine::WNTRmath::Vector3 scaleStart = WNTRengine::WNTRmath::Vector3::One;
	WNTRengine::WNTRmath::Vector3 scaleEnd = WNTRengine::WNTRmath::Vector3::One;
};

class Particle
{
public:
	Particle() = default;

	void Initialize();
	void Terminate();

	void Activate(const ParticleInfo& info,const WNTRengine::WNTRmath::Vector3& pos, const WNTRengine::WNTRmath::Vector3& velocity);
	void Update(float deltaTime);

	template<class Effect>
	void Render(Effect& effect)
	{
		effect.Render(mParticle);
	}

private:
	WNTRengine::Graphics::RenderObject mParticle;
	WNTRengine::Graphics::MeshPC mMesh;
	WNTRengine::Physics::RigidBody mRigidBody;
	WNTRengine::Physics::CollisionShape mShape;

	bool mIsActive = false;
	ParticleInfo mInfo;

	float mLifeTime = 0.0f;
	float mLifeTimeDuration = 0.0f;
};