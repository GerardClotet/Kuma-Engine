#include "SerializerJSON.h"
#include "Globals.h"
SerializerR_JSON::SerializerR_JSON()
{
}

SerializerR_JSON::~SerializerR_JSON()
{
}

R_JSON_File* SerializerR_JSON::R_JSONRead(const char* path)
{

	FILE* file = fopen(path,"rb"); //read binary
	char buffer[100000];

	if (file != nullptr)
		return new R_JSON_File(new rapidjson::FileReadStream(file, buffer, sizeof(buffer)), file);

	else 
		return nullptr;
}

R_JSON_File* SerializerR_JSON::R_JSONWrite(const char* path)
{

	FILE* file = fopen(path, "wb"); //write binary

	char buffer[100000];
	if (file != nullptr)
		return new R_JSON_File(new rapidjson::FileWriteStream(file, buffer, sizeof(buffer)), file);

	else
		return nullptr;
}

void SerializerR_JSON::R_JSON_Close(R_JSON_File* file)
{
	if (file != nullptr)
		file->CloseFile();
	RELEASE(file);
	file = nullptr;
}

R_JSON_Value::R_JSON_Value(rapidjson::Document::AllocatorType* alloc, rapidjson::Type type)
{
	alloc_doc = alloc;
	value = new rapidjson::Value(type);
}

R_JSON_Value::~R_JSON_Value()
{
	for (auto& value : values_vec)
	{
		RELEASE(value);
	}
	values_vec.clear();

	RELEASE(value);
}

void R_JSON_Value::ChangeType(rapidjson::Type type)
{
	switch (type)
	{
	case rapidjson::Type::kObjectType:
		RELEASE(this->value);
		this->value = new rapidjson::Value(rapidjson::kObjectType);
		break;
	case rapidjson::Type::kArrayType:
		RELEASE(this->value);
		this->value = new rapidjson::Value(rapidjson::kArrayType);
		break;
	case rapidjson::Type::kStringType:
		RELEASE(this->value);
		this->value = new rapidjson::Value(rapidjson::kStringType);
		break;
	case rapidjson::Type::kNumberType:
		RELEASE(this->value);
		this->value = new rapidjson::Value(rapidjson::kNumberType);
		break;
	case rapidjson::Type::kTrueType:
		RELEASE(this->value);
		value = new rapidjson::Value(rapidjson::kTrueType);
		break;
	case rapidjson::Type::kFalseType:
		RELEASE(this->value);
		value = new rapidjson::Value(rapidjson::kFalseType);
		break;

	}
}

R_JSON_Value* R_JSON_Value::NewValue(rapidjson::Type t)
{
	R_JSON_Value* ret = new R_JSON_Value(alloc_doc, t);
	values_vec.push_back(ret);
	return ret;
}

void R_JSON_Value::AddValue(const char* name, const R_JSON_Value& val)
{
	
	switch (this->value->GetType())
	{
	case rapidjson::kObjectType:
	{
		std::string str = name;
		rapidjson::Value generic(str.c_str(), str.size(), *alloc_doc);
		this->value->AddMember(generic, *val.value, *alloc_doc);
		break;
	}
	case rapidjson::kArrayType:
		this->value->PushBack(*val.value, *alloc_doc);
		break;
	}
}

R_JSON_Value* R_JSON_Value::GetValue(const char* name)
{
	if (value->IsObject() && value->HasMember(name))
	{
		R_JSON_Value* ret = new R_JSON_Value(alloc_doc);
		ret->value->CopyFrom(value->operator[](name), *alloc_doc, false);
		values_vec.push_back(ret);

		return ret;
	}

	return nullptr;
}

void R_JSON_Value::SetValue(const char* name, R_JSON_Value& val)
{
	if (this->value->HasMember(name))
		this->value->operator[](name) = *val.value; //If it exists modify it
	else
		this->AddValue(name, val); //if not, set a new one
}



rapidjson::Value* R_JSON_Value::GetJSONValue() const
{
	return value;
}

void R_JSON_Value::SetInt(const char* name, int value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc_doc);
	this->value->AddMember(index, value, *alloc_doc);
}

void R_JSON_Value::SetUint(const char* name, uint value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc_doc);
	this->value->AddMember(index, value, *alloc_doc);
}

void R_JSON_Value::SetUint32(const char* name, uint32 value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc_doc);
	this->value->AddMember(index, value, *alloc_doc);
}

