#include "EnigmaCallbacks.hpp"

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

void CallBack::FrameOpen()
{
}

void CallBack::AppendFrame(const char*)
{
}

void CallBack::ClearFrame()
{
}

void CallBack::SetProgress(int)
{
}

void CallBack::SetProgressText(const char*)
{
}

void CallBack::SetOutFile(const char* file)
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
