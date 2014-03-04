/**
  @file  write_object_access.cpp
  @brief Implements the part of the C++ plugin that creates methods for accessing
         variables from other objects.
  
  @section License
    Copyright (C) 2008-2014 Josh Ventura
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

#include "makedir.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/compile_common.h"
#include "compiler/event_reader/event_parser.h"
#include "parser/object_storage.h"
#include "languages/lang_CPP.h"
#include <general/estring.h>
#include <compiler/compile_includes.h>

int lang_CPP::compile_write_obj_access(compile_context &ctex)
{
  ofstream wto;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_objectaccess.h").c_str(),ios_base::out);
    wto << gen_license;
    wto << "// Depending on how many times your game accesses variables via OBJECT.varname, this file may be empty." << endl << endl;
    wto << "namespace enigma" << endl << "{" << endl;

    wto <<
    "  object_locals ldummy;" << endl <<
    "  object_locals *glaccess(int x)" << endl <<
    "  {" << endl << "    object_locals* ri = (object_locals*)fetch_instance_by_int(x);" << endl << "    return ri ? ri : &ldummy;" << endl << "  }" << endl << endl;
    
    /* *************************************************************** *\
    ** First we write a collection of dummy variables to the file.     **
    ** If the requested variable doesn't exist, the dummy is returned. **
    \* *************************************************************** */
    
    // TODO: NEWPARSER: this is something of poly's; it isn't documented or noted in the license,
    // and appears to use var as the type regardless of the requested variable's type (which probably cannot be casted)
    wto <<
    "  var &map_var(std::map<string, var> **vmap, string str)" << endl <<
    "  {" << endl <<
    "      if (*vmap == NULL)" << endl <<
    "        *vmap = new std::map<string, var>();" << endl <<
    "      if ((*vmap)->find(str) == (*vmap)->end())" << endl <<
    "        (*vmap)->insert(std::pair<string, var>(str, 0));" << endl <<
    "      return ((*vmap)->find(str))->second;" << endl <<
    "  }" << endl << endl;
    
    typedef map<full_type, int> utm; // Map of full_types used as dummies to a corresponding unique index from zero to the number of unique types
    utm usedtypes; // The key is a full type specifier, the value is a unique index for naming and referencing
    for (dal_it dait = ctex.dot_accessed_locals.begin(); dait != ctex.dot_accessed_locals.end(); ++dait)
      usedtypes.insert(utm::value_type(dait->second, usedtypes.size())); // Insert the type into the map, and give it a unique index
    
    for (utm::iterator i = usedtypes.begin(); i != usedtypes.end(); i++) {
      full_type ft(i->first); ft.refs.name = "dummy_" + ::tostring(i->second); // Copy the type into a new full_type object, with our dummy name
      wto << "  " << ft.toString() << ";"; // Referenced by " << i->second << " accessors"; // Converts full_type to C++-formatted declaration and writes it straight to the file
    }
    
    /* **************************************************************** *\
    ** Now that we have dummy variables to fall back on, we can write   **
    ** the dot-access functions for each arbitrarily accessed variable. **
    \* **************************************************************** */
    for (dal_it dait = ctex.dot_accessed_locals.begin(); dait != ctex.dot_accessed_locals.end(); dait++)
    {
      const string pmember = dait->first;
      wto << "  " << typeflags_string(dait->second.def, dait->second.flags) << " "
                  << dait->second.refs.toStringLHS()
                  << (dait->second.refs.size()? " (&varaccess_":" &varaccess_")
                  << pmember
                  << (dait->second.refs.size()? "(int x))":"(int x)") << endl
          << "  {" << endl
          << "    object_basic *inst = fetch_instance_by_int(x);" << endl
          << "    if (inst) switch (inst->object_index)" << endl
          << "    {" << endl;
      
      const full_type &correct_type = dait->second;
      
      // Iterate all objects in search of this local
      for (po_i it = ctex.parsed_objects.begin(); it != ctex.parsed_objects.end(); it++)
      {
        // We'll probe this directly since we have no interest in a full search; only this scope
        definition_scope::defiter x = it->second->self.members.find(pmember);
        if (x != it->second->self.members.end())
        {
          if (not(x->second->flags & DEF_TYPED))
            ctex.herr->warning("Variable `" + pmember + "' is declared as non-variable in object `" + it->second->properties->name + "': omitted from access list");
          else {
            definition_typed *dt = (definition_typed*)x->second;
            if (!dt->type)
              ctex.herr->error("LOGIC ERROR! Variable `" + pmember + "' has still not given a type! Compile is liable to fail, but will not be aborted.");
            // string tot = dt->type? typeflags_string(dt->type, dt->modifiers) : "var";
            full_type ntype(dt->type, dt->referencers, dt->modifiers);
            if (ntype.synonymous_with(correct_type))
              wto << "      case " << it->second->properties->id << ": return ((" << it->second->class_name << "*)inst)->" << pmember << "; // " << it->second->properties->name << endl;
            else
              ctex.herr->warning("Since variable `" + pmember + "' is declared with a different type in object `" + it->second->properties->name + "', it will be omitted from access list");
          }
        }
      }

      wto << "      case global: return ((ENIGMA_global_structure*)ENIGMA_global_instance)->" << pmember << ";" << endl;
      if (dait->second.def == enigma_type__var)
        wto << "      default: return map_var(&(((enigma::object_locals*)instance_event_iterator->inst)->vmap), \"" << pmember << "\");"  << endl;
      wto << "    }" << endl;
      wto << "    return dummy_" << usedtypes[dait->second] << ";" << endl;
      wto << "  }" << endl;
    }
    wto << "} // namespace enigma" << endl;
  wto.close();
  return 0;
}
