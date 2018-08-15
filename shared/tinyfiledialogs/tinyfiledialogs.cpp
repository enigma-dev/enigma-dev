/*_________
 /         \ tinyfiledialogs.c v3.3.4 [Mar 15, 2018] zlib licence
 |tiny file| Unique code file created [November 9, 2014]
 | dialogs | Copyright (c) 2014 - 2018 Guillaume Vareille http://ysengrin.com
 \____  ___/ http://tinyfiledialogs.sourceforge.net
      \|     git clone http://git.code.sf.net/p/tinyfiledialogs/code tinyfd
         ____________________________________________
        |                                            |
        |   email: tinyfiledialogs at ysengrin.com   |
        |____________________________________________|
         ___________________________________________________________________
        |                                                                   |
        | the windows only wchar_t UTF-16 prototypes are in the header file |
        |___________________________________________________________________|

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

Thanks for contributions, bug corrections & thorough testing to:
- Don Heyse http://ldglite.sf.net for bug corrections & thorough testing!
- Paul Rouget

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

#define _POSIX_C_SOURCE 2 /* to accept POSIX 2 in old ANSI C standards */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "tinyfiledialogs.h"
/* #define TINYFD_NOLIB */

 #include <limits.h>
 #include <unistd.h>
 #include <dirent.h> /* on old systems try <sys/dir.h> instead */
 #include <termios.h>
 #include <sys/utsname.h>
 #include <signal.h> /* on old systems try <sys/signal.h> instead */
 #define SLASH "/"

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#define MAX_PATH_OR_CMD 1024 /* _MAX_PATH or MAX_PATH */
#define MAX_MULTIPLE_FILES 32

char const tinyfd_version [8] = "3.3.4";

int tinyfd_verbose = 0 ; /* on unix: prints the command line calls */

/* for unix & windows: 0 (graphic mode) or 1 (console mode).
0: try to use a graphic solution, if it fails then it uses console mode.
1: forces all dialogs into console mode even when the X server is present,
  if the package dialog (and a console is present) or dialog.exe is installed.
  on windows it only make sense for console applications */

char tinyfd_response[1024];
/* if you pass "tinyfd_query" as aTitle,
the functions will not display the dialogs
but and return 0 for console mode, 1 for graphic mode.
tinyfd_response is then filled with the retain solution.
possible values for tinyfd_response are (all lowercase)
for graphic mode:
  windows_wchar windows
  applescript kdialog zenity zenity3 matedialog qarma
  python2-tkinter python3-tkinter python-dbus perl-dbus
  gxmessage gmessage xmessage xdialog gdialog
for console mode:
  dialog whiptail basicinput no_solution */

static int gWarningDisplayed = 0 ;

#ifdef _MSC_VER
#pragma warning(disable:4996) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */
#pragma warning(disable:4100) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */
#pragma warning(disable:4706) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */
#endif

static char * getPathWithoutFinalSlash(
        char * const aoDestination, /* make sure it is allocated, use _MAX_PATH */
        char const * const aSource) /* aoDestination and aSource can be the same */
{
        char const * lTmp ;
        if ( aSource )
        {
                lTmp = strrchr(aSource, '/');
                if (!lTmp)
                {
                        lTmp = strrchr(aSource, '\\');
                }
                if (lTmp)
                {
                        strncpy(aoDestination, aSource, lTmp - aSource );
                        aoDestination[lTmp - aSource] = '\0';
                }
                else
                {
                        * aoDestination = '\0';
                }
        }
        else
        {
                * aoDestination = '\0';
        }
        return aoDestination;
}


static char * getLastName(
        char * const aoDestination, /* make sure it is allocated */
        char const * const aSource)
{
        /* copy the last name after '/' or '\' */
        char const * lTmp ;
        if ( aSource )
        {
                lTmp = strrchr(aSource, '/');
                if (!lTmp)
                {
                        lTmp = strrchr(aSource, '\\');
                }
                if (lTmp)
                {
                        strcpy(aoDestination, lTmp + 1);
                }
                else
                {
                        strcpy(aoDestination, aSource);
                }
        }
        else
        {
                * aoDestination = '\0';
        }
        return aoDestination;
}

static void Hex2RGB( char const aHexRGB [8] ,
                                         unsigned char aoResultRGB [3] )
{
        char lColorChannel [8] ;
        if ( aoResultRGB )
        {
                if ( aHexRGB )
                {
                        strcpy(lColorChannel, aHexRGB ) ;
                        aoResultRGB[2] = (unsigned char)strtoul(lColorChannel+5,NULL,16);
                        lColorChannel[5] = '\0';
                        aoResultRGB[1] = (unsigned char)strtoul(lColorChannel+3,NULL,16);
                        lColorChannel[3] = '\0';
                        aoResultRGB[0] = (unsigned char)strtoul(lColorChannel+1,NULL,16);
/* printf("%d %d %d\n", aoResultRGB[0], aoResultRGB[1], aoResultRGB[2]); */
                }
                else
                {
                        aoResultRGB[0]=0;
                        aoResultRGB[1]=0;
                        aoResultRGB[2]=0;
                }
        }
}

static void RGB2Hex( unsigned char const aRGB [3] ,
                                         char aoResultHexRGB [8] )
{
        if ( aoResultHexRGB )
        {
                if ( aRGB )
                {

                        sprintf(aoResultHexRGB, "#%02hhx%02hhx%02hhx",
                                aRGB[0], aRGB[1], aRGB[2]);
                        /* printf("aoResultHexRGB %s\n", aoResultHexRGB); */
                }
                else
                {
                        aoResultHexRGB[0]=0;
                        aoResultHexRGB[1]=0;
                        aoResultHexRGB[2]=0;
                }
        }
}


static int filenameValid( char const * const aFileNameWithoutPath )
{
        if ( ! aFileNameWithoutPath
          || ! strlen(aFileNameWithoutPath)
          || strpbrk(aFileNameWithoutPath , "\\/:*?\"<>|") )
        {
                return 0 ;
        }
        return 1 ;
}

static int fileExists( char const * const aFilePathAndName )
{
        FILE * lIn ;
        if ( ! aFilePathAndName || ! strlen(aFilePathAndName) )
        {
                return 0 ;
        }

        lIn = fopen( aFilePathAndName , "r" ) ;
        if ( ! lIn )
        {
                return 0 ;
        }
        fclose( lIn ) ;
        return 1 ;
}


