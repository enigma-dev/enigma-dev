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

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
string msys_path_from_mingw = "msys\\1.0\\bin\\";

string fixdrive(string p)
{
  if (p[0] != '\\' and p[0] != '/')
    return p;
  
  char buf[3];
  GetCurrentDirectory(3,buf);
  
  if (((*buf >= 'A' and *buf <= 'Z') or (*buf >= 'a' and *buf <= 'z')) and buf[1] == ':')
    return string(buf,2) + p;
  return p;
}

int better_system(const char* jname, const char* param)
{
  DWORD exit_status;
  
  STARTUPINFO StartupInfo;
  PROCESS_INFORMATION ProcessInformation;
  
  ZeroMemory(&StartupInfo, sizeof(StartupInfo));
  StartupInfo.cb = sizeof(StartupInfo);
  
  ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));
  
  char buf[2048];
  sprintf(buf, "\"%s\" %s", jname, param);
  if (CreateProcess(0,(CHAR*)buf,NULL,NULL,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartupInfo,&ProcessInformation ))
  {
    WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
    GetExitCodeProcess(ProcessInformation.hProcess, &exit_status);
    CloseHandle(ProcessInformation.hProcess);
    CloseHandle(ProcessInformation.hThread);
  }
  else
    return -1;
  return exit_status;
}

typedef const char* const EMessage;
EMessage
 msg_welcome__caption =
  "Welcome to ENIGMA!",
 msg_welcome__gnu_not_found = 
  "Welcome to ENIGMA!\n"
  "This seems to be the first time you've run ENIGMA. I looked around, but I was unable "
  "to locate the GCC G++ compiler. Would you like me to install it for you?\n\n"
  "If you are clueless, press \"Yes\".",
 msg_welcome__gnu_found = 
  "Welcome to ENIGMA!\n"
  "This seems to be the first time you've run ENIGMA. I looked around, and I found a "
  "copy of MinGW at the following location:\n%s\r\n\r\n Would you like to use this version?\n\n"
  "Note that this version must include a working MSys distribution in order to comply "
  "with the newest version. ENIGMA is capable of installing this for you."
  "If you are somehow clueless as to how this got there, press \"Yes\", then ask "
  "for help if it doesn't work.",
 msg_welcome__msys_found =
  "Welcome to ENIGMA!\n"
  "This seems to be the first time you've run ENIGMA. I looked around, and I found "
  "what looks like an operable installation of GNU MSys, with make accessible from `make`. "
  "Does this belong to a valid and properly configured installation of MinGW?\n\n"
  "If you are unsure, select \"No\" and ENIGMA can install a new one for you.",
 msg_welcome__gnu_not_msys_found =
  "Welcome to ENIGMA!\n"
  "This seems to be the first time you've run ENIGMA. I looked around, and I found "
  "what looks like an operable installation of MinGW32. The issue is, no installation "
  "of GNU MSys was detected. ENIGMA can attempt to install this over your current "
  "MinGW installation. Would you like to try?\n\nIf you want to be completely safe, "
  "you can click 'no,' uninstall your current MinGW and have ENIGMA install a new one.",
 msg_welcome__gnu_not_msys_found_in_path =
  "This seems to be the first time you've run ENIGMA. I looked around, and I found "
  "what looks like an operable installation of MinGW32. I was unable, however, to "
  "find a working MSys make. On top of that, Windows was the one to locate it via "
  "PATH, so I don't know where it's actually installed. Would you like me to look "
  "for then install MSys in the default location? If not, please install and configure "
  "GNU MSys yourself (the mingw-get packages are msys-base msys-make) and re-run ENIGMA."
  "\n\nOtherwise, press 'yes' to install.",
 install_drive_ok =
  "ENIGMA will install the GCC on the root of this drive by default. The assigned "
  "letter for this drive is \"%s\". Is this drive OK? (You can select a different "
  "drive by pressing \"no\".",
 mingw__already_installed_here = 
  "It seems that MinGW may already be installed. At least one key subdirectory of \\MinGW\\ "
  "on the selected drive was found to already exist. Would you like to continue anyway?\n\n"
  "Selecting \"Yes\" will continue the installation, ignoring this warning.\n"
  "Selecting \"No\" will retry creating the directories (this option exists so you can uninstall the current MinGW and try again)\n"
  "Selecting \"Cancel\" will abort this installation.",
 install_failure =
  "Installation failed. Please submit the feedback in the console window as a bug report on "
  "the forums. You can copy by right clicking the console and choosing \"Mark\", then using "
  "ENTER to copy.",
 java_not_found =
  "Could not find Java.exe. Please install Sun's Java runtime environment so LGM can run.\r\n"
  "http://www.java.com/en/download/manual.jsp\r\n\r\n"
  "If you already have Java, and believe you have received this message in error, you could "
  "try adding it to your system PATH variable.",
 msg_error_nomingw_get =
  "It seems that you have a version of MinGW installed that was not unpacked via mingw-get. "
  "This makes it difficult to install new MinGW packages (including MSys). ENIGMA can install "
  "mingw-get for you, but if this is an older version, it may be best to cancel this install, "
  "uninstall MinGW, then have ENIGMA re-install it. Continue anyway?",
 msg_mgwget_fail =
  "MinGW-Get failed! This application is outside the control of the ENIGMA team and so most likely"
  "failed due to a change in package names. Please report this incident on ENIGMA's forums.\n\n"
  "mingw-get error code: %d",
 msg_caption_problem = "Warning";

