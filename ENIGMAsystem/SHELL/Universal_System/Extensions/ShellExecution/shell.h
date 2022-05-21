#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFmain.h"

#if defined(_WIN32)
#define PROCID unsigned long
#define LOCALPROCID unsigned long
#else
#define PROCID int
#define LOCALPROCID int
#endif

#define PROCLIST   int
#define PROCINFO   int
#define KINFOFLAGS int

namespace enigma_user {

// execute process from the shell, return process id
LOCALPROCID ProcessExecute(std::string command);

// execute process from the shell async, return process id
LOCALPROCID ProcessExecuteAsync(std::string command);

// embed window identifier into parent window identifier
void WindowIdSetParentWindowId(wid_t windowId, wid_t parentWindowId);

// update embed area of window identifier to fill parent window identifier client area
void WindowIdFillParentWindowId(wid_t windowId, wid_t parentWindowId);

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

// suspend process based on process id, return whether succeeded
bool ProcIdSuspend(PROCID procId);

// resume process based on process id, return whether succeeded
bool ProcIdResume(PROCID procId);

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

// get specific process info from process id
PROCINFO ProcInfoFromProcIdEx(PROCID procId, KINFOFLAGS kInfoFlags);

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

// get whether the environment variable of the given name exists
bool EnvironmentGetVariableExists(std::string name);

// set the environment variable with the given name and value
bool EnvironmentSetVariable(std::string name, std::string value);

// unset the environment variable with the given name
bool EnvironmentUnsetVariable(std::string name);

// get temporary directory path
std::string DirectoryGetTemporaryPath();

} // namespace enigma_user