static void wipefile(char const * const aFilename)
{
        int i;
        struct stat st;
        FILE * lIn;

        if (stat(aFilename, &st) == 0)
        {
                if ((lIn = fopen(aFilename, "w")))
                {
                        for (i = 0; i < st.st_size; i++)
                        {
                                fputc('A', lIn);
                        }
                }
                fclose(lIn);
        }
}

/* source and destination can be the same or ovelap*/
static char const * ensureFilesExist(char * const aDestination,
        char const * const aSourcePathsAndNames)
{
        char * lDestination = aDestination;
        char const * p;
        char const * p2;
        size_t lLen;

        if (!aSourcePathsAndNames)
        {
                return NULL;
        }
        lLen = strlen(aSourcePathsAndNames);
        if (!lLen)
        {
                return NULL;
        }

        p = aSourcePathsAndNames;
        while ((p2 = strchr(p, '|')) != NULL)
        {
                lLen = p2 - p;
                memmove(lDestination, p, lLen);
                lDestination[lLen] = '\0';
                if (fileExists(lDestination))
                {
                        lDestination += lLen;
                        *lDestination = '|';
                        lDestination++;
                }
                p = p2 + 1;
        }
        if (fileExists(p))
        {
                lLen = strlen(p);
                memmove(lDestination, p, lLen);
                lDestination[lLen] = '\0';
        }
        else
        {
                *(lDestination - 1) = '\0';
        }
        return aDestination;
}

static int isDarwin( )
{
        static int lsIsDarwin = -1 ;
        struct utsname lUtsname ;
        if ( lsIsDarwin < 0 )
        {
                lsIsDarwin = !uname(&lUtsname) && !strcmp(lUtsname.sysname,"Darwin") ;
        }
        return lsIsDarwin ;
}


static int dirExists( char const * const aDirPath )
{
        DIR * lDir ;
        if ( ! aDirPath || ! strlen( aDirPath ) )
                return 0 ;
        lDir = opendir( aDirPath ) ;
        if ( ! lDir )
        {
                return 0 ;
        }
        closedir( lDir ) ;
        return 1 ;
}


static int detectPresence( char const * const aExecutable )
{
        char lBuff [MAX_PATH_OR_CMD] ;
        char lTestedString [MAX_PATH_OR_CMD] = "which " ;
        FILE * lIn ;

    strcat( lTestedString , aExecutable ) ;
        strcat( lTestedString, " 2>/dev/null ");
    lIn = popen( lTestedString , "r" ) ;
    if ( ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
                && ( ! strchr( lBuff , ':' ) )
                && ( strncmp(lBuff, "no ", 3) ) )
    {   /* present */
        pclose( lIn ) ;
        if (tinyfd_verbose) printf("detectPresence %s %d\n", aExecutable, 1);
        return 1 ;
    }
    else
    {
        pclose( lIn ) ;
        if (tinyfd_verbose) printf("detectPresence %s %d\n", aExecutable, 0);
        return 0 ;
    }
}



static int graphicMode( )
{
        return ( getenv("DISPLAY")
            || (isDarwin() && (!getenv("SSH_TTY") || getenv("DISPLAY") ) ) ) ;
}


static int osascriptPresent( )
{
    static int lOsascriptPresent = -1 ;
    if ( lOsascriptPresent < 0 )
    {
                gWarningDisplayed |= !!getenv("SSH_TTY");
                lOsascriptPresent = detectPresence( "osascript" ) ;
    }
        return lOsascriptPresent && graphicMode() && !getenv("SSH_TTY") ;
}


static int zenityPresent( )
{
        static int lZenityPresent = -1 ;
        if ( lZenityPresent < 0 )
        {
                lZenityPresent = detectPresence("zenity") ;
        }
        return lZenityPresent && graphicMode( ) ;
}


static int zenity3Present()
{
        static int lZenity3Present = -1 ;
        char lBuff [MAX_PATH_OR_CMD] ;
        FILE * lIn ;
		int lIntTmp ;

        if ( lZenity3Present < 0 )
        {
                lZenity3Present = 0 ;
                if ( zenityPresent() )
                {
                        lIn = popen( "zenity --version" , "r" ) ;
                        if ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
                        {
                                if ( atoi(lBuff) >= 3 )
                                {
                                        lZenity3Present = 3 ;
										lIntTmp = atoi(strtok(lBuff,".")+2 ) ;
										if ( lIntTmp >= 18 )
										{
											lZenity3Present = 5 ;
										}
										else if ( lIntTmp >= 10 )
										{
											lZenity3Present = 4 ;
										}
								}
                                else if ( ( atoi(lBuff) == 2 ) && ( atoi(strtok(lBuff,".")+2 ) >= 32 ) )
                                {
                                        lZenity3Present = 2 ;
                                }
                                if (tinyfd_verbose) printf("zenity %d\n", lZenity3Present);
                        }
                        pclose( lIn ) ;
                }
        }
        return graphicMode() ? lZenity3Present : 0 ;
}


static int osx9orBetter( )
{
        static int lOsx9orBetter = -1 ;
        char lBuff [MAX_PATH_OR_CMD] ;
        FILE * lIn ;
        int V,v;

        if ( lOsx9orBetter < 0 )
        {
                lOsx9orBetter = 0 ;
                lIn = popen( "osascript -e \"set osver to system version of (system info)\"" , "r" ) ;
                if ( ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
                        && ( 2 == sscanf(lBuff, "%d.%d", &V, &v) ) )
                {
                        V = V * 100 + v;
                        if ( V >= 1009 )
                        {
                                lOsx9orBetter = 1 ;
                        }
                }
                pclose( lIn ) ;
                if (tinyfd_verbose) printf("Osx10 = %d, %d = %s\n", lOsx9orBetter, V, lBuff) ;
        }
        return lOsx9orBetter ;
}


