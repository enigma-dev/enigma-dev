/** Copyright (C) 2019 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "proto-yaml.h"
#include "../proto_util.h"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <type_traits>
#include <typeinfo>
#include <map>
#include <set>

namespace {

namespace proto = google::protobuf;
using CppType = proto::FieldDescriptor::CppType;
using std::map;
using std::string;

std::string ToLower(string str) {
  for (char &c : str) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
  return str;
}

std::string Hyphenate(string snake) {
  for (char &c : snake) if (c == '_') c = '-';
  return snake;
}

std::string Spaceify(string snake) {
  for (char &c : snake) if (c == '_') c = ' ';
  return snake;
}

std::string Capitalize(string str) {
  if (char &c = str[0]; c >= 'a' && c <= 'z') c -= 'a' - 'A';
  return str;
}

struct FieldCache {
  string message_name;
  map<string, const proto::FieldDescriptor*> fields;

  const proto::FieldDescriptor* field(const std::string &n) {
    if (auto it = fields.find(n); it != fields.end()) return it->second;
    if (auto it = fields.find(ToLower(n)); it != fields.end()) {
      fields.insert({n, it->second});
      return it->second;
    }
    return nullptr;
  }

  void Prime(const proto::Descriptor *desc) {
    if (!desc) {
      message_name = "null";
      return;
    }
    message_name = desc->name();
    const proto::FieldDescriptor *fd;
    for (int i = 0; i < desc->field_count() && (fd = desc->field(i)); ++i) {
      fields[ToLower(Spaceify(fd->name()))] = fd;
      fields[ToLower(Hyphenate(fd->name()))] = fd;
      fields[ToLower(fd->camelcase_name())] = fd;
    }
    for (int i = 0; i < desc->field_count() && (fd = desc->field(i)); ++i) {
      fields[Spaceify(fd->name())] = fd;
      fields[Hyphenate(fd->name())] = fd;
      fields[fd->camelcase_name()] = fd;
      fields[Capitalize(fd->camelcase_name())] = fd;
    }
    for (int i = 0; i < desc->field_count() && (fd = desc->field(i)); ++i) {
      fields[fd->name()] = fd;
    }
  }
};

struct ConstantCache {
  string enum_name;
  map<string, const proto::EnumValueDescriptor*> values;

  const proto::EnumValueDescriptor* value(const std::string &n) {
    if (auto it = values.find(n); it != values.end()) return it->second;
    if (auto it = values.find(ToLower(n)); it != values.end()) {
      values.insert({n, it->second});
      return it->second;
    }
    return nullptr;
  }

  void Prime(const proto::EnumDescriptor *desc) {
    if (!desc) {
      enum_name = "null";
      return;
    }
    enum_name = desc->name();
    const proto::EnumValueDescriptor *vd;
    for (int i = 0; i < desc->value_count() && (vd = desc->value(i)); ++i) {
      values[ToLower(Spaceify(vd->name()))] = vd;
      values[ToLower(Hyphenate(vd->name()))] = vd;
    }
    for (int i = 0; i < desc->value_count() && (vd = desc->value(i)); ++i) {
      values[Spaceify(vd->name())] = vd;
      values[Hyphenate(vd->name())] = vd;
    }
    for (int i = 0; i < desc->value_count() && (vd = desc->value(i)); ++i) {
      values[ToLower(vd->name())] = vd;
    }
    for (int i = 0; i < desc->value_count() && (vd = desc->value(i)); ++i) {
      values[std::to_string(vd->number())] = vd;
    }
    for (int i = 0; i < desc->value_count() && (vd = desc->value(i)); ++i) {
      values[vd->name()] = vd;
    }
  }
};

class DecodeHelper {
  map<const proto::Descriptor*, FieldCache> caches_;
  map<const proto::EnumDescriptor*, ConstantCache> enum_caches_;
  FieldCache &cache_for(const proto::Descriptor* desc) {
    auto it = caches_.insert({desc, {}});
    if (it.second) it.first->second.Prime(desc);
    return it.first->second;
  }
  ConstantCache &cache_for(const proto::EnumDescriptor* desc) {
    auto it = enum_caches_.insert({desc, {}});
    if (it.second) it.first->second.Prime(desc);
    return it.first->second;
  }

 public:
  bool FitMapToMessage(const YAML::Node &yaml_map, proto::Message *out);
  bool FitNodeToField(const YAML::Node &yaml,
                      const proto::FieldDescriptor *field, proto::Message *out);
  bool PutScalarToField(const YAML::Node &yaml_scalar,
                        const proto::FieldDescriptor *field,
                        proto::Message *to);
};


// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ Helpers for proto assignment from YAML nodes.                            ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛


template<typename T> using FieldMutator =
    void (proto::Reflection::*)(proto::Message*,
                                const proto::FieldDescriptor*, T) const;

template<typename CppType>
void PutOrPrint(const YAML::Node &scalar, const proto::FieldDescriptor *field,
                FieldMutator<CppType> set, FieldMutator<CppType> add,
                proto::Message *to) {
  CppType conv;
  const proto::Reflection* refl = to->GetReflection();
  if (YAML::convert<CppType>::decode(scalar, conv)) {
    if (field->is_repeated()) {
      (refl->*add)(to, field, conv);
    } else {
      (refl->*set)(to, field, conv);
    }
  } else {
    std::cerr << "Conversion failure: `"    << scalar.Scalar()
              << "` is not convertable to " << typeid(CppType).name()
              << std::endl;
  }
}

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ DecodeHelper method implementations.                                     ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

bool DecodeHelper::PutScalarToField(const YAML::Node &yaml_scalar,
                                    const proto::FieldDescriptor *field,
                                    proto::Message *to) {
  using CppType = proto::FieldDescriptor::CppType;
  using R = proto::Reflection;

  switch (field->cpp_type()) {
    case CppType::CPPTYPE_INT32: {
      PutOrPrint(yaml_scalar, field, &R::SetInt32, &R::AddInt32, to);
      break;
    }
    case CppType::CPPTYPE_INT64: {
      PutOrPrint(yaml_scalar, field, &R::SetInt64, &R::AddInt64, to);
      break;
    }
    case CppType::CPPTYPE_UINT32: {
      PutOrPrint(yaml_scalar, field, &R::SetUInt32, &R::AddUInt32, to);
      break;
    }
    case CppType::CPPTYPE_UINT64: {
      PutOrPrint(yaml_scalar, field, &R::SetUInt64, &R::AddUInt64, to);
      break;
    }
    case CppType::CPPTYPE_DOUBLE: {
      PutOrPrint(yaml_scalar, field, &R::SetDouble, &R::AddDouble, to);
      break;
    }
    case CppType::CPPTYPE_FLOAT: {
      PutOrPrint(yaml_scalar, field, &R::SetFloat, &R::AddFloat, to);
      break;
    }
    case CppType::CPPTYPE_BOOL: {
      PutOrPrint(yaml_scalar, field, &R::SetBool, &R::AddBool, to);
      break;
    }
    case CppType::CPPTYPE_ENUM: {
      auto &cache = cache_for(field->enum_type());
      if (const auto *ev = cache.value(yaml_scalar.Scalar())) {
        const proto::Reflection* refl = to->GetReflection();
        if (field->is_repeated()) {
          refl->AddEnum(to, field, ev);
        } else {
          refl->SetEnum(to, field, ev);
        }
      } else {
        std::cerr << "Unknown enum value `" << yaml_scalar.Scalar() << "`..."
                  << std::endl;
      }
      break;
    }
    case CppType::CPPTYPE_STRING: {
      const proto::Reflection* refl = to->GetReflection();
      if (field->is_repeated()) {
        refl->AddString(to, field, yaml_scalar.Scalar());
      } else {
        refl->SetString(to, field, yaml_scalar.Scalar());
      }
      break;
    }
    case CppType::CPPTYPE_MESSAGE:
      std::cerr << "Cannot parse string value as message `"
                << cache_for(field->message_type()).message_name
                << "`: " << yaml_scalar.Scalar() << std::endl;
      return false;
    default:
      std::cerr << "Internal error: Unknown Proto CppType!" << std::endl;
      return false;
  };
  return true;
}

bool DecodeHelper::FitMapToMessage(const YAML::Node &yaml_map,
                                   proto::Message *out) {
  FieldCache &cache = cache_for(out->GetDescriptor());
  for (const auto &entry : yaml_map) {
    string field_name = entry.first.as<string>();
    const auto *fd = cache.field(field_name);
    if (!fd) {
      std::cerr << "Yaml2Proto: Can't find field `" << field_name
                << "` in message `" << cache.message_name << "`..."
                << std::endl;
      continue;
    }
    FitNodeToField(entry.second, fd, out);
  }
  return true;
}

std::pair<const proto::FieldDescriptor*, const proto::FieldDescriptor*>
MessageIsKeyValue(const proto::Descriptor *desc) {
  if (!desc || desc->field_count() < 1) return {};
  const proto::FieldDescriptor *kfd = desc->field(0);
  if (kfd->is_repeated()) return {};
  if (ToLower(kfd->name()) != "id"  &&
      ToLower(kfd->name()) != "key" &&
      ToLower(kfd->name()) != "name") {
    return {};
  }
  const proto::FieldDescriptor *vfd =
      (desc->field_count() == 2) ? desc->field(1) : nullptr;
  return {kfd, vfd};
}

bool YamlFitsValueField(const YAML::Node &yaml,
                        FieldCache &kv_field_cache,
                        const proto::FieldDescriptor *value_field) {
  using CppType = proto::FieldDescriptor::CppType;
  if (yaml.Type() != YAML::NodeType::Map) {
    // We must be trying to fit this to the value field, because the only other
    // interpretation is that this would be a map of all fields other than ID.
    // Not a map, so can't logically be anything other than the value field.
    return true;
  }
  if (value_field->cpp_type() != CppType::CPPTYPE_MESSAGE) {
    // For scalar fields (fields that aren't messages), having a map bound would
    // be an error. But we just established above that we must have a map on our
    // hands, so the fact that the "value" field would have a map bound to it
    // means it must not be intended to be our value field.
    return false;
  }

  // This is the hard investigation. We have a map field. Maybe the map is
  // supposed to be from non-ID fields to their values. But there's only one
  // non-ID field in the context in which this method is called. So now the
  // question is whether the map is for values of the "value" field, or just
  // for the value field itself. There's the additional complication that this
  // could be another key-value mapping, but actually, we can solve both cases
  // using the same basic check.

  // The easiest way to tell is if the mapping has keys that aren't from this
  // key-value message.
  for (const auto &subpair : yaml) {
    if (subpair.first.Type() == YAML::NodeType::Scalar &&
        !kv_field_cache.field(subpair.first.Scalar())) {
      // We checked for various spellings of the key/value field names, but we
      // didn't fit the YAML map key to either.
      return true;
    }
  }
  // The YAML file is literally listing `key: { value: X }` instead of `key: X`.
  std::cerr << "Debug: your YAML is so ugly it looks like an error.\n"
            << "Consider saying `key: value` instead of "
               "`key: { value_name: value }`." << std::endl;
  return false;
}

bool DecodeHelper::FitNodeToField(const YAML::Node &yaml,
                                  const proto::FieldDescriptor *field,
                                  proto::Message *out) {
  using CppType = proto::FieldDescriptor::CppType;
  switch (yaml.Type()) {
    case YAML::NodeType::Null: {
      return false;
    }
    case YAML::NodeType::Scalar: {
      return PutScalarToField(yaml, field, out);
    }
    case YAML::NodeType::Sequence: {
      if (!field->is_repeated()) {
        std::cerr << "Sequence value given for non-repeated field "
                  << out->GetDescriptor()->name() << std::endl;
        return false;
      }
      bool success_bit = true;
      const proto::Reflection* refl = out->GetReflection();
      if (field->cpp_type() == CppType::CPPTYPE_MESSAGE) {
        for (const auto &item : yaml) {
          if (item.Type() != YAML::NodeType::Map) {
            std::cerr
                << "Non-map was given for message entry in repeated field `"
                << field->name()
                << "`. Custom string transformers not currently supported.";
            success_bit = false;
            continue;
          }
          success_bit &= FitMapToMessage(item, refl->AddMessage(out, field));
        }
      } else {
        for (const auto &item : yaml) {
          if (item.Type() != YAML::NodeType::Scalar) {
            std::cerr
                << "Non-scalar was given for entry in repeated field `"
                << field->name()
                << "`. Custom transformers not currently supported.";
            success_bit = false;
            continue;
          }
          success_bit &= PutScalarToField(item, field, out);
        }
      }
      return success_bit;
    }
    case YAML::NodeType::Map: {
      if (field->cpp_type() != CppType::CPPTYPE_MESSAGE) {
        std::cerr << "Map was given for non-message field `" << field->name()
                  << "`. Custom YAML transformers not currently supported.";
        return false;
      }
      const proto::Reflection* refl = out->GetReflection();
      if (field->is_repeated()) {
        const proto::Descriptor *subdesc = field->message_type();
        FieldCache &cache = cache_for(subdesc);
        auto [k_field, v_field] = MessageIsKeyValue(subdesc);

        bool success_bit = true;
        for (const auto &kv_pair : yaml) {
          if (kv_pair.first.Type() != YAML::NodeType::Scalar) {
            std::cerr << "YAML-Cpp error: map key for field `" << field->name()
                      << "` is not a scalar!" << std::endl;
            success_bit = false;
            continue;
          }
          string kv_key = kv_pair.first.Scalar();
          proto::Message *sub = refl->AddMessage(out, field);
          if (v_field && YamlFitsValueField(kv_pair.second, cache, v_field)) {
            // This is a canonical YAML representation of a key-value proto.
            // We've determined the two fields to be a key (k_field) and value
            // (v_field), and we've verified that the value of the YAML
            // key-value pair is compatible with the value field of our Proto
            // key-value pair. Now just fit the YAML value to the Proto value.
            FitNodeToField(kv_pair.second, v_field, sub);
          } else if (kv_pair.second.Type() != YAML::NodeType::Map) {
            std::cerr << "Value of map entry `" << kv_key << "` for field `"
                      << field->name() << "`is not a nested  map. This field "
                         "is repeated; if you meant for these values to appear "
                         "in the first entry, add a - before the mapping in "
                         "your YAML file." << std::endl;
            success_bit = false;
            continue;
          } else {  // Only the key was provided by the map struct.
            // Fit the rest of the fields to it directly.
            FitMapToMessage(kv_pair.second, sub);
          }
          if (sub->GetReflection()->HasField(*sub, k_field)) {
            std::cerr << "Redundant assignment of key `" << kv_key
                      << "` in `" << cache.message_name << "` entry."
                      << std::endl;
          }
          // Update the key from our map.
          PutScalarToField(kv_pair.first, k_field, sub);
        }
        return success_bit;
      } else {
        return FitMapToMessage(yaml, refl->MutableMessage(out, field));
      }
    }
    case YAML::NodeType::Undefined: {
      std::cerr << "YAML node is undefined" << std::endl;
      return false;
    }
    default: {
      std::cerr << "Internal error: YAML node is unknown!" << std::endl;
      return false;
    }
  }
}

}  // namespace

bool egm::DecodeYaml(const YAML::Node &yaml, proto::Message *out) {
  if (!out) return false;
  DecodeHelper decoder;
  switch (yaml.Type()) {
    case YAML::NodeType::Null: {
      return false;
    }
    case YAML::NodeType::Scalar: {
      std::cerr << "Scalar value given for Message "
                << out->GetDescriptor()->name() << std::endl;
      return false;
    }
    case YAML::NodeType::Sequence: {
      std::cerr << "Sequence value given for Message "
                << out->GetDescriptor()->name() << std::endl;
      return false;
    }
    case YAML::NodeType::Map: {
      return decoder.FitMapToMessage(yaml, out);
    }
    case YAML::NodeType::Undefined: {
      std::cerr << "YAML node is undefined" << std::endl;
      return false;
    }
  }
  return true;
}
