#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureManager.h"
#include "Material.h"
#include "ModelManager.h"
#include "Animator.h"
#include "Skeleton.h"

namespace WNTRengine::Graphics {

	struct Model;

	class RenderObject 
	{
	public:
		void Terminate();
		Transform transform;
		Material material;
		
		TextureId diffuseMapId;
		TextureId normalMapId;
		TextureId bumpMapId;
		TextureId specMapId;

		ModelId modelId = 0;

		MeshBuffer meshBuffer;

		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(ModelId modelId, const Animator* animator = nullptr);
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, const Animator* animator = nullptr);
	void CleanupRenderGroup(RenderGroup& renderGroup);

	template<class Effect>
	void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
	{
		for (auto& renderObject : renderGroup)
		{
			effect.Render(renderObject);
		}
	}

}