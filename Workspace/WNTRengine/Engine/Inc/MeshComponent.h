#pragma once

#include "Component.h"

namespace WNTRengine
{
	class MeshComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Mesh);

		void Initialize() override;
		void Terminate() override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void DeSerialize(const rapidjson::Value& value) override;
		
		void EditorUI() override;

		bool CastShadow() const { return mCastShadow; }
		const Graphics::Model& GetModel() const { return mModel; }

	private:
		Graphics::Model mModel;
		bool mCastShadow = true;

		struct LoadingData
		{
			std::string shapeType;
			float fParam;
			int param0;
			int param1;
		};
		LoadingData mLoadingData;
	};
}