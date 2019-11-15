#pragma once

#include "Globals.h"
#include "p2Defs.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "Color.h"
#include <vector>
struct R_JSON_Value {
	
public:
	R_JSON_Value(rapidjson::Document::AllocatorType* alloc, rapidjson::Type type = rapidjson::kObjectType);
	~R_JSON_Value();

	

	void ChangeType(rapidjson::Type type);
	R_JSON_Value* NewValue(rapidjson::Type t = rapidjson::kObjectType);

	void AddValue(const char* name, const R_JSON_Value& value);
	R_JSON_Value* GetValue(const char* name);
	void SetValue(const char* name, R_JSON_Value& value);

	rapidjson::Value* GetJSONValue() const;

	void SetInt(const char* name, int value);
	void SetUint(const char* name, uint value);
	void SetUint32(const char* name, uint32 val);
	void SetFloat(const char* name, float value);
	void SetString(const char* name, const char* value);
	void SetBoolean(const char* name, bool value);
	void Set3DVec(const char* name, float3 vec);
	void SetQuat(const char* name, Quat quat);
	void SetColor(const char* name, Color color);
	rapidjson::Document::AllocatorType* alloc_doc = nullptr;
	rapidjson::Value* value = nullptr;
	std::vector<R_JSON_Value*> values_vec;
};


struct R_JSON_File {
public:
	R_JSON_File(rapidjson::FileReadStream* read_st, FILE* file);
	R_JSON_File(rapidjson::FileWriteStream* write_st, FILE* file);


	virtual ~R_JSON_File();

	bool WriteFile();

	R_JSON_Value* NewValue(rapidjson::Type type = rapidjson::kObjectType);

	void AddValue(const char* name, const R_JSON_Value& value);
	R_JSON_Value* GetValue(const char* name);
	void SetValue(const char* name, R_JSON_Value& value);

	void CloseFile();


public:

	FILE* filep = nullptr;
	rapidjson::Document* document = nullptr;
	rapidjson::FileWriteStream* write_stream = nullptr;
	rapidjson::FileReadStream* read_stream = nullptr;
	rapidjson::Document::AllocatorType* alloc_doc = nullptr;
	std::vector<R_JSON_Value*> val_vec;
};

class SerializerR_JSON {
public:

	SerializerR_JSON();
	~SerializerR_JSON();

	R_JSON_File* R_JSONRead(const char* path);
	R_JSON_File* R_JSONWrite(const char* path);

	void R_JSON_Close(R_JSON_File* file);
};