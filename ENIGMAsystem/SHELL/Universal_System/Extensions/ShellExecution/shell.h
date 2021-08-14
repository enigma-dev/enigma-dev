#include "Platforms/General/PFmain.h"
#include "Platforms/platforms_mandatory.h"

namespace enigma_user {

using CrossProcess::PROCID;
using CrossProcess::PROCESS;
using CrossProcess::PROCINFO;
using CrossProcess::PROCLIST;
using CrossProcess::WINDOWID;
using CrossProcess::WINDOW;

// execute process from the shell, return process id
PROCESS ProcessExecute(std::string command);

// execute process from the shell async, return process id
PROCESS ProcessExecuteAsync(std::string command);

//  get whether executed process has quit based on process id
bool CompletionStatusFromExecutedProcess(PROCESS procIndex);

// write to executed process standard input file descriptor based on process id
void ExecutedProcessWriteToStandardInput(PROCESS procIndex, std::string input);

// read from executed process standard output file descriptor based on process id
std::string ExecutedProcessReadFromStandardOutput(PROCESS procIndex);

// free executed process standard input string based on process id
void FreeExecutedProcessStandardInput(PROCESS procIndex);

// free executed process standard ouptut string based on process id
void FreeExecutedProcessStandardOutput(PROCESS procIndex);

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

// get owned window id string from process info at index
wid_t OwnedWindowId(PROCINFO procInfo, int i);

// get amount of owned window id's from process info at index
int OwnedWindowIdLength(PROCINFO procInfo);

// get whether a process exists based on one of its window id's
bool WindowIdExists(wid_t winId);

// kill a process based on one of its window id's, return whether succeeded
bool WindowIdKill(wid_t winId);

// return a window id from native window handle
wid_t WindowIdFromNativeWindow(window_t window); 

} // namespace enigma_user
