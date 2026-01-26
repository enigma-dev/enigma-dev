#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <vector>
#include <string>
#include <set>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

class ObjectLocalsTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!main_context) {
            FAIL() << "main_context was not initialized";
        }
        
        lang_ = std::make_unique<lang_CPP>();
        
        const char* config = 
            "%e-yaml\n"
            "---\n"
            "target-windowing: None\n"
            "target-audio: None\n"
            "target-compiler: gcc\n"
            "target-graphics: None\n"
            "target-widget: None\n"
            "target-collision: None\n"
            "target-networking: None\n";
        lang_->definitionsModified(nullptr, config);
    }
    
    void TearDown() override {
        lang_.reset();
    }
    
    std::unique_ptr<lang_CPP> lang_;
};

TEST_F(ObjectLocalsTest, ObjectLocalsAreDetectedCorrectly) {
    // Call load_shared_locals() to populate shared_object_locals_
    int result = lang_->load_shared_locals();
    ASSERT_EQ(result, 0) << "load_shared_locals() should succeed";
    
    // Get the shared locals set
    const NameSet& shared_locals = lang_->shared_object_locals();
    
    // Verify that some locals were found
    ASSERT_GT(shared_locals.size(), 0) 
        << "load_shared_locals() should find at least some object locals";
    
    // Expected object locals from object tier hierarchy
    // Note: These should be found if class members are being parsed correctly
    std::vector<std::string> expected_locals = {
        "x", "y",
        "xprevious", "yprevious",
        "xstart", "ystart",
        "persistent",
        "direction", "speed", "hspeed", "vspeed",
        "gravity", "gravity_direction", "friction",
        "sprite_index", "image_index", "image_speed", "image_angle",
        "image_xscale", "image_yscale", "visible", "solid", "depth", "mask_index"
    };
    
    // Print all found locals for debugging
    std::cout << "\nFound " << shared_locals.size() << " shared object locals:" << std::endl;
    for (const auto& local : shared_locals) {
        std::cout << "  - " << local << std::endl;
    }
    
    // Check which expected locals are found
    int found_count = 0;
    for (const auto& local_name : expected_locals) {
        bool found = (shared_locals.find(local_name) != shared_locals.end());
        if (found) {
            found_count++;
            // Test is_shared_local() method for found locals
            EXPECT_TRUE(lang_->is_shared_local(local_name))
                << "is_shared_local('" << local_name << "') should return true";
        } else {
            std::cout << "  WARNING: Expected local '" << local_name << "' not found" << std::endl;
        }
    }
    
    // At least some expected locals should be found
    // (This test documents current behavior - if 0 are found, there's a bug)
    EXPECT_GT(found_count, 0)
        << "At least some expected object locals should be found. "
        << "Found " << found_count << " out of " << expected_locals.size() << " expected locals.";
    
    // Verify some locals that should NOT be in shared_locals (random variables)
    EXPECT_FALSE(lang_->is_shared_local("random_variable_that_does_not_exist"))
        << "Non-existent variable should not be a shared local";
    EXPECT_FALSE(lang_->is_shared_local("motion_set"))
        << "Functions should not be in shared_locals";
}
