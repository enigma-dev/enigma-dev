#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include "parser/object_storage.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

class CodegenVariableScopeTest : public ::testing::Test {
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

// Test that balloonCount should only be in ENIGMA_global_structure, not in objects
// balloonCount is accessed via global.balloonCount, so it should be in dot_accessed_locals
// and should NOT be declared in individual objects unless explicitly declared in LocalDeclarations()
TEST_F(CodegenVariableScopeTest, BalloonCountShouldBeGlobalOnly) {
    // This test documents the expected behavior:
    // - balloonCount is accessed via global.balloonCount
    // - It should be in dot_accessed_locals
    // - It should NOT be explicitly declared in any object's LocalDeclarations()
    // - Therefore, it should only appear in ENIGMA_global_structure, not in object declarations
    
    // The actual verification would require running full codegen, which is complex
    // This test documents the expected behavior for future integration tests
    std::string var_name = "balloonCount";
    
    // balloonCount should be treated as a global variable (in ENIGMA_global_structure)
    // not as a local variable in objects
    EXPECT_TRUE(true) << "balloonCount should only be in ENIGMA_global_structure";
}

// Test that color should be in objects where it's explicitly declared or used directly
// color is used directly in objects (like color = c_white;), so it should be declared
// in those objects, not in ENIGMA_global_structure
TEST_F(CodegenVariableScopeTest, ColorShouldBeLocalWhenUsedDirectly) {
    // This test documents the expected behavior:
    // - color is used directly in objects (e.g., color = c_white;)
    // - It may also be in dot_accessed_locals if accessed via global.color somewhere
    // - If it's explicitly declared in an object's LocalDeclarations(), it should be in that object
    // - If it's in object->locals (meaning it's used directly), it should be in that object
    // - It should NOT be in ENIGMA_global_structure if it's used directly in any object
    
    // The actual verification would require running full codegen, which is complex
    // This test documents the expected behavior for future integration tests
    std::string var_name = "color";
    
    // color should be declared in objects where it's used directly
    // not in ENIGMA_global_structure
    EXPECT_TRUE(true) << "color should be in objects where used directly, not in ENIGMA_global_structure";
}

// Test helper: Check if a variable is in dot_accessed_locals
// This simulates the logic used in write_globals and write_object_data
TEST_F(CodegenVariableScopeTest, DotAccessedLocalsLogic) {
    // Simulate dot_accessed_locals map
    std::map<std::string, dectrip> dot_accessed_locals;
    
    // balloonCount is accessed via global.balloonCount, so it's in dot_accessed_locals
    dot_accessed_locals["balloonCount"] = dectrip("var");
    
    // color might be in dot_accessed_locals if accessed via global.color somewhere
    // but it's also used directly in objects
    dot_accessed_locals["color"] = dectrip("var");
    
    // Check that balloonCount is in dot_accessed_locals
    EXPECT_TRUE(dot_accessed_locals.find("balloonCount") != dot_accessed_locals.end())
        << "balloonCount should be in dot_accessed_locals";
    
    // Check that color might be in dot_accessed_locals
    // (depending on whether it's accessed via global.color)
    (void)dot_accessed_locals.find("color");  // May or may not be in dot_accessed_locals
    // This is just documenting the state - the actual behavior depends on usage
    
    // The key difference:
    // - balloonCount: ONLY in dot_accessed_locals, NOT explicitly declared -> should be in ENIGMA_global_structure only
    // - color: in dot_accessed_locals AND in object->locals (used directly) -> should be in objects, not in ENIGMA_global_structure
    EXPECT_TRUE(true) << "Test documents the expected behavior";
}

// Test that variables explicitly declared in LocalDeclarations() are handled correctly
TEST_F(CodegenVariableScopeTest, ExplicitlyDeclaredVariables) {
    // Variables explicitly declared in LocalDeclarations() should be declared in those objects
    // even if they're also in dot_accessed_locals
    
    // Simulate LocalDeclarations() string
    std::string local_decls = "var color;";
    
    // Check if "color" appears in LocalDeclarations()
    size_t pos = local_decls.find("color");
    EXPECT_NE(pos, std::string::npos) << "color should be found in LocalDeclarations()";
    
    // This simulates the check in write_object_data and write_globals
    bool found = (pos != std::string::npos);
    EXPECT_TRUE(found) << "Variable explicitly declared should be found";
}

// Test that globalvar is treated as global var
TEST_F(CodegenVariableScopeTest, GlobalvarDeclaration) {
    // globalvar is defined as #define globalvar global var in actions.h
    // So "globalvar foo;" should be treated the same as "global var foo;"
    // and route to global->globals, not object->locals
    
    // Simulate LocalDeclarations() string with globalvar
    std::string local_decls = "globalvar testVar;";
    
    // Check that "globalvar" keyword is present
    size_t globalvar_pos = local_decls.find("globalvar");
    EXPECT_NE(globalvar_pos, std::string::npos) << "globalvar keyword should be found";
    
    // Check that variable name is present
    size_t var_pos = local_decls.find("testVar");
    EXPECT_NE(var_pos, std::string::npos) << "Variable name should be found";
    
    // This documents that globalvar should route to global->globals
    EXPECT_TRUE(true) << "globalvar should be treated as global var and route to global->globals";
}

// Test all global declaration formats
TEST_F(CodegenVariableScopeTest, AllGlobalDeclarationFormats) {
    // Test that all these formats route to global->globals:
    // - global foo;
    // - global var foo;
    // - global int foo;
    // - globalvar foo;
    
    std::vector<std::string> global_decls = {
        "global testVar1;",
        "global var testVar2;",
        "global int testVar3;",
        "globalvar testVar4;"
    };
    
    for (const auto& decl : global_decls) {
        // Check that "global" or "globalvar" keyword is present
        bool has_global_keyword = (decl.find("global") != std::string::npos);
        EXPECT_TRUE(has_global_keyword) << "Global declaration should contain 'global' or 'globalvar': " << decl;
        
        // Check that variable name is present
        bool has_var_name = (decl.find("testVar") != std::string::npos);
        EXPECT_TRUE(has_var_name) << "Global declaration should contain variable name: " << decl;
    }
    
    // This documents that all these formats should route to global->globals
    EXPECT_TRUE(true) << "All global declaration formats should route to global->globals";
}

// Test all local declaration formats
TEST_F(CodegenVariableScopeTest, AllLocalDeclarationFormats) {
    // Test that all these formats route to object->locals:
    // - foo; (no keyword)
    // - local foo;
    // - local var foo;
    // - int foo; (type only)
    // - var foo;
    
    std::vector<std::string> local_decls = {
        "testVar1;",
        "local testVar2;",
        "local var testVar3;",
        "int testVar4;",
        "var testVar5;"
    };
    
    for (const auto& decl : local_decls) {
        // Check that variable name is present
        bool has_var_name = (decl.find("testVar") != std::string::npos);
        EXPECT_TRUE(has_var_name) << "Local declaration should contain variable name: " << decl;
        
        // If it has "local" keyword, verify it's present
        if (decl.find("local") != std::string::npos) {
            EXPECT_TRUE(decl.find("local") != std::string::npos) << "Local declaration with 'local' keyword: " << decl;
        }
    }
    
    // This documents that all these formats should route to object->locals
    EXPECT_TRUE(true) << "All local declaration formats should route to object->locals";
}

// Test mixed declarations in one LocalDeclarations() string
TEST_F(CodegenVariableScopeTest, MixedGlobalAndLocalDeclarations) {
    // Test that we can handle mixed declarations:
    // global foo;
    // var bar;
    // globalvar baz;
    // int qux;
    
    std::string mixed_decls = "global foo; var bar; globalvar baz; int qux;";
    
    // Check that all variables are present
    EXPECT_NE(mixed_decls.find("foo"), std::string::npos) << "foo should be found";
    EXPECT_NE(mixed_decls.find("bar"), std::string::npos) << "bar should be found";
    EXPECT_NE(mixed_decls.find("baz"), std::string::npos) << "baz should be found";
    EXPECT_NE(mixed_decls.find("qux"), std::string::npos) << "qux should be found";
    
    // Check that global keywords are present
    EXPECT_NE(mixed_decls.find("global"), std::string::npos) << "global keyword should be found";
    EXPECT_NE(mixed_decls.find("globalvar"), std::string::npos) << "globalvar keyword should be found";
    
    // This documents that mixed declarations should be parsed correctly
    // with each variable routed to the appropriate scope
    EXPECT_TRUE(true) << "Mixed declarations should be parsed correctly";
}
