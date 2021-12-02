#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFmain.h"

namespace enigma_user {

ngs::proc::PROCINFO_SPECIFIC PROCINFO_ALLINFO = 1 << 0;
ngs::proc::PROCINFO_SPECIFIC PROCINFO_EXEFILE = 1 << 1;
ngs::proc::PROCINFO_SPECIFIC PROCINFO_CWDPATH = 1 << 2;
ngs::proc::PROCINFO_SPECIFIC PROCINFO_PPROCID = 1 << 3;
ngs::proc::PROCINFO_SPECIFIC PROCINFO_CPROCID = 1 << 4;
ngs::proc::PROCINFO_SPECIFIC PROCINFO_CMDLINE = 1 << 5;
ngs::proc::PROCINFO_SPECIFIC PROCINFO_ENVIRON = 1 << 6;
#if defined(PROCESS_GUIWINDOW_IMPL)
ngs::proc::PROCINFO_SPECIFIC PROCINFO_WINDOWS = 1 << 7;
#endif

// execute process from the shell, return process id
ngs::proc::LOCALPROCID ProcessExecute(std::string command);

// execute process from the shell async, return process id
ngs::proc::LOCALPROCID ProcessExecuteAsync(std::string command);

//  get whether executed process has quit based on process id
bool CompletionStatusFromExecutedProcess(ngs::proc::LOCALPROCID procIndex);

// write to executed process standard input file descriptor based on process id
void ExecutedProcessWriteToStandardInput(ngs::proc::LOCALPROCID procIndex, std::string input);

// read from current process standard input
std::string CurrentProcessReadFromStandardInput();

// read from executed process standard output file descriptor based on process id
std::string ExecutedProcessReadFromStandardOutput(ngs::proc::LOCALPROCID procIndex);

// free executed process standard input string based on process id
void FreeExecutedProcessStandardInput(ngs::proc::LOCALPROCID procIndex);

// free executed process standard ouptut string based on process id
void FreeExecutedProcessStandardOutput(ngs::proc::LOCALPROCID procIndex);

// get process id from self
ngs::proc::PROCID ProcIdFromSelf();

// get parent process id from self
ngs::proc::PROCID ParentProcIdFromSelf();

// get whether process exists based on process id
bool ProcIdExists(ngs::proc::PROCID procId);

// kill process based on process id, return whether succeeded
bool ProcIdKill(ngs::proc::PROCID procId);

// get executable image file path from self
std::string ExecutableFromSelf();

// get executable image file path from process id
std::string ExeFromProcId(ngs::proc::PROCID procId);

// get current working directory from process id
std::string CwdFromProcId(ngs::proc::PROCID procId);

// get process info from process id
ngs::proc::PROCINFO ProcInfoFromProcId(ngs::proc::PROCID procId, ngs::proc::PROCINFO_SPECIFIC specifics);

// free process info data from memory
void FreeProcInfo(ngs::proc::PROCINFO procInfo);

// create a list of all process id's
ngs::proc::PROCLIST ProcListCreate();

// get process id from process list at index
ngs::proc::PROCID ProcessId(ngs::proc::PROCLIST procList, int i);

// get amount of process id's in process list
int ProcessIdLength(ngs::proc::PROCLIST procList);

// free list of process id's from memory
void FreeProcList(ngs::proc::PROCLIST procList);

// get executable image file path from process info data
std::string ExecutableImageFilePath(ngs::proc::PROCINFO procInfo);

// get current working directory ffrom process info data
std::string CurrentWorkingDirectory(ngs::proc::PROCINFO procInfo);

// get parent processs id from process info data
ngs::proc::PROCID ParentProcessId(ngs::proc::PROCINFO procInfo);

// get child process id from process info data at index
ngs::proc::PROCID ChildProcessId(ngs::proc::PROCINFO procInfo, int i);

// get amount of child processes from process info data
int ChildProcessIdLength(ngs::proc::PROCINFO procInfo);

// get command line argument from process info data at index
std::string CommandLine(ngs::proc::PROCINFO procInfo, int i);

// get amount of command line arguments from process info data
int CommandLineLength(ngs::proc::PROCINFO procInfo);

// get environment variable (NAME=VALUE) from process info at index
std::string Environment(ngs::proc::PROCINFO procInfo, int i);

// get amount of anvironment variables from process info at index
int EnvironmentLength(ngs::proc::PROCINFO procInfo);

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
