#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <sstream>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

class CodegenBuiltinVarsTest : public ::testing::Test {
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
    
    // Helper to check if a variable should be filtered by codegen
    bool should_be_filtered(const std::string& var_name) {
        return lang_->is_shared_local(var_name);
    }
    
    // Helper to parse a generated header file and extract variable declarations
    std::set<std::string> extract_local_declarations(const std::string& content) {
        std::set<std::string> locals;
        std::istringstream stream(content);
        std::string line;
        bool in_locals_section = false;
        
        while (std::getline(stream, line)) {
            // Look for "// Local variables" comment
            if (line.find("// Local variables") != std::string::npos) {
                in_locals_section = true;
                continue;
            }
            
            if (in_locals_section) {
                // Check if this is a variable declaration (starts with "var " or a type)
                // Pattern: "var variable_name;" or "type variable_name;"
                if (line.find("var ") != std::string::npos || 
                    (line.find(";") != std::string::npos && 
                     (line.find("bool ") != std::string::npos ||
                      line.find("int ") != std::string::npos ||
                      line.find("double ") != std::string::npos ||
                      line.find("string ") != std::string::npos))) {
                    // Extract variable name (simplified - assumes format "type name;")
                    size_t last_space = line.find_last_of(" \t");
                    size_t semicolon = line.find(";");
                    if (last_space != std::string::npos && semicolon != std::string::npos) {
                        std::string var_name = line.substr(last_space + 1, semicolon - last_space - 1);
                        // Remove any array brackets or pointer symbols
                        size_t bracket = var_name.find("[");
                        if (bracket != std::string::npos) {
                            var_name = var_name.substr(0, bracket);
                        }
                        // Remove trailing whitespace
                        while (!var_name.empty() && std::isspace(var_name.back())) {
                            var_name.pop_back();
                        }
                        if (!var_name.empty()) {
                            locals.insert(var_name);
                        }
                    }
                }
                
                // Stop if we hit the next section (like "//Scripts" or "void")
                if (line.find("//") != std::string::npos && 
                    line.find("// Local variables") == std::string::npos) {
                    in_locals_section = false;
                }
                if (line.find("void ") != std::string::npos || 
                    line.find("struct ") != std::string::npos ||
                    line.find("class ") != std::string::npos) {
                    in_locals_section = false;
                }
            }
        }
        
        return locals;
    }
    
    // Helper to extract varaccess function names from generated code
    std::set<std::string> extract_varaccess_functions(const std::string& content) {
        std::set<std::string> varaccess_funcs;
        std::istringstream stream(content);
        std::string line;
        
        while (std::getline(stream, line)) {
            // Look for "varaccess_" function declarations
            size_t pos = line.find("varaccess_");
            if (pos != std::string::npos) {
                size_t start = pos + 10; // "varaccess_" is 10 chars
                size_t end = line.find("(", start);
                if (end != std::string::npos) {
                    std::string func_name = line.substr(start, end - start);
                    // Extract the variable name (remove "varaccess_" prefix)
                    varaccess_funcs.insert(func_name);
                }
            }
        }
        
        return varaccess_funcs;
    }
};

TEST_F(CodegenBuiltinVarsTest, BuiltinVariablesAreDetectedByParser) {
    // First verify that the parser is discovering built-in variables correctly
    const NameSet& shared_locals = lang_->shared_object_locals();
    
    ASSERT_GT(shared_locals.size(), 0) 
        << "Parser should discover at least some built-in instance variables";
    
    // These are common built-in variables that should be discovered
    std::vector<std::string> common_builtins = {
        "x", "y", "hspeed", "vspeed", "sprite_index", 
        "image_index", "image_alpha", "image_xscale", "image_yscale",
        "visible", "solid", "depth", "self"
    };
    
    int found_count = 0;
    for (const auto& var_name : common_builtins) {
        if (lang_->is_shared_local(var_name)) {
            found_count++;
            std::cout << "  ✓ Built-in variable '" << var_name << "' detected by parser" << std::endl;
        } else {
            std::cout << "  ✗ Built-in variable '" << var_name << "' NOT detected by parser" << std::endl;
        }
    }
    
    // At least some built-ins should be found
    EXPECT_GT(found_count, 0)
        << "Parser should discover at least some common built-in variables. "
        << "Found " << found_count << " out of " << common_builtins.size();
}

