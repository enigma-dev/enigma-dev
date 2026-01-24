// Test for the function-like macro expansion bug fix
// Bug: For function-like macros, the start_idx for extracting the macro body was incorrectly calculated.
// Fix: Correctly track param_end_idx to find the closing ')' of the parameter list in clang_adapter.cpp
//
// The issue: For macros like joystick_check_button(id, numb), the start_idx was wrong,
// causing the first token of the expanded macro to be incorrect (e.g., an unexpected ')').
// This manifested as "Unmatched closing parenthesis" errors when parsing code using such macros.

#include <gtest/gtest.h>
#include "parsing/macros.h"
#include "parsing/lexer.h"
#include "parsing/error_reporting.h"
#include <sstream>
#include <string>

using namespace enigma::parsing;

class ParserMacroNestedCallTest : public ::testing::Test {
protected:
    void SetUp() override {
        err_handler_ = std::make_unique<StdErrorHandler>();
        context_ = &ParseContext::ForPreprocessorEvaluation();
    }
    
    void TearDown() override {
        err_handler_.reset();
    }
    
    // Helper to create a function-like macro with parameters
    // Creates macro: #define name(params...) body
    std::unique_ptr<Macro> createFunctionMacro(const std::string& name,
                                                const std::vector<std::string>& params,
                                                const std::string& body) {
        // Tokenize the body
        auto owned_body = std::make_shared<std::string>(body);
        Lexer lex(owned_body, context_, err_handler_.get());
        lex.UseCppOptions();
        
        TokenVector body_tokens;
        for (auto t = lex.ReadToken(); t.type != TT_ENDOFCODE; t = lex.ReadToken()) {
            body_tokens.push_back(t);
        }
        
        return std::make_unique<Macro>(name, std::vector<std::string>(params), 
                                       false, // not variadic
                                       std::move(body_tokens), err_handler_.get());
    }
    
    // Helper to tokenize argument strings
    TokenVector tokenize(const std::string& code) {
        auto owned_string = std::make_shared<std::string>(code);
        Lexer lex(owned_string, context_, err_handler_.get());
        lex.UseCppOptions();
        TokenVector tokens;
        for (auto t = lex.ReadToken(); t.type != TT_ENDOFCODE; t = lex.ReadToken()) {
            tokens.push_back(t);
        }
        return tokens;
    }
    
    // Helper to convert tokens to string for comparison
    std::string tokensToString(const TokenVector& tokens) {
        std::string result;
        for (const auto& token : tokens) {
            if (!result.empty()) result += " ";
            result += token.content;
        }
        return result;
    }
    
    std::unique_ptr<StdErrorHandler> err_handler_;
    const ParseContext* context_;
};

// Test function-like macro with two parameters
// This tests the scenario where param_end_idx must correctly track the closing ')'
TEST_F(ParserMacroNestedCallTest, TwoParameterMacroExpansion) {
    // Create macro: #define joystick_check_button(id, numb) __joystick_check_button(id, numb)
    std::vector<std::string> params = {"id", "numb"};
    auto macro = createFunctionMacro("joystick_check_button", params, "__joystick_check_button(id, numb)");
    
    // Create arguments for the macro call: joystick_check_button(0, 1)
    TokenVector arg0 = tokenize("0");
    TokenVector arg1 = tokenize("1");
    
    std::vector<TokenVector> args = {arg0, arg1};
    std::vector<TokenVector> args_evald = {arg0, arg1};
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    // Substitute and unroll
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    std::string result_str = tokensToString(result);
    
    // The result should be: __joystick_check_button ( 0 , 1 )
    // NOT start with ')' (which was the bug)
    EXPECT_FALSE(result.empty()) << "Macro expansion should produce tokens";
    
    if (!result.empty()) {
        // First token should be the function name, NOT a closing parenthesis
        EXPECT_EQ(result[0].type, TT_IDENTIFIER)
            << "First token should be identifier, not '" << result[0].content << "'";
        EXPECT_EQ(result[0].content, "__joystick_check_button")
            << "First token should be the function name";
        
        // Should NOT start with ')'
        EXPECT_NE(result[0].content, ")")
            << "BUG: First token is ')' which indicates param_end_idx was wrong";
    }
    
    // Verify the full expansion contains expected content
    EXPECT_NE(result_str.find("__joystick_check_button"), std::string::npos)
        << "Result should contain the function name: " << result_str;
    EXPECT_NE(result_str.find("0"), std::string::npos)
        << "Result should contain first argument: " << result_str;
    EXPECT_NE(result_str.find("1"), std::string::npos)
        << "Result should contain second argument: " << result_str;
}

