#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFmain.h"

namespace enigma_user {

#if !defined(_WIN32)
typedef int PROCID;
#else
typedef unsigned long PROCID;
#endif
typedef PROCID LOCALPROCID;
#if defined(PROCESS_GUIWINDOW_IMPL)
#if defined(_WIN32)
typedef HWND WINDOW;
#elif (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
typedef CGWindowID WINDOW;
#elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__)) || defined(PROCESS_XQUARTZ_IMPL)
typedef Window WINDOW;
#endif
typedef char *WINDOWID;
#endif
typedef int PROCLIST;
typedef int PROCINFO;
typedef const unsigned char PROCINFO_SPECIFIC;
PROCINFO_SPECIFIC PROCINFO_ALLINFO = 1 << 0;
PROCINFO_SPECIFIC PROCINFO_EXEFILE = 1 << 1;
PROCINFO_SPECIFIC PROCINFO_CWDPATH = 1 << 2;
PROCINFO_SPECIFIC PROCINFO_PPROCID = 1 << 3;
PROCINFO_SPECIFIC PROCINFO_CPROCID = 1 << 4;
PROCINFO_SPECIFIC PROCINFO_CMDLINE = 1 << 5;
PROCINFO_SPECIFIC PROCINFO_ENVIRON = 1 << 6;
#if defined(PROCESS_GUIWINDOW_IMPL)
PROCINFO_SPECIFIC PROCINFO_WINDOWS = 1 << 7;
#endif
#if !defined(_MSC_VER)
#pragma pack(push, 8)
#else
#include <pshpack8.h>
#endif
typedef struct {
  char *executable_image_file_path;
  char *current_working_directory;
  PROCID parent_process_id;
  PROCID *child_process_id;
  int child_process_id_length;
  char **commandline;
  int commandline_length;
  char **environment;
  int environment_length;
  #if defined(PROCESS_GUIWINDOW_IMPL)
  WINDOWID *owned_window_id;
  int owned_window_id_length;
  #endif
} PROCINFO_STRUCT;
#if !defined(_MSC_VER)
#pragma pack(pop)
#else
#include <poppack.h>
#endif

// execute process from the shell, return process id
LOCALPROCID ProcessExecute(std::string command);

// execute process from the shell async, return process id
LOCALPROCID ProcessExecuteAsync(std::string command);

//  get whether executed process has quit based on process id
bool CompletionStatusFromExecutedProcess(LOCALPROCID procIndex);

// write to executed process standard input file descriptor based on process id
void ExecutedProcessWriteToStandardInput(LOCALPROCID procIndex, std::string input);

// read from current process standard input
std::string CurrentProcessReadFromStandardInput();

// read from executed process standard output file descriptor based on process id
std::string ExecutedProcessReadFromStandardOutput(LOCALPROCID procIndex);

// free executed process standard input string based on process id
void FreeExecutedProcessStandardInput(LOCALPROCID procIndex);

// free executed process standard ouptut string based on process id
void FreeExecutedProcessStandardOutput(LOCALPROCID procIndex);

// get process id from self
PROCID ProcIdFromSelf();

// get parent process id from self
PROCID ParentProcIdFromSelf();

// get whether process exists based on process id
bool ProcIdExists(PROCID procId);

// kill process based on process id, return whether succeeded
bool ProcIdKill(PROCID procId);

// get executable image file path from self
std::string ExecutableFromSelf();

// get executable image file path from process id
std::string ExeFromProcId(PROCID procId);

// get current working directory from process id
std::string CwdFromProcId(PROCID procId);

// get process info from process id
PROCINFO ProcInfoFromProcId(PROCID procId);

// free process info data from memory
void FreeProcInfo(PROCINFO procInfo);

// create a list of all process id's
PROCLIST ProcListCreate();

// get process id from process list at index
PROCID ProcessId(PROCLIST procList, int i);

// get amount of process id's in process list
int ProcessIdLength(PROCLIST procList);

// free list of process id's from memory
void FreeProcList(PROCLIST procList);

// get executable image file path from process info data
std::string ExecutableImageFilePath(PROCINFO procInfo);

// get current working directory ffrom process info data
std::string CurrentWorkingDirectory(PROCINFO procInfo);

// get parent processs id from process info data
PROCID ParentProcessId(PROCINFO procInfo);

// get child process id from process info data at index
PROCID ChildProcessId(PROCINFO procInfo, int i);

// get amount of child processes from process info data
int ChildProcessIdLength(PROCINFO procInfo);

// get command line argument from process info data at index
std::string CommandLine(PROCINFO procInfo, int i);

// get amount of command line arguments from process info data
int CommandLineLength(PROCINFO procInfo);

// get environment variable (NAME=VALUE) from process info at index
std::string Environment(PROCINFO procInfo, int i);

// get amount of anvironment variables from process info at index
int EnvironmentLength(PROCINFO procInfo);

// get current working directory
std::string DirectoryGetCurrentWorking();

// set current working directory based on a given dname
bool DirectorySetCurrentWorking(std::string dname);

// get the environment variable of the given name
std::string EnvironmentGetVariable(std::string name);

// set the environment variable with the given name and value
bool EnvironmentSetVariable(std::string name, std::string value);

// unset the environment variable with the given name
bool EnvironmentUnsetVariable(std::string name);

// get temporary directory path
std::string DirectoryGetTemporaryPath();

} // namespace enigma_user
