#ifndef PROCESS_GUIWINDOW_IMPL
#define PROCESS_GUIWINDOW_IMPL
#endif

#include <xprocess.hpp>

using namespace xprocess;

namespace enigma_user {

PROCID ProcessExecute(std::string command);
PROCID ProcessExecuteAsync(std::string command);
bool CompletionStatusFromExecutedProcess(PROCID procIndex);
long long ExecutedProcessWriteToStandardInput(PROCID procIndex, std::string input);
std::string CurrentProcessReadFromStandardInput();
std::string ExecutedProcessReadFromStandardOutput(PROCID procIndex);
void SetBufferLimitForStandardOutput(long long limit);
bool FreeExecutedProcessStandardInput(PROCID procIndex);
bool FreeExecutedProcessStandardOutput(PROCID procIndex);
PROCID ProcIdFromSelf();
PROCID ParentProcIdFromSelf();
PROCID ParentProcIdFromProcId(PROCID procId);
bool ProcIdExists(PROCID procId);
bool ProcIdSuspend(PROCID procId);
bool ProcIdResume(PROCID procId);
bool ProcIdKill(PROCID procId);
std::string ExecutableFromSelf();
std::string ExeFromProcId(PROCID procId);
std::string CwdFromProcId(PROCID procId);
PROCINFO ProcInfoFromProcId(PROCID procId);
PROCINFO ProcInfoFromProcIdEx(PROCID procId, KINFOFLAGS kInfoFlags);
void FreeProcInfo(PROCINFO procInfo);
PROCLIST ProcListCreate();
PROCID ProcessId(PROCLIST procList, int i);
int ProcessIdLength(PROCLIST procList);
void FreeProcList(PROCLIST procList);
std::string ExecutableImageFilePath(PROCINFO procInfo);
std::string CurrentWorkingDirectory(PROCINFO procInfo);
PROCID ParentProcessId(PROCINFO procInfo);
PROCID ChildProcessId(PROCINFO procInfo, int i);
int ChildProcessIdLength(PROCINFO procInfo);
std::string CommandLine(PROCINFO procInfo, int i);
int CommandLineLength(PROCINFO procInfo);
std::string Environment(PROCINFO procInfo, int i);
int EnvironmentLength(PROCINFO procInfo);
std::string DirectoryGetCurrentWorking();
bool DirectorySetCurrentWorking(std::string dname);
std::string EnvironmentGetVariable(std::string name);
bool EnvironmentGetVariableExists(std::string name);
bool EnvironmentSetVariable(std::string name, std::string value);
bool EnvironmentUnsetVariable(std::string name);
std::string DirectoryGetTemporaryPath();
char *WindowIdFromNativeWindow(void *window);
void *NativeWindowFromWindowId(char *winId);
bool WindowIdExists(char *winId);
bool WindowIdSuspend(char *winId);
bool WindowIdResume(char *winId);
bool WindowidKill(char *winId);
char *OwnedWindowId(PROCINFO procInfo, int i);
int OwnedWindowIdLength(PROCINFO procInfo);

} // namespace enigma_user
