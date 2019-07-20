#ifndef EMAKE_ENIGMACALLBACKS_HPP
#define EMAKE_ENIGMACALLBACKS_HPP

//This
#include "backend/JavaCallbacks.h"
//Should be:
//#include "backend/EnigmaCallbacks.h"

#include "compiler.pb.h"

#include <fstream>
#include <string>
#include <vector>
#include <mutex>

using ProgressMessage = buffers::ProgressMessage;
using LogMessage = buffers::LogMessage;

class CallBack : public EnigmaCallbacks
{
public:
  CallBack();
  const ProgressMessage& GetProgress() const;
  const LogMessage GetFirstLogMessage(bool &end) const;
  const LogMessage GetNextLogMessage(bool &end) const;
  void ClearLogMessages();
  void ProcessOutput();

private:
  static ProgressMessage progressMessage;
  static std::vector<LogMessage> logMessages;
  static std::vector<LogMessage>::iterator logIt;
  static std::mutex logMutex;
  static std::ifstream outFile;

  static void FrameOpen();
  static void AppendFrame(const char*);
  static void ClearFrame();
  static void SetProgress(int);
  static void SetProgressText(const char*);
  static void SetOutFile(const char*);
  static void ResetRedirect();
  static int Execute(const char*, const char**, bool);
  static Image* CompressImage(char *, int);
};

#endif