string expand_message(string msg, string arg1)
{
  size_t p = msg.find("%s");
  if (p != string::npos)
    msg.replace(p,2,arg1);
  return msg;
}

void get_new_drive_letter(char dl[4])
{
  dl[0] = 'C';
  dl[1] = ':';
  dl[2] = '\\';
  dl[3] = 0;
}

string dirpart(string fqp)
{
  size_t lp = fqp.find_last_of("/\\");
  if (lp == string::npos) return "";
  return fqp.erase(lp+1);
}
string filepart(string fqp)
{
  size_t lp = fqp.find_last_of("/\\");
  if (lp == string::npos) return fqp;
  return fqp.substr(lp+1);
}

bool e_install_mingw(string drive_letter);
bool e_use_existing_install(const char* make,const char *mingwbinpath, const char *auxpath);
bool install_gnu_msys(string mingw_path);
int create_mingw_get_dirs(string path);
string copy_mingw_get_files(string path);

const char* const CONFIG_FILE = "Compilers\\Windows\\gcc.ey";

int main()
{
  /* Check if we've already installed. **/
  puts("Checking configuration");
  FILE *ey = fopen(CONFIG_FILE, "rb"); // The GCC.ey file does not exist until installation has finished, be it manually or by this installer.
  if (!ey)
  {
    /* No Compiler descriptor was found. Start probing around. */
    puts("First time run. Scouring for Make...");
    
    #define tritier(mingw,bin,make) ((mingw_bin_path = ((mingw_path = mingw) + bin)) + (makename = make)).c_str()
    string mingw_path, mingw_bin_path, makename;
    tritier("\\MinGW\\","bin\\","mingw32-make");
    cout << "THIS IS WHAT I THINK: " << mingw_path << " : " << mingw_bin_path << " : " << makename << endl << tritier("\\MinGW\\","bin\\","mingw32-make") << endl;
    int    a = better_system(tritier("","","make"),         "--version");
      cout << "tried 1" << endl;
    if (a)
    {
      a = better_system(tritier("","","mingw32-make"), "--version");
      cout << "tried 2" << endl;
      if (a) 
      {
        search_manually:
        a = better_system(tritier("\\MinGW\\","bin\\","mingw32-make"), "--version");
        cout << "tried 3" << endl;
        if (a) {
          a = better_system(tritier("C:\\MinGW\\","bin\\","mingw32-make"), "--version");
          cout << "tried 4" << endl;
        }
      }
    }
    
    if (a) // If we didn't find it
    {
      if (MessageBox(NULL, msg_welcome__gnu_not_found, msg_welcome__caption, MB_YESNO) == IDYES)
      {
        install_mingw:
        char drive_letter[4] = "\\";
        GetCurrentDirectory(4,drive_letter);
        
        label_get_install_drive:
        string install_drive_confirm = expand_message(install_drive_ok, drive_letter);
        if (MessageBox(NULL,install_drive_confirm.c_str(), "Install Drive", MB_YESNO) == IDNO)
        {
          get_new_drive_letter(drive_letter);
          if (!*drive_letter)
            goto confused_cancel;
          goto label_get_install_drive;
        }
        else
          if (!e_install_mingw(drive_letter))
          {
            MessageBox(NULL,install_failure,"Error",MB_OK);
            goto end;
          }
      }
      confused_cancel: ;
    }
    else // We located the GCC. 
    { 
      printf("MinGW32 Make detected. Accessible from `%s`.\nVerifying MSys...\n\n", mingw_bin_path.c_str());
      
      if (mingw_bin_path == "")
      {
        if (makename == "make")
        {
          if (MessageBox(NULL, msg_welcome__msys_found, msg_welcome__caption, MB_YESNO) == IDYES)
            e_use_existing_install("make","","");
        }
        else
        {
          if (MessageBox(NULL, msg_welcome__gnu_not_msys_found_in_path, msg_welcome__caption, MB_YESNO) == IDYES)
            goto search_manually;
          else goto end;
        }
      }
      
      recheck_msys:
      string msys_make_path = mingw_path + msys_path_from_mingw + "make.exe";
      bool msys_installed_too = !better_system(msys_make_path.c_str(), "--version");
      if (!msys_installed_too)
      {
        if (MessageBox(NULL, msg_welcome__gnu_not_msys_found, msg_welcome__caption, MB_YESNO) == IDYES)
          if (install_gnu_msys(mingw_path))
            goto recheck_msys;
          else
            goto end;
        else goto end;
      }
      
      string allpaths = mingw_bin_path + ";" + mingw_path + msys_path_from_mingw + ";";
      
      string msg = expand_message(msg_welcome__gnu_found, mingw_path);
      switch (MessageBox(NULL, msg.c_str(), msg_welcome__caption, MB_YESNOCANCEL))
      {
        case IDYES:    e_use_existing_install(msys_make_path.c_str(), mingw_bin_path.c_str(), allpaths.c_str());
        case IDNO:     goto install_mingw;
        case IDCANCEL: goto end;
      }
    }
  } else fclose(ey);
  
  puts("Scouring for Java");
  {
    const char *jpath = "java";
    
    char buf[MAX_PATH];
    GetEnvironmentVariable("programfiles", buf, MAX_PATH);
    string pfp = buf; pfp += "\\Java\\jre6\\bin\\java.exe";
    GetEnvironmentVariable("programfiles(x86)", buf, MAX_PATH);
    string pfx86p = buf; pfx86p += "\\Java\\jre6\\bin\\java.exe";
    
    int a = better_system(jpath, "-version");
    if (a)
    {
      a = better_system(jpath = pfp.c_str(), "-version"); // This should hopefully take care of most of it
      if (a)
      {
        a = better_system(jpath = pfx86p.c_str(), "-version"); //One would think this would take care of the rest
        if (a)
        {
          a = better_system(jpath = "\\Program Files\\Java\\jre6\\bin\\java.exe", "-version");
          if (a)
          {
            a = better_system(jpath = "\\Program Files (x86)\\Java\\jre6\\bin\\java.exe", "-version");
            if (a)
            {
              a = better_system(jpath = "C:\\Program Files\\Java\\jre6\\bin\\java.exe", "-version"); //At this point, they're probably running something that uses C:.
              if (a)
                a = better_system(jpath = "C:\\Program Files (x86)\\Java\\jre6\\bin\\java.exe", "-version"); //What a fucked up configuration. *cough* dazappa *cough*
            }
          }
        }
      }
    }
    if (!a)
    {
      printf("Calling `%s -jar l*.jar`\n\n",jpath);
      better_system(jpath,"-jar l*.jar");
    }
    else {
      puts(java_not_found);
      MessageBox(NULL, msg_welcome__gnu_not_found, "Java Problem", MB_OK);
    }
  }
  
  end:
  system("pause");
  return 0;
}