int tinyfd_messageBox(
        char const * const aTitle , /* NULL or "" */
        char const * const aMessage , /* NULL or ""  may contain \n and \t */
        char const * const aDialogType , /* "ok" "okcancel" "yesno" "yesnocancel" */
        char const * const aIconType , /* "info" "warning" "error" "question" */
        int const aDefaultButton ) /* 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel */
{
        char lBuff [MAX_PATH_OR_CMD] ;
        char * lDialogString = NULL ;
        FILE * lIn ;
        int lResult ;
        size_t lTitleLen ;
        size_t lMessageLen ;

        lBuff[0]='\0';

        lTitleLen =  aTitle ? strlen(aTitle) : 0 ;
        lMessageLen =  aMessage ? strlen(aMessage) : 0 ;
        if ( !aTitle || strcmp(aTitle,"tinyfd_query") )
        {
                lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );
        }

        if ( osascriptPresent( ) )
        {
                if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"applescript");return 1;}

                strcpy( lDialogString , "osascript ");
                if ( ! osx9orBetter() ) strcat( lDialogString , " -e \"tell application \\\"System Events\\\"\" -e \"Activate\"");
                strcat( lDialogString , " -e \"try\" -e \"set {vButton} to {button returned} of ( display dialog \\\"") ;
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, aMessage) ;
                }
                strcat(lDialogString, "\\\" ") ;
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, "with title \\\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\\\" ") ;
                }
                strcat(lDialogString, "with icon ") ;
                if ( aIconType && ! strcmp( "error" , aIconType ) )
                {
                        strcat(lDialogString, "stop " ) ;
                }
                else if ( aIconType && ! strcmp( "warning" , aIconType ) )
                {
                        strcat(lDialogString, "caution " ) ;
                }
                else /* question or info */
                {
                        strcat(lDialogString, "note " ) ;
                }
                if ( aDialogType && ! strcmp( "okcancel" , aDialogType ) )
                {
                        if ( ! aDefaultButton )
                        {
                                strcat( lDialogString ,"default button \\\"Cancel\\\" " ) ;
                        }
                }
                else if ( aDialogType && ! strcmp( "yesno" , aDialogType ) )
                {
                        strcat( lDialogString ,"buttons {\\\"No\\\", \\\"Yes\\\"} " ) ;
                        if (aDefaultButton)
                        {
                                strcat( lDialogString ,"default button \\\"Yes\\\" " ) ;
                        }
                        else
                        {
                                strcat( lDialogString ,"default button \\\"No\\\" " ) ;
                        }
                        strcat( lDialogString ,"cancel button \\\"No\\\"" ) ;
                }
                else if ( aDialogType && ! strcmp( "yesnocancel" , aDialogType ) )
                {
                        strcat( lDialogString ,"buttons {\\\"No\\\", \\\"Yes\\\", \\\"Cancel\\\"} " ) ;
                        switch (aDefaultButton)
                        {
                                case 1: strcat( lDialogString ,"default button \\\"Yes\\\" " ) ; break;
                                case 2: strcat( lDialogString ,"default button \\\"No\\\" " ) ; break;
                                case 0: strcat( lDialogString ,"default button \\\"Cancel\\\" " ) ; break;
                        }
                        strcat( lDialogString ,"cancel button \\\"Cancel\\\"" ) ;
                }
                else
                {
                        strcat( lDialogString ,"buttons {\\\"OK\\\"} " ) ;
                        strcat( lDialogString ,"default button \\\"OK\\\" " ) ;
                }
                strcat( lDialogString, ")\" ") ;

                strcat( lDialogString,
"-e \"if vButton is \\\"Yes\\\" then\" -e \"return 1\"\
 -e \"else if vButton is \\\"OK\\\" then\" -e \"return 1\"\
 -e \"else if vButton is \\\"No\\\" then\" -e \"return 2\"\
 -e \"else\" -e \"return 0\" -e \"end if\" " );

                strcat( lDialogString, "-e \"on error number -128\" " ) ;
                strcat( lDialogString, "-e \"0\" " );

                strcat( lDialogString, "-e \"end try\"") ;
                if ( ! osx9orBetter() ) strcat( lDialogString, " -e \"end tell\"") ;
        }
        else if ( zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"zenity");return 1;}
                        strcpy( lDialogString , "szAnswer=$(zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat(lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat(lDialogString, " --");

                if ( aDialogType && ! strcmp( "okcancel" , aDialogType ) )
                {
                                strcat( lDialogString ,
                                                "question --ok-label=Ok --cancel-label=Cancel" ) ;
                }
                else if ( aDialogType && ! strcmp( "yesno" , aDialogType ) )
                {
                                strcat( lDialogString , "question" ) ;
                }
                else if ( aDialogType && ! strcmp( "yesnocancel" , aDialogType ) )
                {
                        strcat( lDialogString , "list --column \"\" --hide-header \"Yes\" \"No\"" ) ;
                }
                else if ( aIconType && ! strcmp( "error" , aIconType ) )
                {
                    strcat( lDialogString , "error" ) ;
                }
                else if ( aIconType && ! strcmp( "warning" , aIconType ) )
                {
                    strcat( lDialogString , "warning" ) ;
                }
                else
                {
                    strcat( lDialogString , "info" ) ;
                }
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title=\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, " --no-wrap --text=\"") ;
                        strcat(lDialogString, aMessage) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( (zenity3Present() >= 3) )
                {
                        strcat( lDialogString , " --icon-name=dialog-" ) ;
                        if ( aIconType && (! strcmp( "question" , aIconType )
                          || ! strcmp( "error" , aIconType )
                          || ! strcmp( "warning" , aIconType ) ) )
                        {
                                strcat( lDialogString , aIconType ) ;
                        }
                        else
                        {
                                strcat( lDialogString , "information" ) ;
                        }
                }

                if ( ! strcmp( "yesnocancel" , aDialogType ) )
                {
                        strcat( lDialogString ,
");if [ $? = 1 ];then echo 0;elif [ $szAnswer = \"No\" ];then echo 2;else echo 1;fi");
                }
                else
                {
                        strcat( lDialogString , ");if [ $? = 0 ];then echo 1;else echo 0;fi");
                }
        }

        if (tinyfd_verbose) printf( "lDialogString: %s\n" , lDialogString ) ;

        if ( ! ( lIn = popen( lDialogString , "r" ) ) )
        {
                free(lDialogString);
                return 0 ;
        }
        while ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
        {}

        pclose( lIn ) ;

        /* printf( "lBuff: %s len: %lu \n" , lBuff , strlen(lBuff) ) ; */
        if ( lBuff[strlen( lBuff ) -1] == '\n' )
        {
                lBuff[strlen( lBuff ) -1] = '\0' ;
        }
        /* printf( "lBuff1: %s len: %lu \n" , lBuff , strlen(lBuff) ) ; */

        if (aDialogType && !strcmp("yesnocancel", aDialogType))
        {
                if ( lBuff[0]=='1' )
                {
                        if ( !strcmp( lBuff+1 , "Yes" )) strcpy(lBuff,"1");
                        else if ( !strcmp( lBuff+1 , "No" )) strcpy(lBuff,"2");
                }
        }
        /* printf( "lBuff2: %s len: %lu \n" , lBuff , strlen(lBuff) ) ; */

        lResult =  !strcmp( lBuff , "2" ) ? 2 : !strcmp( lBuff , "1" ) ? 1 : 0;

        /* printf( "lResult: %d\n" , lResult ) ; */
        free(lDialogString);
        return lResult ;
}

