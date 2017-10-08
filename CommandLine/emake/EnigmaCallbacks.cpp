#include "EnigmaCallbacks.hpp"

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

std::ifstream CallBack::_outFile;

void* CallBack::OutputThread(void*)
{
  if (_outFile.is_open()) {
    std::string line;
    while (true) {
        while (std::getline(_outFile, line)) std::cout << line << std::endl;
        if (!_outFile.eof()) break; // Ensure end of read was EOF.
        _outFile.clear();
    }
  }

  return nullptr;
}

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
  _outFile.open(file);
  pthread_t me;
  pthread_create(&me, nullptr, &OutputThread, nullptr);
}

void CallBack::ResetRedirect()
{
  _outFile.close();
}

int CallBack::Execute(const char*, const char**, bool)
{
  return 0;
}

Image* CallBack::CompressImage(char *, int)
{
  return nullptr;
}
