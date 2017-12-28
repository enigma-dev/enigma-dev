// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_PROJECT_H_
#define FLATBUFFERS_GENERATED_PROJECT_H_

#include "flatbuffers/flatbuffers.h"

#include "Background_generated.h"
#include "Font_generated.h"
#include "GameInformation_generated.h"
#include "Object_generated.h"
#include "Path_generated.h"
#include "Room_generated.h"
#include "Script_generated.h"
#include "Shader_generated.h"
#include "Sound_generated.h"
#include "Sprite_generated.h"
#include "Timeline_generated.h"
#include "Image_generated.h"
#include "Point_generated.h"

struct TreeNode;

struct Project;

struct TreeNode FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_FILE_NAME = 4,
    VT_IS_DIRECTORY = 6,
    VT_CHILDREN = 8
  };
  const flatbuffers::String *file_name() const {
    return GetPointer<const flatbuffers::String *>(VT_FILE_NAME);
  }
  bool is_directory() const {
    return GetField<uint8_t>(VT_IS_DIRECTORY, 0) != 0;
  }
  const flatbuffers::Vector<flatbuffers::Offset<TreeNode>> *children() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<TreeNode>> *>(VT_CHILDREN);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_FILE_NAME) &&
           verifier.Verify(file_name()) &&
           VerifyField<uint8_t>(verifier, VT_IS_DIRECTORY) &&
           VerifyOffset(verifier, VT_CHILDREN) &&
           verifier.Verify(children()) &&
           verifier.VerifyVectorOfTables(children()) &&
           verifier.EndTable();
  }
};

struct TreeNodeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_file_name(flatbuffers::Offset<flatbuffers::String> file_name) {
    fbb_.AddOffset(TreeNode::VT_FILE_NAME, file_name);
  }
  void add_is_directory(bool is_directory) {
    fbb_.AddElement<uint8_t>(TreeNode::VT_IS_DIRECTORY, static_cast<uint8_t>(is_directory), 0);
  }
  void add_children(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TreeNode>>> children) {
    fbb_.AddOffset(TreeNode::VT_CHILDREN, children);
  }
  explicit TreeNodeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  TreeNodeBuilder &operator=(const TreeNodeBuilder &);
  flatbuffers::Offset<TreeNode> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<TreeNode>(end);
    return o;
  }
};

inline flatbuffers::Offset<TreeNode> CreateTreeNode(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> file_name = 0,
    bool is_directory = false,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TreeNode>>> children = 0) {
  TreeNodeBuilder builder_(_fbb);
  builder_.add_children(children);
  builder_.add_file_name(file_name);
  builder_.add_is_directory(is_directory);
  return builder_.Finish();
}

inline flatbuffers::Offset<TreeNode> CreateTreeNodeDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *file_name = nullptr,
    bool is_directory = false,
    const std::vector<flatbuffers::Offset<TreeNode>> *children = nullptr) {
  return CreateTreeNode(
      _fbb,
      file_name ? _fbb.CreateString(file_name) : 0,
      is_directory,
      children ? _fbb.CreateVector<flatbuffers::Offset<TreeNode>>(*children) : 0);
}

