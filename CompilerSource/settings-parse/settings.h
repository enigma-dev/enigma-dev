#ifndef _SETTINGS_H
#define _SETTINGS_H
#include <string>
using std::string;

namespace extensions
{
  struct sdk_descriptor {
    string name, identifier, represents, description, author, build_platforms;
  };
  struct api_descriptor
  {
    string
      windowSys,   graphicsSys,   audioSys,   collisionSys,   widgetSys,   networkSys;
    string
      windowLinks, graphicsLinks, audioLinks, collisionLinks, widgetLinks, networkLinks;
  };
  struct compiler_descriptor {
    string identifier, resfile, buildext, buildname, runprog, runparam;
  };

  extern sdk_descriptor targetSDK;
  extern api_descriptor targetAPI;
  extern compiler_descriptor targetOS;
}
#endif
