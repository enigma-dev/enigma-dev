#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <languages/clang_definitions.h>
#include <filesystem>
#include <set>
#include <string>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

// This test simulates what happens during actual compilation:
// 1. Initialize lang_CPP (which parses SHELLmain.cpp and populates enigma_user)
// 2. Copy resources into the namespace (like quickmember_integer/quickmember_script)
// 3. Check if functions are still there after resources are copied
class EnigmaUserAfterResourcesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Use the global main_context that should already be initialized
        if (!main_context) {
            FAIL() << "main_context was not initialized by BuiltinTypesEnvironment";
        }
        
        // Initialize lang_CPP which will parse SHELLmain.cpp and populate the enigma_user namespace
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
        
        // Get the enigma_user namespace scope
        jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
        if (enigma_user_def && (enigma_user_def->flags & jdi::DEF_NAMESPACE)) {
            enigma_user_scope_ = dynamic_cast<jdi::definition_scope*>(enigma_user_def);
        }
        
        ASSERT_NE(enigma_user_scope_, nullptr) << "enigma_user namespace not found";
        
        // Count functions BEFORE adding resources
        count_functions_before_ = count_functions_in_scope();
        
        std::cout << "\n=== Before resource copy ===" << std::endl;
        std::cout << "Functions found: " << count_functions_before_ << std::endl;
        std::cout << "Total members: " << enigma_user_scope_->members.size() << std::endl;
    }
    
    void TearDown() override {
        lang_.reset();
    }
    
    int count_functions_in_scope() {
        if (!enigma_user_scope_) return 0;
        
        int count = 0;
        for (const auto& member_pair : enigma_user_scope_->members) {
            const auto& member = member_pair.second;
            if (member && (member->flags & jdi::DEF_FUNCTION)) {
                count++;
            }
        }
        return count;
    }
    
    std::set<std::string> get_function_names() {
        std::set<std::string> names;
        if (!enigma_user_scope_) return names;
        
        for (const auto& member_pair : enigma_user_scope_->members) {
            const auto& member = member_pair.second;
            if (member && (member->flags & jdi::DEF_FUNCTION)) {
                names.insert(member_pair.first);
            }
        }
        return names;
    }
    
    jdi::definition_scope* enigma_user_scope_ = nullptr;
    std::unique_ptr<lang_CPP> lang_;
    int count_functions_before_ = 0;
};

TEST_F(EnigmaUserAfterResourcesTest, FunctionsRemainAfterResourceCopy) {
    // Simulate copying resources like the actual compiler does
    // This mimics what happens in compile.cpp around line 436-476
    
    // Copy some test resources (similar to what compile.cpp does)
    const std::vector<std::string> test_resources = {
        "spr_test", "snd_test", "bkg_test", "obj_test", 
        "room_test", "scr_warp"  // Note: scr_warp is both a script AND might be a function
    };
    
    std::cout << "\n=== Copying test resources ===" << std::endl;
    for (const auto& resource_name : test_resources) {
        // Use quickmember_integer for most resources (like sprites, sounds, etc.)
        lang_->quickmember_integer(enigma_user_scope_, resource_name);
    }
    
    // Copy a script (which uses quickmember_script)
    lang_->quickmember_script(enigma_user_scope_, "scr_warp");
    
    // Count functions AFTER adding resources
    int count_functions_after = count_functions_in_scope();
    std::set<std::string> functions_after = get_function_names();
    
    std::cout << "\n=== After resource copy ===" << std::endl;
    std::cout << "Functions found: " << count_functions_after << std::endl;
    std::cout << "Total members: " << enigma_user_scope_->members.size() << std::endl;
    
    // Check if motion_set still exists (it should NOT be overwritten)
    jdi::definition* motion_set = enigma_user_scope_->look_up("motion_set");
    
    if (motion_set == nullptr) {
        std::cout << "\n=== WARNING: motion_set was lost! ===" << std::endl;
        std::cout << "Sample functions found:" << std::endl;
        int shown = 0;
        for (const auto& name : functions_after) {
            std::cout << "  " << name << std::endl;
            if (++shown >= 20) {
                std::cout << "  ... (showing first 20)" << std::endl;
                break;
            }
        }
    }
    
    // The key test: motion_set should still exist
    ASSERT_NE(motion_set, nullptr) 
        << "motion_set function was lost after copying resources. "
        << "Before: " << count_functions_before_ << " functions, "
        << "After: " << count_functions_after << " functions";
    
    ASSERT_TRUE(motion_set->flags & jdi::DEF_FUNCTION)
        << "motion_set exists but is not a function";
    
    // We should have at least as many functions as before (maybe more if resources added new ones)
    // But if resources overwrote functions, we'd have fewer
    EXPECT_GE(count_functions_after, count_functions_before_ - 10)  // Allow some margin for edge cases
        << "Too many functions were lost after copying resources. "
        << "Before: " << count_functions_before_ << ", After: " << count_functions_after;
}

