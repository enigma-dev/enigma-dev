cmake_minimum_required(VERSION 3.11)

set(LIB "compileEGMf")
set(LIB_VERSION "0.1")
set(LIB_DESCRIPTION "enigma compiler")

set(CMAKE_DEBUG_POSTFIX d)

project(${LIB} VERSION ${LIB_VERSION} DESCRIPTION ${LIB_DESCRIPTION})

set(COMPILER_SOURCES
    frontend.cpp
    main.cpp
    gcc_interface/gcc_backend.cpp
    settings.cpp
    syntax/syntax.cpp
    makedir.cpp
    parser/collect_variables.cpp
    parser/object_storage.cpp
    parser/parser_components.cpp
    parser/parser.cpp
    standalone_main.cpp
    filesystem/file_find.cpp
    backend/JavaCallbacks.cpp
    backend/ideprint.cpp
    languages/language_adapter.cpp
    languages/lang_CPP.cpp
    settings-parse/parse_ide_settings.cpp
    settings-parse/crawler.cpp
    general/darray.cpp
    general/macro_integration.cpp
    general/bettersystem.cpp
    general/string.cpp
    JDI/test/debug_lexer.cpp
    JDI/src/Storage/definition.cpp
    JDI/src/Storage/value_funcs.cpp
    JDI/src/Storage/value.cpp
    JDI/src/Storage/full_type.cpp
    JDI/src/Storage/references.cpp
    JDI/src/System/token.cpp
    JDI/src/System/symbols.cpp
    JDI/src/System/lex_buffer.cpp
    JDI/src/System/builtins.cpp
    JDI/src/System/macros.cpp
    JDI/src/System/lex_cpp.cpp
    JDI/src/API/jdi.cpp
    JDI/src/API/lexer_interface.cpp
    JDI/src/API/user_tokens.cpp
    JDI/src/API/error_reporting.cpp
    JDI/src/API/AST.cpp
    JDI/src/API/AST_Export.cpp
    JDI/src/API/context.cpp
    JDI/src/Parser/parse_context.cpp
    JDI/src/Parser/handlers/handle_hypothetical.cpp
    JDI/src/Parser/handlers/handle_enum.cpp
    JDI/src/Parser/handlers/handle_function_impl.cpp
    JDI/src/Parser/handlers/handle_declarators.cpp
    JDI/src/Parser/handlers/handle_templates.cpp
    JDI/src/Parser/handlers/handle_namespace.cpp
    JDI/src/Parser/handlers/handle_scope.cpp
    JDI/src/Parser/handlers/handle_class.cpp
    JDI/src/Parser/handlers/handle_union.cpp
    JDI/src/Parser/readers/read_qualified_definition.cpp
    JDI/src/Parser/readers/read_expression.cpp
    JDI/src/Parser/readers/read_next_token.cpp
    JDI/src/Parser/readers/read_operatorkw_name.cpp
    JDI/src/Parser/readers/read_type.cpp
    JDI/src/Parser/readers/read_template_parameters.cpp
    JDI/src/Parser/base.cpp
    JDI/src/General/debug_macros.cpp
    JDI/src/General/llreader.cpp
    JDI/src/General/parse_basics.cpp
    JDI/src/General/svg_simple.cpp
    compiler/components/write_room_data.cpp
    compiler/components/module_write_sounds.cpp
    compiler/components/module_write_sprites.cpp
    compiler/components/parse_and_link.cpp
    compiler/components/module_write_fonts.cpp
    compiler/components/write_globals.cpp
    compiler/components/module_write_backgrounds.cpp
    compiler/components/handle_templates.cpp
    compiler/components/module_write_paths.cpp
    compiler/components/write_object_data.cpp
    compiler/components/write_object_access.cpp
    compiler/components/write_font_info.cpp
    compiler/components/write_defragged_events.cpp
    compiler/components/parse_secondary.cpp
    compiler/components/write_shader_data.cpp
    compiler/reshandlers/rectpack.cpp
    compiler/reshandlers/refont.cpp
    compiler/jdi_utility.cpp
    compiler/compile_common.cpp
    compiler/compile.cpp
)

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

set(SHARED_SOURCES
   ${ENIGMA_DIR}/shared/eyaml/eyaml.cpp
   ${ENIGMA_DIR}/shared/event_reader/event_parser.cpp
)

add_library(${LIB} SHARED ${COMPILER_SOURCES} ${SHARED_SOURCES})

include_directories(${LIB} PRIVATE "${ENIGMA_DIR}/shared" "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/JDI/src")

set_target_properties(${LIB} PROPERTIES VERSION ${LIB_VERSION})
set_property(TARGET ${LIB} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS " -static")
  target_link_libraries(${LIB} -static-libgcc)
endif()

install(TARGETS ${LIB} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