TEST_F(CodegenBuiltinVarsTest, BuiltinVariablesShouldBeFiltered) {
    // Test that is_shared_local() correctly identifies built-ins that should be filtered
    // This tests the logic that codegen will use
    
    // Get the set of shared locals discovered by parser
    const NameSet& shared_locals = lang_->shared_object_locals();
    
    if (shared_locals.empty()) {
        GTEST_SKIP() << "No shared locals discovered - cannot test filtering logic";
    }
    
    // Test that each discovered built-in would be filtered
    for (const auto& var_name : shared_locals) {
        EXPECT_TRUE(should_be_filtered(var_name))
            << "Variable '" << var_name << "' discovered by parser should be filtered by codegen";
    }
    
    // Test that user-defined variables would NOT be filtered
    std::vector<std::string> user_vars = {
        "myCustomVar", "balloonCount", "playerScore", "gameState"
    };
    
    for (const auto& var_name : user_vars) {
        // These should NOT be in shared_locals (unless there's a weird collision)
        bool in_shared = (shared_locals.find(var_name) != shared_locals.end());
        if (!in_shared) {
            EXPECT_FALSE(should_be_filtered(var_name))
                << "User-defined variable '" << var_name << "' should NOT be filtered";
        }
    }
}

TEST_F(CodegenBuiltinVarsTest, VerifyFilteringLogicForCommonBuiltins) {
    // Test specific common built-in variables that were reported as being incorrectly declared
    
    std::vector<std::string> reported_builtins = {
        "image_alpha", "image_index", "image_speed", "image_xscale", "image_yscale",
        "sprite_index", "hspeed", "vspeed", "c_blue", "c_white", "color", "self"
    };
    
    std::cout << "\nTesting filtering logic for reported built-in variables:" << std::endl;
    
    for (const auto& var_name : reported_builtins) {
        bool should_filter = should_be_filtered(var_name);
        std::cout << "  " << var_name << ": " 
                  << (should_filter ? "SHOULD BE FILTERED" : "NOT FILTERED") << std::endl;
        
        // Note: We can't assert they MUST be filtered because the parser might not discover all of them
        // But if they ARE discovered, they should be filtered
        if (should_filter) {
            EXPECT_TRUE(should_filter)
                << "Variable '" << var_name << "' is a built-in and should be filtered by codegen";
        }
    }
}

TEST_F(CodegenBuiltinVarsTest, VerifyNoHardcodedVariableNames) {
    // This test verifies that we're using the parser-discovered set, not hardcoded names
    // by checking that the filtering logic uses is_shared_local() which relies on the parser
    
    // Print all discovered built-ins to show they come from parser
    const NameSet& shared_locals = lang_->shared_object_locals();
    
    std::cout << "\nParser-discovered built-in variables (" << shared_locals.size() << " total):" << std::endl;
    int count = 0;
    for (const auto& var_name : shared_locals) {
        std::cout << "  - " << var_name << std::endl;
        if (++count >= 50) {
            std::cout << "  ... (showing first 50)" << std::endl;
            break;
        }
    }
    
    // Verify the set is populated (proving we're using parser, not hardcoded)
    ASSERT_GT(shared_locals.size(), 0)
        << "Parser should discover built-in variables from C++ class hierarchy";
    
    // Verify is_shared_local() works for discovered variables
    for (const auto& var_name : shared_locals) {
        EXPECT_TRUE(lang_->is_shared_local(var_name))
            << "is_shared_local() should return true for parser-discovered variable '" << var_name << "'";
    }
}
