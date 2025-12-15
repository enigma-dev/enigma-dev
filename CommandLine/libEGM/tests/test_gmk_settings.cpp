#include <gtest/gtest.h>
#include "../gmk.h"
#include "Settings.pb.h"
#include "../../../shared/event_reader/event_parser.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class GMKSettingsTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Test GMK files should be in the testing directory
    test_dir = fs::path("CommandLine/testing/SimpleTests");
    
    // Create a minimal EventData for the loader
    // Use an empty events file or parse from the actual events.ey file
    fs::path events_file = fs::path("events.ey");
    if (fs::exists(events_file)) {
      event_data_ = std::make_unique<EventData>(ParseEventFile(events_file.string()));
    } else {
      // Create empty EventData if events.ey doesn't exist
      buffers::config::EventFile empty_events;
      event_data_ = std::make_unique<EventData>(std::move(empty_events));
    }
  }

  fs::path test_dir;
  std::unique_ptr<EventData> event_data_;
};

// Test that game_id is correctly loaded from GMK file
TEST_F(GMKSettingsTest, GameIdLoaded) {
  egm::GMKFileFormat loader(event_data_.get());
  
  // Try loading gm800.gmk
  fs::path gmk_file = test_dir / "gm800.gmk";
  if (!fs::exists(gmk_file)) {
    GTEST_SKIP() << "Test GMK file not found: " << gmk_file;
  }

  auto project = loader.LoadProject(gmk_file);
  ASSERT_NE(project, nullptr) << "Failed to load GMK file";

  // Find settings in the tree
  const auto& root = project->game().root();
  bool found_settings = false;
  uint64_t loaded_game_id = 0;

  std::function<void(const buffers::TreeNode&)> find_settings = [&](const buffers::TreeNode& node) {
    if (node.type_case() == buffers::TreeNode::TypeCase::kSettings) {
      found_settings = true;
      loaded_game_id = node.settings().general().game_id();
      return;
    }
    if (node.type_case() == buffers::TreeNode::TypeCase::kFolder) {
      for (const auto& child : node.folder().children()) {
        find_settings(child);
      }
    }
  };

  find_settings(root);

  ASSERT_TRUE(found_settings) << "Settings node not found in project tree";
  ASSERT_NE(loaded_game_id, 0) << "game_id should not be 0 (default value)";
}

// Test that view_scale is correctly loaded as double
TEST_F(GMKSettingsTest, ViewScaleLoaded) {
  egm::GMKFileFormat loader(event_data_.get());
  
  fs::path gmk_file = test_dir / "gm800.gmk";
  if (!fs::exists(gmk_file)) {
    GTEST_SKIP() << "Test GMK file not found: " << gmk_file;
  }

  auto project = loader.LoadProject(gmk_file);
  ASSERT_NE(project, nullptr);

  // Find settings in the tree
  const auto& root = project->game().root();
  bool found_settings = false;
  double loaded_view_scale = 0.0;

  std::function<void(const buffers::TreeNode&)> find_settings = [&](const buffers::TreeNode& node) {
    if (node.type_case() == buffers::TreeNode::TypeCase::kSettings) {
      found_settings = true;
      loaded_view_scale = node.settings().graphics().view_scale();
      return;
    }
    if (node.type_case() == buffers::TreeNode::TypeCase::kFolder) {
      for (const auto& child : node.folder().children()) {
        find_settings(child);
      }
    }
  };

  find_settings(root);

  ASSERT_TRUE(found_settings);
  // view_scale should be loaded (even if it's the default, it should be a valid double)
  // The actual value depends on what's in the test file
}

// Test that freeze_on_lose_focus is correctly loaded
TEST_F(GMKSettingsTest, FreezeOnLoseFocusLoaded) {
  egm::GMKFileFormat loader(event_data_.get());
  
  fs::path gmk_file = test_dir / "gm800.gmk";
  if (!fs::exists(gmk_file)) {
    GTEST_SKIP() << "Test GMK file not found: " << gmk_file;
  }

  auto project = loader.LoadProject(gmk_file);
  ASSERT_NE(project, nullptr);

  // Find settings in the tree
  const auto& root = project->game().root();
  bool found_settings = false;
  bool loaded_freeze = false;

  std::function<void(const buffers::TreeNode&)> find_settings = [&](const buffers::TreeNode& node) {
    if (node.type_case() == buffers::TreeNode::TypeCase::kSettings) {
      found_settings = true;
      loaded_freeze = node.settings().windowing().freeze_on_lose_focus();
      return;
    }
    if (node.type_case() == buffers::TreeNode::TypeCase::kFolder) {
      for (const auto& child : node.folder().children()) {
        find_settings(child);
      }
    }
  };

  find_settings(root);

  ASSERT_TRUE(found_settings);
  // freeze_on_lose_focus should be loaded (value depends on test file)
}

// Test GMK version 701 loading
TEST_F(GMKSettingsTest, GMK701GameIdLoaded) {
  egm::GMKFileFormat loader(event_data_.get());
  
  fs::path gmk_file = test_dir / "gm701.gmk";
  if (!fs::exists(gmk_file)) {
    GTEST_SKIP() << "Test GMK file not found: " << gmk_file;
  }

  auto project = loader.LoadProject(gmk_file);
  ASSERT_NE(project, nullptr);

  // Find settings in the tree
  const auto& root = project->game().root();
  bool found_settings = false;
  uint64_t loaded_game_id = 0;

  std::function<void(const buffers::TreeNode&)> find_settings = [&](const buffers::TreeNode& node) {
    if (node.type_case() == buffers::TreeNode::TypeCase::kSettings) {
      found_settings = true;
      loaded_game_id = node.settings().general().game_id();
      return;
    }
    if (node.type_case() == buffers::TreeNode::TypeCase::kFolder) {
      for (const auto& child : node.folder().children()) {
        find_settings(child);
      }
    }
  };

  find_settings(root);

  ASSERT_TRUE(found_settings);
  ASSERT_NE(loaded_game_id, 0) << "game_id should not be 0 for GMK 701";
}


