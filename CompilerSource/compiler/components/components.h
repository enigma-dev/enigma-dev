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

// Sizable utilities
int link_globals(parsed_object*, EnigmaStruct*,parsed_script*[]);

// IDE_EDITABLEs added before compile
extern int compile_parseAndLink(EnigmaStruct*,parsed_script*[]);
int compile_writeGlobals(EnigmaStruct*,parsed_object*);
extern int compile_writeObjectData(EnigmaStruct*,parsed_object*);
int compile_writeRoomData(EnigmaStruct* es);
int compile_writeDefraggedEvents(EnigmaStruct* es);

// Resources added to module
int module_write_sprites(EnigmaStruct *es, FILE *gameModule);
int module_write_sounds(EnigmaStruct *es, FILE *gameModule);
