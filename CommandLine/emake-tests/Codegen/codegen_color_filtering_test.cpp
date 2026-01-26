#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

class CodegenColorFilteringTest : public ::testing::Test {
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

// Test if color exists in enigma_user namespace
TEST_F(CodegenColorFilteringTest, CheckColorInEnigmaUserNamespace) {
    // Check if color is found in enigma_user namespace
    jdi::definition* color_def = lang_->look_up("color");
    
    if (color_def) {
        // If found, check its parent scope
        if (color_def->parent) {
            std::cout << "DEBUG: color found with parent: " << color_def->parent->name << std::endl;
            std::cout << "DEBUG: color flags: 0x" << std::hex << color_def->flags << std::dec << std::endl;
            
            if (color_def->parent->name == "enigma_user") {
                // It's in enigma_user namespace
                // Check if it's a function (parameter) or a constant
                if (color_def->flags & jdi::DEF_FUNCTION) {
                    std::cout << "DEBUG: color is a function in enigma_user" << std::endl;
                } else {
                    std::cout << "DEBUG: color is a non-function member in enigma_user" << std::endl;
                }
            }
        }
    } else {
        std::cout << "DEBUG: color not found by look_up()" << std::endl;
    }
    
    // Test is_enigma_user_constant
    bool is_constant = lang_->is_enigma_user_constant("color");
    std::cout << "DEBUG: is_enigma_user_constant('color') = " << (is_constant ? "true" : "false") << std::endl;
    
    // This test documents the current state - the actual filtering decision
    // will be made based on investigation results
}

// Test that color is correctly identified if it's a built-in constant
TEST_F(CodegenColorFilteringTest, ColorFilteringLogic) {
    // If color is a built-in constant in enigma_user namespace, it should be filtered
    // If it's a user variable, it should remain
    
    // Check if it's in enigma_user namespace
    bool is_enigma_user = lang_->is_enigma_user_constant("color");
    
    // The filtering logic should use is_enigma_user_constant() if it returns true
    // This test documents the expected behavior
    if (is_enigma_user) {
        std::cout << "DEBUG: color is identified as enigma_user constant - should be filtered" << std::endl;
    } else {
        std::cout << "DEBUG: color is NOT identified as enigma_user constant - may be user variable" << std::endl;
    }
}
