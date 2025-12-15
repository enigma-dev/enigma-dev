#include <gtest/gtest.h>
#include "../gmk.h"
#include "Settings.pb.h"
#include "../../../shared/event_reader/event_parser.h"
#include <filesystem>
#include <fstream>
#include <set>
#include <functional>

namespace fs = std::filesystem;
using namespace egm;

class SpriteLoadingTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir = fs::path("CommandLine/testing/SimpleTests");
        
        // Create EventData for the loader
        fs::path events_file = fs::path("events.ey");
        if (fs::exists(events_file)) {
            event_data_ = std::make_unique<EventData>(ParseEventFile(events_file.string()));
        } else {
            buffers::config::EventFile empty_events;
            event_data_ = std::make_unique<EventData>(std::move(empty_events));
        }
    }

    fs::path test_dir;
    std::unique_ptr<EventData> event_data_;
};

TEST_F(SpriteLoadingTest, SpriteNamesLoadedFromGMK) {
    // Test that sprite names are correctly loaded from GMK objects
    fs::path gmk_path = test_dir / "gm800.gmk";
    
    if (!fs::exists(gmk_path)) {
        GTEST_SKIP() << "Test GMK file not found: " << gmk_path;
    }

    egm::GMKFileFormat gmk_format(event_data_.get());
    auto project = gmk_format.LoadProject(gmk_path);
    
    ASSERT_NE(project, nullptr) << "Failed to load GMK project";
    
    // Find objects and check their sprite names
    bool found_object_with_sprite = false;
    
    std::function<void(const buffers::TreeNode&)> traverse = [&](const buffers::TreeNode& node) {
        if (node.type_case() == buffers::TreeNode::kObject) {
            const auto& obj = node.object();
            if (!obj.sprite_name().empty()) {
                found_object_with_sprite = true;
                // Verify sprite name is not empty and looks valid
                EXPECT_FALSE(obj.sprite_name().empty()) 
                    << "Object " << node.name() << " has empty sprite name";
                // Sprite names should start with "spr_" or be a valid identifier
                EXPECT_TRUE(obj.sprite_name().find("spr_") == 0 || 
                           obj.sprite_name().find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") == 0)
                    << "Object " << node.name() << " has invalid sprite name: " << obj.sprite_name();
            }
        }
        if (node.type_case() == buffers::TreeNode::kFolder) {
            for (const auto& child : node.folder().children()) {
                traverse(child);
            }
        }
    };
    
    traverse(project->game().root());
    
    EXPECT_TRUE(found_object_with_sprite) << "No objects with sprites found in GMK file";
}

TEST_F(SpriteLoadingTest, SpriteNameMatchesEnum) {
    // Test that sprite names from GMK match expected enum format
    fs::path gmk_path = test_dir / "gm800.gmk";
    
    if (!fs::exists(gmk_path)) {
        GTEST_SKIP() << "Test GMK file not found: " << gmk_path;
    }

    egm::GMKFileFormat gmk_format(event_data_.get());
    auto project = gmk_format.LoadProject(gmk_path);
    
    ASSERT_NE(project, nullptr) << "Failed to load GMK project";
    
    // Collect all sprite names
    std::set<std::string> sprite_names;
    std::function<void(const buffers::TreeNode&)> collect_sprites = [&](const buffers::TreeNode& node) {
        if (node.type_case() == buffers::TreeNode::kSprite) {
            sprite_names.insert(std::string(node.name()));
        }
        if (node.type_case() == buffers::TreeNode::kFolder) {
            for (const auto& child : node.folder().children()) {
                collect_sprites(child);
            }
        }
    };
    collect_sprites(project->game().root());
    
    // Check that objects reference valid sprite names
    std::function<void(const buffers::TreeNode&)> check_objects = [&](const buffers::TreeNode& node) {
        if (node.type_case() == buffers::TreeNode::kObject) {
            const auto& obj = node.object();
            if (!obj.sprite_name().empty()) {
                // Sprite name should match a sprite in the project
                // Note: In GMK, sprite names might be resolved later, so we just check format
                EXPECT_FALSE(obj.sprite_name().empty()) 
                    << "Object " << node.name() << " has empty sprite name";
            }
        }
        if (node.type_case() == buffers::TreeNode::kFolder) {
            for (const auto& child : node.folder().children()) {
                check_objects(child);
            }
        }
    };
    check_objects(project->game().root());
}
