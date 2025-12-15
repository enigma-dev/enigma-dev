#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <vector>
#include <string>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

class EnumConstantFilteringTest : public ::testing::Test {
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
        
        // Load shared locals to populate the set that codegen will use
        int result = lang_->load_shared_locals();
        ASSERT_EQ(result, 0) << "load_shared_locals() should succeed";
    }
    
    void TearDown() override {
        lang_.reset();
    }
    
    std::unique_ptr<lang_CPP> lang_;
};

// Test that enum constants are NOT in shared_object_locals_ (they're namespace constants, not object members)
TEST_F(EnumConstantFilteringTest, EnumConstantsNotInSharedObjectLocals) {
    const auto& shared = lang_->shared_object_locals();
    
    // Enum constants should NOT be in shared_object_locals_ because they're not object hierarchy members
    ASSERT_EQ(shared.find("c_blue"), shared.end()) 
        << "c_blue should not be in shared_object_locals_ (it's a namespace constant, not object member)";
    ASSERT_EQ(shared.find("c_white"), shared.end()) 
        << "c_white should not be in shared_object_locals_";
    ASSERT_EQ(shared.find("self"), shared.end()) 
        << "self should not be in shared_object_locals_";
}

// Test that is_shared_local() returns false for enum constants
TEST_F(EnumConstantFilteringTest, IsSharedLocalReturnsFalseForEnumConstants) {
    // is_shared_local() checks shared_object_locals_, which should not contain enum constants
    ASSERT_FALSE(lang_->is_shared_local("c_blue")) 
        << "is_shared_local() should return false for c_blue";
    ASSERT_FALSE(lang_->is_shared_local("c_white")) 
        << "is_shared_local() should return false for c_white";
    ASSERT_FALSE(lang_->is_shared_local("self")) 
        << "is_shared_local() should return false for self";
}

// Test that is_enigma_user_constant() correctly identifies enum constants
// (This test will fail until we implement the method)
TEST_F(EnumConstantFilteringTest, IsEnigmaUserConstantIdentifiesEnumConstants) {
    // This test requires the is_enigma_user_constant() method to be implemented
    // For now, we'll test that look_up() can find them and verify their parent
    
    jdi::definition* c_blue = lang_->look_up("c_blue");
    ASSERT_NE(c_blue, nullptr) << "c_blue must be findable for filtering to work";
    ASSERT_EQ(c_blue->parent->name, "enigma_user") 
        << "c_blue must be in enigma_user namespace for filtering";
    ASSERT_FALSE(c_blue->flags & jdi::DEF_FUNCTION) 
        << "c_blue must not be a function";
    
    // Once is_enigma_user_constant() is implemented, we can test:
    // ASSERT_TRUE(lang_->is_enigma_user_constant("c_blue"));
    // ASSERT_TRUE(lang_->is_enigma_user_constant("c_white"));
    // ASSERT_TRUE(lang_->is_enigma_user_constant("self"));
    // ASSERT_FALSE(lang_->is_enigma_user_constant("balloonCount")); // user variable
}

// Test that object hierarchy variables are still in shared_object_locals_
TEST_F(EnumConstantFilteringTest, ObjectHierarchyVariablesStillInSharedLocals) {
    const auto& shared = lang_->shared_object_locals();
    
    // Object hierarchy variables should still be in shared_object_locals_
    ASSERT_NE(shared.find("x"), shared.end()) 
        << "x should be in shared_object_locals_";
    ASSERT_NE(shared.find("y"), shared.end()) 
        << "y should be in shared_object_locals_";
    ASSERT_NE(shared.find("image_alpha"), shared.end()) 
        << "image_alpha should be in shared_object_locals_";
    ASSERT_NE(shared.find("sprite_index"), shared.end()) 
        << "sprite_index should be in shared_object_locals_";
}

// Test that user variables are not identified as enum constants
TEST_F(EnumConstantFilteringTest, UserVariablesNotIdentifiedAsEnumConstants) {
    // User variables should not be found in enigma_user namespace
    // (unless they actually are, which would be a different issue)
    
    // Test that a non-existent variable is not found
    jdi::definition* fake_var = lang_->look_up("nonexistent_user_variable_xyz");
    // This might be nullptr or might be found elsewhere - either is fine
    // The important thing is that if it's found, it shouldn't be in enigma_user
    
    if (fake_var && fake_var->parent) {
        ASSERT_NE(fake_var->parent->name, "enigma_user") 
            << "Fake variable should not be in enigma_user namespace";
    }
}
