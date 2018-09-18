#pragma once

#include <string>
#include <sstream>

inline std::string string_replace_all(std::string str, std::string substr, std::string nstr)
{
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos)
  {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

inline std::vector<std::string> SplitString(const std::string &str, char delimiter) {
  std::vector<std::string> vec;
  std::stringstream sstr(str);
  std::string tmp;
  while (std::getline(sstr, tmp, delimiter)) vec.push_back(tmp);

  return vec;
}

#include <google/protobuf/descriptor.h>

inline void SetProtoField(const google::protobuf::Reflection* refl, google::protobuf::Message* msg, const google::protobuf::FieldDescriptor* field, std::string v, bool relative = false) {
  using CppType = google::protobuf::FieldDescriptor::CppType;

  switch (field->cpp_type()) {
    case CppType::CPPTYPE_INT32: {
      auto val = std::stol(v);
      if (relative)
        val += refl->GetInt32(*msg, field);
      refl->SetInt32(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_INT64: {
      auto val = std::stoll(v);
      if (relative)
        val += refl->GetInt64(*msg, field);
      refl->SetInt64(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_UINT32: {
      auto val = std::stoul(v);
      if (relative)
        val += refl->GetUInt32(*msg, field);
      refl->SetUInt32(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_UINT64: {
      auto val = std::stoull(v);
      if (relative)
        val += refl->GetUInt32(*msg, field);
      refl->SetUInt32(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_DOUBLE: {
      auto val = std::stod(v);
      if (relative)
        val += refl->GetDouble(*msg, field);
      refl->SetDouble(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_FLOAT: {
      auto val = std::stof(v);
      if (relative)
        val += refl->GetFloat(*msg, field);
      refl->SetFloat(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_BOOL: {
      refl->SetBool(msg, field, (std::stof(v) != 0));
      break;
    }
    case CppType::CPPTYPE_ENUM: {
      auto val = field->enum_type()->FindValueByName(v);
      refl->SetEnum(msg, field, val == NULL ? field->enum_type()->FindValueByNumber(std::stol(v)) : val);
      break;
    }
    case CppType::CPPTYPE_STRING: {
      refl->SetString(msg, field, v);
      break;
    }
    default:
      // error
      break;
  };
}
