#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <set>
#include <string>
#include <fstream>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

// Expected functions in enigma_user namespace (subset of what should be there)
static const std::set<std::string> kExpectedFunctions = {
    "motion_set",
    "motion_add",
    "move_snap",
    "move_wrap",
    "move_towards_point",
    "place_snapped",
    "move_random",
    // Add more as needed
};

class EnigmaUserNamespaceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Find enigma root (assumes test is run from project root)
        enigma_root_ = fs::current_path();
        if (!fs::exists(enigma_root_ / "ENIGMAsystem")) {
            // Try parent directory
            enigma_root_ = enigma_root_.parent_path();
        }
        
        // Initialize context
        context_ = std::make_unique<clang_adapter::ClangContext>();
        
        // Set up language
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
        
        // Instead of parsing SHELLmain.cpp (which requires generated headers),
        // parse a simpler header file that defines enigma_user functions.
        // This tests the namespace parsing functionality directly.
        fs::path move_functions = enigma_root_ / "ENIGMAsystem" / "SHELL" / "Universal_System" / "move_functions.h";
        if (fs::exists(move_functions)) {
            std::vector<std::string> include_dirs;
            include_dirs.push_back((enigma_root_ / "ENIGMAsystem" / "SHELL").u8string());
            include_dirs.push_back((enigma_root_ / "ENIGMAsystem" / "SHELL" / "Universal_System").u8string());
            
            // Create a minimal test file that includes move_functions.h
            // This avoids needing API_Switchboard.h and other generated files
            std::string test_content = 
                "#include \"Universal_System/move_functions.h\"\n"
                "namespace enigma_user {}\n";  // Empty namespace declaration to ensure it exists
            
            fs::path test_file = fs::temp_directory_path() / "enigma_user_test.cpp";
            std::ofstream out(test_file);
            out << test_content;
            out.close();
            
            int result = context_->parse_file(test_file.u8string(), include_dirs);
            
            // Clean up
            fs::remove(test_file);
            
            // Don't fail on parse errors - we still want to test what was parsed
            // The test will check if functions were found regardless
            (void)result;  // Suppress unused variable warning
        } else {
            FAIL() << "move_functions.h not found at: " << move_functions;
        }
    }
    
    void TearDown() override {
        lang_.reset();
        context_.reset();
    }
    
    fs::path enigma_root_;
    std::unique_ptr<clang_adapter::ClangContext> context_;
    std::unique_ptr<lang_CPP> lang_;
};

TEST_F(EnigmaUserNamespaceTest, EnigmaUserNamespaceExists) {
    // Look up enigma_user namespace in the global main_context
    ASSERT_NE(main_context, nullptr) << "main_context should be initialized";
    jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
    
    ASSERT_NE(enigma_user_def, nullptr) 
        << "enigma_user namespace not found in global scope";
    
    ASSERT_TRUE(enigma_user_def->flags & jdi::DEF_NAMESPACE) 
        << "enigma_user is not a namespace (flags: 0x" << std::hex << enigma_user_def->flags << std::dec << ")";
    
    jdi::definition_scope* enigma_user_scope = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def);
    
    ASSERT_NE(enigma_user_scope, nullptr) 
        << "Failed to cast enigma_user to definition_scope";
    
    std::cout << "\n=== enigma_user namespace members ===" << std::endl;
    std::cout << "Total members: " << enigma_user_scope->members.size() << std::endl;
    
    // Collect all member names for debugging
    std::vector<std::string> all_members;
    for (const auto& member_pair : enigma_user_scope->members) {
        all_members.push_back(member_pair.first);
    }
    
    std::cout << "Members: ";
    for (size_t i = 0; i < all_members.size() && i < 20; ++i) {
        std::cout << all_members[i];
        if (i < all_members.size() - 1 && i < 19) std::cout << ", ";
    }
    if (all_members.size() > 20) {
        std::cout << "... (" << all_members.size() << " total)";
    }
    std::cout << std::endl;
}

