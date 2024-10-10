#ifndef PROTO_UTIL_H
#define PROTO_UTIL_H

#include "strings_util.h"
#include <google/protobuf/descriptor.h>

inline void SetProtoField(google::protobuf::Message* msg,
                          const google::protobuf::FieldDescriptor* field,
                          std::string value) {
  const google::protobuf::Reflection* refl = msg->GetReflection();
  using CppType = google::protobuf::FieldDescriptor::CppType;

  switch (field->cpp_type()) {
    case CppType::CPPTYPE_INT32: {
      refl->SetInt32(msg, field, std::stol(value));
      break;
    }
    case CppType::CPPTYPE_INT64: {
      refl->SetInt64(msg, field, std::stoll(value));
      break;
    }
    case CppType::CPPTYPE_UINT32: {
      refl->SetUInt32(msg, field, std::stoul(value));
      break;
    }
    case CppType::CPPTYPE_UINT64: {
      refl->SetUInt32(msg, field, std::stoull(value));
      break;
    }
    case CppType::CPPTYPE_DOUBLE: {
      refl->SetDouble(msg, field, std::stod(value));
      break;
    }
    case CppType::CPPTYPE_FLOAT: {
      refl->SetFloat(msg, field, std::stof(value));
      break;
    }
    case CppType::CPPTYPE_BOOL: {
      refl->SetBool(msg, field, ParseBool(value));
      break;
    }
    case CppType::CPPTYPE_ENUM: {
      auto val = field->enum_type()->FindValueByName(value);
      if (val == NULL)
        val = field->enum_type()->FindValueByNumber(std::stol(value));
      if (val != NULL)
        refl->SetEnum(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_STRING: {
      refl->SetString(msg, field, value);
      break;
    }
    default:
      // error
      break;
  };
}

inline void SetProtoField(google::protobuf::Message* msg,
                          const google::protobuf::FieldDescriptor* field,
                          double value) {
  const google::protobuf::Reflection* refl = msg->GetReflection();
  using CppType = google::protobuf::FieldDescriptor::CppType;

  switch (field->cpp_type()) {
    case CppType::CPPTYPE_INT32: {
      refl->SetInt32(msg, field, (int) value);
      break;
    }
    case CppType::CPPTYPE_INT64: {
      refl->SetInt64(msg, field, (long) value);
      break;
    }
    case CppType::CPPTYPE_UINT32: {
      refl->SetUInt32(msg, field, (unsigned int) value);
      break;
    }
    case CppType::CPPTYPE_UINT64: {
      refl->SetUInt32(msg, field, (unsigned long) value);
      break;
    }
    case CppType::CPPTYPE_DOUBLE: {
      refl->SetDouble(msg, field, value);
      break;
    }
    case CppType::CPPTYPE_FLOAT: {
      refl->SetFloat(msg, field, (float) value);
      break;
    }
    case CppType::CPPTYPE_BOOL: {
      refl->SetBool(msg, field, (bool) value);
      break;
    }
    case CppType::CPPTYPE_ENUM: {
      auto val = field->enum_type()->FindValueByNumber((int) value);
      if (val != NULL)
        refl->SetEnum(msg, field, val);
      break;
    }
    case CppType::CPPTYPE_STRING: {
      refl->SetString(msg, field, std::to_string(value));
      break;
    }
    default:
      // error
      break;
  };
}

inline void SetProtoField(google::protobuf::Message* msg, int field, std::string value) {
  return SetProtoField(msg, msg->GetDescriptor()->FindFieldByNumber(field), value);
}
inline void SetProtoField(google::protobuf::Message* msg, int field, double value) {
  return SetProtoField(msg, msg->GetDescriptor()->FindFieldByNumber(field), value);
}

inline double GetNumericProtoField(
    const google::protobuf::Message &msg,
    const google::protobuf::FieldDescriptor* field) {
  using CppType = google::protobuf::FieldDescriptor::CppType;
  const google::protobuf::Reflection* refl = msg.GetReflection();

  switch (field->cpp_type()) {
    case CppType::CPPTYPE_INT32:  return refl->GetInt32 (msg, field);
    case CppType::CPPTYPE_INT64:  return refl->GetInt64 (msg, field);
    case CppType::CPPTYPE_UINT32: return refl->GetUInt32(msg, field);
    case CppType::CPPTYPE_UINT64: return refl->GetUInt32(msg, field);
    case CppType::CPPTYPE_DOUBLE: return refl->GetDouble(msg, field);
    case CppType::CPPTYPE_FLOAT:  return refl->GetFloat (msg, field);
    case CppType::CPPTYPE_BOOL:   return refl->GetBool  (msg, field);
    case CppType::CPPTYPE_ENUM:   return refl->GetEnum(msg, field)->number();
    case CppType::CPPTYPE_STRING: return std::stod(refl->GetString(msg, field));
    default: return 0;
  };
}

inline double GetNumericProtoField(const google::protobuf::Message &msg, int field) {
  return GetNumericProtoField(msg, msg.GetDescriptor()->FindFieldByNumber(field));
}

#endif
