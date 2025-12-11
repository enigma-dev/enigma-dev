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
#include "clang_adapter.h"
#include <clang-c/Index.h>
#include <filesystem>

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
    for (auto it = iscope->members.begin(); it != iscope->members.end(); ++it) {
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

void parser_init();

namespace {

// Helper to convert enigma::parsing::Macro (already in correct format)
enigma::parsing::Macro TranslateMacro(const enigma::parsing::Macro& macro,
                                      enigma::parsing::ErrorHandler *herr) {
  // Macro is already in the correct format, just return a copy
  return macro;
}

}  // namespace

syntax_error *lang_CPP::definitionsModified(const char* wscode,
                                            const char* targetYaml) {
  cout << "Parsing settings..." << endl;
  parse_ide_settings(targetYaml, &compatibility_opts_);

  cout << targetYaml << endl;

  cout << "Creating swap." << endl;
  delete main_context;
  main_context = new clang_adapter::ClangContext();

  cout << "Dumping whiteSpace definitions..." << endl;
  FILE *of = wscode ? fopen((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h").u8string().c_str(),"wb") : NULL;
  if (of) fputs(wscode,of), fclose(of);

  cout << "Opening ENIGMA for parse..." << endl;

  std::string filepath = (enigma_root/"ENIGMAsystem/SHELL/SHELLmain.cpp").u8string();
  int res = 1;
  DECLARE_TIME_TYPE ts, te;
  
  std::vector<std::string> include_dirs;
  std::vector<std::string> defines;
  
  CURRENT_TIME(ts);
  res = main_context->parse_file(filepath, include_dirs, defines);
  CURRENT_TIME(te);

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
  if (jdi::definition *dstd = main_context->get_global()->look_up("std")) {
    if (dstd->flags & jdi::DEF_NAMESPACE) {
      jdi::definition_scope *j_std = (jdi::definition_scope*) dstd;
      jdi::definition *j_string = j_std->look_up("string");
      if (!j_string) cerr << "Error! std::string was not detected! The parse output probably sucks.";
      else if (!(j_string->flags & jdi::DEF_TYPENAME))
        cerr << "Error! std::string is not a type! The parse output probably sucks.";
      else
        cout << "Successfully parsed std::string, so data is probably good.";
    } else cerr << "ERROR! Namespace enigma_user is... not a namespace!" << endl;
  } else cerr << "ERROR! Namespace std not found!" << endl;

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
  // Create basic type definitions manually
  std::vector<std::string> primitives = {"int", "float", "double", "char", "bool", "void", "long", "short", "unsigned", "signed"};
  for (const auto& prim : primitives) {
    auto def = std::make_unique<clang_adapter::ClangDefinition>(
      prim, main_context->get_global(), jdi::DEF_TYPENAME, clang_getNullCursor());
    main_context->get_global()->members[prim] = std::move(def);
  }

  enigma::parsing::StdErrorHandler hack;  // TODO: FIXME: This should be using a central error handler...
  cout << "Translating macros to EDL...\n";
  auto macros = main_context->get_macros();
  for (const auto &macro_pair : macros) {
    builtin_macros_.insert({macro_pair.first,
                            TranslateMacro(*macro_pair.second, &hack)});
  }

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
    if (parent->parent) {
      cout << parent->parent->name << "::" << parent->name << endl;
    } else {
      cout << parent->name << endl;
    }
    return 3;
  }
  jdi::definition_class *pclass = (jdi::definition_class*)parent;

  // Find the parent object
  cout << "Found parent scope" << endl;

  shared_object_locals_.clear();

  //Iterate the tiers of the parent object
  for (jdi::definition_class *cs = pclass; cs; cs = (cs->ancestors.size() > 0 ? cs->ancestors[0].first : NULL) )
  {
    cout << " >> Checking ancestor " << cs->name << endl;
    for (auto mem = cs->members.begin(); mem != cs->members.end(); ++mem)
      shared_object_locals_.insert(mem->first);
  }

  load_extension_locals();
  return 0;
}

jdi::definition* lang_CPP::look_up(std::string_view n) const {
  // TODO: FIXME: slow-ass conversion still exists...
  std::string name(n);
  // Check in enigma_user namespace first
  if (namespace_enigma_user) {
    jdi::definition* result = namespace_enigma_user->find_local(name);
    if (result) return result;
  }
  // Check in global scope
  if (main_context && main_context->get_global()) {
    return main_context->get_global()->find_local(name);
  }
  return nullptr;
}

// TODO: This could use better plumbing.
lang_CPP::lang_CPP(): evdata_(ParseEventFile((enigma_root/"events.ey").u8string())) {}
