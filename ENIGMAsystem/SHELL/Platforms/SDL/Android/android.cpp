#include "Platforms/General/PFdevice.h"
#include <SDL_android.h>

namespace enigma_user
{

float device_get_tilt_x()
{ 
  float Values[3],tilt_x;
  Android_JNI_GetAccelerometerValues(Values);
  tilt_x = Values[0];
  return tilt_x;
}

float device_get_tilt_y()
{
  float Values[3],tilt_y;
  Android_JNI_GetAccelerometerValues(Values);
  tilt_y = Values[1];
  return tilt_y;

}

float device_get_tilt_z()
{
  float Values[3],tilt_z;
  Android_JNI_GetAccelerometerValues(Values);
  tilt_z = Values[2];
  return tilt_z;

}

}
