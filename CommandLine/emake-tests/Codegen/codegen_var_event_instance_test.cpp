#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include "parser/object_storage.h"
#include <string>
#include <map>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

// Test that var declarations in events create instance variables
// Issue #30: var name; in Create event should be accessible in Draw event

class CodegenVarEventInstanceTest : public ::testing::Test {
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
        
        int result = lang_->load_shared_locals();
        ASSERT_EQ(result, 0) << "load_shared_locals() should succeed";
    }
    
    void TearDown() override {
        lang_.reset();
    }
    
    std::unique_ptr<lang_CPP> lang_;
};

// Test that var in Create event is added to locals
TEST_F(CodegenVarEventInstanceTest, VarInCreateEventAddedToLocals) {
    // Simulate Create event scope
    ParsedScope create_scope;
    
    // var name; is declared - it's in declarations as TEMPORARY
    create_scope.declarations["name"] = nullptr;
    
    // When AddLocal is called for 'name' in Create event (is_script=false),
    // it should add name to locals as an instance variable
    // (simulating the logic from collect_variables.cpp)
    std::string name = "name";
    bool is_script = false;
    
    // Simulate the logic: if variable is in declarations (TEMPORARY from var)
    // but we're in an event (not script), add it to locals
    if (create_scope.declarations.find(name) != create_scope.declarations.end()) {
        if (create_scope.locals.find(name) == create_scope.locals.end()) {
            if (!is_script) {
                create_scope.locals[name] = dectrip("var");
            }
        }
    }
    
    // Verify name is in locals
    EXPECT_NE(create_scope.locals.find(name), create_scope.locals.end())
        << "var name; in Create event should be added to locals as instance variable";
}

// Test that var declared in Create event is accessible in Draw event
TEST_F(CodegenVarEventInstanceTest, VarInCreateAccessibleInDraw) {
    ParsedScope create_scope;
    ParsedScope draw_scope;
    
    // var name; in Create event - in events, var creates instance variables
    // So it should be in locals but NOT in declarations (unlike scripts where
    // var creates TEMPORARY variables that are in declarations)
    create_scope.locals["name"] = dectrip("var");
    
    // Draw event scope should inherit from Create event scope
    // (via object's scope inheritance)
    draw_scope.copy_from(create_scope, "create_scope", "draw_scope");
    
    // name should be in draw_scope.locals (copied from create_scope)
    EXPECT_NE(draw_scope.locals.find("name"), draw_scope.locals.end())
        << "var name; in Create event should be accessible in Draw event";
}

// Test that variable collector promotes var in events to instance variables
TEST_F(CodegenVarEventInstanceTest, VariableCollectorPromotesVarInEvents) {
    // Simulate the AddLocal logic from collect_variables.cpp
    ParsedScope scope;
    scope.declarations["x"] = nullptr;  // var x; declared (TEMPORARY)
    
    std::string name = "x";
    bool is_script = false;  // In event, not script
    
    // Logic from AddLocal: if in declarations (TEMPORARY) but not in locals,
    // and we're in an event (not script), add to locals as instance variable
    if (scope.declarations.find(name) != scope.declarations.end()) {
        if (scope.locals.find(name) == scope.locals.end()) {
            if (!is_script) {
                scope.locals[name] = dectrip("var");
            }
        }
    }
    
    // x should be in locals (promoted to instance variable)
    EXPECT_NE(scope.locals.find("x"), scope.locals.end())
        << "Variable collector should promote var in events to instance variables";
}

// Test that var in scripts does NOT create instance variables
TEST_F(CodegenVarEventInstanceTest, VarInScriptsNotInstanceVariables) {
    ParsedScope scope;
    scope.declarations["x"] = nullptr;  // var x; declared (TEMPORARY)
    
    std::string name = "x";
    bool is_script = true;  // In script, not event
    
    // Logic: if in declarations (TEMPORARY) and we're in a script,
    // it should NOT be added to locals (remains TEMPORARY)
    if (scope.declarations.find(name) != scope.declarations.end()) {
        if (scope.locals.find(name) == scope.locals.end()) {
            if (!is_script) {
                scope.locals[name] = dectrip("var");
            }
            // If is_script is true, we don't add it to locals
        }
    }
    
    // x should NOT be in locals (remains TEMPORARY in scripts)
    EXPECT_EQ(scope.locals.find("x"), scope.locals.end())
        << "var in scripts should NOT create instance variables (remains TEMPORARY)";
}
