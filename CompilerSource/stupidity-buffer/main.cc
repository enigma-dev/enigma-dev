/** Copyright (C) 2013 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
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

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int better_system(const char* jname, const char* param, const char *direc = NULL)
{
  DWORD exit_status;

  if (direc)
    SetCurrentDirectory(direc);

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
 java_not_found =
  "Could not find Java.exe. Please install Sun's Java runtime environment.\r\n"
  "http://www.java.com/en/download/manual.jsp\r\n\r\n"
  "If you already have Java, and believe you have received this message in error, you could "
  "try adding it to your system PATH variable.";

#define fixFont(hwnd) SendMessage(hwnd,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);

char drive_letter[4] = { '\\', 0, 0, 0 };
HWND dlb = NULL, cbb = NULL;

typedef vector<string> CommandLineStringArgs;

void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}

string exepath;
string settingspath;

void output_error(const char* msg) {
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    //First save the current color information
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;

    // Set the new color information
    SetConsoleTextAttribute ( h, FOREGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY );

    cout << "ERROR! ";
	// Restore the original colors
    SetConsoleTextAttribute ( h, wOldColorAttrs);
	cout << msg << "\n";
}

void output_warning(const char* msg) {
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    //First save the current color information
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;

    // Set the new color information
    SetConsoleTextAttribute ( h, FOREGROUND_RED+FOREGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY );

    cout << "WARNING! ";
	// Restore the original colors
    SetConsoleTextAttribute ( h, wOldColorAttrs);
	cout << msg << "\n";
}

#include <ctime>

// Get current date/time, format is MM/DD/YYYY HH:MM:SS
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%m/%d/%Y %X", &tstruct);
    return buf;
}

void install_updates() {

	WritePrivateProfileString("MAIN", "currentversion", "\"0.0.0.0\"", settingspath.c_str());
	string datetime = "\"" + currentDateTime() + "\"";
	WritePrivateProfileString("MAIN", "lastupdated", datetime.c_str(), settingspath.c_str());
}

void show_update_change_log() {

}

void ask_for_updates() {
  char c;
  cin >> c;
  c = tolower(c);

  if (c == 'y') {
    install_updates();
  } else if (c == 'n') {
    return;
  } else if (c == 'g') {
    show_update_change_log();
	ask_for_updates();
  } else if (c == 'a') {
    WritePrivateProfileString("MAIN", "checkforupdates", "0", settingspath.c_str());
  } else {
    puts("Please enter [Y], [N], [G], or [A]");
    ask_for_updates();
  }
  return;
}

void check_for_updates() {
  bool updatesavailable = false;
  if (updatesavailable) {
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    //First save the current color information
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;

    // Set the new color information
    SetConsoleTextAttribute ( h, FOREGROUND_BLUE+FOREGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY );

	char currentversion[256], lastupdated[256];
	GetPrivateProfileString("MAIN", "lastupdated", "Never", lastupdated, 256, settingspath.c_str());
	GetPrivateProfileString("MAIN", "currentversion", "0.0.0.0", currentversion, 256, settingspath.c_str());

    puts("*** Updates Available ***");

    // Restore the original colors
    SetConsoleTextAttribute ( h, wOldColorAttrs);

	string lvtxt = "Latest Version: ";
	puts(lvtxt.c_str());
	string cvtxt = "Current Version: " + string(currentversion);
	puts(cvtxt.c_str());
	string lutxt = "Last Updated: " + string(lastupdated) + "\n";
	puts(lutxt.c_str());

    puts("Would you like to install them?");
    puts("[Y] Yes");
    puts("[N] No");
    puts("[G] See change log");
    puts("[A] Never ask again for updates");
    ask_for_updates();
  }

  return;
}

#include <unistd.h>

int main(int argc, char *argv[])
{
  //if setup script exists; run it then create flag file called "compiled"
  CommandLineStringArgs cmdlineStringArgs(&argv[0], &argv[0 + argc]);

  std::string path = cmdlineStringArgs[0];

  myReplace(path, "\\", "/");
  size_t pos = path.find_last_of("/");
  exepath.assign(path, 0, pos + 1);
  settingspath = exepath + "settings.ini";

  // set the console window title
  system("title ENIGMA Development Environment");
  system("Color 1F");
  puts("Copyright (C) 2013 The ENIGMA Team\n");

  bool checkforupdates = GetPrivateProfileInt("MAIN", "checkforupdates", 1, settingspath.c_str());
  if (checkforupdates) { check_for_updates(); }

  bool checkforjava = GetPrivateProfileInt("MAIN", "checkforjava", 1, settingspath.c_str());
  bool redirectoutput = GetPrivateProfileInt("MAIN", "redirectoutput", 1, settingspath.c_str());

  if (checkforjava) {
	// Ensure that Java is installed
	const char *jpath = "java";

	puts("Checking Java Installation");
	{
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
		if (a)
		{
			output_error(java_not_found);
			if (redirectoutput) {
			  MessageBox(NULL, java_not_found , "Java Runtime Environment", MB_OK|MB_ICONERROR);
			}
			system("pause");
			return 0;
		}
	}
  }

  string setuppath = exepath + "setup";
  string zippath = exepath + "WinPatch.zip";

  bool setupcompleted = GetPrivateProfileInt("MAIN", "setupcompleted", 0, settingspath.c_str());
  bool skippedsetup = GetPrivateProfileInt("MAIN", "skippedsetup", 0, settingspath.c_str());

  if (!skippedsetup && !setupcompleted && INVALID_FILE_ATTRIBUTES == GetFileAttributes(setuppath.c_str()) && GetLastError()== ERROR_FILE_NOT_FOUND)  //If setup script not found
  {
	  output_error("Setup script not found.");
	  bool launchanyway = false;
	  if (redirectoutput) {
		launchanyway = (IDYES == MessageBox(NULL, "Setup script not found. Launch anyway? Useful in cases when compiling from source.", "Error", MB_YESNO|MB_ICONERROR));
	  } else {
	    puts("Launch anyway? Useful in cases when compiling from source. [y/n]");
		char c;
		do{
		  cin >> c;
          c = tolower(c);
		}while( !cin.fail() && c!='y' && c!='n' );
		launchanyway = (c == 'y');
	  }

      if (launchanyway){
          WritePrivateProfileString("MAIN", "skippedsetup", "1", settingspath.c_str());
      } else {
		  if (!redirectoutput) {
            system("pause");
		  }
          return -1;
      }
  }
  else if (!setupcompleted && !skippedsetup)  // make sure not already compiled
  {
    puts("Downloading and Compiling Binaries, please wait...");

	DWORD exit_status;
	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter

	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO); //Only compulsory field

	string bashpath = exepath + "git-bash.bat";
	string cmdline = "\"" + bashpath + "\" \"" + setuppath + "\"";

	if (CreateProcess(NULL,(char *)cmdline.c_str(),NULL,NULL,
    FALSE,0,NULL,NULL,&StartupInfo,&ProcessInfo)) {
	    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
		GetExitCodeProcess(ProcessInfo.hProcess, &exit_status);
		CloseHandle(ProcessInfo.hProcess);
		CloseHandle(ProcessInfo.hThread);

		WritePrivateProfileString("MAIN", "setupcompleted", "1", settingspath.c_str());
		string datetime = "\"" + currentDateTime() + "\"";
		WritePrivateProfileString("MAIN", "dateinstalled", datetime.c_str(), settingspath.c_str());

		bool cleanupsetup = GetPrivateProfileInt("MAIN", "cleanupsetup", 1, settingspath.c_str());
		if (cleanupsetup) {
		  DeleteFile(setuppath.c_str());
		  DeleteFile(zippath.c_str());
		}
		// everythings good now just continue on down below and load lgm
	} else {
		puts("ERROR: Failed to create process for obtaining binaries.");
		system("pause");
		return -1;
	}
  }

  //Set Environment Path
  puts("Setting Environment Path");
  string envpath = getenv("PATH");
  string fullpath = string("PATH=") + exepath + "mingw32/bin;" + exepath + "git/bin;" + envpath;

  putenv(fullpath.c_str());
  puts(fullpath.c_str());

  PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
  STARTUPINFO StartupInfo; //This is an [in] parameter

  //Set Working Directory
  string workpath = exepath + "enigma-dev/"; //Test if subdirectory exists, if it doesn't, then assume exe is in it
  DWORD ftyp = GetFileAttributesA(workpath.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY))
  {
      workpath = exepath;
  }
  
  string output = "Setting Working Directory To: \"" + workpath + "\"";
    string cmdline = "cd \"" + workpath + "\"";
	CreateProcess(NULL,(char *)cmdline.c_str(),NULL,NULL,
    TRUE,CREATE_NO_WINDOW,NULL,NULL,&StartupInfo,&ProcessInfo);
  puts(output.c_str());
  chdir(workpath.c_str());

  //arguments
  string argsasstring = "";
  if (argc > 1) {
	myReplace(cmdlineStringArgs[1], "\\", "/");
	argsasstring += " \"" + cmdlineStringArgs[1] + "\"";
  } else {
	argsasstring += " \"\"";
  }

  //Run Lateral GM
  char idename[256], idecmd[256];
  GetPrivateProfileString("MAIN", "idename", "lateralgm.jar", idename, 256, settingspath.c_str());
  GetPrivateProfileString("MAIN", "idecommand", "java -jar", idecmd, 256, settingspath.c_str());

  string idepath = " \"" + workpath + string(idename) + "\"";
  cmdline = string(idecmd) + idepath;

  cmdline += argsasstring;
  puts(cmdline.c_str());
  SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

  HANDLE h = CreateFile("output_log.txt",
    FILE_WRITE_DATA,
    FILE_SHARE_WRITE | FILE_SHARE_READ,
    &sa,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL );

  ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
  StartupInfo.cb = sizeof(STARTUPINFO); //Only compulsory field
  if (redirectoutput) {
    StartupInfo.dwFlags |= STARTF_USESTDHANDLES;
    StartupInfo.hStdInput = h;
    StartupInfo.hStdError = h;
    StartupInfo.hStdOutput = h;
  }

  DWORD flags = NULL;

  if (redirectoutput) {
	flags += CREATE_NO_WINDOW;
  }

  CreateProcess(NULL,(char *)cmdline.c_str(),NULL,NULL,
    TRUE,flags,NULL,NULL,&StartupInfo,&ProcessInfo);

  bool closecmd = GetPrivateProfileInt("MAIN", "closecmd", 1, settingspath.c_str());
  if (!closecmd)
      system("pause");

  return 0;
}
