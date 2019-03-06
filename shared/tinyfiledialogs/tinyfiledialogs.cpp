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
        while ((p2 = strchr(p, '\n')) != NULL)
        {
                lLen = p2 - p;
                memmove(lDestination, p, lLen);
                lDestination[lLen] = '\0';
                if (fileExists(lDestination))
                {
                        lDestination += lLen;
                        *lDestination = '\n';
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
                return 1 ;
        }
        else
        {
                pclose( lIn ) ;
                return 0 ;
        }
}


static int graphicMode( )
{
        return ( getenv("DISPLAY") != NULL ) ;
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


static int zenity3Present( )
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
                        }
                        pclose( lIn ) ;
                }
        }
        return graphicMode() ? lZenity3Present : 0 ;
}


static int kdialogPresent( )
{
        static int lKdialogPresent = -1 ;
        char lBuff [MAX_PATH_OR_CMD] ;
        FILE * lIn ;
        char * lDesktop;

        if ( lKdialogPresent < 0 )
        {
                lKdialogPresent = detectPresence("kdialog") ;
                if ( lKdialogPresent && !getenv("SSH_TTY") )
                {
                        lIn = popen( "kdialog --attach 2>&1" , "r" ) ;
                        if ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
                        {
                                if ( ! strstr( "Unknown" , lBuff ) )
                                {
                                        lKdialogPresent = 2 ;
                                }
                        }
                        pclose( lIn ) ;

                        if (lKdialogPresent == 2)
                        {
                                lKdialogPresent = 1 ;
                                lIn = popen( "kdialog --passivepopup 2>&1" , "r" ) ;
                                if ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
                                {
                                        if ( ! strstr( "Unknown" , lBuff ) )
                                        {
                                                lKdialogPresent = 2 ;
                                        }
                                }
                                pclose( lIn ) ;
                        }
                }
        }
        return graphicMode() ? lKdialogPresent : 0 ;
}


static std::vector<std::string> SplitString(const std::string &str, char delimiter)
{
        std::vector<std::string> vec;
        std::stringstream sstr(str);
        std::string tmp;

        while (std::getline(sstr, tmp, delimiter))
                vec.push_back(tmp);

        return vec;
}


static std::string string_replace_all(std::string str, std::string substr, std::string newstr)
{
    size_t pos = 0;
    const size_t sublen = substr.length(), newlen = newstr.length();

    while ((pos = str.find(substr, pos)) != std::string::npos)
    {
        str.replace(pos, sublen, newstr);
        pos += newlen;
    }

    return str;
}


static std::string zenityFilter(std::string input)
{
        std::replace(input.begin(), input.end(), ';', ' ');
        std::vector<std::string> stringVec = SplitString(input, '|');
        std::string outputString = "";

        unsigned int index = 0;
        for (const std::string &str : stringVec)
        {
                if (index % 2 == 0)
                {
                        outputString += " --file-filter='" + str + "|";
                }
                else
                {
                        outputString += string_replace_all(str, "*.*", "*") + "'";
                }

                index++;
        }

        return outputString;
}


static std::string kdialogFilter(std::string input)
{
        std::replace(input.begin(), input.end(), ';', ' ');
        std::vector<std::string> stringVec = SplitString(input, '|');
        std::string outputString = " \"";

        std::string even = "";
        std::string odd = "";

        unsigned int index = 0;
        for (const std::string &str : stringVec)
        {
                if (index % 2 != 0)
                {
                        if (index == 1)
                                odd = str;
                        else
                                odd = "\n" + str;

                        outputString += string_replace_all(odd, "*.*", "*") + even;
                }
                else
                {
                        even = "|" + str;
                }

                index++;
        }

        outputString += "\"";
        return outputString;
}