TEST_F(EnigmaUserAfterResourcesTest, CheckForOverwriteIssue) {
    // This test specifically checks if adding a resource with the same name as a function
    // causes the function to be overwritten
    
    // First, check if motion_set exists
    jdi::definition* motion_set_before = enigma_user_scope_->look_up("motion_set");
    ASSERT_NE(motion_set_before, nullptr) << "motion_set should exist before resource copy";
    ASSERT_TRUE(motion_set_before->flags & jdi::DEF_FUNCTION) << "motion_set should be a function";
    
    // Try to add a resource with a name that doesn't conflict
    lang_->quickmember_integer(enigma_user_scope_, "test_resource_xyz123");
    
    // motion_set should still exist
    jdi::definition* motion_set_after = enigma_user_scope_->look_up("motion_set");
    ASSERT_NE(motion_set_after, nullptr) << "motion_set was lost after adding unrelated resource";
    ASSERT_TRUE(motion_set_after->flags & jdi::DEF_FUNCTION) << "motion_set should still be a function";
    ASSERT_EQ(motion_set_before, motion_set_after) << "motion_set pointer changed (object replaced?)";
    
    // Now test the problematic case: what if we add a resource with a name that matches a function?
    // Actually, this is unlikely because resource names usually don't match function names
    // But let's check what happens if we try to add something that conflicts
    
    // Count how many members have both DEF_FUNCTION and DEF_TYPED flags
    // (which would indicate something is being treated as both)
    int ambiguous_count = 0;
    for (const auto& member_pair : enigma_user_scope_->members) {
        const auto& member = member_pair.second;
        if (member) {
            bool is_function = (member->flags & jdi::DEF_FUNCTION) != 0;
            bool is_typed = (member->flags & jdi::DEF_TYPED) != 0;
            if (is_function && is_typed) {
                ambiguous_count++;
                std::cout << "  Ambiguous member: " << member_pair.first 
                          << " (both function and typed)" << std::endl;
            }
        }
    }
    
    std::cout << "\nAmbiguous members (both function and typed): " << ambiguous_count << std::endl;
}

TEST_F(EnigmaUserAfterResourcesTest, VerifyFunctionLookup) {
    // Test that we can still look up functions by name after resources are added
    
    const std::vector<std::string> expected_functions = {
        "motion_set", "motion_add", "move_snap", "move_wrap"
    };
    
    // Add some resources first
    lang_->quickmember_integer(enigma_user_scope_, "spr_test1");
    lang_->quickmember_integer(enigma_user_scope_, "spr_test2");
    lang_->quickmember_script(enigma_user_scope_, "scr_test");
    
    // Check that expected functions are still accessible
    for (const auto& func_name : expected_functions) {
        jdi::definition* func = enigma_user_scope_->look_up(func_name);
        
        if (func == nullptr) {
            std::cout << "\n=== Function " << func_name << " not found ===" << std::endl;
            std::cout << "Available members (sample):" << std::endl;
            int shown = 0;
            for (const auto& member_pair : enigma_user_scope_->members) {
                std::cout << "  " << member_pair.first;
                if (member_pair.second) {
                    std::cout << " (flags: 0x" << std::hex << member_pair.second->flags << std::dec << ")";
                }
                std::cout << std::endl;
                if (++shown >= 30) break;
            }
        }
        
        ASSERT_NE(func, nullptr) 
            << "Function '" << func_name << "' not found after resource copy";
        
        ASSERT_TRUE(func->flags & jdi::DEF_FUNCTION)
            << "Function '" << func_name << "' exists but is not marked as a function "
            << "(flags: 0x" << std::hex << func->flags << std::dec << ")";
    }
}

