/**
  @file  lang_CPP.cpp
  @brief Implements much of the C++ languages adapter class.

  @section License
    Copyright (C) 2008-2012 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "settings.h"
#include <ctime>
#include <cstdio>
#include "languages/lang_CPP.h"

string lang_CPP::get_name() { return "C++"; }

void lang_CPP::load_extension_locals() {
  if (!namespace_enigma)
    return (cout << "ERROR! ENIGMA NAMESPACE NOT FOUND. THIS SHOULD NOT HAPPEN IF PARSE SUCCEEDED." << endl, void());

  for (unsigned i = 0; i < parsed_extensions.size(); i++)
  {
    if (parsed_extensions[i].implements == "")
      continue;

    jdi::definition* implements = namespace_enigma->look_up(parsed_extensions[i].implements);

    if (!implements or !(implements->flags & jdi::DEF_SCOPE)) {
      cout << "ERROR! Extension implements " << parsed_extensions[i].implements << " without defining it!" << endl;
      continue;
    }
    if (!(implements->flags & jdi::DEF_CLASS)) {
      cout << "WARNING! Extension implements non-class " << parsed_extensions[i].implements << "!" << endl;
    }
    jdi::definition_scope *const iscope = (jdi::definition_scope*) implements;
    for (jdi::definition_scope::defiter it = iscope->members.begin(); it != iscope->members.end(); ++it) {
      if ((!it->second->flags) & jdi::DEF_TYPED) { cout << "WARNING: Non-scalar `" << it->first << "' ignored." << endl; continue; }
        shared_object_locals_.insert(it->second->name);
    }
  }
}

#ifdef _WIN32
 #define byte __windows_byte_workaround
 #include <windows.h>
 #undef byte
 #define DLLEXPORT extern "C" __declspec(dllexport)
   #define DECLARE_TIME_TYPE clock_t
   #define CURRENT_TIME(t) t = clock()
   #define PRINT_TIME(ts, te) (((te - ts) * 1000)/CLOCKS_PER_SEC)
#else
 #define DLLEXPORT extern "C"
 #include <cstdio>
 #include <sys/time.h>
   #define DECLARE_TIME_TYPE timeval
   #define CURRENT_TIME(t) gettimeofday(&t,NULL)
   #define PRINT_TIME(ts,te) ((double(te.tv_sec - ts.tv_sec) + double(te.tv_usec - ts.tv_usec)/1000000.0)*1000)
#endif


#include "settings-parse/parse_ide_settings.h"
#include "settings-parse/crawler.h"

#include <System/builtins.h>

void parser_init();

namespace {

/* TODO: it would be better to use this in the future to translate JDI's
 * tokenization to ENIGMA's own token space.  Legacy JDI just treats macros
 * as big string blobs, though (and pre-segmented string blobs).

// taking content here is a hack; old JDI uses gloss token types instead of one symbol per token
enigma::parsing::TokenType TranslateTokenType(jdip::token_t token,
                                              std::string_view content) {
  using enigma::parsing::TokenType;
  switch (token.type) {
    case jdip::TT_DECFLAG:       return TokenType::TT_TYPE_NAME;
    case jdip::TT_DECLARATOR:    return TokenType::TT_TYPE_NAME;
    case jdip::TT_CLASS:         return TokenType::TT_CLASS;
    case jdip::TT_STRUCT:        return TokenType::TT_STRUCT;

    case jdip::TT_IDENTIFIER:    return TokenType::TT_VARNAME;
    case jdip::TT_DEFINITION:    return TokenType::TT_VARNAME;
    case jdip::TT_TEMPLATE:      return TokenType::TT_ERROR;
    case jdip::TT_TYPENAME:      return TokenType::TT_ERROR;
    case jdip::TT_TYPEDEF:       return TokenType::TT_ERROR;
    case jdip::TT_USING:         return TokenType::TT_ERROR;
    case jdip::TT_PUBLIC:        return TokenType::TT_ERROR;
    case jdip::TT_PRIVATE:       return TokenType::TT_ERROR;
    case jdip::TT_PROTECTED:     return TokenType::TT_ERROR;
    case jdip::TT_FRIEND:        return TokenType::TT_ERROR;
    case jdip::TT_COLON:         return TokenType::TT_COLON;
    case jdip::TT_SCOPE:         return TokenType::TT_SCOPEACCESS;

    case jdip::TT_LEFTPARENTH:   return TokenType::TT_BEGINPARENTH;
    case jdip::TT_RIGHTPARENTH:  return TokenType::TT_ENDPARENTH;
    case jdip::TT_LEFTBRACKET:   return TokenType::TT_BEGINBRACKET;
    case jdip::TT_RIGHTBRACKET:  return TokenType::TT_ENDBRACKET;
    case jdip::TT_LEFTBRACE:     return TokenType::TT_BEGINBRACE;
    case jdip::TT_RIGHTBRACE:    return TokenType::TT_ENDBRACE;
    case jdip::TT_LESSTHAN:      return TokenType::TT_LESS;
    case jdip::TT_GREATERTHAN:   return TokenType::TT_GREATER;
    case jdip::TT_TILDE:         return TokenType::TT_TILDE;
    case jdip::TT_OPERATOR:      return enigma::parsing::Lexer::LookUpOperator(content);
    case jdip::TT_COMMA:         return TokenType::TT_COMMA;
    case jdip::TT_SEMICOLON:     return TokenType::TT_SEMICOLON;
    case jdip::TT_STRINGLITERAL: return TokenType::TT_STRINGLIT;
    case jdip::TT_CHARLITERAL:   return TokenType::TT_CHARLIT;
    case jdip::TT_DECLITERAL:    return TokenType::TT_DECLITERAL;
    case jdip::TT_HEXLITERAL:    return TokenType::TT_HEXLITERAL;
    case jdip::TT_OCTLITERAL:    return TokenType::TT_OCTLITERAL;
    case jdip::TTM_CONCAT:       return TokenType::TTM_CONCAT;
    case jdip::TTM_TOSTRING:     return TokenType::TTM_STRINGIFY;
    case jdip::TT_NEW:           return TokenType::TT_S_NEW;
    case jdip::TT_DELETE:        return TokenType::TT_S_DELETE;
    case jdip::TT_ENDOFCODE:     return TokenType::TT_ENDOFCODE;

    // ...JDI doesn't actually do these, right now.
    case jdip::TT_IF:         return TokenType::TT_S_IF;
    case jdip::TT_THEN:       return TokenType::TT_S_THEN;
    case jdip::TT_ELSE:       return TokenType::TT_S_ELSE;
    case jdip::TT_REPEAT:     return TokenType::TT_S_REPEAT;
    case jdip::TT_DO:         return TokenType::TT_S_DO;
    case jdip::TT_WHILE:      return TokenType::TT_S_WHILE;
    case jdip::TT_UNTIL:      return TokenType::TT_S_UNTIL;
    case jdip::TT_FOR:        return TokenType::TT_S_FOR;
    case jdip::TT_SWITCH:     return TokenType::TT_S_SWITCH;
    case jdip::TT_CASE:       return TokenType::TT_S_CASE;
    case jdip::TT_DEFAULT:    return TokenType::TT_S_DEFAULT;
    case jdip::TT_BREAK:      return TokenType::TT_BREAK;
    case jdip::TT_CONTINUE:   return TokenType::TT_CONTINUE;
    case jdip::TT_RETURN:     return TokenType::TT_RETURN;
    case jdip::TT_WITH:       return TokenType::TT_S_WITH;
    case jdip::TT_GLOBAL:     return TokenType::TT_GLOBAL;
    case jdip::TT_LOCAL:      return TokenType::TT_LOCAL;

    case jdip::TT_ENUM:       case jdip::TT_UNION:       case jdip::TT_NAMESPACE:
    case jdip::TT_EXTERN:     case jdip::TT_ASM:         case jdip::TT_OPERATORKW:
    case jdip::TT_CONST_CAST: case jdip::TT_STATIC_CAST: case jdip::TT_DYNAMIC_CAST: case jdip::TT_REINTERPRET_CAST:
    case jdip::TT_ELLIPSIS:   case jdip::TT_MEMBEROF:
    case jdip::TT_SIZEOF:     case jdip::TT_ISEMPTY:
    case jdip::TT_ALIGNAS:    case jdip::TT_ALIGNOF:
    case jdip::TT_DECLTYPE:   case jdip::TT_TYPEID:
    case jdip::TT_AUTO:       case jdip::TT_CONSTEXPR:
    case jdip::TT_TRY:        case jdip::TT_CATCH:     case jdip::TT_NOEXCEPT:
    case jdip::TT_STATIC_ASSERT:
    case jdip::TT_INVALID:
    default:
        return TokenType::TT_ERROR;
  }
}
*/

