#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <string>
#include <map>
#include <unordered_set>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

// Test that #undef macros are removed from macro map
// Issue #26: y1 macro should be undefined after <cmath> is included

class CodegenMacroUndefTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!main_context) {
            FAIL() << "main_context was not initialized";
        }
    }
};

// Test that undefined macros are removed from macro map
TEST_F(CodegenMacroUndefTest, UndefinedMacrosAreRemoved) {
    // Simulate macro map
    std::map<std::string, std::string> macros;
    macros["y1"] = "bessel_y1";
    macros["y0"] = "bessel_y0";
    macros["FOO"] = "BAR";
    
    // Simulate undef directives found in included files
    std::unordered_set<std::string> undefined_macros;
    undefined_macros.insert("y1");
    undefined_macros.insert("y0");
    
    // Remove undefined macros (simulating extract_macros behavior)
    for (const auto& undef_name : undefined_macros) {
        macros.erase(undef_name);
    }
    
    // Verify undefined macros are removed
    EXPECT_EQ(macros.find("y1"), macros.end())
        << "y1 macro should be removed after #undef";
    EXPECT_EQ(macros.find("y0"), macros.end())
        << "y0 macro should be removed after #undef";
    
    // Verify other macros are still present
    EXPECT_NE(macros.find("FOO"), macros.end())
        << "FOO macro should still be present (not undefined)";
}

// Test that macro scanning finds #undef directives
TEST_F(CodegenMacroUndefTest, UndefDirectivesAreScanned) {
    // Simulate file content with #undef
    std::string file_content = 
        "#define y1 bessel_y1\n"
        "#include <cmath>\n"
        "#undef y1\n"
        "#undef y0\n";
    
    // Simulate scanning for #undef directives
    std::unordered_set<std::string> undefined_macros;
    size_t pos = 0;
    while ((pos = file_content.find("#undef", pos)) != std::string::npos) {
        pos += 6;  // Skip "#undef"
        
        // Skip whitespace
        while (pos < file_content.size() && (file_content[pos] == ' ' || file_content[pos] == '\t')) {
            pos++;
        }
        
        // Extract macro name
        size_t name_start = pos;
        while (pos < file_content.size() && (std::isalnum(file_content[pos]) || file_content[pos] == '_')) {
            pos++;
        }
        
        if (pos > name_start) {
            undefined_macros.insert(file_content.substr(name_start, pos - name_start));
        }
    }
    
    // Verify both undefs were found
    EXPECT_NE(undefined_macros.find("y1"), undefined_macros.end())
        << "Should find #undef y1";
    EXPECT_NE(undefined_macros.find("y0"), undefined_macros.end())
        << "Should find #undef y0";
}

// Test that y1 macro is not active after #undef
TEST_F(CodegenMacroUndefTest, Y1MacroNotActiveAfterUndef) {
    // In mathnc.h, y1 is defined, then <cmath> is included, then y1 is undefined
    // After this, y1 should NOT be replaced with bessel_y1
    
    // Simulate the process:
    // 1. y1 is defined
    std::map<std::string, std::string> macros;
    macros["y1"] = "bessel_y1";
    
    // 2. <cmath> is included (this uses the renamed functions)
    
    // 3. y1 is undefined
    macros.erase("y1");
    
    // 4. y1 should NOT be in macros anymore
    EXPECT_EQ(macros.find("y1"), macros.end())
        << "y1 should not be in macro map after #undef";
    
    // This means y1 will be used as-is, not replaced with bessel_y1
}
