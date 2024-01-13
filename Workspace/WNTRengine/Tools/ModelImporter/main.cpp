#include <../Engine/Inc/WNTRengine.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>

using namespace WNTRengine;
using namespace WNTRengine::Graphics;
using namespace WNTRengine::WNTRmath;

using BoneIndexLookup = std::map<std::string, uint32_t>;

struct Arguments
{
	std::filesystem::path inputFileName;
	std::filesystem::path outputFileName;
	float scale = 1.0f;
	bool animOnly = false;
};

std::optional<Arguments> ParseArgs(int  argc, char* argv[])
{
	if (argc < 3)
	{
		return std::nullopt;
	}
	Arguments arguments;
	arguments.inputFileName = argv[argc - 2];
	arguments.outputFileName = argv[argc - 1];
	for (int i = 1; i + 2 < argc; ++i)
	{
		if (strcmp(argv[i], "-scale") == 0)
		{
			arguments.scale = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "-animOnly") == 0)
		{
			arguments.animOnly = atoi(argv[i + 1]) == 1;
			++i;
		}
	}

	return arguments;
}

Vector3 ToVector3(const aiVector3D& v)
{
	return{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}

Quaternion ToQuaternion(const aiQuaternion& q)
{
	return{
		static_cast<float>(q.x),
		static_cast<float>(q.y),
		static_cast<float>(q.z),
		static_cast<float>(q.w)
	};
}
Color ToColor(const aiColor3D& c) 
{
	return{
		static_cast<float>(c.r),
		static_cast<float>(c.g),
		static_cast<float>(c.b),
		1.0f
	};
}

Matrix4 ToMatrix4(const aiMatrix4x4& m)
{
	return{
		static_cast<float>(m.a1), static_cast<float>(m.b1),static_cast<float>(m.c1),static_cast<float>(m.d1),
		static_cast<float>(m.a2), static_cast<float>(m.b2),static_cast<float>(m.c2),static_cast<float>(m.d2),
		static_cast<float>(m.a3), static_cast<float>(m.b3),static_cast<float>(m.c3),static_cast<float>(m.d3),
		static_cast<float>(m.a4), static_cast<float>(m.b4),static_cast<float>(m.c4),static_cast<float>(m.d4)
	};
}

void ExportEmbeddedTexture(const aiTexture* texture, const Arguments& args, const std::filesystem::path fileName)
{
	printf("Extracting embedded texture %s\n", fileName.u8string().c_str());

	std::string fullFileName = args.outputFileName.u8string();
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName.filename().u8string();

	FILE* file = nullptr;
	auto err = fopen_s(&file, fullFileName.c_str(), "wb");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving\n", fullFileName.c_str());
		return;
	}

	size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
	ASSERT(written == texture->mWidth, "Error: failed to extract embedded texture!");
	fclose(file);
}


