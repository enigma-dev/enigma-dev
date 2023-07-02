#include "tmx.h"

#include <gtest/gtest.h>

using namespace egm;

using TypeCase = buffers::TreeNode::TypeCase;

static void readBackgrounds(buffers::TreeNode *root, std::unordered_map<std::string,
                                  buffers::resources::Background *> &backgroundNamePtrMap) {
  if(root->type_case() == TypeCase::kBackground) {
    if(root->has_background() && root->background().has_name()) {
      backgroundNamePtrMap[root->mutable_background()->mutable_name()->data()] = root->mutable_background();
      return;
    }
  }

  for (int i=0; i < root->folder().children_size(); ++i) {
    readBackgrounds(root->mutable_folder()->mutable_children(i), backgroundNamePtrMap);
  }
}

void readRoom(buffers::TreeNode *root, buffers::resources::EGMRoom *&egmRoom) {
  if(root->type_case() == TypeCase::kRoom) {
      egmRoom = root->mutable_room();
      return;
  }

  for (int i=0; i < root->folder().children_size(); ++i) {
    readRoom(root->mutable_folder()->mutable_children(i), egmRoom);
  }
}

TEST(TiledImporterTest, CheckMapWithObjects) {
  TMXFileFormat tmxFileFormat(NULL);
  const fs::path fPath("./CommandLine/emake-tests/data/sandbox.tmx");

  std::unique_ptr<buffers::Project> sandboxProject = tmxFileFormat.LoadProject(fPath);

  EXPECT_TRUE(sandboxProject);

  if(sandboxProject) {
    buffers::TreeNode *root = sandboxProject->mutable_game()->mutable_root();

    EXPECT_TRUE(root);

    if(root) {
      buffers::resources::EGMRoom *room = NULL;

      readRoom(root, room);
      EXPECT_TRUE(room);

      std::unordered_map<std::string, buffers::resources::Background *> backgroundNamePtrMap;
      readBackgrounds(root, backgroundNamePtrMap);

      EXPECT_NE(backgroundNamePtrMap.size(), 0);
      EXPECT_EQ(room->objectgroups_size(), 11);
      EXPECT_EQ(room->mutable_objectgroups(1)->id(), 10);
      EXPECT_EQ(room->mutable_objectgroups(1)->parallaxx(), 0.5);
      EXPECT_EQ(room->mutable_objectgroups(1)->objects_size(), 5);
      EXPECT_EQ(room->mutable_objectgroups(1)->mutable_objects(1)->id(), 91);
      EXPECT_EQ(room->mutable_objectgroups(1)->mutable_objects(1)->gid(), 2147483655);
      EXPECT_EQ(room->mutable_objectgroups(1)->mutable_objects(1)->x(), 373.939);
      EXPECT_EQ(room->mutable_objectgroups(1)->mutable_objects(1)->width(), 384);
      EXPECT_EQ(room->mutable_objectgroups(1)->mutable_objects(1)->height(), 128);
      // y was corrected for origin in RGM
      EXPECT_EQ(
            room->mutable_objectgroups(1)->mutable_objects(1)->y() + room->mutable_objectgroups(1)->mutable_objects(1)->height(),
            627.121);
      EXPECT_EQ(room->mutable_objectgroups(2)->mutable_objects(2)->rotation(), -10.4469);
    }
  }
}