/* returns NULL on cancel */
char const * tinyfd_inputBox(
        char const * const aTitle , /* NULL or "" */
        char const * const aMessage , /* NULL or "" may NOT contain \n nor \t */
        char const * const aDefaultInput ) /* NULL or "" */
{
        static char lBuff[MAX_PATH_OR_CMD];
        char * lDialogString = NULL;
        FILE * lIn ;
        int lResult ;
        int lWasBasicXterm = 0 ;
        size_t lTitleLen ;
        size_t lMessageLen ;

        lBuff[0]='\0';

        lTitleLen =  aTitle ? strlen(aTitle) : 0 ;
        lMessageLen =  aMessage ? strlen(aMessage) : 0 ;
        if ( !aTitle || strcmp(aTitle,"tinyfd_query") )
        {
                lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );
        }

        if ( osascriptPresent( ) )
        {
                if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"applescript");return (char const *)1;}
                strcpy( lDialogString , "osascript ");
                if ( ! osx9orBetter() ) strcat( lDialogString , " -e \"tell application \\\"System Events\\\"\" -e \"Activate\"");
                strcat( lDialogString , " -e \"try\" -e \"display dialog \\\"") ;
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, aMessage) ;
                }
                strcat(lDialogString, "\\\" ") ;
                strcat(lDialogString, "default answer \\\"") ;
                if ( aDefaultInput && strlen(aDefaultInput) )
                {
                        strcat(lDialogString, aDefaultInput) ;
                }
                strcat(lDialogString, "\\\" ") ;
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, "with title \\\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\\\" ") ;
                }
                strcat(lDialogString, "with icon note\" ") ;
                strcat(lDialogString, "-e \"\\\"1\\\" & text returned of result\" " );
                strcat(lDialogString, "-e \"on error number -128\" " ) ;
                strcat(lDialogString, "-e \"0\" " );
                strcat(lDialogString, "-e \"end try\"") ;
                if ( ! osx9orBetter() ) strcat(lDialogString, " -e \"end tell\"") ;
        }
        else if ( zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"zenity");return (char const *)1;}
                        strcpy( lDialogString , "szAnswer=$(zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat( lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat( lDialogString ," --entry" ) ;

                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title=\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, " --text=\"") ;
                        strcat(lDialogString, aMessage) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aDefaultInput && strlen(aDefaultInput) )
                {
                        strcat(lDialogString, " --entry-text=\"") ;
                        strcat(lDialogString, aDefaultInput) ;
                        strcat(lDialogString, "\"") ;
                }
                strcat( lDialogString ,
                                ");if [ $? = 0 ];then echo 1$szAnswer;else echo 0$szAnswer;fi");
        }

        if (tinyfd_verbose) printf( "lDialogString: %s\n" , lDialogString ) ;
        lIn = popen( lDialogString , "r" );
        if ( ! lIn  )
        {
                if ( fileExists("/tmp/tinyfd.txt") )
                {
                        wipefile("/tmp/tinyfd.txt");
                        remove("/tmp/tinyfd.txt");
                }
                if ( fileExists("/tmp/tinyfd0.txt") )
                {
                        wipefile("/tmp/tinyfd0.txt");
                        remove("/tmp/tinyfd0.txt");
                }
                free(lDialogString);
                return NULL ;
        }
        while ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
        {}

        pclose( lIn ) ;

        if ( fileExists("/tmp/tinyfd.txt") )
        {
                wipefile("/tmp/tinyfd.txt");
                remove("/tmp/tinyfd.txt");
        }
        if ( fileExists("/tmp/tinyfd0.txt") )
        {
                wipefile("/tmp/tinyfd0.txt");
                remove("/tmp/tinyfd0.txt");
        }

        /* printf( "len Buff: %lu\n" , strlen(lBuff) ) ; */
        /* printf( "lBuff0: %s\n" , lBuff ) ; */
        if ( lBuff[strlen( lBuff ) -1] == '\n' )
        {
                lBuff[strlen( lBuff ) -1] = '\0' ;
        }
        /* printf( "lBuff1: %s len: %lu \n" , lBuff , strlen(lBuff) ) ; */
        if ( lWasBasicXterm )
        {
                if ( strstr(lBuff,"^[") ) /* esc was pressed */
                {
                        free(lDialogString);
                        return NULL ;
                }
        }

        lResult =  strncmp( lBuff , "1" , 1) ? 0 : 1 ;
        /* printf( "lResult: %d \n" , lResult ) ; */
        if ( ! lResult )
        {
                free(lDialogString);
                return NULL ;
        }
        /* printf( "lBuff+1: %s\n" , lBuff+1 ) ; */
        free(lDialogString);

        return lBuff+1 ;
}