int tinyfd_messageBox(
        char const * const aTitle , /* NULL or "" */
        char const * const aMessage , /* NULL or "" */
        char const * const aDialogType , /* "ok" "okcancel" "yesno" "yesnocancel" */
        char const * const aIconType , /* "info" "warning" "error" "question" */
        int const aDialogEngine) /* 0 for Zenity, 1 for KDialog */
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
        lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );

        if ( aDialogEngine == 0 && zenityPresent() )
        {
                if ( zenityPresent() )
                {
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
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {
                strcpy( lDialogString , "kdialog" ) ;
                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }

                strcat( lDialogString , " --" ) ;
                if ( aDialogType && ( ! strcmp( "okcancel" , aDialogType )
                        || ! strcmp( "yesno" , aDialogType ) || ! strcmp( "yesnocancel" , aDialogType ) ) )
                {
                        if ( aIconType && ( ! strcmp( "warning" , aIconType )
                                || ! strcmp( "error" , aIconType ) ) )
                        {
                                strcat( lDialogString , "warning" ) ;
                        }
                        if ( ! strcmp( "yesnocancel" , aDialogType ) )
                        {
                                strcat( lDialogString , "yesnocancel" ) ;
                        }
                        else
                        {
                                strcat( lDialogString , "yesno" ) ;
                        }
                }
                else if ( aIconType && ! strcmp( "error" , aIconType ) )
                {
                        strcat( lDialogString , "error" ) ;
                }
                else if ( aIconType && ! strcmp( "warning" , aIconType ) )
                {
                        strcat( lDialogString , "sorry" ) ;
                }
                else
                {
                        strcat( lDialogString , "msgbox" ) ;
                }
                strcat( lDialogString , " \"" ) ;
                if ( aMessage )
                {
                        strcat( lDialogString , aMessage ) ;
                }
                strcat( lDialogString , "\"" ) ;
                if ( aDialogType && ! strcmp( "okcancel" , aDialogType ) )
                {
                        strcat( lDialogString ,
                                " --yes-label Ok --no-label Cancel" ) ;
                }
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }

                if ( ! strcmp( "yesnocancel" , aDialogType ) )
                {
                        strcat( lDialogString , "; x=$? ;if [ $x = 0 ] ;then echo 1;elif [ $x = 1 ] ;then echo 2;else echo 0;fi");
                }
                else
                {
                        strcat( lDialogString , ";if [ $? = 0 ];then echo 1;else echo 0;fi");
                }
        }

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

