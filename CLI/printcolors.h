/**
* @file printcolors.h
* @brief Header file of various print functions with color.
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

#ifndef PRINTCOLORS_H_INCLUDED
#define PRINTCOLORS_H_INCLUDED

#define RESET           0
#define BRIGHT          1
#define DIM             2
#define UNDERLINE       3
#define BLINK           4
#define REVERSE         7
#define HIDDEN          8

#define BLACK           0
#define RED             1
#define GREEN           2
#define YELLOW          3
#define BLUE            4
#define MAGENTA         5
#define CYAN            6
#define WHITE           7

unsigned long RGBA2DWORD(int iR, int iG, int iB, int iA);

void textcolor(int attr, int fg, int bg);
void textcolorbg(int attr, int bg);
void textcolorfg(int attr, int fg);

void printfln(char* msg);
void println();
void printfc(char* msg, int attr, int fg);
void printfcln(char* msg, int attr, int fg);
void printfc(char* msg, int attr, int bg, int fg);

void clearconsole();

void print_error(char* msg);
void print_warning(char* msg);
void print_notice(char* msg);

#endif // PRINTCOLORS_H_INCLUDED
