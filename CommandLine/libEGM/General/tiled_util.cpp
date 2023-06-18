#include "tiled_util.h"

using CppType = google::protobuf::FieldDescriptor::CppType;

void AddTiledResource(buffers::TreeNode *protoNode, std::string resType, const pugi::xml_node &xmlNode,
                 std::unordered_map<std::string, int> &resourceTypeIdCountMap, const std::filesystem::path& tiledFilePath) {
  using buffers::TreeNode;
  using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
  using FactoryMap = std::unordered_map<std::string, FactoryFunction>;

  static const FactoryMap factoryMap({
    { "sprite",     &TreeNode::mutable_sprite       },
    { "sound",      &TreeNode::mutable_sound        },
    { "background", &TreeNode::mutable_background   },
    { "path",       &TreeNode::mutable_path         },
    { "script",     &TreeNode::mutable_script       },
    { "shader",     &TreeNode::mutable_shader       },
    { "font",       &TreeNode::mutable_font         },
    { "timeline",   &TreeNode::mutable_timeline     },
    { "object",     &TreeNode::mutable_object       },
    { "room",       &TreeNode::mutable_egm_room     },
    { "datafile",   &TreeNode::mutable_include      },
    { "Config",     &TreeNode::mutable_settings     }
  });

  auto createFunc = factoryMap.find(resType);
  if (createFunc != factoryMap.end()) {
      auto *res = createFunc->second(protoNode);
      PackTiledRes(xmlNode, res, resourceTypeIdCountMap, tiledFilePath);
      return;
  }
  protoNode->mutable_unknown();
  egm::errStream << "Unsupported resource type: " << resType << " " << xmlNode.value() << std::endl;
}

void PackTiledRes(const pugi::xml_node &xmlNode, google::protobuf::Message *m,
             std::unordered_map<std::string, int> &resourceTypeIdCountMap, const std::filesystem::path& tiledFilePath) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();

  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();

    if (oneof && refl->HasOneof(*m, oneof)) continue;

    const google::protobuf::FieldOptions opts = field->options();

    // WIP Changes: Whether to use extension or field name or both for tsx property name?
    /*if(!opts.HasExtension(buffers::tmx))
      continue;

    // tmx_option_string is used to fetch correct attribute from current xml node
    std::string tsxPropertyName = opts.GetExtension(buffers::tmx);*/

    std::string tsxPropertyName = "";
    if(opts.HasExtension(buffers::tmx))
      tsxPropertyName = opts.GetExtension(buffers::tmx);
    else
      tsxPropertyName = field->name();

    pugi::xml_attribute attr = xmlNode.attribute(tsxPropertyName.c_str());

    if(field->name() == "id" && opts.HasExtension(buffers::id_start)) {
      // not sure what Getextension returns in cases when id_start extension is absent
      int id = opts.GetExtension(buffers::id_start) + resourceTypeIdCountMap[m->GetTypeName()]++;
      refl->SetInt32(m, field, id);
    }
    else if(!attr.empty()) {
      switch (field->cpp_type()) {
        case CppType::CPPTYPE_MESSAGE: {
          // google::protobuf::Message *msg = refl->MutableMessage(m, field);
          // TODO: handle message case
          // PackRes(resMap, dir, ids, child, msg, depth + 1);
          break;
        }
        case CppType::CPPTYPE_INT32: {
          refl->SetInt32(m, field, attr.as_int());
          break;
        }
        case CppType::CPPTYPE_INT64: {
          refl->SetInt64(m, field, attr.as_int());
          break;
        }
        case CppType::CPPTYPE_UINT32: {
          refl->SetUInt32(m, field, attr.as_uint());
          break;
        }
        case CppType::CPPTYPE_UINT64: {
          refl->SetUInt64(m, field, attr.as_uint());
          break;
        }
        case CppType::CPPTYPE_DOUBLE: {
          refl->SetDouble(m, field, attr.as_double());
          break;
        }
        case CppType::CPPTYPE_FLOAT: {
          refl->SetFloat(m, field, attr.as_float());
          break;
        }
        case CppType::CPPTYPE_BOOL: {
          refl->SetBool(m, field, (attr.as_int() != 0));
          break;
        }
        case CppType::CPPTYPE_ENUM: {
          refl->SetEnum(m, field, field->enum_type()->FindValueByNumber(attr.as_int()));
          break;
        }
        case CppType::CPPTYPE_STRING: {
          const bool isFilePath = opts.GetExtension(buffers::file_path);
          std::string value;
          if(isFilePath) {
            std::string parentDirPath = tiledFilePath.parent_path().string()+"/";
            value = parentDirPath + attr.as_string();
          }
          else {
            value = attr.as_string();
          }

          refl->SetString(m, field, value);
          break;
        }
      }
    }
  }
}