/* returns NULL on cancel */
char const * tinyfd_passwordBox(
        char const * const aTitle , /* NULL or "" */
        char const * const aMessage , /* NULL or "" may NOT contain \n nor \t */
        char const * const aDefaultInput ) /* NULL or "" */
{
        static char lBuff[MAX_PATH_OR_CMD];
        char * lDialogString = NULL;
        FILE * lIn ;
        int lResult ;
        int lWasBasicXterm = 0 ;
        size_t lTitleLen ;
        size_t lMessageLen ;

        lBuff[0]='\0';

        lTitleLen =  aTitle ? strlen(aTitle) : 0 ;
        lMessageLen =  aMessage ? strlen(aMessage) : 0 ;
        if ( !aTitle || strcmp(aTitle,"tinyfd_query") )
        {
                lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );
        }

        if ( osascriptPresent( ) )
        {
                if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"applescript");return (char const *)1;}
                strcpy( lDialogString , "osascript ");
                if ( ! osx9orBetter() ) strcat( lDialogString , " -e \"tell application \\\"System Events\\\"\" -e \"Activate\"");
                strcat( lDialogString , " -e \"try\" -e \"display dialog \\\"") ;
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, aMessage) ;
                }
                strcat(lDialogString, "\\\" ") ;
                strcat(lDialogString, "default answer \\\"") ;
                if ( aDefaultInput && strlen(aDefaultInput) )
                {
                        strcat(lDialogString, aDefaultInput) ;
                }
                strcat(lDialogString, "\\\" ") ;

                strcat(lDialogString, "hidden answer true ") ;

                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, "with title \\\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\\\" ") ;
                }
                strcat(lDialogString, "with icon note\" ") ;
                strcat(lDialogString, "-e \"\\\"1\\\" & text returned of result\" " );
                strcat(lDialogString, "-e \"on error number -128\" " ) ;
                strcat(lDialogString, "-e \"0\" " );
                strcat(lDialogString, "-e \"end try\"") ;
                if ( ! osx9orBetter() ) strcat(lDialogString, " -e \"end tell\"") ;
        }
        else if ( zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"zenity");return (char const *)1;}
                        strcpy( lDialogString , "szAnswer=$(zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat( lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat( lDialogString ," --entry" ) ;

                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title=\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, " --text=\"") ;
                        strcat(lDialogString, aMessage) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aDefaultInput && strlen(aDefaultInput) )
                {
                        strcat(lDialogString, " --entry-text=\"") ;
                        strcat(lDialogString, aDefaultInput) ;
                        strcat(lDialogString, "\"") ;
                }

		strcat(lDialogString, " --hide-text") ;

                strcat( lDialogString ,
                                ");if [ $? = 0 ];then echo 1$szAnswer;else echo 0$szAnswer;fi");
        }

        if (tinyfd_verbose) printf( "lDialogString: %s\n" , lDialogString ) ;
        lIn = popen( lDialogString , "r" );
        if ( ! lIn  )
        {
                if ( fileExists("/tmp/tinyfd.txt") )
                {
                        wipefile("/tmp/tinyfd.txt");
                        remove("/tmp/tinyfd.txt");
                }
                if ( fileExists("/tmp/tinyfd0.txt") )
                {
                        wipefile("/tmp/tinyfd0.txt");
                        remove("/tmp/tinyfd0.txt");
                }
                free(lDialogString);
                return NULL ;
        }
        while ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
        {}

        pclose( lIn ) ;

        if ( fileExists("/tmp/tinyfd.txt") )
        {
                wipefile("/tmp/tinyfd.txt");
                remove("/tmp/tinyfd.txt");
        }
        if ( fileExists("/tmp/tinyfd0.txt") )
        {
                wipefile("/tmp/tinyfd0.txt");
                remove("/tmp/tinyfd0.txt");
        }

        /* printf( "len Buff: %lu\n" , strlen(lBuff) ) ; */
        /* printf( "lBuff0: %s\n" , lBuff ) ; */
        if ( lBuff[strlen( lBuff ) -1] == '\n' )
        {
                lBuff[strlen( lBuff ) -1] = '\0' ;
        }
        /* printf( "lBuff1: %s len: %lu \n" , lBuff , strlen(lBuff) ) ; */
        if ( lWasBasicXterm )
        {
                if ( strstr(lBuff,"^[") ) /* esc was pressed */
                {
                        free(lDialogString);
                        return NULL ;
                }
        }

        lResult =  strncmp( lBuff , "1" , 1) ? 0 : 1 ;
        /* printf( "lResult: %d \n" , lResult ) ; */
        if ( ! lResult )
        {
                free(lDialogString);
                return NULL ;
        }
        /* printf( "lBuff+1: %s\n" , lBuff+1 ) ; */
        free(lDialogString);

        return lBuff+1 ;
}

std::vector<std::string> SplitString(const std::string &str, char delimiter) {
    std::vector<std::string> vec;
    std::stringstream sstr(str);
    std::string tmp;
    while (std::getline(sstr, tmp, delimiter)) vec.push_back(tmp);

    return vec;
}

std::string zenityFilter(std::string input) {

    std::vector<std::string> stringVec = SplitString(input, '|'); //split at delimeter

    std::string outputString = "";

    unsigned int index = 0;
    for (const std::string &str : stringVec) { //loops our array

        if (index % 2 == 0) { //code for even |
            outputString += " --file-filter='" + str + " | "; //concat string
        } else { //code for odd |
            outputString += str + "'"; //concat string
        }

        index++;
    }

    std::replace(outputString.begin(), outputString.end(), ';', ' ');

    return outputString;

}


char const * tinyfd_saveFileDialog(
    char const * const aTitle , /* NULL or "" */
    char const * const aDefaultPathAndFile , /* NULL or "" */
    int const aNumOfFilterPatterns , /* 0 */
    char const * const * const aFilterPatterns , /* NULL or {"*.jpg","*.png"} */
    char const * const aSingleFilterDescription ) /* NULL or "image files" */
{

        static char lBuff [MAX_PATH_OR_CMD] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        char lString [MAX_PATH_OR_CMD] ;
        FILE * lIn ;
        lBuff[0]='\0';

        if ( osascriptPresent( ) )
        {
                if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"applescript");return (char const *)1;}
                strcpy( lDialogString , "osascript ");
                if ( ! osx9orBetter() ) strcat( lDialogString , " -e \"tell application \\\"Finder\\\"\" -e \"Activate\"");
                strcat( lDialogString , " -e \"try\" -e \"POSIX path of ( choose file name " );
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, "with prompt \\\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\\\" ") ;
                }
                getPathWithoutFinalSlash( lString , aDefaultPathAndFile ) ;
                if ( strlen(lString) )
                {
                        strcat(lDialogString, "default location \\\"") ;
                        strcat(lDialogString, lString ) ;
                        strcat(lDialogString , "\\\" " ) ;
                }
                getLastName( lString , aDefaultPathAndFile ) ;
                if ( strlen(lString) )
                {
                        strcat(lDialogString, "default name \\\"") ;
                        strcat(lDialogString, lString ) ;
                        strcat(lDialogString , "\\\" " ) ;
                }
                strcat( lDialogString , ")\" " ) ;
                strcat(lDialogString, "-e \"on error number -128\" " ) ;
                strcat(lDialogString, "-e \"end try\"") ;
                if ( ! osx9orBetter() ) strcat( lDialogString, " -e \"end tell\"") ;
        }
        else if ( zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"zenity");return (char const *)1;}
                        strcpy( lDialogString , "zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat( lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat(lDialogString, " --file-selection --save --confirm-overwrite" ) ;

                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title=\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aDefaultPathAndFile && strlen(aDefaultPathAndFile) )
                {
                        strcat(lDialogString, " --filename=\"") ;
                        strcat(lDialogString, aDefaultPathAndFile) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aNumOfFilterPatterns > 0 )
                {
                        strcat( lDialogString , (char *)zenityFilter(aSingleFilterDescription).c_str() ) ;
                }
        }

        if (tinyfd_verbose) printf( "lDialogString: %s\n" , lDialogString ) ;
    if ( ! ( lIn = popen( lDialogString , "r" ) ) )
    {
        return NULL ;
    }
    while ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
    {}
    pclose( lIn ) ;
    if ( lBuff[strlen( lBuff ) -1] == '\n' )
    {
        lBuff[strlen( lBuff ) -1] = '\0' ;
    }
        /* printf( "lBuff: %s\n" , lBuff ) ; */
        if ( ! strlen(lBuff) )
        {
                return NULL;
        }
    getPathWithoutFinalSlash( lString , lBuff ) ;
    if ( strlen( lString ) && ! dirExists( lString ) )
    {
        return NULL ;
    }
        getLastName(lString,lBuff);
        if ( ! filenameValid(lString) )
        {
                return NULL;
        }
    return lBuff ;
}