static inline bool CopyFile2(const char* x, const char* y)
{
  printf("      => Copy `%s`\n",x);
  return CopyFile(x,y,FALSE);
}

#define ierror(x...) return (printf(x), puts("\n"), FALSE)
#define or_toggle_potential_error() and (++potentialError, (GetLastError() != ERROR_ALREADY_EXISTS))
bool e_install_mingw(string dl)
{
  for (int i=0; i<10; i++)
    putc('\n',stdout);
  puts("Starting MinGW Install.");
  
  puts("* Creating MinGW install directory.");
  if (!CreateDirectory((dl + "MinGW").c_str(), NULL) and GetLastError() != ERROR_ALREADY_EXISTS)
    ierror("Failed to create main MinGW directory. Abort.");
  
  install_begin:
    if (create_mingw_get_dirs(dl)) // Warn the user if MinGW-get was already installed
      switch (MessageBox(NULL,mingw__already_installed_here, "Warning", MB_YESNOCANCEL))
      {
        case IDYES:    break;
        case IDNO:     goto install_begin;
        case IDCANCEL: return FALSE;
      }
   string mget = copy_mingw_get_files(dl);
   if (mget == "") ierror("Failed to copy MinGW-Get files. Abort.");
    
    puts("   * Calling MinGW-Get");
    
    int install_p = better_system(mget.c_str(), "install mingw32-make gcc g++ gdb msys-base msys-bash msys-make");
    if (install_p) {
      char m[1024]; sprintf(m,msg_mgwget_fail,install_p);
      MessageBox (NULL,m,"WARNING",MB_OK);
    }
    else puts("All requested components were installed correctly.");
  
    e_use_existing_install((dl + "MinGW\\" + msys_path_from_mingw + "make.exe").c_str(), (dl + "MinGW\\bin\\").c_str(), (dl + "MinGW\\msys\\1.0\\bin;" + dl + "MinGW\\bin\\;").c_str());
  return TRUE;
}

