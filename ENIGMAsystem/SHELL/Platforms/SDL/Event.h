#ifndef E_SDL_EVENT_HANDLER_H
#define E_SDL_EVENT_HANDLER_H

#include <SDL2/SDL.h>

namespace enigma {

class SDL_Event_Handler {
 public:
  SDL_Event_Handler();
  int processEvents();

 protected:
  void windowEvent(const SDL_Event *event);
  
  void windowResized(const SDL_Event *event);
  void windowFocusGain(const SDL_Event *event);
  void windowFocusLost(const SDL_Event *event);
  /*void unkownEvent(const SDL_Event *event) {}
  void windowShown(const SDL_Event *event) {}
  void windowHidden(const SDL_Event *event) {}
  void windowExposed(const SDL_Event *event) {}
  void windowMoved(const SDL_Event *event) {}
  void windowMinimized(const SDL_Event *event) {}
  void windowMaximized(const SDL_Event *event) {}
  void windowRestored(const SDL_Event *event) {}
  void windowEntered(const SDL_Event *event) {}
  void windowLeave(const SDL_Event *event) {}
  void windowClose(const SDL_Event *event) {}
  void windowTakeFocus(const SDL_Event *event) {}
  void windowHitTest(const SDL_Event *event) {}
  
  void mouseMotion(const SDL_Event *event) {}
  
  void joystickAxisMotion(const SDL_Event *event) {}
  void joyBallAxisMotion(const SDL_Event *event) {}
  void joyHatMotion(const SDL_Event *event) {}
  void joyButtonDown(const SDL_Event *event) {}
  void joyButtonUp(const SDL_Event *event) {}*/
  void joyDeviceAdded(const SDL_Event *event);
  void joyDeviceRemoved(const SDL_Event *event);
  
  void controllerButtonDown(const SDL_Event *event);
  void controllerButtonUp(const SDL_Event *event);
  /*void controllerAxisMotion(const SDL_Event *event) {}
  void controllerButtonDown(const SDL_Event *event) {}
  void controllerButtonUp(const SDL_Event *event) {}
  void controllerDeviceAdded(const SDL_Event *event) {}
  void controllerDeviceRemoved(const SDL_Event *event) {}
  void controllerDeviceRemmaped(const SDL_Event *event) {}
  
  void audioDeviceAdded(const SDL_Event *event) {}
  void audioDeviceRemoved(const SDL_Event *event) {}
  
  void fingerMotion(const SDL_Event *event) {}
  void fingerDown(const SDL_Event *event) {}
  void fingerUp(const SDL_Event *event) {}
  void multigesture(const SDL_Event *event) {}
  void dollarGesture(const SDL_Event *event) {}
  void dollarRecord(const SDL_Event *event) {}
  
  void dropFile(const SDL_Event *event) {}
  void dropText(const SDL_Event *event) {}
  void dropBegin(const SDL_Event *event) {}
  void dropEnd(const SDL_Event *event) {}
  */

  void keyboardDown(const SDL_Event *event);
  void keyboardUp(const SDL_Event *event);
  void textEdit(const SDL_Event *event);
  void textInput(const SDL_Event *event);

  void mouseButtonDown(const SDL_Event *event);
  void mouseButtonUp(const SDL_Event *event);
  void mouseWheel(const SDL_Event *event);

  void quit(const SDL_Event *event);
};

}  // namespace enigma

#endif