/* in case of multiple files, the separator is | */
char const * tinyfd_openFileDialog(
    char const * const aTitle , /* NULL or "" */
    char const * const aDefaultPathAndFile , /* NULL or "" */
    int const aNumOfFilterPatterns , /* 0 */
    char const * const * const aFilterPatterns , /* NULL or {"*.jpg","*.png"} */
    char const * const aSingleFilterDescription , /* NULL or "image files" */
    int const aAllowMultipleSelects ) /* 0 or 1 */
{
        static char lBuff [MAX_MULTIPLE_FILES*MAX_PATH_OR_CMD] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        char lString [MAX_PATH_OR_CMD] ;
        int i ;
        FILE * lIn ;
        char * p ;
        char const * p2 ;
        lBuff[0]='\0';

        if ( osascriptPresent( ) )
        {
                if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"applescript");return (char const *)1;}
                strcpy( lDialogString , "osascript ");
                if ( ! osx9orBetter() ) strcat( lDialogString , " -e \"tell application \\\"System Events\\\"\" -e \"Activate\"");
                strcat( lDialogString , " -e \"try\" -e \"" );
    if ( ! aAllowMultipleSelects )
    {


                        strcat( lDialogString , "POSIX path of ( " );
                }
                else
                {
                        strcat( lDialogString , "set mylist to " );
                }
                strcat( lDialogString , "choose file " );
            if ( aTitle && strlen(aTitle) )
            {
                        strcat(lDialogString, "with prompt \\\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\\\" ") ;
            }
                getPathWithoutFinalSlash( lString , aDefaultPathAndFile ) ;
                if ( strlen(lString) )
                {
                        strcat(lDialogString, "default location \\\"") ;
                        strcat(lDialogString, lString ) ;
                        strcat(lDialogString , "\\\" " ) ;
                }
                if ( aNumOfFilterPatterns > 0 )
                {
                        strcat(lDialogString , "of type {\\\"" );
                        strcat( lDialogString , aFilterPatterns [0] + 2 ) ;
                        strcat( lDialogString , "\\\"" ) ;
                        for ( i = 1 ; i < aNumOfFilterPatterns ; i ++ )
                        {
                                strcat( lDialogString , ",\\\"" ) ;
                                strcat( lDialogString , aFilterPatterns [i] + 2) ;
                                strcat( lDialogString , "\\\"" ) ;
                        }
                        strcat( lDialogString , "} " ) ;
                }
                if ( aAllowMultipleSelects )
                {
                        strcat( lDialogString , "multiple selections allowed true \" " ) ;
                        strcat( lDialogString ,
                                        "-e \"set mystring to POSIX path of item 1 of mylist\" " );
                        strcat( lDialogString ,
                                        "-e \"repeat with  i from 2 to the count of mylist\" " );
                        strcat( lDialogString , "-e \"set mystring to mystring & \\\"|\\\"\" " );
                        strcat( lDialogString ,
                        "-e \"set mystring to mystring & POSIX path of item i of mylist\" " );
                        strcat( lDialogString , "-e \"end repeat\" " );
                        strcat( lDialogString , "-e \"mystring\" " );
                }
                else
                {
                        strcat( lDialogString , ")\" " ) ;
                }
                strcat(lDialogString, "-e \"on error number -128\" " ) ;
                strcat(lDialogString, "-e \"end try\"") ;
                if ( ! osx9orBetter() ) strcat( lDialogString, " -e \"end tell\"") ;
        }
        else if ( zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"zenity");return (char const *)1;}
                        strcpy( lDialogString , "zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat( lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat( lDialogString , " --file-selection" ) ;

                if ( aAllowMultipleSelects )
                {
                        strcat( lDialogString , " --multiple" ) ;
                }
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title=\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aDefaultPathAndFile && strlen(aDefaultPathAndFile) )
                {
                        strcat(lDialogString, " --filename=\"") ;
                        strcat(lDialogString, aDefaultPathAndFile) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aNumOfFilterPatterns > 0 )
                {
                        strcat( lDialogString , (char *)zenityFilter(aSingleFilterDescription).c_str() ) ;
                }
        }

    if (tinyfd_verbose) printf( "lDialogString: %s\n" , lDialogString ) ;
    if ( ! ( lIn = popen( lDialogString , "r" ) ) )
    {
        return NULL ;
    }
        lBuff[0]='\0';
        p=lBuff;
        while ( fgets( p , sizeof( lBuff ) , lIn ) != NULL )
        {
                p += strlen( p );
        }
    pclose( lIn ) ;
    if ( lBuff[strlen( lBuff ) -1] == '\n' )
    {
        lBuff[strlen( lBuff ) -1] = '\0' ;
    }
    /* printf( "lBuff: %s\n" , lBuff ) ; */
        /* printf( "lBuff2: %s\n" , lBuff ) ; */
        if ( ! strlen( lBuff )  )
        {
                return NULL;
        }
        if ( aAllowMultipleSelects && strchr(lBuff, '|') )
        {
                p2 = ensureFilesExist( lBuff , lBuff ) ;
        }
        else if ( fileExists(lBuff) )
        {
                p2 = lBuff ;
        }
        else
        {
                return NULL ;
        }
        /* printf( "lBuff3: %s\n" , p2 ) ; */

        return p2 ;
}


