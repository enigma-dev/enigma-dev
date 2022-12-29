#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFmain.h"

#if defined(__MACH__) && defined(__APPLE__)
#ifndef PROCESS_GUIWINDOW_IMPL
#define PROCESS_GUIWINDOW_IMPL
#endif
#endif

#include "cproc/cproc.hpp"

using namespace ngs::cproc;

namespace enigma_user {

CPROCID ProcessExecute(std::string command);
CPROCID ProcessExecuteAsync(std::string command);
bool CompletionStatusFromExecutedProcess(CPROCID procIndex);
void ExecutedProcessWriteToStandardInput(CPROCID procIndex, std::string input);
std::string CurrentProcessReadFromStandardInput();
std::string ExecutedProcessReadFromStandardOutput(CPROCID procIndex);
void FreeExecutedProcessStandardInput(CPROCID procIndex);
void FreeExecutedProcessStandardOutput(CPROCID procIndex);
XPROCID ProcIdFromSelf();
XPROCID ParentProcIdFromSelf();
bool ProcIdExists(XPROCID procId);
bool ProcIdSuspend(XPROCID procId);
bool ProcIdResume(XPROCID procId);
bool ProcIdKill(XPROCID procId);
std::string ExecutableFromSelf();
std::string ExeFromProcId(XPROCID procId);
std::string CwdFromProcId(XPROCID procId);
PROCINFO ProcInfoFromProcId(XPROCID procId);
PROCINFO ProcInfoFromProcIdEx(XPROCID procId, KINFOFLAGS kInfoFlags);
void FreeProcInfo(PROCINFO procInfo);
PROCLIST ProcListCreate();
XPROCID ProcessId(PROCLIST procList, int i);
int ProcessIdLength(PROCLIST procList);
void FreeProcList(PROCLIST procList);
std::string ExecutableImageFilePath(PROCINFO procInfo);
std::string CurrentWorkingDirectory(PROCINFO procInfo);
XPROCID ParentProcessId(PROCINFO procInfo);
XPROCID ChildProcessId(PROCINFO procInfo, int i);
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
WINDOWID WindowIdFromNativeWindow(WINDOW window);
WINDOW NativeWindowFromWindowId(WINDOWID winid);
bool WindowIdExists(WINDOWID winId);
bool WindowIdSuspend(WINDOWID winId);
bool WindowIdResume(WINDOWID winId);
bool WindowidKill(WINDOWID winId);
WINDOWID OwnedWindowId(PROCINFO procInfo, int i);
int OwnedWindowIdLength(PROCINFO procInfo);

} // namespace enigma_user
