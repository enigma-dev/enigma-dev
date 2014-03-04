/**
  @file  write_resource_names.cpp
  @brief Implements the part that writes resource names and IDs to the engine.
    
  @section License
    Copyright (C) 2008-2013 Josh Ventura
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

#include <fstream>
#include <sstream>
#include <languages/lang_CPP.h>

#include <makedir.h> // FIXME: This is ludicrous

int lang_CPP::compile_write_resource_names(compile_context &ctex)
{
  ofstream wto((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h").c_str(),ios_base::out);
    wto << gen_license;
    
    stringstream ss;
    
    size_t max = 0;
    wto << "namespace enigma_user {\nenum //object names\n{\n";
    for (po_i i = ctex.parsed_objects.begin(); i != ctex.parsed_objects.end(); i++) {
      if (i->first >= max) max = i->first + 1;
      wto << "  " << i->second->properties->name << " = " << i->first << ",\n";
      ss << "    case " << i->first << ": return \"" << i->second->properties->name << "\"; break;\n";
    } wto << "};\n}\nnamespace enigma { size_t object_idmax = " << max << "; }\n\n";

    wto << "namespace enigma_user {\nstring object_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //sprite names\n{\n";
    for (int i = 0; i < ctex.es->spriteCount; i++) {
      if (ctex.es->sprites[i].id >= max) max = ctex.es->sprites[i].id + 1;
      wto << "  " << ctex.es->sprites[i].name << " = " << ctex.es->sprites[i].id << ",\n";
      ss << "    case " << ctex.es->sprites[i].id << ": return \"" << ctex.es->sprites[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t sprite_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring sprite_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //background names\n{\n";
    for (int i = 0; i < ctex.es->backgroundCount; i++) {
      if (ctex.es->backgrounds[i].id >= max) max = ctex.es->backgrounds[i].id + 1;
      wto << "  " << ctex.es->backgrounds[i].name << " = " << ctex.es->backgrounds[i].id << ",\n";
      ss << "    case " << ctex.es->backgrounds[i].id << ": return \"" << ctex.es->backgrounds[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t background_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring background_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //font names\n{\n";
    for (int i = 0; i < ctex.es->fontCount; i++) {
      if (ctex.es->fonts[i].id >= max) max = ctex.es->fonts[i].id + 1;
      wto << "  " << ctex.es->fonts[i].name << " = " << ctex.es->fonts[i].id << ",\n";
      ss << "    case " << ctex.es->fonts[i].id << ": return \"" << ctex.es->fonts[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t font_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring font_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //timeline names\n{\n";
    for (int i = 0; i < ctex.es->timelineCount; i++) {
        if (ctex.es->timelines[i].id >= max) max = ctex.es->timelines[i].id + 1;
        wto << "  " << ctex.es->timelines[i].name << " = " << ctex.es->timelines[i].id << ",\n";
        ss << "    case " << ctex.es->timelines[i].id << ": return \"" << ctex.es->timelines[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t timeline_idmax = " << max << "; }\n\n";

  wto << "namespace enigma_user {\nstring timeline_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //path names\n{\n";
    for (int i = 0; i < ctex.es->pathCount; i++) {
        if (ctex.es->paths[i].id >= max) max = ctex.es->paths[i].id + 1;
        wto << "  " << ctex.es->paths[i].name << " = " << ctex.es->paths[i].id << ",\n";
        ss << "    case " << ctex.es->paths[i].id << ": return \"" << ctex.es->paths[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t path_idmax = " << max << "; }\n\n";

  wto << "namespace enigma_user {\nstring path_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //sound names\n{\n";
    for (int i = 0; i < ctex.es->soundCount; i++) {
      if (ctex.es->sounds[i].id >= max) max = ctex.es->sounds[i].id + 1;
      wto << "  " << ctex.es->sounds[i].name << " = " << ctex.es->sounds[i].id << ",\n";
      ss << "    case " << ctex.es->sounds[i].id << ": return \"" << ctex.es->sounds[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t sound_idmax = " <<max << "; }\n\n";

  wto << "namespace enigma_user {\nstring sound_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //script names\n{\n";
    for (int i = 0; i < ctex.es->scriptCount; i++) {
      if (ctex.es->scripts[i].id >= max) max = ctex.es->scripts[i].id + 1;
      wto << "  " << ctex.es->scripts[i].name << " = " << ctex.es->scripts[i].id << ",\n";
      ss << "    case " << ctex.es->scripts[i].id << ": return \"" << ctex.es->scripts[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t script_idmax = " <<max << "; }\n\n";

  wto << "namespace enigma_user {\nstring script_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //shader names\n{\n";
    for (int i = 0; i < ctex.es->shaderCount; i++) {
      if (ctex.es->shaders[i].id >= max) max = ctex.es->shaders[i].id + 1;
      wto << "  " << ctex.es->shaders[i].name << " = " << ctex.es->shaders[i].id << ",\n";
      ss << "    case " << ctex.es->shaders[i].id << ": return \"" << ctex.es->shaders[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t shader_idmax = " <<max << "; }\n\n";

  wto << "namespace enigma_user {\nstring shader_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );
    
    max = 0;
    wto << "namespace enigma_user {\nenum //room names\n{\n";
    for (int i = 0; i < ctex.es->roomCount; i++) {
      if (ctex.es->rooms[i].id >= max) max = ctex.es->rooms[i].id + 1;
      wto << "  " << ctex.es->rooms[i].name << " = " << ctex.es->rooms[i].id << ",\n";
    }
    wto << "};}\nnamespace enigma { size_t room_idmax = " <<max << "; }\n\n";
  wto.close();
  
  
  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h").c_str(),ios_base::out);
    wto << lang_CPP::gen_license;
    wto << "#ifndef NEW_OBJ_PREFIX\n#  define NEW_OBJ_PREFIX\n#endif\n\n";
    for (po_i i = ctex.parsed_objects.begin(); i != ctex.parsed_objects.end(); i++)
    {
      wto << "case " << i->second->properties->id << ":\n";
      wto << "    NEW_OBJ_PREFIX new enigma::" << i->second->class_name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << "\n\n#undef NEW_OBJ_PREFIX\n";
  wto.close();
  
  return 0;
}