bool e_use_existing_install(const char* make,const char *binpath, const char *auxpath)
{
  FILE *cff = fopen(CONFIG_FILE, "wb");
  if (cff)
  {
    fprintf(cff,
            "%%e-yaml\n"
            "---\n"
            "Name: GNU GCC G++\n"
            "Native: Yes\n"
            "Maintainer: Josh / ENIGMA.exe #This is a generated file\n"
            "\n"
            "# Some info about it\n"
            "path: %s\n"
            "make: %s\n"
            "defines:  %scpp -dM -x c++ -E $blank\n"
            "searchdirs: %sgcc -E -x c++ -v $blank\n"
            "searchdirs-start: \"#include <...> search starts here:\"\n"
            "searchdirs-end: \"End of search list.\"\n"
            "\n",
            auxpath, make, binpath, binpath);
    fclose(cff);
  }
  return TRUE;
}

bool install_gnu_msys(string mingw_path)
{
  string mget = mingw_path + msys_path_from_mingw;
  bool mingwget = !better_system((mingw_path + "mingw-get").c_str(), "--version");
  if (!mingwget)
  {
    if (create_mingw_get_dirs(mingw_path))
      MessageBox(NULL, "NOTHING MAKES SENSE ANYMORE!\n\nENIGMA will attempt to recover.", "ERROR", MB_OK);
    mget = copy_mingw_get_files(mingw_path);
    if (mget == "") return (MessageBox(NULL, "Failed to copy MinGW-Get files. Could not install MSys.", "ERROR", MB_OK), 0);
  }
  
  int install_p = better_system(mget.c_str(), "install msys-base msys-bash msys-make");
  if (install_p) {
    char m[1024]; sprintf(m,msg_mgwget_fail,install_p);
    MessageBox (NULL,m,"WARNING",MB_OK);
  }
  
  return 1;
}

int create_mingw_get_dirs(string path)
{
  int potentialError = 0;
  puts("* Installing MinGW-Get.");
    puts("   * Creating Directories");
      if (!CreateDirectory((path + "MinGW\\bin\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW binary directory. Abort.");
      if (!CreateDirectory((path + "MinGW\\libexec\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW libexec directory. Abort.");
      if (!CreateDirectory((path + "MinGW\\libexec\\mingw-get\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get LibExec that is 1/2 pint of ice creamdirectory. Abort.");
      if (!CreateDirectory((path + "MinGW\\var\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get var directory. Abort.");
      if (!CreateDirectory((path + "MinGW\\var\\lib\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get lib directory. Abort.");
      if (!CreateDirectory((path + "MinGW\\var\\lib\\mingw-get\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get lib subdirectory. Abort.");
      if (!CreateDirectory((path + "MinGW\\var\\lib\\mingw-get\\data\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get data directory. Abort.");
    
  return potentialError;
}

#undef ierror
#define ierror(x...) return (printf(x), puts("\n"), "")
string copy_mingw_get_files(string path)
{
    puts("   * Copying Files");
    string mget = path + "MinGW\\bin\\mingw-get.exe";
      if (!CopyFile2("Autoconf\\mingw-get\\bin\\mingw-get.exe", mget.c_str()))
        ierror("Failed to copy MinGW-Get's executable! (error code %d)",(int)GetLastError());
      if (!CopyFile2("Autoconf\\mingw-get\\libexec\\mingw-get\\lastrites.exe", (path + "MinGW\\libexec\\mingw-get\\lastrites.exe").c_str()))
        ierror("Failed to copy MinGW-Get's lastrites! (error code %d)",(int)GetLastError());
      if (!CopyFile2("Autoconf\\mingw-get\\libexec\\mingw-get\\mingw-get-0.dll", (path + "MinGW\\libexec\\mingw-get\\mingw-get-0.dll").c_str()))
        ierror("Failed to copy MinGW-Get's DLL! (error code %d)",(int)GetLastError());
      if (!CopyFile2("Autoconf\\mingw-get\\var\\lib\\mingw-get\\data\\defaults.xml", (path + "MinGW\\var\\lib\\mingw-get\\data\\defaults.xml").c_str()))
        ierror("Failed to copy MinGW-Get's defaults! (error code %d)",(int)GetLastError());
    return mget;
}
