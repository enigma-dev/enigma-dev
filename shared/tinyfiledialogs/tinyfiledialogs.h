/*_________
 /         \ tinyfiledialogs.h v3.3.4 [Mar 15, 2018] zlib licence
 |tiny file| Unique header file created [November 9, 2014]
 | dialogs | Copyright (c) 2014 - 2018 Guillaume Vareille http://ysengrin.com
 \____  ___/ http://tinyfiledialogs.sourceforge.net
      \|     git clone http://git.code.sf.net/p/tinyfiledialogs/code tinyfd
		 ____________________________________________
		|                                            |
		|   email: tinyfiledialogs at ysengrin.com   |
		|____________________________________________|
         ________________________________________________________________________
        |                                                                        |
        | the windows only wchar_t UTF-16 prototypes are at the end of this file |
        |________________________________________________________________________|

Please upvote my stackoverflow answer https://stackoverflow.com/a/47651444

tiny file dialogs (cross-platform C C++)
InputBox PasswordBox MessageBox ColorPicker
OpenFileDialog SaveFileDialog SelectFolderDialog
Native dialog library for WINDOWS MAC OSX GTK+ QT CONSOLE & more
SSH supported via automatic switch to console mode or X11 forwarding

a C file and a header (add them to your C or C++ project) with 8 functions:
- beep
- notify popup
- message & question
- input & password
- save file
- open file(s)
- select folder
- color picker

Complements OpenGL Vulkan GLFW GLUT GLUI VTK SFML TGUI
SDL Ogre Unity3d ION OpenCV CEGUI MathGL GLM CPW GLOW
IMGUI MyGUI GLT NGL STB & GUI less programs

NO INIT
NO MAIN LOOP
NO LINKING
NO INCLUDE

The dialogs can be forced into console mode

Windows (XP to 10) ASCII MBCS UTF-8 UTF-16
- native code & vbs create the graphic dialogs
- enhanced console mode can use dialog.exe from
http://andrear.altervista.org/home/cdialog.php
- basic console input

Unix (command line calls) ASCII UTF-8
- applescript, kdialog, zenity
- python (2 or 3) + tkinter + python-dbus (optional)
- dialog (opens a console if needed)
- basic console input
The same executable can run across desktops & distributions

C89 & C++98 compliant: tested with C & C++ compilers
VisualStudio MinGW-gcc GCC Clang TinyCC OpenWatcom-v2 BorlandC SunCC ZapCC
on Windows Mac Linux Bsd Solaris Minix Raspbian
using Gnome Kde Enlightenment Mate Cinnamon Unity Lxde Lxqt Xfce
WindowMaker IceWm Cde Jds OpenBox Awesome Jwm Xdm

Bindings for LUA and C# dll, Haskell
Included in LWJGL(java), Rust, Allegrobasic

- License -

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.  If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

// THIS IS NOT THE ORIGINAL SOFTWARE; UNUSED CODE HAS BEEN REMOVED AND THE CONTENTS REMAINING HAVE BEEN MODIFIED.

#ifndef TINYFILEDIALOGS_H
#define TINYFILEDIALOGS_H

/* #define TINYFD_NOLIB */
/* On windows, define TINYFD_NOLIB here
if you don't want to include the code creating the graphic dialogs.
Then you won't need to link against Comdlg32.lib and Ole32.lib */

/* if tinydialogs.c is compiled as C++ code rather than C code,
you may need to comment out:
extern "C" {
and the corresponding closing bracket near the end of this file:
}
*/

#ifdef	__cplusplus
extern "C" {
#endif

int tinyfd_messageBox(
	char const * const aTitle , /* NULL or "" */
	char const * const aMessage , /* NULL or "" may contain \n \t */
	char const * const aDialogType , /* "ok" "okcancel" "yesno" "yesnocancel" */
	char const * const aIconType , /* "info" "warning" "error" "question" */
	int const aDefaultButton , /* 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel */
	int const aDialogEngine ) ; /* 0 for Zenity, 1 for KDialog */
	/* 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel */

char const * tinyfd_inputBox(
	char const * const aTitle , /* NULL or "" */
	char const * const aMessage , /* NULL or "" may NOT contain \n \t on windows */
	char const * const aDefaultInput ,  /* NULL or "" */
	int const aDialogEngine ) ; /* 0 for Zenity, 1 for KDialog */
	/* returns NULL on cancel */

char const * tinyfd_passwordBox(
	char const * const aTitle , /* NULL or "" */
	char const * const aMessage , /* NULL or "" may NOT contain \n nor \t */
	char const * const aDefaultInput , /* NULL or "" */
	int const aDialogEngine ) ; /* 0 for Zenity, 1 for KDialog */
        /* returns NULL on cancel */

char const * tinyfd_saveFileDialog(
	char const * const aTitle , /* NULL or "" */
	char const * const aDefaultPathAndFile , /* NULL or "" */
	int const aNumOfFilterPatterns , /* 0 */
	char const * const * const aFilterPatterns , /* NULL | {"*.jpg","*.png"} */
	char const * const aSingleFilterDescription , /* NULL | "text files" */
	int const aDialogEngine ) ; /* 0 for Zenity, 1 for KDialog */
	/* returns NULL on cancel */

char const * tinyfd_openFileDialog(
	char const * const aTitle , /* NULL or "" */
	char const * const aDefaultPathAndFile , /* NULL or "" */
	int const aNumOfFilterPatterns , /* 0 */
	char const * const * const aFilterPatterns , /* NULL {"*.jpg","*.png"} */
	char const * const aSingleFilterDescription , /* NULL | "image files" */
	int const aAllowMultipleSelects , /* 0 or 1 */
	int const aDialogEngine ) ; /* 0 for Zenity, 1 for KDialog */
	/* in case of multiple files, the separator is \n */
	/* returns NULL on cancel */

char const * tinyfd_selectFolderDialog(
	char const * const aTitle , /* NULL or "" */
	char const * const aDefaultPath , /* NULL or "" */
	int const aDialogEngine ) ; /* 0 for Zenity, 1 for KDialog */
	/* returns NULL on cancel */

char const * tinyfd_colorChooser(
	char const * const aTitle , /* NULL or "" */
	char const * const aDefaultHexRGB , /* NULL or "#FF0000" */
	unsigned char const aDefaultRGB[3] , /* { 0 , 255 , 255 } */
	unsigned char aoResultRGB[3] , /* { 0 , 0 , 0 } */
	int const aDialogEngine ) ; /* 0 for Zenity, 1 for KDialog */
	/* returns the hexcolor as a string "#FF0000" */
	/* aoResultRGB also contains the result */
	/* aDefaultRGB is used only if aDefaultHexRGB is NULL */
	/* aDefaultRGB and aoResultRGB can be the same array */
	/* returns NULL on cancel */

#ifdef	__cplusplus
}
#endif

#endif /* TINYFILEDIALOGS_H */
