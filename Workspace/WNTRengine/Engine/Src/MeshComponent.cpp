#include "Precompiled.h"
#include "MeshComponent.h"

#include "GameWorld.h"
#include "RenderService.h"
#include "SaveUtil.h"

using namespace WNTRengine;
using namespace WNTRengine::Graphics;

void MeshComponent::Initialize()
{
	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Register(this);
}
void MeshComponent::Terminate()
{
	RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
	rs->Unregister(this);
}

void MeshComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	rapidjson::Value shapeValue(rapidjson::kObjectType);
	rapidjson::Value materialValue(rapidjson::kObjectType);
	rapidjson::Value texturesValue(rapidjson::kObjectType);
	SaveUtil::SaveString("Type", mLoadingData.shapeType.c_str(), doc, shapeValue);
	if (mLoadingData.shapeType == "Cube")
	{
		SaveUtil::SaveFloat("size", mLoadingData.fParam, doc, shapeValue);
	}
	else if (mLoadingData.shapeType == "Sphere")
	{
		SaveUtil::SaveInt("Slices", mLoadingData.param0, doc, shapeValue);
		SaveUtil::SaveInt("Rings", mLoadingData.param1, doc, shapeValue);
		SaveUtil::SaveFloat("Radius", mLoadingData.fParam, doc, shapeValue);
	}
	else if (mLoadingData.shapeType == "Plane")
	{
		SaveUtil::SaveInt("Rows", mLoadingData.param0, doc, shapeValue);
		SaveUtil::SaveInt("Columns", mLoadingData.param1, doc, shapeValue);
		SaveUtil::SaveFloat("Spacing", mLoadingData.fParam, doc, shapeValue);
	}
	Model::MaterialData& material = mModel.materialData[0];
	SaveUtil::SaveColor("ColorAmbient", material.material.ambient, doc, materialValue);
	SaveUtil::SaveColor("ColorDiffuse", material.material.diffuse, doc, materialValue);
	SaveUtil::SaveColor("ColorSpecular", material.material.specular, doc, materialValue);
	SaveUtil::SaveColor("ColorEmissive", material.material.emissive, doc, materialValue);
	SaveUtil::SaveFloat("SpecularPower", material.material.power, doc, materialValue);

	if (!material.diffuseMapName.empty())
	{
		SaveUtil::SaveString("DiffuseMap", material.diffuseMapName.c_str(), doc, texturesValue);
	}
	if (!material.normalMapName.empty())
	{
		SaveUtil::SaveString("NormalMap", material.normalMapName.c_str(), doc, texturesValue);
	}
	if (!material.bumpMapName.empty())
	{
		SaveUtil::SaveString("BumpMap", material.bumpMapName.c_str(), doc, texturesValue);
	}
	if (!material.specularMapName.empty())
	{
		SaveUtil::SaveString("SpecularMap", material.specularMapName.c_str(), doc, texturesValue);
	}


	componentValue.AddMember("Shape", shapeValue, doc.GetAllocator());
	componentValue.AddMember("Material", materialValue, doc.GetAllocator());
	componentValue.AddMember("Textures", texturesValue, doc.GetAllocator());
	SaveUtil::SaveBool("CastShadow", mCastShadow, doc, componentValue);

	value.AddMember("MeshComponent", componentValue, doc.GetAllocator());
}

