#include "Event.h"
#include "Window.h"
#include "Gamepad.h"
#include "Joystick.h"

#include "Platforms/General/PFmain.h"    //game_end
#include "Platforms/General/PFwindow.h"  // mouse / keyboard

#include "Graphics_Systems/General/GScolors.h"  // draw_clear

#include <iostream>

namespace enigma {
SDL_Event_Handler::SDL_Event_Handler() {}

int SDL_Event_Handler::processEvents() {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_WINDOWEVENT:
        windowEvent(&e);
        break;
      case SDL_KEYDOWN:
        keyboardDown(&e);
        break;
      case SDL_KEYUP:
        keyboardUp(&e);
        break;
      case SDL_TEXTEDITING:
        //textEdit(&e);
        break;
      case SDL_TEXTINPUT:
        textInput(&e);
        break;
      case SDL_MOUSEMOTION:
        //mouseMotion(&e);
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouseButtonDown(&e);
        break;
      case SDL_MOUSEBUTTONUP:
        mouseButtonUp(&e);
        break;
      case SDL_MOUSEWHEEL:
        mouseWheel(&e);
        break;
      case SDL_JOYAXISMOTION:
        //joystickAxisMotion(&e);
        break;
      case SDL_JOYBALLMOTION:
        //joyBallAxisMotion(&e);
        break;
      case SDL_JOYHATMOTION:
        //joyHatMotion(&e);
        break;
      case SDL_JOYBUTTONDOWN:
        //joyButtonDown(&e);
        break;
      case SDL_JOYBUTTONUP:
        //joyButtonUp(&e);
        break;
      case SDL_JOYDEVICEADDED:
        joyDeviceAdded(&e);
        break;
      case SDL_JOYDEVICEREMOVED:
        joyDeviceRemoved(&e);
        break;
      case SDL_CONTROLLERAXISMOTION:
        //controllerAxisMotion(&e);
        break;
      case SDL_CONTROLLERBUTTONDOWN:
        controllerButtonDown(&e);
        break;
      case SDL_CONTROLLERBUTTONUP:
        controllerButtonUp(&e);
        break;
      case SDL_CONTROLLERDEVICEADDED:
        //controllerDeviceAdded(&e);
        break;
      case SDL_CONTROLLERDEVICEREMOVED:
        //controllerDeviceRemoved(&e);
        break;
      case SDL_CONTROLLERDEVICEREMAPPED:
        //controllerDeviceRemmaped(&e);
        break;
#if SDL_VERSION_ATLEAST(2, 0, 4)
      case SDL_AUDIODEVICEADDED:
        //audioDeviceAdded(&e);
        break;
      case SDL_AUDIODEVICEREMOVED:
        //audioDeviceRemoved(&e);
        break;
#endif
      case SDL_QUIT:
        quit(&e);
        return 1;
      case SDL_FINGERMOTION:
        //fingerMotion(&e);
        break;
      case SDL_FINGERDOWN:
        //fingerDown(&e);
        break;
      case SDL_FINGERUP:
        //fingerUp(&e);
        break;
      case SDL_MULTIGESTURE:
        //multigesture(&e);
        break;
      case SDL_DOLLARGESTURE:
        //dollarGesture(&e);
        break;
      case SDL_DOLLARRECORD:
        //dollarRecord(&e);
        break;
      case SDL_DROPFILE:
        //dropFile(&e);
        break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
      case SDL_DROPTEXT:
        //dropText(&e);
        break;
      case SDL_DROPBEGIN:
        //dropBegin(&e);
        break;
      case SDL_DROPCOMPLETE:
        //dropEnd(&e);
        break;
#endif
    }
  }

  return 0;
}

int SDL_map_button_enum(const char button) {
  switch (button) {
    case SDL_BUTTON_LEFT: return 0;
    case SDL_BUTTON_RIGHT: return 1;
    case SDL_BUTTON_MIDDLE: return 2;
    default: return -1;
  }
}

