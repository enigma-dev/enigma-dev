#include "EnigmaCallbacks.hpp"

ProgressMessage CallBack::progressMessage;
std::vector<LogMessage> CallBack::logMessages;
std::vector<LogMessage>::iterator CallBack::logIt;
std::mutex CallBack::logMutex;

CallBack::CallBack()
{
  dia_open = &CallBack::FrameOpen;
  dia_add = &CallBack::AppendFrame;
  dia_clear = &CallBack::ClearFrame;
  dia_progress = &CallBack::SetProgress;
  dia_progress_text = &CallBack::SetProgressText;
  output_redirect_file = &CallBack::SetOutFile;
  output_redirect_reset = &CallBack::ResetRedirect;
  ide_execute = &CallBack::Execute;
  ide_compress_data = &CallBack::CompressImage;
}

const ProgressMessage& CallBack::GetProgress() const {
  return progressMessage;
}

const LogMessage& CallBack::GetFirstLogMessage(bool &end) const {
  if (logMessages.empty()) { end = true; return LogMessage(); }
  end = false;
  logMutex.lock();
  logIt = logMessages.begin();
  return *logIt;
}

const LogMessage& CallBack::GetNextLogMessage(bool &end) const {
  ++logIt;
  if (logIt == logMessages.end()) {
    end = true;
    logMutex.unlock();
    return LogMessage();
  } else {
    end = false;
    const LogMessage& ret = *logIt;
    return ret;
  }
}

void CallBack::ClearLogMessages() {
  logMutex.lock();
  logMessages.clear();
  logMutex.unlock();
}

void CallBack::FrameOpen()
{
}

void CallBack::AppendFrame(const char* text)
{
  LogMessage msg;

  msg.set_severity(LogMessage::FINE);
  msg.set_message(text);

  logMutex.lock();
  logMessages.emplace_back(msg);
  logMutex.unlock();
}

void CallBack::ClearFrame()
{
}

void CallBack::SetProgress(int progress)
{
  progressMessage.set_progress(progress);
}

void CallBack::SetProgressText(const char* text)
{
  progressMessage.set_message(text);
}

void CallBack::SetOutFile(const char* /*file*/)
{
}

void CallBack::ResetRedirect()
{
}

int CallBack::Execute(const char*, const char**, bool)
{
  return 0;
}

Image* CallBack::CompressImage(char *, int)
{
  return nullptr;
}
