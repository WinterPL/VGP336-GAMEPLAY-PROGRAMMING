#pragma once

namespace WNTRengine
{
	namespace SaveUtil
	{
		void SaveVetor3     (const char* key, const WNTRmath::Vector3& v, rapidjson::Document& doc, rapidjson::Value& member);
		void SaveQuaternion (const char* key, const WNTRmath::Quaternion& q, rapidjson::Document& doc, rapidjson::Value& member);
		void SaveFloat      (const char* key, float value, rapidjson::Document& doc, rapidjson::Value& member);
		void SaveInt        (const char* key, int value, rapidjson::Document& doc, rapidjson::Value& member);
		void SaveBool       (const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& member);
		void SaveString     (const char* key,const const char* value, rapidjson::Document& doc, rapidjson::Value& member);
		void SaveStringArray(const char* key, const std::vector<std::string>& values, rapidjson::Document& doc, rapidjson::Value& member);
	}
}