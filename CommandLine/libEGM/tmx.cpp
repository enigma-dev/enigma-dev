#include "tmx.h"
#include "tsx.h"

#include "pugixml.hpp"

using CppType = google::protobuf::FieldDescriptor::CppType;

namespace egm {
namespace {

class TMXMapLoader : public pugi::xml_tree_walker {
public:
  TMXMapLoader(buffers::TreeNode *root, const fs::path &fPath) : tmxPath(fPath) {
    root->set_name("/");
    nodes.push_back(root);
    tiledEnigmaResourceMap["map"] = "room";
    tiledEnigmaResourceMap["tileset"] = "background";
  }

private:
  std::vector<buffers::TreeNode *> nodes;
  fs::path tmxPath;
  std::unordered_map<std::string, std::string> tiledEnigmaResourceMap;
  std::unordered_map<std::string, buffers::TreeNode *> resourceFolderRefs;
  // TODO: Remove this hack after "folder name fix PR" gets merged
  int idx=0;

  virtual bool for_each(pugi::xml_node &xmlNode) {
    if(tiledEnigmaResourceMap.find(xmlNode.name()) == tiledEnigmaResourceMap.end()){
      errStream << "Unsupported resource type: " << xmlNode.name() << std::endl;
      return true;
    }

    std::string resType = tiledEnigmaResourceMap[xmlNode.name()];
    std::string name = xmlNode.attribute("name").value();
    // TODO: Remove this hack after "folder name fix PR" gets merged
    if(name.empty())
      name = resType+std::to_string(idx++);

    buffers::TreeNode *folderNode;
    if(resourceFolderRefs.find(resType) == resourceFolderRefs.end()){
      folderNode = nodes.back()->mutable_folder()->add_children();
      std::string fixName = resType + 's'; // add an 's' at the end
      fixName[0] -= 32; // make first character capital
      folderNode->set_name(fixName);
    }
    else
      folderNode = resourceFolderRefs[resType];

    if(folderNode == NULL){
      std::cout << "Folder with name \""<< resType <<"\" not found" << std::endl;
      return false;
    }

//    buffers::TreeNode *resNode = folderNode->mutable_folder()->add_children();
    // this node should be resource, not sure what it is right now
    // continue with the integration of AddResource, PackRes and/or PackBuffer

    if(resType == "background"){
      // if image attribute exist then this tileset is contained inside a map and is referring to an external tileset
      std::string imageSrc = xmlNode.attribute("source").value();
      if(!imageSrc.empty()){
        std::string parentDirPath = tmxPath.parent_path().string()+"/";
        std::string tsxPath = parentDirPath+imageSrc;

        // load tsx xml
        pugi::xml_document tilesetDoc;
        if(!tilesetDoc.load_file(tsxPath.c_str())){
          std::cout << "Could not found tsx file with path: "<< tsxPath << std::endl;
          return false;
        }

        TSXTilesetLoader background_walker(tsxPath, nodes, folderNode);

        tilesetDoc.traverse(background_walker);

        // if image child node exist with a non-empty source, then its a single image tileset
//        pugi::xml_node imgChild = tilesetDoc.child()
      }
    }
    else{
      buffers::TreeNode *resNode = folderNode->mutable_folder()->add_children();
      resNode->set_name(name);
      AddResource(resNode, resType, xmlNode);
    }

    return true;
  }

  void AddResource(buffers::TreeNode *protoNode, std::string resType, pugi::xml_node &xmlNode) {
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

  void PackRes(pugi::xml_node &xmlNode, google::protobuf::Message *m, const std::string& resType, std::string fieldPrefix = "") {
    if(resType == "room"){
      std::string naam = "objectgroup";
      pugi::xml_object_range<pugi::xml_named_node_iterator> child1 = xmlNode.children(naam.c_str());

      int count = 0;
      for(pugi::xml_node c : child1){
        count++;
      }

      if(count == 0)
        std::cout<<"node "<<naam<<" not found in child"<<std::endl;
      else
        std::cout<<count<<" "<<naam<<" child node(s) found. Depth:"<<depth()<<std::endl;
    }

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
      // if forward slash '/' is found in tmx_option_string then correct attribute is present in child node
      // for example in Background.proto fields with options [(tmx) = "image/source"] has a forward
      // slash in it and it indicates that in .tsx file, source should be fetched from child image node
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
            std::string parentDirPath = tmxPath.parent_path().string()+"/";
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
};

} // anonymous namespace

std::unique_ptr<buffers::Project> TMXFileFormat::LoadProject(const fs::path& fPath) const {
  std::cout<<"here tmx 2"<<std::endl;

  pugi::xml_document doc;
  if(!doc.load_file(fPath.c_str())) return nullptr;

  std::unique_ptr<buffers::Project> proj = std::make_unique<buffers::Project>();
  buffers::Game* game = proj->mutable_game();

  TMXMapLoader map_walker(game->mutable_root(), fPath);
  doc.traverse(map_walker);

  return proj;
}

} // namespace egm
