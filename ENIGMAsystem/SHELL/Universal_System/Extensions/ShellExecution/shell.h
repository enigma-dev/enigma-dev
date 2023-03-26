#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFmain.h"

#ifndef PROCESS_GUIWINDOW_IMPL
#define PROCESS_GUIWINDOW_IMPL
#endif

#include "apiprocess/process.hpp"
#include "xprocess.hpp"

using ngs::ps::NGS_PROCID;
using namespace xprocess;

namespace enigma_user {

NGS_PROCID ProcessExecute(std::string command);
NGS_PROCID ProcessExecuteAsync(std::string command);
bool CompletionStatusFromExecutedProcess(NGS_PROCID procIndex);
ssize_t ExecutedProcessWriteToStandardInput(NGS_PROCID procIndex, std::string input);
std::string CurrentProcessReadFromStandardInput();
std::string ExecutedProcessReadFromStandardOutput(NGS_PROCID procIndex);
bool FreeExecutedProcessStandardInput(NGS_PROCID procIndex);
bool FreeExecutedProcessStandardOutput(NGS_PROCID procIndex);
NGS_PROCID ProcIdFromSelf();
NGS_PROCID ParentProcIdFromSelf();
NGS_PROCID ParentProcIdFromProcId(NGS_PROCID procId);
bool ProcIdExists(NGS_PROCID procId);
bool ProcIdSuspend(NGS_PROCID procId);
bool ProcIdResume(NGS_PROCID procId);
bool ProcIdKill(NGS_PROCID procId);
std::string ExecutableFromSelf();
std::string ExeFromProcId(NGS_PROCID procId);
std::string CwdFromProcId(NGS_PROCID procId);
PROCINFO ProcInfoFromProcId(NGS_PROCID procId);
PROCINFO ProcInfoFromProcIdEx(NGS_PROCID procId, KINFOFLAGS kInfoFlags);
void FreeProcInfo(PROCINFO procInfo);
PROCLIST ProcListCreate();
NGS_PROCID ProcessId(PROCLIST procList, int i);
int ProcessIdLength(PROCLIST procList);
void FreeProcList(PROCLIST procList);
std::string ExecutableImageFilePath(PROCINFO procInfo);
std::string CurrentWorkingDirectory(PROCINFO procInfo);
NGS_PROCID ParentProcessId(PROCINFO procInfo);
NGS_PROCID ChildProcessId(PROCINFO procInfo, int i);
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
