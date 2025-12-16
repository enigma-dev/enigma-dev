#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

// Declare extern for main_context
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

// This test mimics the EXACT scenario from the actual compilation:
// 1. Initialize lang_CPP (parses SHELLmain.cpp)
// 2. Create a GameData with resources (like the actual project file)
// 3. Call compileProto which does the resource copying
// 4. Check namespace_enigma_user at the same point as compile.cpp line 480
class EnigmaUserDebugTest : public ::testing::Test {
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

// Access the namespace_enigma_user from lang_CPP
// We need to check what it points to - it's a private member, but we can test via the look_up method
TEST_F(EnigmaUserDebugTest, CheckNamespaceAfterInitialization) {
    // After initialization, check what namespace_enigma_user points to
    // We can't access it directly, but we can check via main_context
    
    jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_def, nullptr) << "enigma_user namespace should exist";
    
    jdi::definition_scope* enigma_user_scope = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def);
    ASSERT_NE(enigma_user_scope, nullptr);
    
    std::cout << "\n=== After lang_CPP initialization ===" << std::endl;
    std::cout << "enigma_user namespace members: " << enigma_user_scope->members.size() << std::endl;
    
    int function_count = 0;
    for (const auto& member_pair : enigma_user_scope->members) {
        if (member_pair.second && (member_pair.second->flags & jdi::DEF_FUNCTION)) {
            function_count++;
        }
    }
    std::cout << "Functions: " << function_count << std::endl;
    
    // Should have many functions
    EXPECT_GT(function_count, 1000) 
        << "Expected >1000 functions in enigma_user after initialization, got " << function_count;
}

TEST_F(EnigmaUserDebugTest, SimulateResourceCopyScenario) {
    // This test simulates what happens in compile.cpp:
    // - Create a minimal GameData with a script
    // - Simulate the resource copying that happens in compile()
    // - Check what namespace_enigma_user points to
    
    // First, verify initial state
    jdi::definition* enigma_user_def_before = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_def_before, nullptr);
    jdi::definition_scope* enigma_user_scope_before = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def_before);
    ASSERT_NE(enigma_user_scope_before, nullptr);
    
    int functions_before = 0;
    for (const auto& member_pair : enigma_user_scope_before->members) {
        if (member_pair.second && (member_pair.second->flags & jdi::DEF_FUNCTION)) {
            functions_before++;
        }
    }
    
    std::cout << "\n=== Before resource copy ===" << std::endl;
    std::cout << "Members: " << enigma_user_scope_before->members.size() << std::endl;
    std::cout << "Functions: " << functions_before << std::endl;
    
    // Now simulate copying resources like compile.cpp does
    // Use globals_scope = namespace_enigma_user (line 427)
    jdi::definition_scope* globals_scope = enigma_user_scope_before;
    
    // Copy a script (like compile.cpp line 452)
    lang_->quickmember_script(globals_scope, "scr_warp");
    
    // Copy some other resources
    lang_->quickmember_integer(globals_scope, "obj_test");
    lang_->quickmember_integer(globals_scope, "spr_test");
    
    // Now check what the namespace looks like
    jdi::definition* enigma_user_def_after = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_def_after, nullptr);
    ASSERT_EQ(enigma_user_def_after, enigma_user_def_before) 
        << "enigma_user namespace definition changed!";
    
    jdi::definition_scope* enigma_user_scope_after = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def_after);
    ASSERT_NE(enigma_user_scope_after, nullptr);
    ASSERT_EQ(enigma_user_scope_after, enigma_user_scope_before)
        << "enigma_user scope pointer changed!";
    
    int functions_after = 0;
    for (const auto& member_pair : enigma_user_scope_after->members) {
        if (member_pair.second && (member_pair.second->flags & jdi::DEF_FUNCTION)) {
            functions_after++;
        }
    }
    
    std::cout << "\n=== After resource copy ===" << std::endl;
    std::cout << "Members: " << enigma_user_scope_after->members.size() << std::endl;
    std::cout << "Functions: " << functions_after << std::endl;
    
    // Functions should have increased (scr_warp was added as a script/function)
    EXPECT_GE(functions_after, functions_before)
        << "Functions decreased after resource copy. Before: " << functions_before 
        << ", After: " << functions_after;
    
    // Check that motion_set still exists
    jdi::definition* motion_set = enigma_user_scope_after->look_up("motion_set");
    ASSERT_NE(motion_set, nullptr) 
        << "motion_set function disappeared! This indicates functions are being lost.";
    
    // The key test: we should still have many functions, not just 1
    EXPECT_GT(functions_after, 1000)
        << "Too few functions found. Expected >1000, got " << functions_after
        << ". This matches the bug where only 1 function (scr_warp) is found.";
}

