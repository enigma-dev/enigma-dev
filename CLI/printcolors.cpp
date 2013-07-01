/**
* @file printcolors.cpp
* @brief Source file of various print functions with color.
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

using namespace std;

#include "printcolors.h"

unsigned long RGBA2DWORD(int iR, int iG, int iB, int iA)
{
    return (((((iR << 8) + iG) << 8) + iB) << 8) + iA;
}

void textcolor(int attr, int fg, int bg) {
    char command[13];

    /* Command is the control command to the terminal */
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    printf("%s", command);
}

void textcolorbg(int attr, int bg) {
    char command[13];

    /* Command is the control command to the terminal */
    sprintf(command, "%c[%d;%dm", 0x1B, attr, bg + 40);
    printf("%s", command);
}

void textcolorfg(int attr, int fg) {
    char command[13];

    /* Command is the control command to the terminal */
    sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
    printf("%s", command);
}

void printfln(char* msg) {
    cout << msg << endl;
}

void println() {
    cout << endl;
}

void printfc(char* msg, int attr, int fg) {
    textcolorfg(attr, fg);
    printf(msg);
}

void printfcln(char* msg, int attr, int fg) {
    printfc(msg, attr, fg);
    println();
}

void printfc(char* msg, int attr, int bg, int fg) {
    textcolor(attr, fg, bg);
    printf(msg);
}

void clearconsole() {
    printf("\e[1;1H\e[2J");
}

void print_error(char* msg) {
    printfc("ERROR! ", BRIGHT, RED);
    printfcln(msg, RESET, WHITE);
}

void print_warning(char* msg) {
    printfc("WARNING! ", BRIGHT, YELLOW);
    printfcln(msg, RESET, WHITE);
}

void print_notice(char* msg) {
    printfc("NOTICE! ", BRIGHT, CYAN);
    printfcln(msg, RESET, WHITE);
}
