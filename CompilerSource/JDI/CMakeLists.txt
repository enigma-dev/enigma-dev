project("JustDefineIt")

cmake_minimum_required(VERSION 3.10)

set(JDI_HDRS
  src/API/AST_forward.h
  src/API/AST_operator.h
  src/API/compile_settings.h
  src/API/AST.h
  src/API/error_reporting.h
  src/API/parse_config.h
  src/API/error_context.h
  src/API/user_tokens.h
  src/API/context.h
  src/Parser/is_potential_constructor.h
  src/Parser/context_parser.h
  src/Parser/handlers/handle_function_impl.h
  src/Parser/cclass_base.h
  src/Storage/definition_forward.h
  src/Storage/definition.h
  src/Storage/full_type.h
  src/Storage/value_funcs.h
  src/Storage/arg_key.h
  src/Storage/references.h
  src/Storage/value.h
  src/User/token_types.h
  src/User/token_cases.h
  src/General/quickreference.h
  src/General/quickvector.h
  src/General/parse_basics.h
  src/General/strings.h
  src/General/llreader.h
  src/General/debug_macros.h
  src/General/svg_simple.h
  src/General/quickstack.h
  src/System/lex_cpp.h
  src/System/builtins.h
  src/System/macros.h
  src/System/symbols.h
  src/System/token.h
  src/System/type_usage_flags.h
)

set(JDI_SRCS
  src/API/user_tokens.cpp
  src/API/AST_operator.cpp
  src/API/AST.cpp
  src/API/AST_Export.cpp
  src/API/context.cpp
  src/API/error_reporting.cpp
  src/Parser/base.cpp
  src/Parser/readers/read_template_parameters.cpp
  src/Parser/readers/read_qualified_definition.cpp
  src/Parser/readers/read_expression.cpp
  src/Parser/readers/read_next_token.cpp
  src/Parser/readers/read_operatorkw_name.cpp
  src/Parser/readers/read_type.cpp
  src/Parser/context_parser.cpp
  src/Parser/handlers/handle_namespace.cpp
  src/Parser/handlers/handle_enum.cpp
  src/Parser/handlers/handle_friend.cpp
  src/Parser/handlers/handle_operatorkw_cast.cpp
  src/Parser/handlers/handle_scope.cpp
  src/Parser/handlers/handle_function_impl.cpp
  src/Parser/handlers/handle_declarators.cpp
  src/Parser/handlers/handle_union.cpp
  src/Parser/handlers/handle_hypothetical.cpp
  src/Parser/handlers/handle_class.cpp
  src/Parser/handlers/handle_templates.cpp
  src/Storage/definition_duplicate.cpp
  src/Storage/value.cpp
  src/Storage/value_funcs.cpp
  src/Storage/full_type.cpp
  src/Storage/arg_key.cpp
  src/Storage/definition.cpp
  src/Storage/references.cpp
  src/General/svg_simple.cpp
  src/General/parse_basics.cpp
  src/General/llreader.cpp
  src/General/debug_macros.cpp
  src/System/macros.cpp
  src/System/lex_cpp.cpp
  src/System/builtins.cpp
  src/System/symbols.cpp
  src/System/token.cpp
)

# Global include dirs
include_directories("src/")

# Global cxx flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WARNING_FLAGS
  "-Wshadow \
  -Winit-self \
  -Wredundant-decls \
  -Wcast-align \
  -Wundef \
  -Wfloat-equal \
  -Wunreachable-code \
  -Wmissing-declarations \
  -Wmissing-include-dirs \
  -Wswitch-enum \
  -Wswitch-default \
  -Wmain \
  -pedantic-errors \
  -pedantic \
  -Wextra \
  -Wall \
  -Werror"
)

# Debug
add_executable("Debug" ${JDI_HDRS} ${JDI_SRCS} "test/MAIN.cc")
set_target_properties("Debug" PROPERTIES OUTPUT_NAME "JustDefineIt")
set_target_properties("Debug" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Debug")
set_target_properties("Debug" PROPERTIES COMPILE_FLAGS "${WARNING_FLAGS} -g -D_GLIBCXX_DEBUG -DDEBUG_MODE")

# Debug Render
add_executable("Debug-Render" ${JDI_HDRS} ${JDI_SRCS} "test/MAIN.cc")
set_target_properties("Debug-Render" PROPERTIES OUTPUT_NAME "JustDefineIt")
set_target_properties("Debug-Render" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Debug-Render")
set_target_properties("Debug-Render" PROPERTIES COMPILE_FLAGS "${WARNING_FLAGS} -g -D_GLIBCXX_DEBUG -DDEBUG_MODE -DRENDER_ASTS")

# Release
add_executable("Release" ${JDI_HDRS} ${JDI_SRCS} "test/MAIN.cc")
set_target_properties("Release" PROPERTIES OUTPUT_NAME "JustDefineIt")
set_target_properties("Release" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release")
set_target_properties("Release" PROPERTIES COMPILE_FLAGS "${WARNING_FLAGS} -fomit-frame-pointer -fexpensive-optimizations -O3")
set_target_properties("Release" PROPERTIES LINK_FLAGS "-s")

# Profile
add_executable("Profile" ${JDI_HDRS} ${JDI_SRCS} "test/MAIN.cc")
set_target_properties("Profile" PROPERTIES OUTPUT_NAME "JustDefineIt")
set_target_properties("Profile" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Profile")
set_target_properties("Profile" PROPERTIES COMPILE_FLAGS "${WARNING_FLAGS} -pg -g")
set_target_properties("Profile" PROPERTIES LINK_FLAGS "-pg")

# Test
set(TESTING_SRCS 
  "test/Testing/error_handler.h" 
  "test/Testing/matchers.h"
  "test/Lexer/lexer_test.cc"
  "test/General/error_handler_test.cc"
)

find_package(GTest REQUIRED)

add_executable("Test" ${JDI_HDRS} ${JDI_SRCS} ${TESTING_SRCS})
target_include_directories("Test" PRIVATE "test/")
set_target_properties("Test" PROPERTIES OUTPUT_NAME "JustDefineIt")
set_target_properties("Test" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Test")
set_target_properties("Test" PROPERTIES COMPILE_FLAGS "${WARNING_FLAGS} -g -D_GLIBCXX_DEBUG -DDEBUG_MODE")
target_link_libraries("Test" GTest::GTest GTest::Main)