void MeshComponent::DeSerialize(const rapidjson::Value& value)
{
	Model::MeshData& meshData = mModel.meshData.emplace_back();
	Model::MaterialData& material = mModel.materialData.emplace_back();
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			mLoadingData.shapeType = shapeData["Type"].GetString();
			if (mLoadingData.shapeType == "Cube")
			{
				//const float size = shapeData["Size"].GetFloat();
				//meshData.mesh = MeshBuilder::CreateCubePX(size);
			}
			else if (mLoadingData.shapeType == "Sphere")
			{
				mLoadingData.param0 = shapeData["Slices"].GetInt();
				mLoadingData.param1 = shapeData["Rings"].GetInt();
				mLoadingData.fParam = shapeData["Radius"].GetFloat();
				meshData.mesh = MeshBuilder::CreateSphere(mLoadingData.param0, mLoadingData.param1, mLoadingData.fParam);
			}
			else if (mLoadingData.shapeType == "Plane")
			{
				mLoadingData.param0 = shapeData["Rows"].GetInt();
				mLoadingData.param1 = shapeData["Columns"].GetInt();
				mLoadingData.fParam = shapeData["Spacing"].GetFloat();
				meshData.mesh = MeshBuilder::CreateGroupPlane(mLoadingData.param0, mLoadingData.param1, mLoadingData.fParam);
			}
			else
			{
				ASSERT(false, "Shape: %s Shape is invalid", mLoadingData.shapeType);
			}
		}
	}

	if (value.HasMember("Material"))
	{
		const auto& materialData = value["Material"].GetObj();
		if (materialData.HasMember("ColorAmbient"))
		{
			const auto& color = materialData["ColorAmbient"].GetArray();
			material.material.ambient.r = color[0].GetFloat();
			material.material.ambient.g = color[1].GetFloat();
			material.material.ambient.b = color[2].GetFloat();
			material.material.ambient.a = color[3].GetFloat();
		}
		if (materialData.HasMember("ColorDiffuse"))
		{
			const auto& color = materialData["ColorDiffuse"].GetArray();
			material.material.diffuse.r = color[0].GetFloat();
			material.material.diffuse.g = color[1].GetFloat();
			material.material.diffuse.b = color[2].GetFloat();
			material.material.diffuse.a = color[3].GetFloat();
		}
		if (materialData.HasMember("ColorSpecular"))
		{
			const auto& color = materialData["ColorSpecular"].GetArray();
			material.material.specular.r = color[0].GetFloat();
			material.material.specular.g = color[1].GetFloat();
			material.material.specular.b = color[2].GetFloat();
			material.material.specular.a = color[3].GetFloat();
		}
		if (materialData.HasMember("ColorEmissive"))
		{
			const auto& color = materialData["ColorEmissive"].GetArray();
			material.material.emissive.r = color[0].GetFloat();
			material.material.emissive.g = color[1].GetFloat();
			material.material.emissive.b = color[2].GetFloat();
			material.material.emissive.a = color[3].GetFloat();
		}
		if (materialData.HasMember("SpecularPower"))
		{
			material.material.power = materialData["SpecularPower"].GetFloat();
		}
	}

	if (value.HasMember("Textures"))
	{
		const auto& textureData = value["Textures"].GetObj();
		if (textureData.HasMember("DiffuseMap"))
		{
			material.diffuseMapName = textureData["DiffuseMap"].GetString();
		}
		if (textureData.HasMember("NormalMap"))
		{
			material.normalMapName = textureData["NormalMap"].GetString();
		}
		if (textureData.HasMember("BumpMap"))
		{
			material.bumpMapName = textureData["BumpMap"].GetString();
		}
		if (textureData.HasMember("SpecularMap"))
		{
			material.specularMapName = textureData["SpecularMap"].GetString();
		}
	}
	if (value.HasMember("CastShadow"))
	{
		mCastShadow = value["CastShadow"].GetBool();
	}
}

void MeshComponent::EditorUI()
{
	std::string headerTag = "MeshComponent##" + GetOwner().GetName();
	if (ImGui::CollapsingHeader(headerTag.c_str()))
	{
		Material& material = mModel.materialData[0].material;
		ImGui::ColorEdit4("Ambient", &material.ambient.r);
		ImGui::ColorEdit4("Diffuse", &material.diffuse.r);
		ImGui::ColorEdit4("Specular", &material.specular.r);
		ImGui::ColorEdit4("Emissive", &material.emissive.r);
		ImGui::DragFloat4("Power", &material.power);
		ImGui::Checkbox("CastShadow", &mCastShadow);
		if (ImGui::Button("Update"))
		{
			RenderService* rs = GetOwner().GetWorld().GetService<RenderService>();
			rs->Unregister(this);
			rs->Register(this);
		}
	}
}