char const * tinyfd_selectFolderDialog(
        char const * const aTitle , /* "" */
        char const * const aDefaultPath ) /* "" */
{
        static char lBuff [MAX_PATH_OR_CMD] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        FILE * lIn ;
        lBuff[0]='\0';

        if ( osascriptPresent( ))
        {
                if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"applescript");return (char const *)1;}
                strcpy( lDialogString , "osascript ");
                if ( ! osx9orBetter() ) strcat( lDialogString , " -e \"tell application \\\"System Events\\\"\" -e \"Activate\"");
                strcat( lDialogString , " -e \"try\" -e \"POSIX path of ( choose folder ");
                if ( aTitle && strlen(aTitle) )
                {
                strcat(lDialogString, "with prompt \\\"") ;
                strcat(lDialogString, aTitle) ;
                strcat(lDialogString, "\\\" ") ;
                }
                if ( aDefaultPath && strlen(aDefaultPath) )
                {
                        strcat(lDialogString, "default location \\\"") ;
                        strcat(lDialogString, aDefaultPath ) ;
                        strcat(lDialogString , "\\\" " ) ;
                }
                strcat( lDialogString , ")\" " ) ;
                strcat(lDialogString, "-e \"on error number -128\" " ) ;
                strcat(lDialogString, "-e \"end try\"") ;
                if ( ! osx9orBetter() ) strcat( lDialogString, " -e \"end tell\"") ;
        }
        else if ( zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"zenity");return (char const *)1;}
                        strcpy( lDialogString , "zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat( lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat( lDialogString , " --file-selection --directory" ) ;

                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title=\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                if ( aDefaultPath && strlen(aDefaultPath) )
                {
                        strcat(lDialogString, " --filename=\"") ;
                        strcat(lDialogString, aDefaultPath) ;
                        strcat(lDialogString, "\"") ;
                }
        }

    if (tinyfd_verbose) printf( "lDialogString: %s\n" , lDialogString ) ;
    if ( ! ( lIn = popen( lDialogString , "r" ) ) )
    {
        return NULL ;
    }
        while ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
        {}
        pclose( lIn ) ;
    if ( lBuff[strlen( lBuff ) -1] == '\n' )
    {
        lBuff[strlen( lBuff ) -1] = '\0' ;
    }
        /* printf( "lBuff: %s\n" , lBuff ) ; */
        if ( ! strlen( lBuff ) || ! dirExists( lBuff ) )
        {
                return NULL ;
        }
        return lBuff ;
}