struct Project FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_FILE_NAME = 4,
    VT_FILE_VERSION = 6,
    VT_TREE_ROOT = 8,
    VT_SPRITES = 10,
    VT_SOUNDS = 12,
    VT_BACKGROUNDS = 14,
    VT_PATHS = 16,
    VT_SCRIPTS = 18,
    VT_SHADERS = 20,
    VT_FONTS = 22,
    VT_TIMELINES = 24,
    VT_OBJECTS = 26,
    VT_ROOMS = 28
  };
  const flatbuffers::String *file_name() const {
    return GetPointer<const flatbuffers::String *>(VT_FILE_NAME);
  }
  int32_t file_version() const {
    return GetField<int32_t>(VT_FILE_VERSION, 0);
  }
  const TreeNode *tree_root() const {
    return GetPointer<const TreeNode *>(VT_TREE_ROOT);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Sprite>> *sprites() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Sprite>> *>(VT_SPRITES);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Sound>> *sounds() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Sound>> *>(VT_SOUNDS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Background>> *backgrounds() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Background>> *>(VT_BACKGROUNDS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Path>> *paths() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Path>> *>(VT_PATHS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Script>> *scripts() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Script>> *>(VT_SCRIPTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Shader>> *shaders() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Shader>> *>(VT_SHADERS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Font>> *fonts() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Font>> *>(VT_FONTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Timeline>> *timelines() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Timeline>> *>(VT_TIMELINES);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Object>> *objects() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Object>> *>(VT_OBJECTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Room>> *rooms() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Room>> *>(VT_ROOMS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_FILE_NAME) &&
           verifier.Verify(file_name()) &&
           VerifyField<int32_t>(verifier, VT_FILE_VERSION) &&
           VerifyOffset(verifier, VT_TREE_ROOT) &&
           verifier.VerifyTable(tree_root()) &&
           VerifyOffset(verifier, VT_SPRITES) &&
           verifier.Verify(sprites()) &&
           verifier.VerifyVectorOfTables(sprites()) &&
           VerifyOffset(verifier, VT_SOUNDS) &&
           verifier.Verify(sounds()) &&
           verifier.VerifyVectorOfTables(sounds()) &&
           VerifyOffset(verifier, VT_BACKGROUNDS) &&
           verifier.Verify(backgrounds()) &&
           verifier.VerifyVectorOfTables(backgrounds()) &&
           VerifyOffset(verifier, VT_PATHS) &&
           verifier.Verify(paths()) &&
           verifier.VerifyVectorOfTables(paths()) &&
           VerifyOffset(verifier, VT_SCRIPTS) &&
           verifier.Verify(scripts()) &&
           verifier.VerifyVectorOfTables(scripts()) &&
           VerifyOffset(verifier, VT_SHADERS) &&
           verifier.Verify(shaders()) &&
           verifier.VerifyVectorOfTables(shaders()) &&
           VerifyOffset(verifier, VT_FONTS) &&
           verifier.Verify(fonts()) &&
           verifier.VerifyVectorOfTables(fonts()) &&
           VerifyOffset(verifier, VT_TIMELINES) &&
           verifier.Verify(timelines()) &&
           verifier.VerifyVectorOfTables(timelines()) &&
           VerifyOffset(verifier, VT_OBJECTS) &&
           verifier.Verify(objects()) &&
           verifier.VerifyVectorOfTables(objects()) &&
           VerifyOffset(verifier, VT_ROOMS) &&
           verifier.Verify(rooms()) &&
           verifier.VerifyVectorOfTables(rooms()) &&
           verifier.EndTable();
  }
};

struct ProjectBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_file_name(flatbuffers::Offset<flatbuffers::String> file_name) {
    fbb_.AddOffset(Project::VT_FILE_NAME, file_name);
  }
  void add_file_version(int32_t file_version) {
    fbb_.AddElement<int32_t>(Project::VT_FILE_VERSION, file_version, 0);
  }
  void add_tree_root(flatbuffers::Offset<TreeNode> tree_root) {
    fbb_.AddOffset(Project::VT_TREE_ROOT, tree_root);
  }
  void add_sprites(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Sprite>>> sprites) {
    fbb_.AddOffset(Project::VT_SPRITES, sprites);
  }
  void add_sounds(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Sound>>> sounds) {
    fbb_.AddOffset(Project::VT_SOUNDS, sounds);
  }
  void add_backgrounds(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Background>>> backgrounds) {
    fbb_.AddOffset(Project::VT_BACKGROUNDS, backgrounds);
  }
  void add_paths(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Path>>> paths) {
    fbb_.AddOffset(Project::VT_PATHS, paths);
  }
  void add_scripts(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Script>>> scripts) {
    fbb_.AddOffset(Project::VT_SCRIPTS, scripts);
  }
  void add_shaders(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Shader>>> shaders) {
    fbb_.AddOffset(Project::VT_SHADERS, shaders);
  }
  void add_fonts(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Font>>> fonts) {
    fbb_.AddOffset(Project::VT_FONTS, fonts);
  }
  void add_timelines(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Timeline>>> timelines) {
    fbb_.AddOffset(Project::VT_TIMELINES, timelines);
  }
  void add_objects(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Object>>> objects) {
    fbb_.AddOffset(Project::VT_OBJECTS, objects);
  }
  void add_rooms(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Room>>> rooms) {
    fbb_.AddOffset(Project::VT_ROOMS, rooms);
  }
  explicit ProjectBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ProjectBuilder &operator=(const ProjectBuilder &);
  flatbuffers::Offset<Project> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Project>(end);
    return o;
  }
};

inline flatbuffers::Offset<Project> CreateProject(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> file_name = 0,
    int32_t file_version = 0,
    flatbuffers::Offset<TreeNode> tree_root = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Sprite>>> sprites = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Sound>>> sounds = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Background>>> backgrounds = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Path>>> paths = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Script>>> scripts = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Shader>>> shaders = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Font>>> fonts = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Timeline>>> timelines = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Object>>> objects = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Room>>> rooms = 0) {
  ProjectBuilder builder_(_fbb);
  builder_.add_rooms(rooms);
  builder_.add_objects(objects);
  builder_.add_timelines(timelines);
  builder_.add_fonts(fonts);
  builder_.add_shaders(shaders);
  builder_.add_scripts(scripts);
  builder_.add_paths(paths);
  builder_.add_backgrounds(backgrounds);
  builder_.add_sounds(sounds);
  builder_.add_sprites(sprites);
  builder_.add_tree_root(tree_root);
  builder_.add_file_version(file_version);
  builder_.add_file_name(file_name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Project> CreateProjectDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *file_name = nullptr,
    int32_t file_version = 0,
    flatbuffers::Offset<TreeNode> tree_root = 0,
    const std::vector<flatbuffers::Offset<Sprite>> *sprites = nullptr,
    const std::vector<flatbuffers::Offset<Sound>> *sounds = nullptr,
    const std::vector<flatbuffers::Offset<Background>> *backgrounds = nullptr,
    const std::vector<flatbuffers::Offset<Path>> *paths = nullptr,
    const std::vector<flatbuffers::Offset<Script>> *scripts = nullptr,
    const std::vector<flatbuffers::Offset<Shader>> *shaders = nullptr,
    const std::vector<flatbuffers::Offset<Font>> *fonts = nullptr,
    const std::vector<flatbuffers::Offset<Timeline>> *timelines = nullptr,
    const std::vector<flatbuffers::Offset<Object>> *objects = nullptr,
    const std::vector<flatbuffers::Offset<Room>> *rooms = nullptr) {
  return CreateProject(
      _fbb,
      file_name ? _fbb.CreateString(file_name) : 0,
      file_version,
      tree_root,
      sprites ? _fbb.CreateVector<flatbuffers::Offset<Sprite>>(*sprites) : 0,
      sounds ? _fbb.CreateVector<flatbuffers::Offset<Sound>>(*sounds) : 0,
      backgrounds ? _fbb.CreateVector<flatbuffers::Offset<Background>>(*backgrounds) : 0,
      paths ? _fbb.CreateVector<flatbuffers::Offset<Path>>(*paths) : 0,
      scripts ? _fbb.CreateVector<flatbuffers::Offset<Script>>(*scripts) : 0,
      shaders ? _fbb.CreateVector<flatbuffers::Offset<Shader>>(*shaders) : 0,
      fonts ? _fbb.CreateVector<flatbuffers::Offset<Font>>(*fonts) : 0,
      timelines ? _fbb.CreateVector<flatbuffers::Offset<Timeline>>(*timelines) : 0,
      objects ? _fbb.CreateVector<flatbuffers::Offset<Object>>(*objects) : 0,
      rooms ? _fbb.CreateVector<flatbuffers::Offset<Room>>(*rooms) : 0);
}

inline flatbuffers::TypeTable *TreeNodeTypeTable();

inline flatbuffers::TypeTable *ProjectTypeTable();

inline flatbuffers::TypeTable *TreeNodeTypeTable() {
  static flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 1, 0 }
  };
  static flatbuffers::TypeFunction type_refs[] = {
    TreeNodeTypeTable
  };
  static const char *names[] = {
    "file_name",
    "is_directory",
    "children"
  };
  static flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 3, type_codes, type_refs, nullptr, names, nullptr
  };
  return &tt;
}

inline flatbuffers::TypeTable *ProjectTypeTable() {
  static flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_INT, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 1, 1 },
    { flatbuffers::ET_SEQUENCE, 1, 2 },
    { flatbuffers::ET_SEQUENCE, 1, 3 },
    { flatbuffers::ET_SEQUENCE, 1, 4 },
    { flatbuffers::ET_SEQUENCE, 1, 5 },
    { flatbuffers::ET_SEQUENCE, 1, 6 },
    { flatbuffers::ET_SEQUENCE, 1, 7 },
    { flatbuffers::ET_SEQUENCE, 1, 8 },
    { flatbuffers::ET_SEQUENCE, 1, 9 },
    { flatbuffers::ET_SEQUENCE, 1, 10 }
  };
  static flatbuffers::TypeFunction type_refs[] = {
    TreeNodeTypeTable,
    SpriteTypeTable,
    SoundTypeTable,
    BackgroundTypeTable,
    PathTypeTable,
    ScriptTypeTable,
    ShaderTypeTable,
    FontTypeTable,
    TimelineTypeTable,
    ObjectTypeTable,
    RoomTypeTable
  };
  static const char *names[] = {
    "file_name",
    "file_version",
    "tree_root",
    "sprites",
    "sounds",
    "backgrounds",
    "paths",
    "scripts",
    "shaders",
    "fonts",
    "timelines",
    "objects",
    "rooms"
  };
  static flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 13, type_codes, type_refs, nullptr, names, nullptr
  };
  return &tt;
}

inline const Project *GetProject(const void *buf) {
  return flatbuffers::GetRoot<Project>(buf);
}

inline bool VerifyProjectBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Project>(nullptr);
}

inline void FinishProjectBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Project> root) {
  fbb.Finish(root);
}

#endif  // FLATBUFFERS_GENERATED_PROJECT_H_
