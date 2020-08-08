/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "settings.h"

#include "backend/GameData.h"
#include "compiler/compile_common.h"
#include "languages/lang_CPP.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int lang_CPP::compile_writeFontInfo(const GameData &game)
{
  ofstream wto((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_fontinfo.h").u8string().c_str(),ios_base::out);
  wto << license
      << "#ifndef JUST_DEFINE_IT_RUN" << endl
      << "#undef INCLUDED_FROM_SHELLMAIN" << endl
      << "#endif" << endl
      << "#include \"fonts/fonts.h\"" << endl
      << "#include <map>" << endl
      << endl;

  int maxid = -1, rawfontcount = 0;
  wto << "namespace enigma {" << endl;
  wto << "namespace fonts {" << endl;
  wto << "std::map<int, SpriteFont> exeFonts = {" << endl;
  for (auto font_it = game.fonts.begin(); font_it != game.fonts.end(); ++font_it) {
   
    const auto& font = *font_it;
    wto << "    { " << font.id() << ", SpriteFont(\"" //begin font
        << font.name         << "\", \""  // string name;
        << font->font_name() << "\", "    // string fontName;
        << font->size()      << ", "      // int size;
        << font->bold()      << ", "      // bool bold;
        << font->italic()    << ", ";     // bool italic;

        std::stringstream ranges;
        ranges << "{ "; //begin glyph ranges
        //FIXME: should be has_image() but lgm writter bugged and always writes a image even if one doesnt exist
        if (font->glyphs_size() > 0)  { // font used pre-rendered texture
         // TODO: this is where gmx with a pre-rendered texture should be handled
         // iterate font.glyphs() and write them out to IDE_FONTS like below. I've already packed the texture
        } else { // we rendered the texture
          for (auto range = font.raw_font.ranges.begin(); range != font.raw_font.ranges.end(); ++range) {
            ranges << "GlyphRange(";
            ranges << range->start                 << ", ";
            ranges << range->start + range->size() << ", { "; // begin glyphs
            for (auto glyph = range->glyphs.begin(); glyph != range->glyphs.end(); ++glyph) {
              ranges << "{ "; // begin glyph
              ranges << glyph->x   << ", ";
              ranges << glyph->y   << ", ";
              ranges << glyph->x2  << ", ";
              ranges << glyph->y2  << ", ";
              ranges << glyph->tx  << ", ";
              ranges << glyph->ty  << ", ";
              ranges << glyph->tx2 << ", ";
              ranges << glyph->ty2 << ", ";
              ranges << glyph->xs;
              ranges << "}"; // end glyph
              if (std::next(glyph) != range->glyphs.end())
                ranges << ", ";
            }
            ranges << " }"; // end glyphs
            ranges << " )"; //end range
            if (std::next(range) != font.raw_font.ranges.end())
              ranges << " ,";
          }
        }
          
       wto << font.raw_font.lineHeight    << ", "
           << font.raw_font.yOffset        << ", "
           << font.image_data.width        << ", " 
           << font.image_data.height  << ", " 
           << "nullptr" << ", " 
           << ranges.str()                << " }"; // end ranges

        wto << ") }"; // end SpriteFont & end map index
        
        if (std::next(font_it) != game.fonts.end())
          wto << ",\n"; 

    if (font.id() > maxid)
      maxid = font.id();
    rawfontcount++;
  }
  wto << "    \n};" << endl; // end fonts
  wto << endl << "int exeFontsMaxID = " << maxid << ";" << endl;
  wto << "}" << endl; //namespace fonts
  wto << "}" << endl; //namespace enigma
  wto.close();
  return 0;
}