void SDL_Event_Handler::windowEvent(const SDL_Event *event) {
  switch (event->window.event) {
    case SDL_WINDOWEVENT_SHOWN:
      //windowShown(event);
      break;
    case SDL_WINDOWEVENT_HIDDEN:
      //windowHidden(event);
      break;
    case SDL_WINDOWEVENT_EXPOSED:
      //windowExposed(event);
      break;
    case SDL_WINDOWEVENT_MOVED:
      //windowMoved(event);
      break;
    case SDL_WINDOWEVENT_RESIZED:
      windowResized(event);
      break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      break;  // will trigger above event too
    case SDL_WINDOWEVENT_MINIMIZED:
      //windowMinimized(event);
      break;
    case SDL_WINDOWEVENT_MAXIMIZED:
      //windowMaximized(event);
      break;
    case SDL_WINDOWEVENT_RESTORED:
      //windowRestored(event);
      break;
    case SDL_WINDOWEVENT_ENTER:
      //windowEntered(event);
      break;
    case SDL_WINDOWEVENT_LEAVE:
      //windowLeave(event);
      break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      windowFocusGain(event);
      break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
      windowFocusLost(event);
      break;
    case SDL_WINDOWEVENT_CLOSE:
      //windowClose(event);
      break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
    case SDL_WINDOWEVENT_TAKE_FOCUS:
      //windowTakeFocus(event);
      break;
    case SDL_WINDOWEVENT_HIT_TEST:
      //windowHitTest(event);
      break;
#endif
    default:
      //unkownEvent(event);
      break;
  }
}

void SDL_Event_Handler::windowFocusGain(const SDL_Event *event) {
  game_window_focused = true;
  pausedSteps = 0;
}

void SDL_Event_Handler::windowFocusLost(const SDL_Event *event) {
  game_window_focused = false;
}

void SDL_Event_Handler::windowResized(const SDL_Event *event) {
  enigma_user::draw_clear(enigma_user::window_get_color());
}

void SDL_Event_Handler::joyDeviceAdded(const SDL_Event *event) {
  addGamepad(event->cdevice.which);
  int joystick_count = SDL_NumJoysticks();
  if (joystick_count != joysticks.size()) {
    joysticks.resize(joystick_count, nullptr);
  }
  joysticks_open();
}

void SDL_Event_Handler::joyDeviceRemoved(const SDL_Event *event) {
  removeGamepad(event->cdevice.which);
  int joystick_count = SDL_NumJoysticks();
  if (joystick_count < 0) {
    joysticks.resize(0);
  } else {
    joysticks.resize(joystick_count);
  }
  joysticks_close();
}

void SDL_Event_Handler::controllerButtonDown(const SDL_Event *event) {
  setGamepadButton(event->cdevice.which, event->cbutton.button, true);
}

void SDL_Event_Handler::controllerButtonUp(const SDL_Event *event) {
  setGamepadButton(event->cdevice.which, event->cbutton.button, false);
}

void SDL_Event_Handler::keyboardDown(const SDL_Event *event) {
  int key = enigma::keyboard::keymap[event->key.keysym.sym];
  enigma::last_keybdstatus[key] = enigma::keybdstatus[key];
  enigma::keybdstatus[key] = true;

  if (key == enigma_user::vk_backspace && !enigma_user::keyboard_string.empty()) enigma_user::keyboard_string.pop_back();
}

void SDL_Event_Handler::keyboardUp(const SDL_Event *event) {
  int key = enigma::keyboard::keymap[event->key.keysym.sym];
  enigma::last_keybdstatus[key] = enigma::keybdstatus[key];
  enigma::keybdstatus[key] = false;
}

void SDL_Event_Handler::textInput(const SDL_Event *event) {
  enigma_user::keyboard_string += event->text.text;
  enigma_user::keyboard_lastchar = enigma_user::keyboard_string.back();
}

void SDL_Event_Handler::mouseButtonDown(const SDL_Event *event) {
  int btn = SDL_map_button_enum(event->button.button);
  if (btn < 0) return;
  SDL_CaptureMouse(SDL_TRUE);
  enigma::last_mousestatus[btn] = enigma::mousestatus[btn];
  enigma::mousestatus[btn] = true;
}

void SDL_Event_Handler::mouseButtonUp(const SDL_Event *event) {
  int btn = SDL_map_button_enum(event->button.button);
  if (btn < 0) return;
  if (!SDL_GetMouseState(NULL, NULL)) SDL_CaptureMouse(SDL_FALSE);
  enigma::last_mousestatus[btn] = enigma::mousestatus[btn];
  enigma::mousestatus[btn] = false;
}

void SDL_Event_Handler::mouseWheel(const SDL_Event *event) {
  enigma_user::mouse_hscrolls += event->wheel.x;
  enigma_user::mouse_vscrolls += event->wheel.y;
}

void SDL_Event_Handler::quit(const SDL_Event *event) { enigma_user::game_end(); }
}  // namespace enigma
