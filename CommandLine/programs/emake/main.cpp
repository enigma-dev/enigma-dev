/**
* @file main.cpp
* @brief Source file implementing the main entry build of the ENIGMA make program.
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
* This file is part of ENIGMA.
*
* ENIGMA is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the License, or (at your option) any later version.
*
* ENIGMA is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* ENIGMA. If not, see <http://www.gnu.org/licenses/>.
**/

#include <iostream>
#include <string>
#include "gmk\src\include/gmk.hpp"
#include "library.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "ENIGMA Make" << endl;
    Gmk::GmkFile* gmk = new Gmk::GmkFile();
    gmk->Load(argv[1]);

    void *result = LoadPluginLib();
    //void *result;
    if (result == NULL)
    {
        //wxMessageDialog (NULL, "Failed to communicate with the plugin. Have you compiled and built ENIGMA yet?",
                        // "Plugin Loading Failed", wxOK|wxCENTRE|wxICON_ERROR, wxDefaultPosition).ShowModal();
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
    ));

    // copy gmk to enigma resources and call compile

    delete gmk;
    return 0;
}