std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial, aiTextureType textureType,
	const Arguments& args, const std::string& suffix, uint32_t materialIndex)
{
	const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
	if (textureCount == 0)
	{
		return  "";
	}

	std::filesystem::path textureName;
	aiString texturePath;
	if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
	{
		if (texturePath.C_Str()[0] == '*')
		{
			std::string fileName = args.inputFileName.u8string();
			fileName.erase(fileName.length() - 4);
			fileName += suffix;
			fileName += texturePath.C_Str()[1];

			ASSERT(scene->HasTextures(),"Error: no embedded texture found!");

			int textureIndex = atoi(texturePath.C_Str() + 1);
			ASSERT(textureIndex < scene->mNumTextures, "Error: Invalid texture index!");

			const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
			ASSERT(embeddedTexture->mHeight == 0, "Error: Uncompressed texture found!");

			if (embeddedTexture->CheckFormat("jpg"))
			{
				fileName += ".jpg";
			}
			else if (embeddedTexture->CheckFormat("png"))
			{
				fileName += ".png";
			}
			else 
			{
				ASSERT(false, "Error: Unrecognized texture format!");
			}
			ExportEmbeddedTexture(embeddedTexture, args, fileName);
			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
		else if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
		{
			std::filesystem::path embeddedFilePath = texturePath.C_Str();
			std::string fileName = args.inputFileName.u8string();
			fileName.erase(fileName.length() - 4);
			fileName += suffix;
			fileName += "_" + std::to_string(materialIndex);
			fileName += embeddedFilePath.extension().u8string();

			ExportEmbeddedTexture(embeddedTexture, args, fileName);
			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
		else
		{
			std::filesystem::path filePath = texturePath.C_Str();
			std::string fileName = args.inputFileName.u8string();

			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
	}

	return textureName.filename().u8string();
}

Bone* BuildSkeleton(const aiNode& sceneNode, Bone* parent, Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	Bone* bone = nullptr;
	std::string boneName = sceneNode.mName.C_Str();
	auto iter = boneIndexLookup.find(boneName);
	if (iter != boneIndexLookup.end())
	{
		bone = skeleton.bones[iter->second].get();
	}
	else 
	{
		bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = Matrix4::Identity;
		if (boneName.empty())
		{
			bone->name = "NoName" + std::to_string(bone->index);
		}
		else
		{
			bone->name = std::move(boneName);
		}
		boneIndexLookup.emplace(bone->name, bone->index);
	}

	if (skeleton.root == nullptr && parent == nullptr)
	{
		skeleton.root = bone;
	}

	bone->parent = parent;
	bone->parentIndex = parent ? parent->index : -1;
	bone->toParentTransform = ToMatrix4(sceneNode.mTransformation);

	bone->children.reserve(sceneNode.mNumChildren);
	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*(sceneNode.mChildren[i]), bone, skeleton, boneIndexLookup);
		bone->children.push_back(child);
		bone->childrenIndices.push_back(child->index);
	}
	return bone;
}

uint32_t GetBoneIndex(const aiBone* nodeBone, BoneIndexLookup& boneIndexMap)
{
	std::string boneName = nodeBone->mName.C_Str();
	ASSERT(!boneName.empty(), "ERROR: aiBone does not have a name");
	auto iter = boneIndexMap.find(boneName);

	ASSERT(false, "ERROR: aiBone was not found in the index map");
	return iter->second;
}

void SetBoneOffsetTransform(const aiBone* nodeBone, Skeleton& skeleton, BoneIndexLookup& boneIndexMap)
{
	uint32_t boneIndex = GetBoneIndex(nodeBone, boneIndexMap);
		Bone* bone = skeleton.bones[boneIndex].get();
		bone->offsetTransform = ToMatrix4(nodeBone->mOffsetMatrix);
	
}


int main(int  argc, char* argv[]) 
{
	const auto argOpt = ParseArgs(argc, argv);
	if (argOpt.has_value() == false) 
	{
		printf("No arguments present to import fbx model\n");
		return -1;
	}

	const auto& arguments = argOpt.value();
	Assimp::Importer importer;
	const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(arguments.inputFileName.u8string().c_str(), flags);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}
	
	printf("Importing %s...\n", arguments.inputFileName.u8string().c_str());
	
	Model model;
	BoneIndexLookup boneIndexLookup;

	if (scene->HasMeshes())
	{
		printf("Build skeleton . . . \n");

		model.skeleton = std::make_unique<Skeleton>();
		BuildSkeleton(*scene->mRootNode, nullptr, *model.skeleton, boneIndexLookup);
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const auto& aiMesh = scene->mMeshes[meshIndex];
			if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			{
				continue;
			}
			if (aiMesh->HasBones()) {
				for (uint32_t b = 0; b < aiMesh->mNumBones; ++b)
				{
					const auto bone = aiMesh->mBones[b];
					SetBoneOffsetTransform(bone, *model.skeleton, boneIndexLookup);
				}
			}
		}
		for (auto& bone : model.skeleton->bones)
		{
			bone->offsetTransform._41 *= arguments.scale;
			bone->offsetTransform._42 *= arguments.scale;
			bone->offsetTransform._43 *= arguments.scale;
			bone->toParentTransform._41 *= arguments.scale;
			bone->toParentTransform._42 *= arguments.scale;
			bone->toParentTransform._43 *= arguments.scale;
		}

		if (!arguments.animOnly)
		{
		printf("Reading Mesh Data . . . ");
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const auto& aiMesh = scene->mMeshes[meshIndex];
			if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			{
				continue;
			}

			const uint32_t numVertices = aiMesh->mNumVertices;
			const uint32_t numFaces = aiMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			auto& meshData = model.meshData.emplace_back();

			printf("Reading material index . . .\n ");
			const uint32_t materialIndex = aiMesh->mMaterialIndex;

			printf("Reading Vertices . . .\n ");

			auto& mesh = meshData.mesh;
			mesh.vertices.reserve(numVertices);

			const aiVector3D* positions = aiMesh->mVertices;
			const aiVector3D* normals = aiMesh->mNormals;
			const aiVector3D* tangents = aiMesh->HasTangentsAndBitangents() ? aiMesh->mTangents:nullptr;
			const aiVector3D* texCoords = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0] : nullptr;
			for (uint32_t v = 0; v < numVertices; ++v)
			{
				auto& vertex = mesh.vertices.emplace_back();
				vertex.position = ToVector3(positions[v]) * arguments.scale;
				vertex.normal = ToVector3(normals[v]);
				vertex.tangent = tangents ? ToVector3(tangents[v]) : Vector3::Zero;
				if (texCoords)
				{
					vertex.uvCoord = { texCoords[v].x,texCoords[v].y };
				}
			}

			printf("Reading indices . . .\n ");
			mesh.indices.reserve(numIndices);
			const auto& aiFaces = aiMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				const auto& aiFace = aiFaces[i];
				mesh.indices.push_back(aiFace.mIndices[0]);
				mesh.indices.push_back(aiFace.mIndices[1]);
				mesh.indices.push_back(aiFace.mIndices[2]);
			}

			if (aiMesh->HasBones())
			{
				printf("Reading bone weights . . .\n");
				std::vector<int> numWeightsAdded(mesh.vertices.size());
				for (uint32_t b = 0; b < aiMesh->mNumBones; ++b)
				{
					const aiBone* bone = aiMesh->mBones[b];
					uint32_t boneIndex = GetBoneIndex(bone, boneIndexLookup);
					for (uint32_t w = 0; w < bone->mNumWeights; ++w)
					{
						const aiVertexWeight& weight = bone->mWeights[w];
						Vertex& vertex = mesh.vertices[weight.mVertexId];
						int& count = numWeightsAdded[weight.mVertexId];
							if (count < Vertex::MaxBones)
							{
								vertex.boneIndices[count] = boneIndex;
								vertex.boneWeights[count] = weight.mWeight;
								++count;
							}
						}
					}
				}
			}
		}
	}

	if (!arguments.animOnly && scene->HasMaterials())
	{
		printf("Reading Material Data . . .");

		const uint32_t numMaterials = scene->mNumMaterials;
		model.materialData.reserve(numMaterials);
		for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			const auto& aiMaterial = scene->mMaterials[materialIndex];
			aiColor3D ambient, diffuse, emissive, specular;
			ai_real specularPower = 1.0f;

			aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT,  ambient);
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE,  diffuse);
			aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
			aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			 
			auto& materialData = model.materialData.emplace_back();
			materialData.material.ambient = ToColor(ambient);
			materialData.material.diffuse = ToColor(diffuse);
			materialData.material.emissive = ToColor(emissive);
			materialData.material.specular = ToColor(specular);
			materialData.material.power = specularPower;

			materialData.diffuseMapName = FindTexture(scene, aiMaterial, aiTextureType_DIFFUSE, arguments, "_diffuse", materialIndex);
			materialData.normalMapName = FindTexture(scene, aiMaterial, aiTextureType_NORMALS, arguments, "_normal", materialIndex);
			materialData.bumpMapName = FindTexture(scene, aiMaterial, aiTextureType_DISPLACEMENT, arguments, "_bump", materialIndex);
			materialData.specularMapName = FindTexture(scene, aiMaterial, aiTextureType_SPECULAR, arguments, "_specular", materialIndex);
		}
	}

	if ( scene->HasAnimations())
	{
		printf("Building animations . . .\n");
		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			const auto aiAnim = scene->mAnimations[animIndex];
			auto& animClip = model.animationClips.emplace_back();
			if (aiAnim->mName.length > 0)
			{
				animClip.name = aiAnim->mName.C_Str();
			}
			else
			{
				animClip.name = "Anim" + std::to_string(animIndex);
			}
			animClip.tickDuration = static_cast<float>(aiAnim->mDuration);
			animClip.ticksPerSecond = static_cast<float>(aiAnim->mTicksPerSecond);

			printf("Reading bone animations for%s . . .\n", animClip.name.c_str());
			animClip.boneAnimations.resize(model.skeleton->bones.size());
			for (uint32_t boneAnimIndex = 0; boneAnimIndex < aiAnim->mNumChannels; ++boneAnimIndex)
			{
				const auto aiBoneAnim = aiAnim->mChannels[boneAnimIndex];
				const int boneIndex = boneIndexLookup[aiBoneAnim->mNodeName.C_Str()];
				auto& boneAnimation = animClip.boneAnimations[boneIndex];
				boneAnimation = std::make_unique<Animation>();

				AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumPositionKeys; ++keyIndex)
				{
					auto& posKey = aiBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(ToVector3(posKey.mValue)* arguments.scale, static_cast<float>(posKey.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					auto& rotKey = aiBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(ToQuaternion(rotKey.mValue), static_cast<float>(rotKey.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					auto& scaleKey = aiBoneAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(ToVector3(scaleKey.mValue), static_cast<float>(scaleKey.mTime));
				}
				*boneAnimation = builder.Build();
			}
		}
	}


	if (!arguments.animOnly)
	{
		printf("Saving model . . .\n");
		ModelIO::SaveModel(arguments.outputFileName, model);


		printf("Saving material . . .\n");
		ModelIO::SaveMaterial(arguments.outputFileName, model);

		printf("Saving skeleton . . .\n");
		ModelIO::SaveSkeleton(arguments.outputFileName, model);
	}

	printf("Saving Animatioan . . .\n");
	ModelIO::SaveAnimations(arguments.outputFileName, model);

	printf("All done!");

	return 0;
}


