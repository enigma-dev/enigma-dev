#include "EnigmaCallbacks.hpp"

#include <iostream>
#include <pthread.h>
#include <unistd.h>

bool CallBack::_isOutputting = false;
std::ifstream CallBack::_outFile;

void* CallBack::OutputThread(void*)
{
    if (_outFile)
    {
        _outFile.seekg (0, _outFile.end);

        int length = _outFile.tellg();

        _outFile.seekg (0, _outFile.beg);

        char * buffer = new char [length];

        _outFile.read (buffer,length);

        std::cout << buffer;

        _outFile.close();

        delete[] buffer;
    }

    return 0;
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
}

void CallBack::ResetRedirect()
{
    //FIXME: only outputs when done...

    //_isOutputting = true;

    pthread_t me;
    pthread_create(&me, NULL, &OutputThread, NULL);

    //_isOutputting = false;
}

int CallBack::Execute(const char*, const char**, bool)
{
    return 0;
}

Image* CallBack::CompressImage(char *, int)
{
    return nullptr;
}
