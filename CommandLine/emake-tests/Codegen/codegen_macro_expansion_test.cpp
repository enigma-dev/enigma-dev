#include <gtest/gtest.h>
#include "parsing/macros.h"
#include "parsing/lexer.h"
#include "parsing/error_reporting.h"
#include <sstream>
#include <string>

using namespace enigma::parsing;

class CodegenMacroExpansionTest : public ::testing::Test {
protected:
    void SetUp() override {
        err_handler_ = std::make_unique<StdErrorHandler>();
        // Use ForPreprocessorEvaluation() which returns a const reference
        context_ = &ParseContext::ForPreprocessorEvaluation();
    }
    
    void TearDown() override {
        err_handler_.reset();
    }
    
    // Helper to create a variadic macro similar to string(...)
    std::unique_ptr<Macro> createStringMacro() {
        // Create macro: #define string(...) toString(__VA_ARGS__)
        std::vector<std::string> params;  // Empty params list, variadic
        bool is_variadic = true;
        
        // Create tokens for the macro body: toString(__VA_ARGS__)
        TokenVector value_tokens;
        
        // toString
        CodeSnippet snippet1;
        snippet1.content = "toString";
        Token token1(TT_IDENTIFIER, snippet1);
        value_tokens.push_back(token1);
        
        // (
        CodeSnippet snippet2;
        snippet2.content = "(";
        Token token2(TT_BEGINPARENTH, snippet2);
        value_tokens.push_back(token2);
        
        // __VA_ARGS__
        CodeSnippet snippet3;
        snippet3.content = "__VA_ARGS__";
        Token token3(TT_IDENTIFIER, snippet3);
        value_tokens.push_back(token3);
        
        // )
        CodeSnippet snippet4;
        snippet4.content = ")";
        Token token4(TT_ENDPARENTH, snippet4);
        value_tokens.push_back(token4);
        
        return std::make_unique<Macro>("string", std::move(params), is_variadic, 
                                       std::move(value_tokens), err_handler_.get());
    }
    
    // Helper to tokenize a string
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

// Test that variadic macro with single argument expands correctly
// Note: This test verifies the macro substitution logic directly since we can't
// modify the const ParseContext from ForPreprocessorEvaluation()
TEST_F(CodegenMacroExpansionTest, VariadicMacroSingleArgument) {
    auto macro = createStringMacro();
    
    // Test direct macro substitution: string(score) should expand to toString(score)
    // Create argument: score
    CodeSnippet arg_snippet;
    arg_snippet.content = "score";
    Token arg_token(TT_IDENTIFIER, arg_snippet);
    TokenVector arg_tokens;
    arg_tokens.push_back(arg_token);
    
    // Create args vector (for variadic macro with one variadic arg)
    std::vector<TokenVector> args;
    args.push_back(arg_tokens);  // The variadic argument
    
    // Preprocess the argument
    std::vector<TokenVector> args_evald;
    args_evald.push_back(arg_tokens);
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    // Substitute and unroll
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    
    std::string result_str = tokensToString(result);
    // Should expand to toString(score), not toString(__VA_ARGS__)
    EXPECT_NE(result_str.find("toString"), std::string::npos) 
        << "Macro should expand to toString";
    EXPECT_NE(result_str.find("score"), std::string::npos) 
        << "Macro should include the argument 'score'";
    EXPECT_EQ(result_str.find("__VA_ARGS__"), std::string::npos) 
        << "__VA_ARGS__ should be substituted, not appear literally. Result: " << result_str;
}

// Test that variadic macro with multiple arguments expands correctly
TEST_F(CodegenMacroExpansionTest, VariadicMacroMultipleArguments) {
    auto macro = createStringMacro();
    
    // Create arguments: x, y
    CodeSnippet arg1_snippet, arg2_snippet;
    arg1_snippet.content = "x";
    arg2_snippet.content = "y";
    Token arg1_token(TT_IDENTIFIER, arg1_snippet);
    Token arg2_token(TT_IDENTIFIER, arg2_snippet);
    
    // For variadic macros, all variadic args go into a single argument vector
    TokenVector variadic_args;
    variadic_args.push_back(arg1_token);
    // Add comma token
    CodeSnippet comma_snippet;
    comma_snippet.content = ",";
    Token comma_token(TT_COMMA, comma_snippet);
    variadic_args.push_back(comma_token);
    variadic_args.push_back(arg2_token);
    
    std::vector<TokenVector> args;
    args.push_back(variadic_args);  // All variadic args in one vector
    
    std::vector<TokenVector> args_evald;
    args_evald.push_back(variadic_args);
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    std::string result_str = tokensToString(result);
    
    EXPECT_NE(result_str.find("toString"), std::string::npos);
    EXPECT_NE(result_str.find("x"), std::string::npos);
    EXPECT_NE(result_str.find("y"), std::string::npos);
    EXPECT_EQ(result_str.find("__VA_ARGS__"), std::string::npos)
        << "Result: " << result_str;
}

// Test that variadic macro with no arguments expands correctly
TEST_F(CodegenMacroExpansionTest, VariadicMacroNoArguments) {
    auto macro = createStringMacro();
    
    // No arguments provided - empty args vector
    std::vector<TokenVector> args;  // Empty - no variadic args
    
    std::vector<TokenVector> args_evald;
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    std::string result_str = tokensToString(result);
    
    EXPECT_NE(result_str.find("toString"), std::string::npos);
    // When no args provided, __VA_ARGS__ should expand to empty (no tokens between parentheses)
    EXPECT_EQ(result_str.find("__VA_ARGS__"), std::string::npos)
        << "Result: " << result_str;
}

// Test direct macro substitution (not via lexer, testing SubstituteAndUnroll directly)
TEST_F(CodegenMacroExpansionTest, DirectMacroSubstitution) {
    auto macro = createStringMacro();
    
    // Create argument: score
    CodeSnippet arg_snippet;
    arg_snippet.content = "score";
    Token arg_token(TT_IDENTIFIER, arg_snippet);
    TokenVector arg_tokens;
    arg_tokens.push_back(arg_token);
    
    // Create args vector (for variadic macro with one variadic arg)
    std::vector<TokenVector> args;
    args.push_back(arg_tokens);  // The variadic argument
    
    // Preprocess the argument
    std::vector<TokenVector> args_evald;
    args_evald.push_back(arg_tokens);
    
    Macro::StringifiedSet stringified_macros;
    CodeSnippet test_snippet;
    test_snippet.content = std::string("test");
    ErrorContext errc(err_handler_.get(), test_snippet);
    
    // Substitute and unroll
    TokenVector result = macro->SubstituteAndUnroll(args, args_evald, errc, stringified_macros);
    
    std::string result_str = tokensToString(result);
    
    // Should be toString(score), not toString(__VA_ARGS__)
    EXPECT_NE(result_str.find("toString"), std::string::npos);
    EXPECT_NE(result_str.find("score"), std::string::npos);
    EXPECT_EQ(result_str.find("__VA_ARGS__"), std::string::npos)
        << "Result: " << result_str;
}
