#ifndef PROTO_UTIL_H
#define PROTO_UTIL_H

#include "strings_util.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/reflection.h>
#include "options.pb.h"  // For default_value extension accessor

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

inline void ApplyProtoDefaults(google::protobuf::Message* msg) {
  if (!msg) return;
  
  const google::protobuf::Descriptor* desc = msg->GetDescriptor();
  const google::protobuf::Reflection* refl = msg->GetReflection();
  using CppType = google::protobuf::FieldDescriptor::CppType;
  
  // First pass: Create nested messages (this marks them as set with explicit field presence)
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor* field = desc->field(i);
    
    if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
      if (field->is_repeated()) {
        // For repeated fields, apply defaults to each element
        for (int j = 0; j < refl->FieldSize(*msg, field); j++) {
          ApplyProtoDefaults(refl->MutableRepeatedMessage(msg, field, j));
        }
      } else {
        // For singular message fields, create the message if it doesn't exist
        // With explicit field presence, MutableMessage creates the message and marks the field as set
        google::protobuf::Message* nestedMsg = refl->MutableMessage(msg, field);
        if (nestedMsg) {
          // Recursively apply defaults to the nested message
          ApplyProtoDefaults(nestedMsg);
        }
      }
    }
  }
  
  // Second pass: Apply default_value options to primitive fields in this message
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor* field = desc->field(i);
    const google::protobuf::FieldOptions& opts = field->options();
    
    // Skip message and repeated fields
    if (field->is_repeated() || field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
      continue;
    }
    
    // Skip if field is already set
    if (refl->HasField(*msg, field)) {
      continue;
    }
    
    // Get default_value extension using generated accessor
    std::string defaultVal = opts.GetExtension(buffers::default_value);
    if (!defaultVal.empty()) {
      std::cerr << "[ApplyProtoDefaults] Applying default to field: " << field->name() 
                << " = " << defaultVal << std::endl;
      SetProtoField(msg, field, defaultVal);
      // Verify it was set
      if (!refl->HasField(*msg, field)) {
        std::cerr << "[ApplyProtoDefaults] WARNING: Field was not set after SetProtoField: " << field->name() << std::endl;
      }
    } else {
      std::cerr << "[ApplyProtoDefaults] No default_value extension for field: " << field->name() << std::endl;
    }
  }
}

#endif