TEST_F(EnigmaUserDebugTest, CheckIfNamespacePointerChanges) {
    // This test checks if namespace_enigma_user might be pointing to a different scope
    // after resource copying. The bug shows only 54 members, which suggests it might
    // be pointing to a different scope entirely.
    
    jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_def, nullptr);
    
    jdi::definition_scope* enigma_user_scope = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def);
    ASSERT_NE(enigma_user_scope, nullptr);
    
    // Check what the scope actually contains
    std::cout << "\n=== Checking namespace contents ===" << std::endl;
    std::cout << "Total members: " << enigma_user_scope->members.size() << std::endl;
    
    // Check if this is actually the global scope (which would have fewer members)
    jdi::definition_scope* global_scope = 
        dynamic_cast<jdi::definition_scope*>(main_context->get_global());
    
    if (global_scope && enigma_user_scope == global_scope) {
        std::cout << "WARNING: enigma_user_scope IS the global scope!" << std::endl;
        FAIL() << "namespace_enigma_user is pointing to global scope instead of enigma_user namespace!";
    }
    
    // Check if it's a different scope
    std::cout << "enigma_user scope address: " << (void*)enigma_user_scope << std::endl;
    std::cout << "global scope address: " << (void*)global_scope << std::endl;
    
    // Count functions
    int function_count = 0;
    std::vector<std::string> sample_functions;
    for (const auto& member_pair : enigma_user_scope->members) {
        if (member_pair.second && (member_pair.second->flags & jdi::DEF_FUNCTION)) {
            function_count++;
            if (sample_functions.size() < 10) {
                sample_functions.push_back(member_pair.first);
            }
        }
    }
    
    std::cout << "Functions: " << function_count << std::endl;
    std::cout << "Sample functions: ";
    for (const auto& name : sample_functions) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    // If we only have a few members (like 54), that's the bug
    if (enigma_user_scope->members.size() < 100) {
        FAIL() << "namespace_enigma_user has only " << enigma_user_scope->members.size() 
               << " members (expected >1000). This matches the bug scenario!";
    }
}