enigma::parsing::Macro TranslateMacro(const jdip::macro_type &macro,
                                      enigma::parsing::ErrorHandler *herr) {
  using namespace enigma::parsing;
  if (macro.is_function()) {
    std::vector<std::string> arg_list =
        ((const jdip::macro_function*) &macro)->args;
    return enigma::parsing::Macro(macro.name, std::move(arg_list),
          macro.is_variadic(), macro.valueString(), herr);
  }
  return enigma::parsing::Macro(macro.name, macro.valueString(), herr);
}

}  // namespace

syntax_error *lang_CPP::definitionsModified(const char* wscode,
                                            const char* targetYaml) {
  cout << "Parsing settings..." << endl;
  parse_ide_settings(targetYaml, &compatibility_opts_);

  cout << targetYaml << endl;

  cout << "Creating swap." << endl;
  delete main_context;
  main_context = new jdi::context();

  cout << "Dumping whiteSpace definitions..." << endl;
  FILE *of = wscode ? fopen((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h").u8string().c_str(),"wb") : NULL;
  if (of) fputs(wscode,of), fclose(of);

  cout << "Opening ENIGMA for parse..." << endl;

  llreader f((enigma_root/"ENIGMAsystem/SHELL/SHELLmain.cpp").u8string().c_str());
  int res = 1;
  DECLARE_TIME_TYPE ts, te;
  if (f.is_open()) {
    CURRENT_TIME(ts);
    res = main_context->parse_C_stream(f, "SHELLmain.cpp");
    CURRENT_TIME(te);
  }

  jdi::definition *d;
  if ((d = main_context->get_global()->look_up("variant"))) {
    enigma_type__variant = d;
    if (!(d->flags & jdi::DEF_TYPENAME))
      cerr << "ERROR! ENIGMA's variant is not a type!" << endl;
    else
      cout << "Successfully loaded builtin variant type" << endl;
  } else cerr << "ERROR! No variant type found!" << endl;
  if ((d = main_context->get_global()->look_up("var"))) {
    enigma_type__var = d;
    if (!(d->flags & jdi::DEF_TYPENAME))
      cerr << "ERROR! ENIGMA's var is not a type!" << endl;
    else
      cout << "Successfully loaded builtin var type" << endl;
  } else cerr << "ERROR! No var type found!" << endl;
  if ((d = main_context->get_global()->look_up("enigma"))) {
    if (d->flags & jdi::DEF_NAMESPACE) {
      namespace_enigma = (jdi::definition_scope*) d;
      if ((d = namespace_enigma->look_up("varargs"))) {
        enigma_type__varargs = d;
        if (!(d->flags & jdi::DEF_TYPENAME))
          cerr << "ERROR! ENIGMA's varargs is not a type!" << endl;
        else
          cout << "Successfully loaded builtin varargs type" << endl;
      } else cerr << "ERROR! No varargs type found!" << endl;
    } else cerr << "ERROR! Namespace enigma is... not a namespace!" << endl;
  } else cerr << "ERROR! Namespace enigma not found!" << endl;
  namespace_enigma_user = main_context->get_global();
  if ((d = main_context->get_global()->look_up("enigma_user"))) {
    if (d->flags & jdi::DEF_NAMESPACE) {
      namespace_enigma_user = (jdi::definition_scope*) d;
    } else cerr << "ERROR! Namespace enigma_user is... not a namespace!" << endl;
  } else cerr << "ERROR! Namespace enigma_user not found!" << endl;

  if (res) {
    cout << "ERROR in parsing engine file: The parser isn't happy. Don't worry, it's never happy.\n";

    ide_passback_error.set(0,0,0,"Parse failed; details in stdout. Bite me.");
    cout << "Continuing anyway." << endl;
    // return &ide_passback_error;
  } else {
    cout << "Successfully parsed ENIGMA's engine (" << PRINT_TIME(ts,te) << "ms)\n"
    << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    //cout << "Namespace std contains " << global_scope.members["std"]->members.size() << " items.\n";
  }

  cout << "Creating dummy primitives for old ENIGMA" << endl;
  for (jdip::tf_iter it = jdip::builtin_declarators.begin(); it != jdip::builtin_declarators.end(); ++it) {
    main_context->get_global()->members[it->first] = new jdi::definition(it->first, main_context->get_global(), jdi::DEF_TYPENAME);
  }

  enigma::parsing::StdErrorHandler hack;  // TODO: FIXME: This should be using a central error handler...
  cout << "Translating macros to EDL...\n";
  for (const auto &macro_pair : main_context->get_macros())
    builtin_macros_.insert({macro_pair.first,
                            TranslateMacro(*macro_pair.second, &hack)});

  cout << "Initializing EDL Parser...\n";

  parser_init();

  cout << "Grabbing locals...\n";

  load_shared_locals();  // Extensions were separated above

  cout << "Determining build target...\n";

  extensions::determine_target();

  cout << " Done.\n";

  return &ide_passback_error;
}

#include "compiler/compile_common.h"

int lang_CPP::load_shared_locals() {
  cout << "Finding parent..."; fflush(stdout);

  // Find namespace enigma
  jdi::definition* pscope = main_context->get_global()->look_up("enigma");
  if (!pscope or !(pscope->flags & jdi::DEF_SCOPE)) {
    cerr << "ERROR! Can't find namespace enigma!" << endl;
    return 1;
  }
  jdi::definition_scope* ns_enigma = (jdi::definition_scope*)pscope;
  jdi::definition* parent = ns_enigma->look_up(system_get_uppermost_tier());
    if (!parent) {
      cerr << "ERROR! Failed to find parent scope `" << system_get_uppermost_tier() << endl;
      return 2;
    }
  if (not(parent->flags & jdi::DEF_CLASS)) {
    cerr << "PARSE ERROR! Parent class is not a class?" << endl;
    cout << parent->parent->name << "::" << parent->name << ":  " << parent->toString() << endl;
    return 3;
  }
  jdi::definition_class *pclass = (jdi::definition_class*)parent;

  // Find the parent object
  cout << "Found parent scope" << endl;

  shared_object_locals_.clear();

  //Iterate the tiers of the parent object
  for (jdi::definition_class *cs = pclass; cs; cs = (cs->ancestors.size() ? cs->ancestors[0].def : NULL) )
  {
    cout << " >> Checking ancestor " << cs->name << endl;
    for (jdi::definition_scope::defiter mem = cs->members.begin(); mem != cs->members.end(); ++mem)
      shared_object_locals_.insert(mem->first);
  }

  load_extension_locals();
  return 0;
}

jdi::definition* lang_CPP::look_up(const string &name) const {
  auto builtin = jdip::builtin_declarators.find(name);
  if (builtin != jdip::builtin_declarators.end()) return builtin->second->def;
  return namespace_enigma_user->find_local(name);
}

// TODO: This could use better plumbing.
lang_CPP::lang_CPP(): evdata_(ParseEventFile((enigma_root/"events.ey").u8string())) {}

