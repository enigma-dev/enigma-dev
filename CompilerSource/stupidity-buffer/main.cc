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
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

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
  if (CreateProcess(jname,(CHAR*)buf,NULL,NULL,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartupInfo,&ProcessInformation ))
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
 welcome__caption =
  "Welcome to ENIGMA!",
 welcome__gnu_not_found = 
  "Welcome to ENIGMA!\n"
  "This seems to be the first time you've run ENIGMA. I looked around, but I was unable "
  "to locate the GCC G++ compiler. Would you like me to install it for you?\n\n"
  "If you are clueless, press \"Yes\".",
 welcome__gnu_found = 
  "Welcome to ENIGMA!\n"
  "This seems to be the first time you've run ENIGMA. I looked around, and I found a "
  "copy of MinGW at the following location:\n%s\r\n\r\n Would you like to use this version?\n\n"
  "Note that this version must include a working MSys distribution in order to comply "
  "with the newest version. ENIGMA is capable of installing this"
  "If you are somehow clueless as to how this got there, press \"Yes\", then ask "
  "for help if it doesn't work.",
 welcome__msys_found =
  "Welcome to ENIGMA!\n"
  "This seems to be the first time you've run ENIGMA. I looked around, and I found "
  "what looks like an operable installation of GNU MSys Make, accessible from `make`. "
  "Does this belong to a valid and properly configured installation of MinGW?\n\n"
  "If you are unsure, select \"No\" and ENIGMA can install a new one for you.",
 install_drive_ok =
  "ENIGMA will install the GCC on the root of this drive by default. The assigned "
  "letter for this drive is \"%s\". Is this drive OK? (You can select a different "
  "drive by pressing \"no\".",
 mingw__already_installed_here = 
  "It seems that MinGW may already be installed. At least one key subdirectory of \\MinGW\\ "
  "on the selected drive was found to already exist. Would you like to continue anyway?\n\n"
  "Selecting \"yes\" will continue the installation, ignoring this warning.\n"
  "Selecting \"No\" will retry creating the directories (this is so you can uninstall the current MinGW and try again)\n"
  "Selecting \"Cancel\" will abort this installation, sending you back to the drive selection window",
 install_failure =
  "Installation failed. Please submit the feedback in the console window as a bug report on "
  "the forums. You can copy by right clicking the console and choosing \"Mark\", then using "
  "ENTER to copy.",
 java_not_found =
  "Could not find Java.exe. Please install Sun's Java runtime environment so LGM can run.\r\n"
  "http://www.java.com/en/download/manual.jsp\r\n\r\n"
  "If you already have Java, and believe you have received this message in error, you could "
  "try adding it to your system PATH variable.";

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
  dl[2] = '/';
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
bool e_use_existing_install(const char* mpath, const char* make, const char *auxpath);

const char* const CONFIG_FILE = "Compilers\\Windows\\gcc.ey";