// Test macro with single parameter (simpler case)
TEST_F(ParserMacroNestedCallTest, SingleParameterMacroExpansion) {
    // Create macro: #define negate(x) -(x)
    std::vector<std::string> params = {"x"};
    auto macro = createFunctionMacro("negate", params, "-(x)");
    
    // Create argument: 5
    TokenVector arg0 = tokenize("5");
    
    std::vector<TokenVector> args = {arg0};
    std::vector<TokenVector> args_evald = {arg0};
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    std::string result_str = tokensToString(result);
    
    // Should be: - ( 5 )
    EXPECT_FALSE(result.empty());
    
    if (!result.empty()) {
        // First token should be '-', not ')'
        EXPECT_EQ(result[0].content, "-")
            << "First token should be '-', got '" << result[0].content << "'";
    }
    
    EXPECT_NE(result_str.find("5"), std::string::npos)
        << "Result should contain the argument: " << result_str;
}

// Test macro with nested function call in body
TEST_F(ParserMacroNestedCallTest, NestedFunctionCallInBody) {
    // Create macro: #define MY_MAX(lhs, rhs) compute_max(lhs, rhs)
    // Use unique parameter names that won't appear in other tokens
    std::vector<std::string> params = {"lhs", "rhs"};
    auto macro = createFunctionMacro("MY_MAX", params, "compute_max(lhs, rhs)");
    
    // Create arguments: 10, 20
    TokenVector arg0 = tokenize("10");
    TokenVector arg1 = tokenize("20");
    
    std::vector<TokenVector> args = {arg0, arg1};
    std::vector<TokenVector> args_evald = {arg0, arg1};
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    std::string result_str = tokensToString(result);
    
    // Should be: compute_max ( 10 , 20 )
    EXPECT_FALSE(result.empty());
    
    if (!result.empty()) {
        EXPECT_EQ(result[0].type, TT_IDENTIFIER)
            << "First token should be identifier 'compute_max'";
        EXPECT_EQ(result[0].content, "compute_max")
            << "First token should be 'compute_max', got '" << result[0].content << "'";
    }
    
    // Verify arguments were substituted
    EXPECT_NE(result_str.find("10"), std::string::npos);
    EXPECT_NE(result_str.find("20"), std::string::npos);
    // Parameter names should be fully substituted and not appear in result
    EXPECT_EQ(result_str.find("lhs"), std::string::npos)
        << "Parameter 'lhs' should be substituted. Result: " << result_str;
    EXPECT_EQ(result_str.find("rhs"), std::string::npos)
        << "Parameter 'rhs' should be substituted. Result: " << result_str;
}

// Test macro with complex expression as argument
TEST_F(ParserMacroNestedCallTest, ComplexExpressionAsArgument) {
    // Create macro: #define SQUARE(x) ((x) * (x))
    std::vector<std::string> params = {"x"};
    auto macro = createFunctionMacro("SQUARE", params, "((x) * (x))");
    
    // Create argument: a + b (complex expression)
    TokenVector arg0 = tokenize("a + b");
    
    std::vector<TokenVector> args = {arg0};
    std::vector<TokenVector> args_evald = {arg0};
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    std::string result_str = tokensToString(result);
    
    // Should be: ( ( a + b ) * ( a + b ) )
    EXPECT_FALSE(result.empty());
    
    if (!result.empty()) {
        // First token should be '(', not anything else
        EXPECT_EQ(result[0].content, "(")
            << "First token should be '(', got '" << result[0].content << "'";
    }
    
    // The expression should appear twice (once for each x)
    size_t first_a = result_str.find("a");
    size_t second_a = result_str.find("a", first_a + 1);
    EXPECT_NE(first_a, std::string::npos);
    EXPECT_NE(second_a, std::string::npos)
        << "Expression 'a + b' should appear twice";
}

// Test that parentheses are balanced in the output
TEST_F(ParserMacroNestedCallTest, BalancedParenthesesInOutput) {
    // Create macro: #define CALL(f, x) f(x)
    std::vector<std::string> params = {"f", "x"};
    auto macro = createFunctionMacro("CALL", params, "f(x)");
    
    TokenVector arg0 = tokenize("myFunc");
    TokenVector arg1 = tokenize("42");
    
    std::vector<TokenVector> args = {arg0, arg1};
    std::vector<TokenVector> args_evald = {arg0, arg1};
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    
    // Count parentheses to ensure they're balanced
    int paren_count = 0;
    for (const auto& token : result) {
        if (token.type == TT_BEGINPARENTH) paren_count++;
        else if (token.type == TT_ENDPARENTH) paren_count--;
        
        // Should never go negative (more ')' than '(')
        EXPECT_GE(paren_count, 0)
            << "Unbalanced parentheses: more ')' than '(' at this point";
    }
    
    // Should end balanced
    EXPECT_EQ(paren_count, 0)
        << "Unbalanced parentheses in macro expansion output";
}
