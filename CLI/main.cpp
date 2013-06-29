/** Copyright (C) 2013 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <iostream>
#include <stdio.h>

using namespace std;

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

void printfc(char* msg, int attr, int fg) {
    textcolorfg(attr, fg);
    printf(msg);
}

void printfc(char* msg, int attr, int bg, int fg) {
    textcolor(attr, fg, bg);
    printf(msg);
}

void printfln(char* msg) {
    cout << msg << endl;
}

void println() {
    cout << endl;
}

void clearconsole() {
    printf("\e[1;1H\e[2J");
}

int main()
{
    clearconsole();
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

        if (input == "help")
        {
            printfln(HELPTEXT);
        } else if (input == "license") {
            printfln(LICENSETEXT);
        } else if (input == "version") {
            printfln("I have no idea what version you are using.");
        } else if (input == "exit") {
            close = true;
        } else if (input == "build") {

        } else if (input == "settings") {

        } else if (input == "list") {

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
