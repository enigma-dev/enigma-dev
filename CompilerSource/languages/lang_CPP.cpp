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
#include "languages/clang_adapter.h"  // For ClangContext full definition

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
    // JDI removed - defiter is now std::map iterator
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

// JDI removed - using clang_adapter instead
// #include <System/builtins.h>

namespace {

// JDI removed - macro_type doesn't exist
// clang_adapter already returns enigma::parsing::Macro, so just return it as-is
// This function may no longer be needed, but keeping for compatibility
enigma::parsing::Macro TranslateMacro(const enigma::parsing::Macro &macro,
                                      enigma::parsing::ErrorHandler *herr) {
  // Macros from clang_adapter are already in the correct format
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
  cout.flush();

  // JDI removed - llreader and parse_stream don't exist in clang_adapter
  // Use parse_file instead
  int res = 1;
  DECLARE_TIME_TYPE ts, te;
  CURRENT_TIME(ts);
  // Include the codegen_directory where generated headers like API_Switchboard.h are placed
  std::vector<std::string> extra_include_dirs;
  extra_include_dirs.push_back(codegen_directory.u8string());
  res = main_context->parse_file((enigma_root/"ENIGMAsystem/SHELL/SHELLmain.cpp").u8string(), extra_include_dirs);
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
  cerr << "\n*** DEBUG: Looking up enigma_user namespace... ***" << endl;
  cerr.flush();  // Use cerr and flush to ensure it appears
  if ((d = main_context->get_global()->look_up("enigma_user"))) {
    cerr << "*** DEBUG: Found enigma_user definition, flags: 0x" << std::hex << d->flags << std::dec << " ***" << endl;
    if (d->flags & jdi::DEF_NAMESPACE) {
      namespace_enigma_user = (jdi::definition_scope*) d;
      
      // Print all functions in enigma_user namespace
      cerr << "\n*** === Functions in enigma_user namespace === ***" << endl;
      clang_adapter::ClangDefinitionScope* enigma_user_scope = 
          dynamic_cast<clang_adapter::ClangDefinitionScope*>(namespace_enigma_user);
      if (enigma_user_scope) {
        cerr << "*** DEBUG: Successfully cast to ClangDefinitionScope, members count: " 
             << enigma_user_scope->members.size() << " ***" << endl;
        int function_count = 0;
        int total_members = 0;
        for (const auto& member_pair : enigma_user_scope->members) {
          total_members++;
          const std::string& name = member_pair.first;
          const auto& member = member_pair.second;
          if (!member) continue;
          
          if (member->flags & jdi::DEF_FUNCTION) {
            function_count++;
            clang_adapter::ClangDefinitionFunction* func = 
                dynamic_cast<clang_adapter::ClangDefinitionFunction*>(member.get());
            if (func) {
              cerr << "  Function: " << name 
                   << " (overloads: " << func->overloads.size() 
                   << ", template overloads: " << func->template_overloads.size() << ")";
              for (const auto& overload_pair : func->overloads) {
                const auto& overload = overload_pair.second;
                if (overload) {
                  cerr << " [" << overload->params.size() << " params";
                  if (overload->is_variadic) cerr << ", variadic";
                  cerr << "]";
                }
              }
              cerr << endl;
              cerr.flush();
            } else {
              cerr << "  Function: " << name << " (NOT ClangDefinitionFunction - flags: 0x" 
                   << std::hex << member->flags << std::dec << ")" << endl;
            }
          }
        }
        cerr << "Total members: " << total_members << ", Functions found: " << function_count << endl;
        cerr << "*** === End enigma_user functions === ***\n" << endl;
        cerr.flush();  // Use cerr and flush to ensure it appears
      } else {
        cerr << "*** WARNING: enigma_user is not a ClangDefinitionScope! Type: " 
             << typeid(*namespace_enigma_user).name() << endl;
      }
    } else {
      cerr << "ERROR! Namespace enigma_user is... not a namespace! Flags: 0x" 
           << std::hex << d->flags << std::dec << endl;
    }
  } else {
    cerr << "ERROR! Namespace enigma_user not found!" << endl;
    cerr << "*** DEBUG: Available top-level definitions: ***" << endl;
    clang_adapter::ClangDefinitionScope* global_scope = 
        dynamic_cast<clang_adapter::ClangDefinitionScope*>(main_context->get_global());
    if (global_scope) {
      for (const auto& member_pair : global_scope->members) {
        cerr << "  " << member_pair.first << " (flags: 0x" << std::hex 
             << (member_pair.second ? member_pair.second->flags : 0) << std::dec << ")" << endl;
      }
    }
  }
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
  // Initialize builtin type constants from clang context
  if (main_context) {
    jdi::definition* d;
    if ((d = main_context->get_global()->look_up("int"))) {
      jdi::builtin_type__int = d;
    }
    // Can add more builtin types here as needed: char, void, float, double, bool, etc.
  }

  enigma::parsing::StdErrorHandler hack;  // TODO: FIXME: This should be using a central error handler...
  cout << "Translating macros to EDL...\n";
  for (const auto &macro_pair : main_context->get_macros())
    builtin_macros_.insert({macro_pair.first,
                            TranslateMacro(*macro_pair.second, &hack)});

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
  // JDI removed - ancestors structure changed in clang_adapter
  // ancestors is now vector<pair<ClangDefinitionClass*, int>>, not with .def
  for (jdi::definition_class *cs = pclass; cs; cs = (cs->ancestors.size() ? cs->ancestors[0].first : NULL) )
  {
    cout << " >> Checking ancestor " << cs->name << endl;
    // JDI removed - defiter is now std::map iterator
    for (auto mem = cs->members.begin(); mem != cs->members.end(); ++mem)
      shared_object_locals_.insert(mem->first);
  }

  load_extension_locals();
  return 0;
}

jdi::definition* lang_CPP::look_up(std::string_view n) const {
  // TODO: FIXME: slow-ass conversion still exists...
  std::string name(n);
  // JDI removed - builtin_declarators no longer exists, use main_context lookup instead
  // auto builtin = jdi::builtin_declarators.find(name);
  // if (builtin != jdi::builtin_declarators.end()) return builtin->second->def;
  if (main_context) {
    jdi::definition* found = main_context->look_up(name);
    if (found) return found;
  }
  return namespace_enigma_user ? namespace_enigma_user->find_local(name) : nullptr;
}

// TODO: This could use better plumbing.
lang_CPP::lang_CPP(): evdata_(ParseEventFile((enigma_root/"events.ey").u8string())) {}
