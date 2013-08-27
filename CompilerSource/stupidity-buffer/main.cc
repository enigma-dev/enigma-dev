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
  "Could not find Java.exe. Please install Sun's Java runtime environment so LGM can run.\r\n"
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

#include <unistd.h>

int main(int argc, char *argv[])
{
  //if init script exists; run it then create flag file called "compiled"
  CommandLineStringArgs cmdlineStringArgs(&argv[0], &argv[0 + argc]);

  std::string path = cmdlineStringArgs[0];
  std::string exepath;

  myReplace(path, "\\", "/");
  size_t pos = path.find_last_of("/");
  exepath.assign(path, 0, pos + 1);

  string settingspath = exepath + "settings.ini";

  bool checkforjava = GetPrivateProfileInt("MAIN", "checkforjava", 1, settingspath.c_str());

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
			puts(java_not_found);
			MessageBox(NULL, java_not_found , "Java Problem", MB_OK);
			system("pause");
			return 0;
		}
	}
  }

  string initpath = exepath + "init";

  bool setupcompleted = GetPrivateProfileInt("MAIN", "setupcompleted", 0, settingspath.c_str());

  if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(initpath.c_str()) && GetLastError()== ERROR_FILE_NOT_FOUND)  //If init script not found
  {
      puts("ERROR: Initialization script not found.");
	  system("pause");
	  return -1;
  }
  else if (!setupcompleted)  // make sure not already compiled
  {
    puts("Downloading and Compiling Binaries, please wait...");

	DWORD exit_status;
	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter

	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO); //Only compulsory field

	string bashpath = exepath + "git-bash.bat";
	string cmdline = bashpath + " " + initpath;

	if (CreateProcess(NULL,(char *)cmdline.c_str(),NULL,NULL,
    FALSE,0,NULL,NULL,&StartupInfo,&ProcessInfo)) {
	    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
		GetExitCodeProcess(ProcessInfo.hProcess, &exit_status);
		CloseHandle(ProcessInfo.hProcess);
		CloseHandle(ProcessInfo.hThread);

		//create empty file to detect for initialization being successfull
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		WritePrivateProfileString("MAIN", "setupcompleted", "1", settingspath.c_str());
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
  string fullpath = string("PATH=") + exepath + "mingw32/bin;" + envpath;

  putenv(fullpath.c_str());
  puts(fullpath.c_str());

  PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
  STARTUPINFO StartupInfo; //This is an [in] parameter

  //Set Working Directory
  string workpath = exepath + "enigma-dev";
  string output = "Setting Working Directory To:" + workpath;
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
  GetPrivateProfileString("MAIN", "idecommand", "java -jar ", idecmd, 256, settingspath.c_str());

  string idepath = "\"" + exepath + "enigma-dev/" + string(idename) + "\"";
  cmdline = string(idecmd) + idepath;


  cmdline += argsasstring;
  puts(cmdline.c_str());
  SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

  HANDLE h = CreateFile("output_log.txt",
    FILE_APPEND_DATA,
    FILE_SHARE_WRITE | FILE_SHARE_READ,
    &sa,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL );

  bool redirectoutput = GetPrivateProfileInt("MAIN", "redirectoutput", 1, settingspath.c_str());

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

  system("pause");
  return 0;
}