int main()
{
  char drive_letter[44] = "\\";
  GetCurrentDirectory(44,drive_letter);
  
  puts(drive_letter);
  return 0;
  
  /* Check if we've already installed. **/
  puts("Checking configuration");
  FILE *ey = fopen(CONFIG_FILE, "rb"); // The GCC.ey file does not exist until installation has finished, be it manually or by this installer.
  if (!ey)
  {
    /* No Compiler descriptor was found. Start probing around. */
    puts("First time run. Scouring for Make...");
    
    const char *mpath = "make", *msmpath = "make";
    int a = better_system(mpath, "--version");
    if (a) a = better_system(mpath = "mingw32-make", "--version");
    if (a) a = better_system(mpath = "\\MinGW\\bin\\make.exe", "--version");
    if (a) a = better_system(mpath = "\\MinGW\\bin\\mingw32-make.exe", "--version");
    if (a) a = better_system(mpath = "C:\\MinGW\\bin\\mingw32-make.exe", "--version");
    if (a) a = better_system(mpath = "C:\\MinGW\\bin\\make.exe", "--version");
    
    if (a) // If we didn't find it
    {
      if (MessageBox(NULL, welcome__gnu_not_found, welcome__caption, MB_YESNO) == IDYES)
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
      printf("MinGW32 Make detected. Accessible from `%s`.\nVerifying MSys...\n\n", mpath);
      
      if (string("make") == mpath)
      {
        if (MessageBox(NULL, welcome__msys_found, welcome__caption, MB_YESNO) == IDYES)
      }
      
      string msg = expand_message(welcome__gnu_found, mpath); 
      if (MessageBox(NULL, msg.c_str(), welcome__caption, MB_YESNO) == IDYES)
        e_use_existing_install(dirpart(mpath).c_str(), filepart(mpath).c_str());
      else
        goto install_mingw;
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
      MessageBox(NULL, welcome__gnu_not_found, "Java Problem", MB_OK);
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
  
  // Warn the user if MinGW was already installed
  bool potentialError = false;
  
  install_begin:
  puts("* Installing MinGW-Get.");
    puts("   * Creating Directories");
      if (!CreateDirectory((dl + "MinGW\\bin\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW binary directory. Abort.");
      if (!CreateDirectory((dl + "MinGW\\libexec\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW libexec directory. Abort.");
      if (!CreateDirectory((dl + "MinGW\\libexec\\mingw-get\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get LibExec directory. Abort.");
      if (!CreateDirectory((dl + "MinGW\\var\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get var directory. Abort.");
      if (!CreateDirectory((dl + "MinGW\\var\\lib\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get lib directory. Abort.");
      if (!CreateDirectory((dl + "MinGW\\var\\lib\\mingw-get\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get lib subdirectory. Abort.");
      if (!CreateDirectory((dl + "MinGW\\var\\lib\\mingw-get\\data\\").c_str(), NULL) or_toggle_potential_error())
        ierror("Failed to create MinGW-Get data directory. Abort.");
    
    if (potentialError)
      switch (MessageBox(NULL,mingw__already_installed_here, "Warning", MB_YESNOCANCEL))
      {
        case IDYES:    break;
        case IDNO:     goto install_begin;
        case IDCANCEL: return FALSE;
      }
    
    puts("   * Copying Files");
    string mget = dl + "MinGW\\bin\\mingw-get.exe";
      if (!CopyFile2("Autoconf\\mingw-get\\bin\\mingw-get.exe", mget.c_str()))
        ierror("Failed to copy MinGW-Get's executable! (error code %d)",(int)GetLastError());
      if (!CopyFile2("Autoconf\\mingw-get\\libexec\\mingw-get\\lastrites.exe", (dl + "MinGW\\libexec\\mingw-get\\lastrites.exe").c_str()))
        ierror("Failed to copy MinGW-Get's lastrites! (error code %d)",(int)GetLastError());
      if (!CopyFile2("Autoconf\\mingw-get\\libexec\\mingw-get\\mingw-get-0.dll", (dl + "MinGW\\libexec\\mingw-get\\mingw-get-0.dll").c_str()))
        ierror("Failed to copy MinGW-Get's DLL! (error code %d)",(int)GetLastError());
      if (!CopyFile2("Autoconf\\mingw-get\\var\\lib\\mingw-get\\data\\defaults.xml", (dl + "MinGW\\var\\lib\\mingw-get\\data\\defaults.xml").c_str()))
        ierror("Failed to copy MinGW-Get's defaults! (error code %d)",(int)GetLastError());
    
    int install_p = 0;
    puts("   * Calling MinGW-Get");
      install_p = better_system(mget.c_str(), "install mingw32-make gcc g++ gdb msys-base msys-bash msys-make");
    if (install_p) {
      char m[512]; sprintf(m,"MinGW-Get failed! This application is outside the control of the ENIGMA team and so most likely"
                             "failed due to a change in package names. Please report this incident on ENIGMA's forums.\n\n"
                             "mingw-get error code: %d",install_p);
      MessageBox (NULL,m,"WARNING",MB_OK);
    }
    else puts("All requested components were installed correctly.");
  
    e_use_existing_install((dl + "MinGW\msys\1.0\bin").c_str(), "mingw32-make.exe", (dl + "MinGW\msys\1.0\bin; " + dl + "MinGW\\bin\\").c_str());
  return TRUE;
}

bool e_use_existing_install(const char* mpath, const char* make, const char *auxpath)
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
            "Path: %s\n"
            "Make: %s\n"
            "AuxPath: %s\n",
            "\n",
            mpath, make, auxpath);
    fclose(cff);
  }
  return TRUE;
}

