#include "Precompiled.h"
#include "MeshComponent.h"

#include "GameWorld.h"
#include "RenderService.h"

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
void MeshComponent::DeSerialize(rapidjson::Value& value)
{
	Model::MeshData& meshData = mModel.meshData.emplace_back();
	Model::MaterialData& material = mModel.materialData.emplace_back();
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			const std::string shapeType = shapeData["Type"].GetString();
			if (shapeType == "Cube")
			{
				//const float size = shapeData["Size"].GetFloat();
				//meshData.mesh = MeshBuilder::CreateCubePX(size);
			}
			else if (shapeType == "Sphere")
			{
				const float slices = shapeData["Slices"].GetFloat();
				const float rings = shapeData["Rings"].GetInt();
				const float radius = shapeData["Rings"].GetFloat();
				meshData.mesh = MeshBuilder::CreateSphere(slices,rings,radius);
			}
			else if (shapeType == "Plane")
			{
				const int rows = shapeData["Rows"].GetInt();
				const int columns = shapeData["Columns"].GetInt();
				const float spacing = shapeData["Spacing"].GetFloat();
				meshData.mesh = MeshBuilder::CreateGroupPlane(rows,columns,spacing);
			}
			else
			{
				ASSERT(false, "Shape: %s Shape is invalid", shapeType);
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
}