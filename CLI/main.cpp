/**
* @file main.cpp
* @brief Source file of main entry point for the program.
*
* Write a description about the file here...
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
*
* This file is a part of the ENIGMA Development Environment.
*
* ENIGMA is free software: you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the license or any later version.
*
* This application and its source code is distributed AS-IS, WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along
* with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <iostream>
#include <stdio.h>

#include "library.h"
#include "printcolors.h"

using namespace std;

#define TITLETEXT \
"ENIGMA Development Environment\n\
Command Line Interface by Robert B. Colton\n\
Copyright (C) 2013 GNU GPL v3\n"\

#define HELPTEXT \
"version          prints the current version of your ENIGMA configuration\n\
build -[m] -[f]  builds a project file for the given mode\n\
settings         configures build settings\n\
extensions       configures extension settings\n\
list -[k]        lists functions and other keywords\n\
definitions      reparses all definitions\n\
syntax -[f]      parses the file for syntax errors\n\
help -[c]        prints this menu with information regarding commands\n\
clear or cls     clears this output\n\
license          retrieves the license information"\

#define LICENSETEXT \
"Copyright (C) 2007-2013\
\n\
\nENIGMA is free software: you can redistribute it and/or modify it under the\
\nterms of the GNU General Public License as published by the Free Software\
\nFoundation, version 3 of the license or any later version.\
\n\
\nThis application and its source code is distributed AS-IS, WITHOUT ANY\
\nWARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS\
\nFOR A PARTICULAR PURPOSE. See the GNU General Public License for more\
\ndetails.\
\n\
\nYou should have received a copy of the GNU General Public License along\
\nwith this program. If not, see <http://www.gnu.org/licenses/>"

unsigned long RGBA2DWORD(int iR, int iG, int iB, int iA)
{
  return (((((iR << 8) + iG) << 8) + iB) << 8) + iA;
}

void Compile(char* location, int mode)
{
    EnigmaStruct* es = new EnigmaStruct();
    es->gameSettings.alwaysOnTop = true;
    es->gameSettings.gameId = 03434534;
    es->filename = "examplegamefilename";
    Room rms[1];
    rms[0] = Room();
    rms[0].drawBackgroundColor = true;
    rms[0].width = 500;
    rms[0].height=500;
    rms[0].creationCode = "";
    rms[0].name = "exampleroom";
    rms[0].id = 0543;
    rms[0].speed = 30;
    rms[0].caption = "Example Game Room Caption";
    rms[0].instanceCount = 0;
    rms[0].backgroundColor = RGBA2DWORD(0, 149, 255, 255);
    GmObject obj[10];

    obj[0] = GmObject();
    //obj[0].name="pissmonkey";
    obj[0].id = 0;
  //  Sprite spr[0];
  //  Font fnt[0];
  //  Timeline tln[0];
  //  Script scr[0];
  //  Path pth[0];
  //  Background bgd[0];

    es->rooms = rms;
    es->roomCount = 1;
    //es->gmObjects = obj;
    //es->gmObjectCount = 1;
  //  es->scripts = scr;
  //  es->fonts = fnt;
  //  es->sprites = spr;
  //  es->timelines = tln;
   // es->backgrounds = bgd;
    compileEGMf(es, location, mode);
}

int main()
{
    void *result = LoadPluginLib();
    if (result == NULL)
    {
        print_error("Failed to load the plugin. Perhaps you have not compiled ENIGMA yet?");
    }
    definitionsModified("", ((const char*) "%e-yaml\n"
    "---\n"
    "treat-literals-as: 0\n"
    "sample-lots-of-radios: 0\n"
    "inherit-equivalence-from: 0\n"
    "sample-checkbox: on\n"
    "sample-edit: DEADBEEF\n"
    "sample-combobox: 0\n"
    "inherit-strings-from: 0\n"
    "inherit-escapes-from: 0\n"
    "inherit-increment-from: 0\n"
    " \n"
    "target-audio: OpenAL\n"
    "target-windowing: xlib\n"
    "target-compiler: gcc\n"
    "target-graphics: OpenGL1\n"
    "target-widget: None\n"
    "target-collision: BBox\n"
    "target-networking: None\n"
    "extensions:Universal_System/Extensions/Paths\n"
    ));
    //clearconsole();
    printfln(TITLETEXT);

    printf("Type ");
    printfc("help", DIM, YELLOW);
    printfc(" for information on commands, or ", RESET, WHITE);
    printfc("exit", DIM, RED);
    textcolorfg(RESET, WHITE);
    printf(" to quit\n");

    bool close = false;
    while (!close) {

        string input;
        cin >> input;

        if (input == "help") {
            printfln(HELPTEXT);
        } else if (input == "license") {
            printfln(LICENSETEXT);
        } else if (input == "version") {
            printfln("I have no idea what version you are using.");
        } else if (input == "exit") {
            close = true;
        } else if (input == "build") {
            printfln("Where would you like to output the executable to?");
            string loc;
            cin >> loc;
            printfln("Please enter your build mode: run/debug/design/compile/rebuild");
            string mode;
            cin >> mode;
            int bmode = -1;
            if (mode == "run") {
              bmode = emode_run;
            } else if (mode == "debug") {
              bmode = emode_debug;
            } else if (mode == "design") {
              bmode = emode_design;
            } else if (mode == "compile") {
              bmode = emode_compile;
            } else if (mode == "rebuild") {
              bmode = emode_rebuild;
            } else {
              printfln("Unknown build mode");
            }
            if (bmode >= 0) {
              Compile((char*)loc.c_str(), bmode);
            }
        } else if (input == "settings") {

        } else if (input == "list") {
            printfln("What would you like to list? functions/globals/typenames/all/other");
            cin >> input;
            char* currentResource = (char*)first_available_resource();
            bool other = false;
            printfln("***** START *****");
            while (!resources_atEnd())
            {
                other = true;
                if (resource_isFunction() && (input == "functions" || input == "all")) {
                    printfc(currentResource, DIM, CYAN);
                    other = false;
                }
                if (resource_isGlobal() && (input == "globals" || input == "all")) {
                    printfc(currentResource, DIM, YELLOW);
                    other = false;
                }
                if (resource_isTypeName() && (input == "typenames" || input == "all")) {
                    printfc(currentResource, DIM, RED);
                    other = false;
                }
                if ((input == "all" || input == "other") && other) {
                    textcolorfg(RESET, WHITE);
                    printf(currentResource);
                }
                printf(" ");
                currentResource = (char*)next_available_resource();
            }

            textcolorfg(RESET, WHITE);
            println();
            printfln("***** END *****");
        } else if (input == "definitions") {

        } else if (input == "syntax") {

        } else if (input == "cls" || input == "clear") {
            clearconsole();
        } else {
            printfln("Unknown command");
        }

    }

    return 0;
}