int tinyfd_errorMessageBox(
	char const * const aTitle , /* NULL or "" */
	char const * const aMessage , /* NULL or "" */
	int const aFatalError , /* 0 for abort/retry/ignore , 1 for abort */
	int const aDialogEngine ) /* 0 for Zenity, 1 for KDialog */
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
        lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );

        if ( aDialogEngine == 0 && zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        strcpy( lDialogString , "szAnswer=$(zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat(lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat(lDialogString, " --error");
                if ( zenity3Present() )
                {
                        strcat( lDialogString , " --ok-label Abort" ) ;
                        strcat( lDialogString , " --extra-button Retry" ) ;
                        strcat( lDialogString , " --extra-button Ignore" ) ;
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
                        strcat( lDialogString , " --icon-name=dialog-error" ) ;
                }

                if ( !aFatalError )
                {
                        strcat( lDialogString ,
                                ");if [ $? = 0 ];then echo 2;elif [ $szAnswer = \"Retry\" ];then echo 1;elif [ $szAnswer = \"Ignore\" ];then echo 0;else echo 0;fi");
                }
                else
                {
                        strcat( lDialogString , ");if [ $? = 0 ];then echo 2;else echo 2;fi");
                }
        }
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {
                strcpy( lDialogString , "kdialog" ) ;
                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }

                strcat( lDialogString , " --warningyesnocancel" ) ;
                strcat( lDialogString , " \"" ) ;
                if ( aMessage )
                {
                        strcat( lDialogString , aMessage ) ;
                }
                strcat( lDialogString , "\"" ) ;
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                strcat( lDialogString , " --yes-label Abort" ) ;
                strcat( lDialogString , " --no-label Retry" ) ;
                strcat( lDialogString , " --cancel-label Ignore" ) ;

                if ( !aFatalError )
                {
                        strcat( lDialogString , "; x=$? ;if [ $x = 0 ] ;then echo 2;elif [ $x = 1 ] ;then echo 1;else echo 0;fi");
                }
                else
                {
                        strcat( lDialogString , ";if [ $? = 0 ];then echo 2;else echo 2;fi");
                }
        }

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

        if ( !aFatalError )
        {
                if ( lBuff[0]=='1' )
                {
                        if ( !strcmp( lBuff+1 , "Abort" )) strcpy(lBuff,"2");
                        else if ( !strcmp( lBuff+1 , "Retry" )) strcpy(lBuff,"1");
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
        char const * const aMessage , /* NULL or "" */
        char const * const aDefaultInput , /* NULL or "" */
        int const aDialogEngine) /* 0 for Zenity, 1 for KDialog */
{
        static char lBuff[MAX_PATH_OR_CMD];
        char * lDialogString = NULL;
        FILE * lIn ;
        int lResult ;
        size_t lTitleLen ;
        size_t lMessageLen ;

        lBuff[0]='\0';

        lTitleLen =  aTitle ? strlen(aTitle) : 0 ;
        lMessageLen =  aMessage ? strlen(aMessage) : 0 ;
        lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );

        if ( aDialogEngine == 0 && zenityPresent() )
        {
                if ( zenityPresent() )
                {
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
                strcat( lDialogString , ");if [ $? = 0 ];then echo 1$szAnswer;else echo 0$szAnswer;fi");
        }
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {
                strcpy( lDialogString , "szAnswer=$(kdialog" ) ;

                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }

                strcat(lDialogString, " --inputbox ") ;
                strcat(lDialogString, "\"") ;
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, aMessage ) ;
                }
                strcat(lDialogString , "\" \"" ) ;
                if ( aDefaultInput && strlen(aDefaultInput) )
                {
                        strcat(lDialogString, aDefaultInput ) ;
                }
                strcat(lDialogString , "\"" ) ;
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                strcat( lDialogString ,
                        ");if [ $? = 0 ];then echo 1$szAnswer;else echo 0$szAnswer;fi");
        }

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
        char const * const aMessage , /* NULL or "" */
        char const * const aDefaultInput , /* NULL or "" */
        int const aDialogEngine) /* 0 for Zenity, 1 for KDialog */
{
        static char lBuff[MAX_PATH_OR_CMD];
        char * lDialogString = NULL;
        FILE * lIn ;
        int lResult ;
        size_t lTitleLen ;
        size_t lMessageLen ;

        lBuff[0]='\0';

        lTitleLen =  aTitle ? strlen(aTitle) : 0 ;
        lMessageLen =  aMessage ? strlen(aMessage) : 0 ;
        lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );

        if ( aDialogEngine == 0 && zenityPresent() )
        {
                if ( zenityPresent() )
                {
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
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {
                strcpy( lDialogString , "szAnswer=$(kdialog" ) ;

                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }

                strcat(lDialogString, " --password ") ;
                strcat(lDialogString, "\"") ;
                if ( aMessage && strlen(aMessage) )
                {
                        strcat(lDialogString, aMessage ) ;
                }
                strcat(lDialogString , "\" \"" ) ;
                if ( aDefaultInput && strlen(aDefaultInput) )
                {
                        strcat(lDialogString, aDefaultInput ) ;
                }
                strcat(lDialogString , "\"" ) ;
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
                strcat( lDialogString , ");if [ $? = 0 ];then echo 1$szAnswer;else echo 0$szAnswer;fi");
        }

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


char const * tinyfd_saveFileDialog(
        char const * const aTitle , /* NULL or "" */
        char const * const aDefaultPathAndFile , /* NULL or "" */
        int const aNumOfFilterPatterns , /* 0 */
        char const * const * const aFilterPatterns , /* NULL or {"*.jpg","*.png"} */
        char const * const aSingleFilterDescription , /* NULL or "image files" */
        int const aDialogEngine) /* 0 for Zenity, 1 for KDialog */
{

        static char lBuff [MAX_PATH_OR_CMD] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        char lString [MAX_PATH_OR_CMD] ;
        int i ;
        FILE * lIn ;
        lBuff[0]='\0';

        if ( aDialogEngine == 0 && zenityPresent() )
        {
                if ( zenityPresent() )
                {
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
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {

                strcpy( lDialogString , "kdialog" ) ;
                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }
                strcat( lDialogString , " --getsavefilename /" ) ;
                if ( aDefaultPathAndFile && strlen(aDefaultPathAndFile) )
                {
                        if ( aDefaultPathAndFile[0] != '/' )
                        {
                                strcat(lDialogString, "\"$PWD/\"") ;
                        }
                        strcat(lDialogString, "\"") ;
                        strcat(lDialogString, aDefaultPathAndFile ) ;
                        strcat(lDialogString , "\"" ) ;
                }
                else
                {
                        strcat(lDialogString, "\"$PWD/\"") ;
                }
                if ( aNumOfFilterPatterns > 0 )
                {
                        strcat( lDialogString , (char *)kdialogFilter(aSingleFilterDescription).c_str() ) ;
                }
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
        }

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


/* in case of multiple files, the separator is \n */
char const * tinyfd_openFileDialog(
        char const * const aTitle , /* NULL or "" */
        char const * const aDefaultPathAndFile , /* NULL or "" */
        int const aNumOfFilterPatterns , /* 0 */
        char const * const * const aFilterPatterns , /* NULL or {"*.jpg","*.png"} */
        char const * const aSingleFilterDescription , /* NULL or "image files" */
        int const aAllowMultipleSelects , /* 0 or 1 */
        int const aDialogEngine) /* 0 for Zenity, 1 for KDialog */
{
        static char lBuff [MAX_MULTIPLE_FILES*MAX_PATH_OR_CMD] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        char lString [MAX_PATH_OR_CMD] ;
        int i ;
        FILE * lIn ;
        char * p ;
        char const * p2 ;
        int lWasKdialog = 0 ;
        lBuff[0]='\0';

        if ( aDialogEngine == 0 && zenityPresent() )
        {
                if ( zenityPresent() )
                {
                        strcpy( lDialogString , "zenity" ) ;
                        if ( (zenity3Present() >= 4) && !getenv("SSH_TTY") )
                        {
                                strcat( lDialogString, " --attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                        }
                }

                strcat( lDialogString , " --file-selection" ) ;

                if ( aAllowMultipleSelects )
                {
                        strcat( lDialogString , " --multiple --separator='\n'" ) ;
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
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {
                lWasKdialog = 1 ;

                strcpy( lDialogString , "kdialog" ) ;
                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }
                strcat( lDialogString , " --getopenfilename /" ) ;
                if ( aDefaultPathAndFile && strlen(aDefaultPathAndFile) )
                {
                        if ( aDefaultPathAndFile[0] != '/' )
                        {
                                strcat(lDialogString, "\"$PWD/\"") ;
                        }
                        strcat(lDialogString, "\"") ;
                        strcat(lDialogString, aDefaultPathAndFile ) ;
                        strcat(lDialogString , "\"" ) ;
                }
                else
                {
                        strcat(lDialogString, "\"$PWD/\"") ;
                }
                if ( aNumOfFilterPatterns > 0 )
                {
                        strcat( lDialogString , (char *)kdialogFilter(aSingleFilterDescription).c_str() ) ;
                }
                if ( aAllowMultipleSelects )
                {
                        strcat( lDialogString , " --multiple --separate-output" ) ;
                }
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
        }

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
        if ( lWasKdialog && aAllowMultipleSelects )
        {
                p = lBuff ;
        }
        /* printf( "lBuff2: %s\n" , lBuff ) ; */
        if ( ! strlen( lBuff )  )
        {
                return NULL;
        }
        if ( aAllowMultipleSelects && strchr(lBuff, '\n') )
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
        char const * const aDefaultPath , /* "" */
        int const aDialogEngine) /* 0 for Zenity, 1 for KDialog */
{
        static char lBuff [MAX_PATH_OR_CMD] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        FILE * lIn ;
        lBuff[0]='\0';

        if ( aDialogEngine == 0 && zenityPresent() )
        {
                if ( zenityPresent() )
                {
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
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {
                strcpy( lDialogString , "kdialog" ) ;
                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }
                strcat( lDialogString , " --getexistingdirectory /" ) ;
                if ( aDefaultPath && strlen(aDefaultPath) )
                {
                        if ( aDefaultPath[0] != '/' )
                        {
                                strcat(lDialogString, "\"$PWD/\"") ;
                        }
                        strcat(lDialogString, "\"") ;
                        strcat(lDialogString, aDefaultPath ) ;
                        strcat(lDialogString , "\"" ) ;
                }
                else
                {
                        strcat(lDialogString, "\"$PWD/\"") ;
                }
                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
        }

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
        unsigned char aoResultRGB[3] , /* { 0 , 0 , 0 } */
        int const aDialogEngine) /* 0 for Zenity, 1 for KDialog */
{
        static char lBuff [128] ;
        char lTmp [128] ;
        char lDialogString [MAX_PATH_OR_CMD] ;
        char lDefaultHexRGB[8];
        char * lpDefaultHexRGB;
        unsigned char lDefaultRGB[3];
        FILE * lIn ;
        int lWasZenity3 = 0 ;
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

        if ( aDialogEngine == 0 && zenity3Present() )
        {
                lWasZenity3 = 1 ;
                if ( zenity3Present() )
                {
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
        else if ( aDialogEngine == 1 && kdialogPresent() )
        {
                strcpy( lDialogString , "kdialog" ) ;
                if ( kdialogPresent() == 2 )
                {
                        strcat(lDialogString, " --attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2)"); /* contribution: Paul Rouget */
                }
                sprintf( lDialogString + strlen(lDialogString) , " --getcolor --default '%s'" , lpDefaultHexRGB ) ;

                if ( aTitle && strlen(aTitle) )
                {
                        strcat(lDialogString, " --title \"") ;
                        strcat(lDialogString, aTitle) ;
                        strcat(lDialogString, "\"") ;
                }
        }

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
        else
        {
                Hex2RGB(lBuff,aoResultRGB);
        }
        /* printf("%d %d %d\n", aoResultRGB[0],aoResultRGB[1],aoResultRGB[2]); */
        /* printf( "lBuff: %s\n" , lBuff ) ; */
        return lBuff ;
}
