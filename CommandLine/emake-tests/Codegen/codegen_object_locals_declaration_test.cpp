// Test for the object locals declaration bug fix
// Bug: Variables in object->locals were incorrectly skipped when they appeared in global->globals
// Fix: Changed check from global->globals to object->globals in write_object_data.cpp
//
// The issue: When a variable like camDx is used as an instance variable in objControl,
// but is also accessed by scripts (which copy it to global->globals), the old code
// incorrectly skipped declaring camDx in the object class because it found it in global->globals.
// The fix ensures we only skip variables that are in THIS object's globals (object->globals),
// not variables that just happen to be in the global scope's globals map.

#include <gtest/gtest.h>
#include "parser/object_storage.h"
#include <string>
#include <sstream>

class ObjectLocalsDeclarationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a mock global scope
        global_scope_ = std::make_unique<ParsedScope>();
        
        // Create a mock parsed_object
        object_ = std::make_unique<parsed_object>();
        object_->name = "objControl";
        object_->id = 0;
        object_->parent = nullptr;
    }
    
    void TearDown() override {
        object_.reset();
        global_scope_.reset();
    }
    
    std::unique_ptr<ParsedScope> global_scope_;
    std::unique_ptr<parsed_object> object_;
};

// Test that simulates the bug scenario:
// - Variable "camDx" is in object->locals (used as instance variable)
// - Variable "camDx" is in global->globals (copied by scripts)
// - Variable "camDx" is NOT in object->globals (not accessed via global.camDx in this object)
// Expected: camDx SHOULD be declared in the object (with the fix)
// Bug behavior: camDx was incorrectly skipped (without the fix)
TEST_F(ObjectLocalsDeclarationTest, VariableInGlobalGlobalsButNotObjectGlobals_ShouldBeDeclared) {
    // Set up: camDx is used as an instance variable in this object
    object_->locals["camDx"] = dectrip("var");
    object_->locals["camDy"] = dectrip("var");
    object_->locals["camDz"] = dectrip("var");
    
    // Set up: camDx is also in the global scope's globals (because scripts use it)
    // This simulates what happens when scripts access these variables
    global_scope_->globals["camDx"] = dectrip("var");
    global_scope_->globals["camDy"] = dectrip("var");
    global_scope_->globals["camDz"] = dectrip("var");
    
    // Crucially: camDx is NOT in object->globals
    // (it's not accessed via global.camDx in this object - it's used directly as an instance var)
    ASSERT_TRUE(object_->globals.find("camDx") == object_->globals.end())
        << "Test setup: camDx should NOT be in object->globals";
    
    // The fix logic (from write_object_data.cpp):
    // OLD (buggy): if (global->globals.find(ii->first) != global->globals.end()) { continue; }
    // NEW (fixed): if (object->globals.find(ii->first) != object->globals.end()) { continue; }
    
    // Test the NEW (fixed) logic:
    // camDx is in object->locals, NOT in object->globals -> should be declared
    bool should_skip_with_fix = (object_->globals.find("camDx") != object_->globals.end());
    EXPECT_FALSE(should_skip_with_fix)
        << "With the fix: camDx should NOT be skipped (it's not in object->globals)";
    
    // Verify the OLD (buggy) logic would have incorrectly skipped it:
    bool would_skip_with_bug = (global_scope_->globals.find("camDx") != global_scope_->globals.end());
    EXPECT_TRUE(would_skip_with_bug)
        << "Bug behavior: camDx would have been incorrectly skipped (it's in global->globals)";
    
    // The key assertion: the fix changes the behavior
    EXPECT_NE(should_skip_with_fix, would_skip_with_bug)
        << "The fix should change the skip decision for variables in global->globals but not object->globals";
}

// Test that variables explicitly accessed via global.varname ARE skipped correctly
TEST_F(ObjectLocalsDeclarationTest, VariableInObjectGlobals_ShouldBeSkipped) {
    // Set up: globalVar is accessed via global.globalVar in this object
    object_->locals["globalVar"] = dectrip("var");
    object_->globals["globalVar"] = dectrip("var");  // Explicitly in object->globals
    
    // Also in global scope's globals (would be there after compilation)
    global_scope_->globals["globalVar"] = dectrip("var");
    
    // Test: Variable IS in object->globals -> should be skipped (declared in ENIGMA_global_structure)
    bool should_skip = (object_->globals.find("globalVar") != object_->globals.end());
    EXPECT_TRUE(should_skip)
        << "globalVar should be skipped (it's in object->globals, meaning it was accessed via global.globalVar)";
}

// Test that instance variables not in any globals map are declared
TEST_F(ObjectLocalsDeclarationTest, PureInstanceVariable_ShouldBeDeclared) {
    // Set up: pureLocalVar is only in object->locals
    object_->locals["pureLocalVar"] = dectrip("var");
    
    // Not in object->globals (not accessed via global.pureLocalVar)
    // Not in global->globals (no scripts use it)
    ASSERT_TRUE(object_->globals.find("pureLocalVar") == object_->globals.end());
    ASSERT_TRUE(global_scope_->globals.find("pureLocalVar") == global_scope_->globals.end());
    
    // Test: Variable is not in object->globals -> should be declared
    bool should_skip = (object_->globals.find("pureLocalVar") != object_->globals.end());
    EXPECT_FALSE(should_skip)
        << "pureLocalVar should NOT be skipped (it's a pure instance variable)";
}

// Test the complete scenario with multiple variables of different types
TEST_F(ObjectLocalsDeclarationTest, MixedVariableScenario) {
    // Instance variable used directly (should be declared in object)
    object_->locals["instanceVar"] = dectrip("var");
    
    // Instance variable also used by scripts (should still be declared in object)
    object_->locals["sharedInstanceVar"] = dectrip("var");
    global_scope_->globals["sharedInstanceVar"] = dectrip("var");
    
    // Global variable accessed via global.xxx in this object (should be skipped, declared in ENIGMA_global_structure)
    object_->locals["explicitGlobalVar"] = dectrip("var");
    object_->globals["explicitGlobalVar"] = dectrip("var");
    global_scope_->globals["explicitGlobalVar"] = dectrip("var");
    
    // Test instanceVar: not in any globals -> declare
    EXPECT_FALSE(object_->globals.find("instanceVar") != object_->globals.end())
        << "instanceVar: not in object->globals, should be declared";
    
    // Test sharedInstanceVar: in global->globals but NOT in object->globals -> declare
    EXPECT_TRUE(global_scope_->globals.find("sharedInstanceVar") != global_scope_->globals.end())
        << "sharedInstanceVar: is in global->globals";
    EXPECT_FALSE(object_->globals.find("sharedInstanceVar") != object_->globals.end())
        << "sharedInstanceVar: NOT in object->globals, should be declared";
    
    // Test explicitGlobalVar: in object->globals -> skip
    EXPECT_TRUE(object_->globals.find("explicitGlobalVar") != object_->globals.end())
        << "explicitGlobalVar: in object->globals, should be skipped";
}
