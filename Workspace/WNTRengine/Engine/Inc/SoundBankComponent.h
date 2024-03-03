#pragma once

#include "Component.h"

namespace WNTRengine
{
	class SoundBankComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::SoundEffect);

		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void DeSerialize(const rapidjson::Value& value) override;

		void DebugUI() override;

		void Play(const std::string& key);
		void Stop(const std::string& key);
		void StopAll();

	private:
		struct SoundData
		{
			std::string fileName;
			Audio::SoundId soundId;
			bool isLooping;
		};
		using SoundEffects = std::unordered_map<std::string,SoundData>;
		SoundEffects mSoundEffects;

	};
}