TEST_F(EnigmaUserNamespaceTest, ExpectedFunctionsExist) {
    // Look up enigma_user namespace in the global main_context
    ASSERT_NE(main_context, nullptr) << "main_context should be initialized";
    jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_def, nullptr);
    
    jdi::definition_scope* enigma_user_scope = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def);
    ASSERT_NE(enigma_user_scope, nullptr);
    
    // Count functions in namespace
    int function_count = 0;
    std::set<std::string> found_functions;
    std::vector<std::string> all_functions;
    
    for (const auto& member_pair : enigma_user_scope->members) {
        const std::string& name = member_pair.first;
        const auto& member = member_pair.second;
        if (!member) continue;
        
        if (member->flags & jdi::DEF_FUNCTION) {
            function_count++;
            all_functions.push_back(name);
            found_functions.insert(name);
        }
    }
    
    std::cout << "\n=== Function check ===" << std::endl;
    std::cout << "Total functions found: " << function_count << std::endl;
    std::cout << "Expected functions: " << kExpectedFunctions.size() << std::endl;
    
    // Check for expected functions
    std::vector<std::string> missing_functions;
    for (const auto& expected_func : kExpectedFunctions) {
        if (found_functions.find(expected_func) == found_functions.end()) {
            missing_functions.push_back(expected_func);
        }
    }
    
    if (!missing_functions.empty()) {
        std::cout << "\nMissing expected functions:" << std::endl;
        for (const auto& missing : missing_functions) {
            std::cout << "  - " << missing << std::endl;
        }
        
        std::cout << "\nAll functions found (" << all_functions.size() << "):" << std::endl;
        for (size_t i = 0; i < all_functions.size(); ++i) {
            std::cout << "  " << (i+1) << ". " << all_functions[i] << std::endl;
            if (i >= 49) {  // Limit output
                std::cout << "  ... (" << (all_functions.size() - 50) << " more)" << std::endl;
                break;
            }
        }
    }
    
    // This test will fail and show us what's missing
    EXPECT_GE(function_count, kExpectedFunctions.size()) 
        << "Found only " << function_count << " functions, expected at least " 
        << kExpectedFunctions.size() << " functions in enigma_user namespace";
    
    for (const auto& expected_func : kExpectedFunctions) {
        EXPECT_TRUE(found_functions.count(expected_func) > 0)
            << "Expected function '" << expected_func << "' not found in enigma_user namespace";
    }
}

TEST_F(EnigmaUserNamespaceTest, MoveFunctionsExist) {
    // Specifically test move_functions.h functions
    ASSERT_NE(main_context, nullptr) << "main_context should be initialized";
    jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
    ASSERT_NE(enigma_user_def, nullptr);
    
    jdi::definition_scope* enigma_user_scope = 
        dynamic_cast<jdi::definition_scope*>(enigma_user_def);
    ASSERT_NE(enigma_user_scope, nullptr);
    
    // Check for motion_set specifically
    jdi::definition* motion_set = enigma_user_scope->look_up("motion_set");
    
    if (motion_set == nullptr) {
        // Debug: print what functions ARE available
        std::cout << "\n=== Available functions in enigma_user ===" << std::endl;
        int count = 0;
        for (const auto& member_pair : enigma_user_scope->members) {
            if (member_pair.second && (member_pair.second->flags & jdi::DEF_FUNCTION)) {
                std::cout << "  " << member_pair.first << std::endl;
                count++;
                if (count >= 20) {
                    std::cout << "  ... (showing first 20)" << std::endl;
                    break;
                }
            }
        }
    }
    
    ASSERT_NE(motion_set, nullptr) 
        << "motion_set function not found in enigma_user namespace";
    
    ASSERT_TRUE(motion_set->flags & jdi::DEF_FUNCTION) 
        << "motion_set is not marked as a function (flags: 0x" 
        << std::hex << motion_set->flags << std::dec << ")";
}
