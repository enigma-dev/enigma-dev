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

#include <ctime>
#include <cstdio>
#include "languages/lang_CPP.h"

string lang_CPP::get_name() { return "C++"; }

void lang_CPP::load_extension_locals()
{
  locals.clear();
  
  if (!namespace_enigma)
    return (cout << "ERROR! ENIGMA NAMESPACE NOT FOUND. THIS SHOULD NOT HAPPEN IF PARSE SUCCEEDED." << endl, void());
  
  for (unsigned i = 0; i < parsed_extensions.size(); i++)
  {
    if (parsed_extensions[i].implements == "")
      continue;
    
    jdi::definition* found = namespace_enigma->look_up(parsed_extensions[i].implements);
    
    if (!found) {
      cout << "ERROR! Extension implements " << parsed_extensions[i].implements << " without defining it!" << endl;
      return;
    }
    if (~found->flags & jdi::DEF_CLASS) {
      cout << "ERROR! Extension implements non-class " << parsed_extensions[i].implements << "!" << endl;
      return;
    }
    
    jdi::definition_class *cadd = (jdi::definition_class*)found;
    for (jdi::definition_scope::defiter it = cadd->members.begin(); it != cadd->members.end(); it++) {
      if (!it->second->flags & jdi::DEF_TYPED) { cout << "WARNING: Non-scalar `" << it->first << "' ignored." << endl; continue; }
      jdi::definition_typed *t = (jdi::definition_typed*)it->second;
      locals[t->name] = t->type ? t->type->name : "var";
    }
  }
}


const char* heaping_pile_of_dog_shit = "ERROR: Unknown";

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
   #define DECLARE_TIME() clock_t cs, ce
   #define START_TIME() cs = clock()
   #define STOP_TIME() ce = clock()
   #define PRINT_TIME() (((ce - cs) * 1000)/CLOCKS_PER_SEC)
#else
 #define dllexport extern "C"
 #include <cstdio>
 #include <sys/time.h>
   #define DECLARE_TIME()  timeval ts, tn
   #define START_TIME() gettimeofday(&ts,NULL);
   #define STOP_TIME() gettimeofday(&tn,NULL);
   #define PRINT_TIME() ((double(tn.tv_sec - ts.tv_sec) + double(tn.tv_usec - ts.tv_usec)/1000000.0)*1000)
#endif


#include "settings-parse/parse_ide_settings.h"
#include "settings-parse/crawler.h"

#include <System/builtins.h>

extern jdi::definition *enigma_type__var, *enigma_type__variant, *enigma_type__varargs;
void parser_init();

syntax_error *lang_CPP::definitionsModified(const char* wscode, const char* targetYaml)
{
  cout << "Parsing settings..." << endl;
    parse_ide_settings(targetYaml);
  
  cout << targetYaml << endl;
  
  cout << "Creating swap." << endl;
  delete main_context;
  main_context = new jdi::context();
  
  cout << "Dumping whiteSpace definitions...";
  FILE *of = wscode ? fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h","wb") : NULL;
  if (of) fputs(wscode,of), fclose(of);
  
  cout << "Opening ENIGMA for parse..." << endl;
  
  llreader f("ENIGMAsystem/SHELL/SHELLmain.cpp");
  int res = 1;
  DECLARE_TIME();
  if (f.is_open()) {
    START_TIME();
    res = main_context->parse_C_stream(f, "SHELLmain.cpp");
    STOP_TIME();
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
      if ((d = ((jdi::definition_scope*)d)->look_up("varargs"))) {
        enigma_type__varargs = d;
        if (!(d->flags & jdi::DEF_TYPENAME))
          cerr << "ERROR! ENIGMA's varargs is not a type!" << endl;
        else
          cout << "Successfully loaded builtin varargs type" << endl;
      } else cerr << "ERROR! No varargs type found!" << endl;
    } else cerr << "ERROR! Namespace enigma is... not a namespace!" << endl;
  } else cerr << "ERROR! Namespace enigma not found!" << endl;
  
  if (res) {
    cout << "ERROR in parsing engine file: The parser isn't happy. Don't worry, it's never fucking happy. Have a turd.\n";
    cout << heaping_pile_of_dog_shit;
    
    ide_passback_error.set(0,0,0,"Parse failed; details in stdout. Bite me.");
    cout << "Continuing anyway." << endl;
    // return &ide_passback_error;
  } else {    
    cout << "Successfully parsed ENIGMA's engine (" << PRINT_TIME() << "ms)\n"
    << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    //cout << "Namespace std contains " << global_scope.members["std"]->members.size() << " items.\n";
  }
  
  cout << "Creating dummy primitives for old ENIGMA" << endl;
  for (jdip::tf_iter it = jdip::builtin_declarators.begin(); it != jdip::builtin_declarators.end(); ++it) {
    main_context->get_global()->members[it->first] = new jdi::definition(it->first, main_context->get_global(), jdi::DEF_TYPENAME);
  }
  
  cout << "Initializing EDL Parser...\n";
  
  parser_init();
  
  cout << "Grabbing locals...\n";
  
  shared_locals_load(requested_extensions);
  
  cout << "Determining build target...\n";
  
  extensions::determine_target();
  
  cout << " Done.\n";
  
  return &ide_passback_error;
}

#include "compiler/compile_common.h"

int lang_CPP::load_shared_locals()
{
  cout << "Finding parent..." << endl;
  
  jdi::definition_class *parent_class;
  
  // Find the parent object
  jdi::definition_scope::defiter parent = namespace_enigma->members.find(system_get_uppermost_tier());
  if (parent != namespace_enigma->members.end())
    { cout << "ERROR! Failed to find parent class!" << endl; return -1; }
  if (~parent->second->flags & jdi::DEF_CLASS)
    { cout << "ERROR! Parent class is not a class after all..." << endl; return -1; }
  
  parent_class = (jdi::definition_class*)parent->second;
  shared_object_locals.clear();

  //Iterate the tiers of the parent object
  for (jdi::definition_class *cs = parent_class; cs; cs = (cs->ancestors.empty() ? NULL : cs->ancestors[0].def) )
  {
    cout << " >> Checking ancestor " << cs->name << endl;
    for (jdi::definition_scope::defiter mem = cs->members.begin(); mem != cs->members.end(); ++mem) {
      shared_object_locals[mem->first] = 0;
      cout << "  - Found `" << mem->first << "'" << endl;
    }
    cout << endl;
  }

  load_extension_locals();
  //dump_read_locals(shared_object_locals);
  return 0;
}

lang_CPP::~lang_CPP() {
  
}