TEST_F(EnigmaUserDebugTest, ReproduceActualBugScenario) {
    // Try to reproduce the exact bug: after resources are copied, 
    // namespace_enigma_user only shows 54 members and 1 function
    
    // First check: what does the lookup return?
    jdi::definition* enigma_user_lookup = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_lookup, nullptr) << "enigma_user lookup failed!";
    
    jdi::definition_scope* globals_scope = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_lookup);
    ASSERT_NE(globals_scope, nullptr);
    
    std::cout << "\n=== Initial state ===" << std::endl;
    std::cout << "enigma_user scope from lookup has " << globals_scope->members.size() << " members" << std::endl;
    
    // Check if this matches what namespace_enigma_user should point to
    // We can't access it directly, but we can check if look_up("motion_set") works
    jdi::definition* motion_set_before = globals_scope->look_up("motion_set");
    if (motion_set_before) {
        std::cout << "motion_set found (flags: 0x" << std::hex << motion_set_before->flags << std::dec << ")" << std::endl;
    } else {
        std::cout << "WARNING: motion_set NOT found in enigma_user scope!" << std::endl;
    }
    
    // Copy resources exactly like the actual project does
    // (1 script, 18 objects, 3 rooms, etc. - matching balloon_pop.project.gmk)
    std::vector<std::string> scripts = {"scr_warp"};
    std::vector<std::string> objects = {
        "obj_blue", "obj_yellow", "obj_hand", "obj_extra"
        // Add more to match the 18 objects
    };
    std::vector<std::string> rooms = {"room0", "room1", "room2"};
    
    std::cout << "\n=== Reproducing bug scenario ===" << std::endl;
    std::cout << "Copying " << scripts.size() << " scripts..." << std::endl;
    for (const auto& name : scripts) {
        lang_->quickmember_script(globals_scope, name);
    }
    
    std::cout << "Copying " << objects.size() << " objects..." << std::endl;
    for (const auto& name : objects) {
        lang_->quickmember_integer(globals_scope, name);
    }
    
    std::cout << "Copying " << rooms.size() << " rooms..." << std::endl;
    for (const auto& name : rooms) {
        lang_->quickmember_integer(globals_scope, name);
    }
    
    // Now check the namespace - this should match what compile.cpp sees at line 480
    jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_def, nullptr);
    
    jdi::definition_scope* enigma_user_scope = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def);
    ASSERT_NE(enigma_user_scope, nullptr);
    
    // Check if it's the same scope we were adding to
    if (enigma_user_scope != globals_scope) {
        std::cout << "\n*** BUG DETECTED: Scope mismatch! ***" << std::endl;
        std::cout << "globals_scope address: " << (void*)globals_scope << std::endl;
        std::cout << "enigma_user_scope address: " << (void*)enigma_user_scope << std::endl;
        std::cout << "globals_scope members: " << globals_scope->members.size() << std::endl;
        std::cout << "enigma_user_scope members: " << enigma_user_scope->members.size() << std::endl;
        
        FAIL() << "The scope we're adding resources to is different from the scope "
               << "looked up via enigma_user! This explains why functions are missing!";
    }
    
    // Count members and functions
    int total_members = enigma_user_scope->members.size();
    int function_count = 0;
    
    for (const auto& member_pair : enigma_user_scope->members) {
        if (member_pair.second && (member_pair.second->flags & jdi::DEF_FUNCTION)) {
            function_count++;
        }
    }
    
    std::cout << "\n=== After resource copy (matching compile.cpp line 480) ===" << std::endl;
    std::cout << "Total members: " << total_members << std::endl;
    std::cout << "Functions found: " << function_count << std::endl;
    
    // This is the bug: if we only have ~54 members and 1 function, we've reproduced it
    if (total_members < 100 && function_count < 10) {
        std::cout << "\n*** BUG REPRODUCED! ***" << std::endl;
        std::cout << "This matches the actual bug where only 54 members and 1 function are found." << std::endl;
        
        // Show what members ARE present
        std::cout << "\nMembers present:" << std::endl;
        int shown = 0;
        for (const auto& member_pair : enigma_user_scope->members) {
            std::cout << "  " << member_pair.first;
            if (member_pair.second) {
                std::cout << " (flags: 0x" << std::hex << member_pair.second->flags << std::dec << ")";
            }
            std::cout << std::endl;
            if (++shown >= 60) break;
        }
    }
    
    // The test should fail if we've reproduced the bug
    EXPECT_GT(total_members, 1000) 
        << "Only " << total_members << " members found (expected >1000). Bug reproduced!";
    EXPECT_GT(function_count, 1000)
        << "Only " << function_count << " functions found (expected >1000). Bug reproduced!";
}

TEST_F(EnigmaUserDebugTest, CheckIfNamespaceLookupFails) {
    // This test checks if the lookup for "enigma_user" namespace fails
    // which would cause namespace_enigma_user to stay as get_global()
    
    jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
    
    if (enigma_user_def == nullptr) {
        std::cout << "\n*** BUG REPRODUCED: enigma_user lookup failed! ***" << std::endl;
        std::cout << "This would cause namespace_enigma_user to stay as get_global()" << std::endl;
        
        // Show what IS in the global scope
        clang_adapter::ClangDefinitionScope* global_scope = 
            dynamic_cast<clang_adapter::ClangDefinitionScope*>(main_context->get_global());
        if (global_scope) {
            std::cout << "\nGlobal scope members (first 30):" << std::endl;
            int shown = 0;
            for (const auto& member_pair : global_scope->members) {
                std::cout << "  " << member_pair.first;
                if (member_pair.second) {
                    std::cout << " (flags: 0x" << std::hex << member_pair.second->flags << std::dec << ")";
                }
                std::cout << std::endl;
                if (++shown >= 30) break;
            }
        }
        
        FAIL() << "enigma_user namespace lookup failed! This is the root cause.";
    }
    
    ASSERT_TRUE(enigma_user_def->flags & jdi::DEF_NAMESPACE)
        << "enigma_user exists but is not a namespace";
    
    std::cout << "\n=== Lookup succeeded ===" << std::endl;
    std::cout << "enigma_user namespace found correctly" << std::endl;
}
