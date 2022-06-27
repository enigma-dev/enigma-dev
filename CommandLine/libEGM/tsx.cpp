#include "tsx.h"
#include "action.h"
#include "strings_util.h"

#include <functional>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using CppType = google::protobuf::FieldDescriptor::CppType;

namespace egm{

TSXTilesetLoader::TSXTilesetLoader(buffers::TreeNode *root, const fs::path &fPath) : tsxPath(fPath) {
  root->set_name("/");
  nodes.push_back(root);
  tiledEnigmaResourceNameMap["tileset"] = "background";
}

// constructor which fills up the background folder of a pre-existing project
TSXTilesetLoader::TSXTilesetLoader(const fs::path &fPath,
                 std::vector<buffers::TreeNode *> &existingTreeNode,
                 buffers::TreeNode *existingBgFolderRef) : tsxPath(fPath) {
  nodes = existingTreeNode;
  backgroundFolderRef = existingBgFolderRef;
  tiledEnigmaResourceNameMap["tileset"] = "background";
}

bool TSXTilesetLoader::for_each(pugi::xml_node& xmlNode) {
  std::string resName = xmlNode.attribute("name").value();

  // resources for xml nodes which are not present in tiledEnigmaResourceNameMap are not created
  if(tiledEnigmaResourceNameMap.find(xmlNode.name()) == tiledEnigmaResourceNameMap.end()) {
    std::cout << "Unsupported resource type: " << xmlNode.name() << std::endl;
    return true;
  }

  // TODO: Add folder structure to separate different types of resources

  std::string resType = tiledEnigmaResourceNameMap[xmlNode.name()];

  // add new resource according to resType
  if(backgroundFolderRef == NULL){
    backgroundFolderRef = nodes.back()->mutable_folder()->add_children();
    std::string fixName = resType + 's'; // add an 's' at the end
    fixName[0] -= 32; // make first character capital
    backgroundFolderRef->set_name(fixName);
  }

  pugi::xml_node imgNode = xmlNode.child("image");
  if(imgNode.empty()){
    pugi::xml_object_range<pugi::xml_named_node_iterator> tileChildrenItr = xmlNode.children("tile");

    std::cout<<"Adding tile as background"<<std::endl;

    for(pugi::xml_node tileChild : tileChildrenItr){
      buffers::TreeNode *protoNode = backgroundFolderRef->mutable_folder()->add_children();
      std::string tileId = tileChild.attribute("id").value();
      protoNode->set_name(resName+tileId);
      AddResource(protoNode, resType, tileChild);
    }
  }
  else{
    buffers::TreeNode *protoNode = backgroundFolderRef->mutable_folder()->add_children();
    protoNode->set_name(resName);
    AddResource(protoNode, resType, xmlNode);
  }


  return true;
}

void TSXTilesetLoader::AddResource(buffers::TreeNode *protoNode, std::string resType, pugi::xml_node &xmlNode) {
  using buffers::TreeNode;
  using FactoryFunction = std::function<google::protobuf::Message *(TreeNode*)>;
  using FactoryMap = std::unordered_map<std::string, FactoryFunction>;

  static const FactoryMap factoryMap({
    { "sprite", &TreeNode::mutable_sprite },
    { "sound", &TreeNode::mutable_sound },
    { "background", &TreeNode::mutable_background },
    { "path", &TreeNode::mutable_path },
    { "script", &TreeNode::mutable_script },
    { "shader", &TreeNode::mutable_shader },
    { "font", &TreeNode::mutable_font },
    { "timeline", &TreeNode::mutable_timeline },
    { "object", &TreeNode::mutable_object },
    { "room", &TreeNode::mutable_room },
    { "datafile", &TreeNode::mutable_include },
    { "Config", &TreeNode::mutable_settings },
  });

  auto createFunc = factoryMap.find(resType);
  if (createFunc != factoryMap.end()) {
      auto *res = createFunc->second(protoNode);
      PackRes(xmlNode, res, resType);
      return;
  }
  protoNode->mutable_unknown();
  errStream << "Unsupported resource type: " << resType << " " << xmlNode.value() << std::endl;
}

void TSXTilesetLoader::PackRes(pugi::xml_node &xmlNode, google::protobuf::Message *m,
                               const std::string& resType, std::string fieldPrefix) {
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  const google::protobuf::Reflection *refl = m->GetReflection();

  // for tsx tilesets use_as_tileset will always be true
  if(resType == "background"){
    const google::protobuf::FieldDescriptor *useAsTilesetField = desc->FindFieldByName("use_as_tileset");
    if(useAsTilesetField){
      refl->SetBool(m, useAsTilesetField, true);
    }
  }

  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    const google::protobuf::OneofDescriptor *oneof = field->containing_oneof();

    if (oneof && refl->HasOneof(*m, oneof)) continue;

    const google::protobuf::FieldOptions opts = field->options();

    // tmx_option_string is used to fetch correct attribute from current xml node
    std::string tsxPropertyName = fieldPrefix + opts.GetExtension(buffers::tmx);

    pugi::xml_attribute attr;

    // TODO: needs improvement
    // if forward slash '/' is found in tmx_option_string then correct attribute is present child node
    // for example in Background.proto fields with options [(tmx) = "image/source"] has a forward
    // slash in it and it indicates that in .tsx file, source should be fectched from child image node
    std::size_t splitPos = tsxPropertyName.find("/");
    if(splitPos != std::string::npos){
      const std::string childNodeName = tsxPropertyName.substr(0, splitPos);
      const std::string childNodeAttrName = tsxPropertyName.substr(splitPos + 1);
      pugi::xml_node child = xmlNode.child(childNodeName.c_str());
      if(!child.empty()){
        attr = child.attribute(childNodeAttrName.c_str());
      }
    }
    // otherwise get the attribute directly
    else {
      attr = xmlNode.attribute(tsxPropertyName.c_str());
    }

    if(attr.empty())
      continue;

    switch (field->cpp_type()) {
      case CppType::CPPTYPE_MESSAGE: {
        google::protobuf::Message *msg = refl->MutableMessage(m, field);
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
          std::string parentDirPath = tsxPath.parent_path().string()+"/";
          value = parentDirPath + attr.as_string();
        }
        else{
          value = attr.as_string();
        }

        refl->SetString(m, field, value);
        break;
      }
    }
  }
}

std::unique_ptr<buffers::Project> TSXFileFormat::LoadProject(const fs::path& fPath) const {

  pugi::xml_document doc;
  if(!doc.load_file(fPath.c_str())) return nullptr;

  std::unique_ptr<buffers::Project> proj = std::make_unique<buffers::Project>();
  buffers::Game* game = proj->mutable_game();

  TSXTilesetLoader background_walker(game->mutable_root(), fPath);
  doc.traverse(background_walker);

  return proj;
}

} // namespace egm
