#ifndef EMAKE_ENIGMACALLBACKS_HPP
#define EMAKE_ENIGMACALLBACKS_HPP

//This
#include "backend/JavaCallbacks.h"
//Should be:
//#include "backend/EnigmaCallbacks.h"

#include <fstream>

class CallBack : public EnigmaCallbacks
{
public:
  CallBack();

  static void FrameOpen();
  static void AppendFrame(const char*);
  static void ClearFrame();
  static void SetProgress(int);
  static void SetProgressText(const char*);
  static void SetOutFile(const char*);
  static void ResetRedirect();
  static int Execute(const char*, const char**, bool);
  static Image* CompressImage(char *, int);
  static void* OutputThread(void*);
  static std::ifstream _outFile;
  static bool _isOutputting;
};

#endif
