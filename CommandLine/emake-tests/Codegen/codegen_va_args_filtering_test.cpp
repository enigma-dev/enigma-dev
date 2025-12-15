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

class CodegenVAArgsFilteringTest : public ::testing::Test {
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

// Test that __VA_ARGS__ is not a built-in constant in enigma_user namespace
TEST_F(CodegenVAArgsFilteringTest, VAArgsNotInEnigmaUserNamespace) {
    // __VA_ARGS__ is a C preprocessor macro, not a constant in enigma_user namespace
    jdi::definition* va_args = lang_->look_up("__VA_ARGS__");
    // It might not be found at all, or if found, it shouldn't be in enigma_user
    if (va_args) {
        if (va_args->parent) {
            ASSERT_NE(va_args->parent->name, "enigma_user") 
                << "__VA_ARGS__ should not be in enigma_user namespace";
        }
    }
}

// Test that __VA_ARGS__ is not treated as an enigma_user constant
TEST_F(CodegenVAArgsFilteringTest, VAArgsNotEnigmaUserConstant) {
    // __VA_ARGS__ is a preprocessor macro handled by the macro system
    // It should not be identified as an enigma_user constant
    ASSERT_FALSE(lang_->is_enigma_user_constant("__VA_ARGS__")) 
        << "__VA_ARGS__ should not be identified as enigma_user constant";
}

// Note: Full integration test would require running actual codegen,
// which is complex to set up in unit tests. The filtering logic will be
// tested via integration tests with real projects.
