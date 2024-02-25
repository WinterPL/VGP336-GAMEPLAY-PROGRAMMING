#include "Precompiled.h"
#include "SaveUtil.h"

using namespace WNTRengine;

void SaveUtil::SaveVetor3(const char* key, const WNTRmath::Vector3& v, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	rapidjson::Value vecArray(rapidjson::kArrayType);
	vecArray.PushBack(v.x, doc.GetAllocator());
	vecArray.PushBack(v.y, doc.GetAllocator());
	vecArray.PushBack(v.z, doc.GetAllocator());
	member.AddMember(str, vecArray, doc.GetAllocator());
}	 
	 
void SaveUtil::SaveQuaternion(const char* key, const WNTRmath::Quaternion& q, rapidjson::Document& doc, rapidjson::Value& member)
{	 
	rapidjson::GenericStringRef<char> str(key);
	rapidjson::Value quatArray(rapidjson::kArrayType);
	quatArray.PushBack(q.x, doc.GetAllocator());
	quatArray.PushBack(q.y, doc.GetAllocator());
	quatArray.PushBack(q.z, doc.GetAllocator());
	quatArray.PushBack(q.w, doc.GetAllocator());
	member.AddMember(str, quatArray, doc.GetAllocator());
}	 
	 
void SaveUtil::SaveFloat(const char* key, float value, rapidjson::Document& doc, rapidjson::Value& member)
{	
	rapidjson::GenericStringRef<char> str(key);
	member.AddMember(str,value, doc.GetAllocator());
}	 
	 
void SaveUtil::SaveInt(const char* key, int value, rapidjson::Document& doc, rapidjson::Value& member)
{	
	rapidjson::GenericStringRef<char> str(key);
	member.AddMember(str, value, doc.GetAllocator());
}	 
	 
void SaveUtil::SaveBool(const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	member.AddMember(str, value, doc.GetAllocator());
}

void WNTRengine::SaveUtil::SaveString(const char* key, const char* value, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	rapidjson::GenericStringRef<char> strValue(value);
	member.AddMember(str, strValue, doc.GetAllocator());
}

void WNTRengine::SaveUtil::SaveStringArray(const char* key, const std::vector<std::string>& values, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	rapidjson::Value strArray(rapidjson::kArrayType);
	for (const std::string& v : values)
	{
		rapidjson::GenericStringRef<char> strValue(v.c_str());
		strArray.PushBack(strArray, doc.GetAllocator());
	}
	member.AddMember(str, strArray, doc.GetAllocator());
}
