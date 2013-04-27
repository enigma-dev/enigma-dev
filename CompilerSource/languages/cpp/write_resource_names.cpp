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

int lang_CPP::compile_write_resource_names(compile_context &ctex)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h",ios_base::out);
  wto << gen_license;
  stringstream ss;
  int max;
  
  max = 0;
  wto << "enum //object names\n{\n";
  for (po_i i = ctex.parsed_objects.begin(); i != ctex.parsed_objects.end(); ++i) {
    if (i->first >= max) max = i->first + 1;
    wto << "  " << i->second->properties->name << " = " << i->first << ",\n";
    ss << "    case " << i->first << ": return \"" << i->second->properties->name << "\"; break;\n";
  } wto << "};\nnamespace enigma { size_t object_idmax = " << max << "; }\n\n";

  wto << "string object_get_name(int i) {\n switch (i) {\n";
   wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
   ss.str( "" );

  max = 0;
  wto << "enum //sprite names\n{\n";
  for (int i = 0; i < ctex.es->spriteCount; ++i) {
    if (ctex.es->sprites[i].id >= max) max = ctex.es->sprites[i].id + 1;
    wto << "  " << ctex.es->sprites[i].name << " = " << ctex.es->sprites[i].id << ",\n";
    ss << "    case " << ctex.es->sprites[i].id << ": return \"" << ctex.es->sprites[i].name << "\"; break;\n";
  } wto << "};\nnamespace enigma { size_t sprite_idmax = " << max << "; }\n\n";

   wto << "string sprite_get_name(int i) {\n switch (i) {\n";
   wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
   ss.str( "" );

  max = 0;
  wto << "enum //background names\n{\n";
  for (int i = 0; i < ctex.es->backgroundCount; ++i) {
    if (ctex.es->backgrounds[i].id >= max) max = ctex.es->backgrounds[i].id + 1;
    wto << "  " << ctex.es->backgrounds[i].name << " = " << ctex.es->backgrounds[i].id << ",\n";
    ss << "    case " << ctex.es->backgrounds[i].id << ": return \"" << ctex.es->backgrounds[i].name << "\"; break;\n";
  } wto << "};\nnamespace enigma { size_t background_idmax = " << max << "; }\n\n";

   wto << "string background_get_name(int i) {\n switch (i) {\n";
   wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
   ss.str( "" );

  max = 0;
  wto << "enum //font names\n{\n";
  for (int i = 0; i < ctex.es->fontCount; ++i) {
    if (ctex.es->fonts[i].id >= max) max = ctex.es->fonts[i].id + 1;
    wto << "  " << ctex.es->fonts[i].name << " = " << ctex.es->fonts[i].id << ",\n";
    ss << "    case " << ctex.es->fonts[i].id << ": return \"" << ctex.es->fonts[i].name << "\"; break;\n";
  } wto << "};\nnamespace enigma { size_t font_idmax = " << max << "; }\n\n";

   wto << "string font_get_name(int i) {\n switch (i) {\n";
   wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
   ss.str( "" );

  max = 0;
  wto << "enum //timeline names\n{\n";
  for (int i = 0; i < ctex.es->timelineCount; ++i) {
      if (ctex.es->timelines[i].id >= max) max = ctex.es->timelines[i].id + 1;
        wto << "  " << ctex.es->timelines[i].name << " = " << ctex.es->timelines[i].id << ",\n";
        ss << "    case " << ctex.es->timelines[i].id << ": return \"" << ctex.es->timelines[i].name << "\"; break;\n";
  } wto << "};\nnamespace enigma { size_t timeline_idmax = " << max << "; }\n\n";

  wto << "string timeline_get_name(int i) {\n switch (i) {\n";
       wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
       ss.str( "" );

      max = 0;
    wto << "enum //path names\n{\n";
    for (int i = 0; i < ctex.es->pathCount; ++i) {
        if (ctex.es->paths[i].id >= max) max = ctex.es->paths[i].id + 1;
          wto << "  " << ctex.es->paths[i].name << " = " << ctex.es->paths[i].id << ",\n";
          ss << "    case " << ctex.es->paths[i].id << ": return \"" << ctex.es->paths[i].name << "\"; break;\n";
    } wto << "};\nnamespace enigma { size_t path_idmax = " << max << "; }\n\n";

  wto << "string path_get_name(int i) {\n switch (i) {\n";
       wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
       ss.str( "" );

      max = 0;
      wto << "enum //sound names\n{\n";
      for (int i = 0; i < ctex.es->soundCount; ++i) {
        if (ctex.es->sounds[i].id >= max) max = ctex.es->sounds[i].id + 1;
        wto << "  " << ctex.es->sounds[i].name << " = " << ctex.es->sounds[i].id << ",\n";
        ss << "    case " << ctex.es->sounds[i].id << ": return \"" << ctex.es->sounds[i].name << "\"; break;\n";
      } wto << "};\nnamespace enigma { size_t sound_idmax = " <<max << "; }\n\n";

  wto << "string sound_get_name(int i) {\n switch (i) {\n";
       wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
       ss.str( "" );

      max = 0;
      wto << "enum //script names\n{\n";
      for (int i = 0; i < ctex.es->scriptCount; ++i) {
        if (ctex.es->scripts[i].id >= max) max = ctex.es->scripts[i].id + 1;
        wto << "  " << ctex.es->scripts[i].name << " = " << ctex.es->scripts[i].id << ",\n";
        ss << "    case " << ctex.es->scripts[i].id << ": return \"" << ctex.es->scripts[i].name << "\"; break;\n";
      } wto << "};\nnamespace enigma { size_t script_idmax = " <<max << "; }\n\n";

  wto << "string script_get_name(int i) {\n switch (i) {\n";
       wto << ss.str() << " default: return \"<undefined>\";}};\n\n";
       ss.str( "" );

      max = 0;
      wto << "enum //room names\n{\n";
      for (int i = 0; i < ctex.es->roomCount; ++i) {
        if (ctex.es->rooms[i].id >= max) max = ctex.es->rooms[i].id + 1;
        wto << "  " << ctex.es->rooms[i].name << " = " << ctex.es->rooms[i].id << ",\n";
      }
      wto << "};\nnamespace enigma { size_t room_idmax = " <<max << "; }\n\n";
  
  wto.close();
  return 0;
}
