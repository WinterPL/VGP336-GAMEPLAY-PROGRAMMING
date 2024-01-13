#include "Particle.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::Physics;
using namespace WNTRengine::WNTRmath;

void Particle::Initialize()
{
	mIsActive = false;
	mLifeTime = 0.0f;
	mLifeTimeDuration = 0.0f;

	mMesh = MeshBuilder::CreateSpherePC(10, 10, 0.5);
	mParticle.meshBuffer.Initialize(nullptr, sizeof(VertexPC), mMesh.vertices.size(),
		mMesh.indices.data(), mMesh.indices.size());

	mShape.InitializeEmpty();
	mRigidBody.Initialize(mParticle.transform, mShape, 0.1f);
}

void Particle::Terminate()
{
	mRigidBody.Terminate();
	mShape.Terminate();
	mParticle.Terminate();
}

void Particle::Activate(const ParticleInfo& info, const WNTRengine::WNTRmath::Vector3& pos, const WNTRengine::WNTRmath::Vector3& velocity)
{
	mLifeTime = 0.0f;
	mLifeTimeDuration = info.lifeTime;
	mIsActive = true;
	mInfo = info;
	mRigidBody.SetPosition(pos);
	mRigidBody.SetVelocity(velocity);
}
void Particle::Update(float deltaTime)
{
	if (mIsActive)
	{
		mLifeTime += deltaTime;
		float t = Clamp(mLifeTime / mLifeTimeDuration, 0.0f, 1.0f);

		Color currentColor = Lerp(mInfo.colorStart, mInfo.colorEnd, t);
		mParticle.transform.scale = Lerp(mInfo.scaleStart, mInfo.scaleEnd, t);
		for (uint32_t i = 0; i < mMesh.vertices.size(); ++i)
		{
			mMesh.vertices[i].color = currentColor;
		}
		mParticle.meshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());


	}
}