void R_JSON_Value::SetFloat(const char* name, float value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc_doc);
	this->value->AddMember(index, value, *alloc_doc);
}

void R_JSON_Value::SetString(const char* name, const char* value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc_doc);
	std::string str2 = value;
	rapidjson::Value val(str2.c_str(), str2.size(), *alloc_doc);
	this->value->AddMember(index, val, *alloc_doc);
}

void R_JSON_Value::SetBoolean(const char* name, bool value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc_doc);
	this->value->AddMember(index, value, *alloc_doc);
}

void R_JSON_Value::Set3DVec(const char* name, float3 vec)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc_doc);

	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(vec.x, *alloc_doc);
	a.PushBack(vec.y, *alloc_doc);
	a.PushBack(vec.z, *alloc_doc);

	this->value->AddMember(index, a, *alloc_doc);
}

void R_JSON_Value::SetQuat(const char* name, Quat quat)
{
	std::string s = name;
	rapidjson::Value index(s.c_str(), s.size(), *alloc_doc);
	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(quat.x, *alloc_doc);
	a.PushBack(quat.y, *alloc_doc);
	a.PushBack(quat.z, *alloc_doc);
	a.PushBack(quat.w, *alloc_doc);

	this->value->AddMember(index, a, *alloc_doc);
}

void R_JSON_Value::SetColor(const char* name, Color color)
{
	std::string s = name;
	rapidjson::Value index(s.c_str(), s.size(), *alloc_doc);
	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(color.r, *alloc_doc);
	a.PushBack(color.g, *alloc_doc);
	a.PushBack(color.b, *alloc_doc);
	a.PushBack(color.a, *alloc_doc);

	this->value->AddMember(index, a, *alloc_doc);
}

void R_JSON_Value::GetInt(const char* name, int value)
{
	std::string str = name;
	unsigned char uC = (unsigned)name;
	encoding_doc->supportUnicode;
	//encoding_doc->GetRange(str.c_str());
	rapidjson::Value i(value);
	
	this->value->FindMember(name);
	this->value->GetInt();
	//rapidjson::Value() ep
}

void R_JSON_Value::GetString(const char* name, const char* value)
{
}

R_JSON_File::R_JSON_File(rapidjson::FileReadStream* read_st, FILE* file): read_stream(read_st),filep(file)
{
	
	
	document = new rapidjson::Document();
	document->ParseStream(*read_st);
	LOG("haha");
	alloc_doc = &document->GetAllocator();
}

R_JSON_File::R_JSON_File(rapidjson::FileWriteStream* write_st, FILE* file) : write_stream(write_st), filep(file)
{
	LOG("hehe");
	document = new rapidjson::Document();
	document->SetObject();

	alloc_doc = &document->GetAllocator();
}

R_JSON_File::~R_JSON_File()
{
	for (auto& value : val_vec)
	{
		RELEASE(value);
	}
	val_vec.clear();

	RELEASE(document);
	//RELEASE(allocator); //PETA EL PROGRAMA POR DE-ALLOCATING 
	RELEASE(read_stream);
	RELEASE(write_stream);
}

bool R_JSON_File::WriteFile()
{
	bool ret = false;
	if (write_stream != nullptr)
	{
		rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(*write_stream);
		ret = document->Accept(writer);

		return ret;
	}
	else
		return ret;
}

R_JSON_Value* R_JSON_File::NewValue(rapidjson::Type type)
{
	R_JSON_Value* ret = new R_JSON_Value(alloc_doc, type);
	val_vec.push_back(ret);
	return ret;
}

void R_JSON_File::AddValue(const char* name, const R_JSON_Value& value)
{
	std::string str = name;
	rapidjson::Value genetic(str.c_str(), str.size(), *alloc_doc);
	document->AddMember(genetic, *value.GetJSONValue(), *alloc_doc);
}

R_JSON_Value* R_JSON_File::GetValue(const char* name)
{
	if (document->IsObject() && document->HasMember(name))
	{
		R_JSON_Value* ret = new R_JSON_Value(alloc_doc);
		ret->GetJSONValue()->CopyFrom(document->operator[](name), *alloc_doc, false);
		val_vec.push_back(ret);

		return ret;
	}

	return nullptr;
}

void R_JSON_File::SetValue(const char* name, R_JSON_Value& value)
{
	if (this->document->HasMember(name))
		this->document->operator[](name) = *value.GetJSONValue();
	else
		this->AddValue(name, value);
}

void R_JSON_File::CloseFile()
{
	fclose(filep);
}