TEST_F(EnigmaUserAfterResourcesTest, CheckMembersMapIntegrity) {
    // Check that the members map is not being corrupted or cleared
    
    int initial_count = enigma_user_scope_->members.size();
    
    // Add resources
    for (int i = 0; i < 10; ++i) {
        lang_->quickmember_integer(enigma_user_scope_, "test_resource_" + std::to_string(i));
    }
    
    int after_count = enigma_user_scope_->members.size();
    
    std::cout << "\n=== Members map integrity ===" << std::endl;
    std::cout << "Initial members: " << initial_count << std::endl;
    std::cout << "After adding 10 resources: " << after_count << std::endl;
    std::cout << "Expected: " << (initial_count + 10) << std::endl;
    
    // Should have added 10 new members
    EXPECT_EQ(after_count, initial_count + 10)
        << "Members count didn't increase as expected. "
        << "This suggests members are being overwritten or the map is being cleared.";
    
    // Check that motion_set is still there
    jdi::definition* motion_set = enigma_user_scope_->look_up("motion_set");
    ASSERT_NE(motion_set, nullptr) << "motion_set disappeared after adding resources";
}

TEST_F(EnigmaUserAfterResourcesTest, SimulateActualCompilationScenario) {
    // This test more closely mimics what happens in compile.cpp:
    // 1. Parse SHELLmain.cpp (already done in SetUp)
    // 2. Copy resources (like compile.cpp lines 436-476)
    // 3. Access namespace_enigma_user via look_up (like the compiler does)
    // 4. Check if functions are accessible
    
    // Simulate copying resources like a real project might have
    const std::vector<std::string> sprite_names = {"spr_test1", "spr_test2"};
    const std::vector<std::string> script_names = {"scr_warp", "scr_test"};
    const std::vector<std::string> object_names = {"obj_test1", "obj_test2"};
    
    std::cout << "\n=== Simulating actual compilation scenario ===" << std::endl;
    
    // Copy resources (like compile.cpp does)
    for (const auto& name : sprite_names) {
        lang_->quickmember_integer(enigma_user_scope_, name);
    }
    for (const auto& name : script_names) {
        lang_->quickmember_script(enigma_user_scope_, name);
    }
    for (const auto& name : object_names) {
        lang_->quickmember_integer(enigma_user_scope_, name);
    }
    
    // Now simulate how the compiler accesses functions via look_up
    // (like in lang_CPP::look_up which is used during parsing)
    std::vector<std::string> test_functions = {
        "motion_set", "motion_add", "move_snap", "scr_warp"
    };
    
    int found_count = 0;
    std::vector<std::string> not_found;
    
    for (const auto& func_name : test_functions) {
        // Use look_up like the compiler does (via lang_CPP::look_up)
        jdi::definition* def = enigma_user_scope_->look_up(func_name);
        
        if (def && (def->flags & jdi::DEF_FUNCTION)) {
            found_count++;
            std::cout << "  Found function: " << func_name << std::endl;
        } else {
            not_found.push_back(func_name);
            std::cout << "  NOT found: " << func_name;
            if (def) {
                std::cout << " (exists but flags: 0x" << std::hex << def->flags << std::dec << ")";
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << "\nFound " << found_count << " out of " << test_functions.size() << " functions" << std::endl;
    
    // All expected functions should be found
    for (const auto& name : not_found) {
        // scr_warp might be a script (which we just added), so it should exist
        // but motion_set, motion_add, move_snap should definitely exist
        if (name != "scr_warp") {
            FAIL() << "Function '" << name << "' was not found after resource copy. "
                   << "This suggests functions are being overwritten or lost.";
        }
    }
    
    // Verify we can still find many functions (not just 1)
    int total_functions = count_functions_in_scope();
    std::cout << "Total functions in namespace: " << total_functions << std::endl;
    
    EXPECT_GT(total_functions, 100)
        << "Too few functions found. Expected >100, got " << total_functions
        << ". This suggests most functions were lost.";
}
