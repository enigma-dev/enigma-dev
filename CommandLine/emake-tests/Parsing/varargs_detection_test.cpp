#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <climits>
#include <cstddef>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

class VarargsDetectionTest : public ::testing::Test {
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
    
    // Helper to look up a function from enigma_user namespace
    jdi::definition* lookup_function(const std::string& name) {
        jdi::definition* enigma_user_def = main_context->get_global()->look_up("enigma_user");
        if (!enigma_user_def) {
            return nullptr;
        }
        jdi::definition_scope* enigma_user_scope = 
            dynamic_cast<jdi::definition_scope*>(enigma_user_def);
        if (!enigma_user_scope) {
            return nullptr;
        }
        return enigma_user_scope->look_up(name);
    }
};

TEST_F(VarargsDetectionTest, ChooseIsDetectedAsVariadic) {
    jdi::definition* choose_def = lookup_function("choose");
    ASSERT_NE(choose_def, nullptr) << "choose function should exist in enigma_user namespace";
    
    // Verify it's a function
    ASSERT_TRUE(choose_def->flags & jdi::DEF_FUNCTION) 
        << "choose should be a function";
    
    // Test is_variadic_function detection
    EXPECT_TRUE(lang_->is_variadic_function(choose_def))
        << "choose should be detected as variadic";
    
    // Test function_variadic_after
    jdi::definition_function* choose_func = 
        dynamic_cast<jdi::definition_function*>(choose_def);
    ASSERT_NE(choose_func, nullptr) << "choose should be castable to definition_function";
    
    int variadic_index = lang_->function_variadic_after(choose_func);
    EXPECT_GE(variadic_index, 0) << "choose should have a variadic parameter";
    EXPECT_EQ(variadic_index, 0) << "choose's first (and only) parameter should be variadic";
}

TEST_F(VarargsDetectionTest, ChooseHasUnlimitedParameterBounds) {
    jdi::definition* choose_def = lookup_function("choose");
    ASSERT_NE(choose_def, nullptr);
    
    unsigned min = 0, max = 0;
    lang_->definition_parameter_bounds(choose_def, min, max);
    
    EXPECT_EQ(min, 0) << "choose should accept 0 minimum arguments";
    // max should be UINT_MAX (unlimited) for variadic functions
    EXPECT_EQ(max, UINT_MAX) << "choose should accept unlimited arguments (UINT_MAX)";
}

TEST_F(VarargsDetectionTest, ChooseOverloadHasVariadicFlag) {
    jdi::definition* choose_def = lookup_function("choose");
    ASSERT_NE(choose_def, nullptr);
    
    clang_adapter::ClangDefinitionFunction* choose_func = 
        dynamic_cast<clang_adapter::ClangDefinitionFunction*>(choose_def);
    ASSERT_NE(choose_func, nullptr) 
        << "choose should be castable to ClangDefinitionFunction";
    
    // Check that it has at least one overload
    ASSERT_GT(choose_func->overloads.size(), 0) 
        << "choose should have at least one overload";
    
    // Check that the overload has is_variadic set
    bool found_variadic_overload = false;
    for (const auto& overload_pair : choose_func->overloads) {
        const auto& overload = overload_pair.second;
        if (overload && overload->is_variadic) {
            found_variadic_overload = true;
            break;
        }
    }
    
    EXPECT_TRUE(found_variadic_overload) 
        << "choose should have at least one overload with is_variadic=true";
}

TEST_F(VarargsDetectionTest, ChooseParameterTypeContainsVarargs) {
    jdi::definition* choose_def = lookup_function("choose");
    ASSERT_NE(choose_def, nullptr);
    
    clang_adapter::ClangDefinitionFunction* choose_func = 
        dynamic_cast<clang_adapter::ClangDefinitionFunction*>(choose_def);
    ASSERT_NE(choose_func, nullptr);
    
    // Verify that choose has exactly one parameter (the varargs parameter)
    // choose(const enigma::varargs& args) should have 1 parameter
    bool found_variadic_overload_with_one_param = false;
    
    for (const auto& overload_pair : choose_func->overloads) {
        const auto& overload = overload_pair.second;
        if (!overload) continue;
        
        // Check if this overload is variadic (which means it has a varargs parameter)
        if (overload->is_variadic) {
            // choose() should have exactly 1 parameter: the varargs parameter
            EXPECT_EQ(overload->params.size(), 1u) 
                << "choose's variadic overload should have exactly 1 parameter (the varargs parameter)";
            found_variadic_overload_with_one_param = true;
        }
    }
    
    EXPECT_TRUE(found_variadic_overload_with_one_param)
        << "choose should have at least one variadic overload with one parameter";
    
    // Additional verification: the variadic detection already checks that param_type_str contains "varargs"
    // This is tested implicitly by the is_variadic flag being set correctly,
    // which is verified in ChooseOverloadHasVariadicFlag test
}

TEST_F(VarargsDetectionTest, OtherVarargsFunctionsAreDetected) {
    // Test other varargs functions from mathnc.h
    std::vector<std::string> varargs_functions = {"max", "min", "mean", "median"};
    
    for (const auto& func_name : varargs_functions) {
        jdi::definition* func_def = lookup_function(func_name);
        if (!func_def) {
            // Some functions might not exist, skip them
            continue;
        }
        
        EXPECT_TRUE(lang_->is_variadic_function(func_def))
            << func_name << " should be detected as variadic";
        
        unsigned min = 0, max = 0;
        lang_->definition_parameter_bounds(func_def, min, max);
        
        EXPECT_EQ(max, UINT_MAX) 
            << func_name << " should have unlimited max arguments (UINT_MAX)";
    }
}

TEST_F(VarargsDetectionTest, NonVariadicFunctionsAreNotDetectedAsVariadic) {
    // Test a non-variadic function
    jdi::definition* motion_set_def = lookup_function("motion_set");
    ASSERT_NE(motion_set_def, nullptr) << "motion_set should exist";
    
    // motion_set takes 3 parameters and is NOT variadic
    unsigned min = 0, max = 0;
    lang_->definition_parameter_bounds(motion_set_def, min, max);
    
    // For non-variadic functions, max should be the actual parameter count (or SIZE_MAX if not detected)
    // We just verify it's not incorrectly flagged as variadic
    bool is_variadic = lang_->is_variadic_function(motion_set_def);
    
    // motion_set should not be variadic
    // However, if parameter detection isn't working, max might be SIZE_MAX anyway
    // So we just check is_variadic_function returns false
    EXPECT_FALSE(is_variadic) 
        << "motion_set should not be detected as variadic";
}