/* returns the hexcolor as a string "#FF0000" */
/* aoResultRGB also contains the result */
/* aDefaultRGB is used only if aDefaultHexRGB is NULL */
/* aDefaultRGB and aoResultRGB can be the same array */
char const * tinyfd_colorChooser(
        char const * const aTitle , /* NULL or "" */
        char const * const aDefaultHexRGB , /* NULL or "#FF0000"*/
        unsigned char const aDefaultRGB[3] , /* { 0 , 255 , 255 } */
        unsigned char aoResultRGB[3] ) /* { 0 , 0 , 0 } */
{
        static char lBuff [128] ;
        char lTmp [128] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        char lDefaultHexRGB[8];
        char * lpDefaultHexRGB;
        unsigned char lDefaultRGB[3];
        FILE * lIn ;
        int lWasZenity3 = 0 ;
        int lWasOsascript = 0 ;
        lBuff[0]='\0';

        if ( aDefaultHexRGB )
        {
                Hex2RGB( aDefaultHexRGB , lDefaultRGB ) ;
                lpDefaultHexRGB = (char *) aDefaultHexRGB ;
        }
        else
        {
                lDefaultRGB[0]=aDefaultRGB[0];
                lDefaultRGB[1]=aDefaultRGB[1];
                lDefaultRGB[2]=aDefaultRGB[2];
                RGB2Hex( aDefaultRGB , lDefaultHexRGB ) ;
                lpDefaultHexRGB = (char *) lDefaultHexRGB ;
        }

        if ( osascriptPresent( ) )
        {
            if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"applescript");return (char const *)1;}
            lWasOsascript = 1 ;
            strcpy( lDialogString , "osascript");
            
            if ( ! osx9orBetter() )
            {
                strcat( lDialogString , " -e \"tell application \\\"System Events\\\"\" -e \"Activate\"");
                strcat( lDialogString , " -e \"try\" -e \"set mycolor to choose color default color {");
            }
            else
            {
                strcat( lDialogString ,
                       " -e \"try\" -e \"tell app (path to frontmost application as Unicode text) \
                       to set mycolor to choose color default color {");
            }
            
            sprintf(lTmp, "%d", 256 * lDefaultRGB[0] ) ;
            strcat(lDialogString, lTmp ) ;
            strcat(lDialogString, "," ) ;
            sprintf(lTmp, "%d", 256 * lDefaultRGB[1] ) ;
            strcat(lDialogString, lTmp ) ;
            strcat(lDialogString, "," ) ;
            sprintf(lTmp, "%d", 256 * lDefaultRGB[2] ) ;
            strcat(lDialogString, lTmp ) ;
            strcat(lDialogString, "}\" " ) ;
            strcat( lDialogString ,
                   "-e \"set mystring to ((item 1 of mycolor) div 256 as integer) as string\" " );
            strcat( lDialogString ,
                   "-e \"repeat with i from 2 to the count of mycolor\" " );
            strcat( lDialogString ,
                   "-e \"set mystring to mystring & \\\" \\\" & ((item i of mycolor) div 256 as integer) as string\" " );
            strcat( lDialogString , "-e \"end repeat\" " );
            strcat( lDialogString , "-e \"mystring\" ");
            strcat(lDialogString, "-e \"on error number -128\" " ) ;
            strcat(lDialogString, "-e \"end try\"") ;
            if ( ! osx9orBetter() ) strcat( lDialogString, " -e \"end tell\"") ;
        }
        else if ( zenity3Present() )
        {
                lWasZenity3 = 1 ;
                if ( zenity3Present() )
                {
                        if (aTitle&&!strcmp(aTitle,"tinyfd_query")){strcpy(tinyfd_response,"zenity3");return (char const *)1;}
                        strcpy( lDialogString , "zenity" );
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat( lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat( lDialogString , " --color-selection --show-palette" ) ;
                sprintf( lDialogString + strlen(lDialogString), " --color=%s" , lpDefaultHexRGB ) ;

                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title=\"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
        }

        if (tinyfd_verbose) printf( "lDialogString: %s\n" , lDialogString ) ;
        if ( ! ( lIn = popen( lDialogString , "r" ) ) )
        {
                return NULL ;
    }
        while ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
        {
        }
        pclose( lIn ) ;
    if ( ! strlen( lBuff ) )
    {
        return NULL ;
    }
        /* printf( "len Buff: %lu\n" , strlen(lBuff) ) ; */
        /* printf( "lBuff0: %s\n" , lBuff ) ; */
    if ( lBuff[strlen( lBuff ) -1] == '\n' )
    {
        lBuff[strlen( lBuff ) -1] = '\0' ;
    }

    if ( lWasZenity3 )
    {
                if ( lBuff[0] == '#' )
                {
                        if ( strlen(lBuff)>7 )
                        {
                                lBuff[3]=lBuff[5];
                                lBuff[4]=lBuff[6];
                                lBuff[5]=lBuff[9];
                                lBuff[6]=lBuff[10];
                                lBuff[7]='\0';
                        }
                Hex2RGB(lBuff,aoResultRGB);
                }
                else if ( lBuff[3] == '(' ) {
                        sscanf(lBuff,"rgb(%hhu,%hhu,%hhu",
                                        & aoResultRGB[0], & aoResultRGB[1],& aoResultRGB[2]);
                        RGB2Hex(aoResultRGB,lBuff);
                }
                else if ( lBuff[4] == '(' ) {
                        sscanf(lBuff,"rgba(%hhu,%hhu,%hhu",
                                        & aoResultRGB[0], & aoResultRGB[1],& aoResultRGB[2]);
                        RGB2Hex(aoResultRGB,lBuff);
                }
    }
    else if ( lWasOsascript )
    {
                /* printf( "lBuff: %s\n" , lBuff ) ; */
        sscanf(lBuff,"%hhu %hhu %hhu",
                           & aoResultRGB[0], & aoResultRGB[1],& aoResultRGB[2]);
        RGB2Hex(aoResultRGB,lBuff);
    }
        /* printf("%d %d %d\n", aoResultRGB[0],aoResultRGB[1],aoResultRGB[2]); */
        /* printf( "lBuff: %s\n" , lBuff ) ; */
        return lBuff ;
}


/*
int main( int argc , char * argv[] )
{
char const * lTmp;
char const * lTheSaveFileName;
char const * lTheOpenFileName;
char const * lTheSelectFolderName;
char const * lTheHexColor;
char const * lWillBeGraphicMode;
unsigned char lRgbColor[3];
FILE * lIn;
char lBuffer[1024];
char lString[1024];
char const * lFilterPatterns[2] = { "*.txt", "*.text" };

tinyfd_verbose = argc - 1;

lWillBeGraphicMode = tinyfd_inputBox("tinyfd_query", NULL, NULL);

strcpy(lBuffer, "v");
strcat(lBuffer, tinyfd_version);
if (lWillBeGraphicMode)
{
    strcat(lBuffer, "\ngraphic mode: ");
}
else
{
    strcat(lBuffer, "\nconsole mode: ");
}
strcat(lBuffer, tinyfd_response);
strcat(lBuffer, "\n");
strcat(lBuffer, tinyfd_needs+78);
strcpy(lString, "tinyfiledialogs");
tinyfd_messageBox(lString, lBuffer, "ok", "info", 0);

tinyfd_notifyPopup("the title", "the message\n\tfrom outer-space", "info");

if (lWillBeGraphicMode && !tinyfd_forceConsole)
{
        tinyfd_forceConsole = ! tinyfd_messageBox("Hello World",
                "graphic dialogs [yes] / console mode [no]?",
                "yesno", "question", 1);
}

lTmp = tinyfd_inputBox(
        "a password box", "your password will be revealed", NULL);

if (!lTmp) return 1;

strcpy(lString, lTmp);

lTheSaveFileName = tinyfd_saveFileDialog(
        "let us save this password",
        "passwordFile.txt",
        2,
        lFilterPatterns,
        NULL);

if (!lTheSaveFileName)
{
        tinyfd_messageBox(
                "Error",
                "Save file name is NULL",
                "ok",
                "error",
                1);
        return 1;
}

lIn = fopen(lTheSaveFileName, "w");
if (!lIn)
{
        tinyfd_messageBox(
                "Error",
                "Can not open this file in write mode",
                "ok",
                "error",
                1);
        return 1;
}
fputs(lString, lIn);
fclose(lIn);

lTheOpenFileName = tinyfd_openFileDialog(
        "let us read the password back",
        "",
        2,
        lFilterPatterns,
        NULL,
        0);

if (!lTheOpenFileName)
{
        tinyfd_messageBox(
                "Error",
                "Open file name is NULL",
                "ok",
                "error",
                1);
        return 1;
}

lIn = fopen(lTheOpenFileName, "r");

if (!lIn)
{
        tinyfd_messageBox(
                "Error",
                "Can not open this file in read mode",
                "ok",
                "error",
                1);
        return(1);
}
lBuffer[0] = '\0';
fgets(lBuffer, sizeof(lBuffer), lIn);
fclose(lIn);

tinyfd_messageBox("your password is",
        lBuffer, "ok", "info", 1);

lTheSelectFolderName = tinyfd_selectFolderDialog(
        "let us just select a directory", NULL);

if (!lTheSelectFolderName)
{
        tinyfd_messageBox(
                "Error",
                "Select folder name is NULL",
                "ok",
                "error",
                1);
        return 1;
}

tinyfd_messageBox("The selected folder is",
        lTheSelectFolderName, "ok", "info", 1);

lTheHexColor = tinyfd_colorChooser(
        "choose a nice color",
        "#FF0077",
        lRgbColor,
        lRgbColor);

if (!lTheHexColor)
{
        tinyfd_messageBox(
                "Error",
                "hexcolor is NULL",
                "ok",
                "error",
                1);
        return 1;
}

tinyfd_messageBox("The selected hexcolor is",
        lTheHexColor, "ok", "info", 1);

        tinyfd_beep();

        return 0;
}
*/

#ifdef _MSC_VER
#pragma warning(default:4996)
#pragma warning(default:4100)
#pragma warning(default:4706)
#endif
