/** Copyright (C) 2013, 2016 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "XIgamepad.h"

#include <windows.h>
#include <XInput.h>

namespace enigma_user {

bool gamepad_is_supported() {
  return true;
}

bool gamepad_is_connected(int num) {
  XINPUT_STATE controllerState = {};
  // Get the state
  DWORD dwResult = XInputGetState(num, &controllerState);
  if (dwResult == ERROR_SUCCESS) {
    return true;
  } else {
    return false;
  }
}

int gamepad_get_device_count() {
  return XUSER_MAX_COUNT;
}

string gamepad_get_description(int device) {
  if (gamepad_is_connected(device)) {
    return "Xbox 360 Controller (XInput STANDARD GAMEPAD)";
  } else {
    return "No device connected";
  }
}

int gamepad_get_battery_type(int device) {
  XINPUT_BATTERY_INFORMATION batteryInformation = {};
  // Get the state
  DWORD dwResult = XInputGetBatteryInformation(
      device, XINPUT_DEVTYPE_GAMEPAD, &batteryInformation);
  if (dwResult == ERROR_SUCCESS) {
    switch (batteryInformation.BatteryType) {
      case BATTERY_TYPE_DISCONNECTED:
        return gp_disconnected;
      case BATTERY_TYPE_WIRED:
        return gp_wired;
      case BATTERY_TYPE_ALKALINE:
        return gp_alkaline;
      case BATTERY_TYPE_NIMH:
        return gp_nimh;
      case BATTERY_TYPE_UNKNOWN:
        return gp_unknown;
      default:
        break;
    }
  }
  return 0;
}

int gamepad_get_battery_charge(int device) {
  XINPUT_BATTERY_INFORMATION batteryInformation = {};
  // Get the state
  DWORD dwResult = XInputGetBatteryInformation(
      device, XINPUT_DEVTYPE_GAMEPAD, &batteryInformation);
  if (dwResult == ERROR_SUCCESS) {
    switch (batteryInformation.BatteryLevel) {
      case BATTERY_LEVEL_EMPTY:
        return gp_empty;
      case BATTERY_LEVEL_LOW:
        return gp_low;
      case BATTERY_LEVEL_MEDIUM:
        return gp_medium;
      case BATTERY_LEVEL_FULL:
        return gp_full;
      default:
        break;
    }
  }
  return 0;
}

float gamepad_get_button_threshold(int device) {

}

void gamepad_set_button_threshold(int device, float threshold) {

}

void gamepad_set_axis_deadzone(int device, float deadzone) {

}

void gamepad_set_vibration(int device, float left, float right) {
  // Create a Vibraton State
  XINPUT_VIBRATION vibration = {};

  // Set the Vibration Values
  vibration.wLeftMotorSpeed = left * 65535;
  vibration.wRightMotorSpeed = right * 65535;

  // Vibrate the controller
  XInputSetState(device, &vibration);
}

int gamepad_axis_count(int device) {
  return 4;
}
  
float gamepad_axis_value(int device, int axis) {
  XINPUT_STATE controllerState = {};

  // Get the state
  DWORD dwResult = XInputGetState(device, &controllerState);
  if (dwResult == ERROR_SUCCESS) {
    float ret = 0;
    switch (axis) {
      case gp_axislh:
        ret = controllerState.Gamepad.sThumbLX;
        break;
      case gp_axislv:
        ret = -controllerState.Gamepad.sThumbLY - 1;
        break;
      case gp_axisrh:
        ret = controllerState.Gamepad.sThumbRX;
        break;
      case gp_axisrv:
        ret = -controllerState.Gamepad.sThumbRY - 1;
        break;
      default:
        return 0;
    }
    if (ret > 0) {
      return ret / 32767;
    } else {
      return ret / 32768;
    }
  } else {
    return 0;
  }
}

WORD digitalButtons[20] = {
    XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_Y,
    XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER, 0, 0,
    XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_LEFT_THUMB,
    XINPUT_GAMEPAD_RIGHT_THUMB, XINPUT_GAMEPAD_DPAD_UP,
    XINPUT_GAMEPAD_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_LEFT,
    XINPUT_GAMEPAD_DPAD_RIGHT, 0, 0, 0, 0
};

bool gamepad_button_check(int device, int button) {
  XINPUT_STATE controllerState = {};

  // Get the state
  XInputGetState(device, &controllerState);

  if (controllerState.Gamepad.wButtons & digitalButtons[button]) {
    return true;
  } else {
    return false;
  }
}

bool gamepad_button_check_pressed(int device, int button) {
  XINPUT_STATE controllerState = {};

  // Get the state
  XInputGetState(device, &controllerState);

  if (controllerState.Gamepad.wButtons & digitalButtons[button]) {
    return true;
  } else {
    return false;
  }
}

bool gamepad_button_check_released(int device, int button) {
  XINPUT_STATE controllerState = {};

  // Get the state
  XInputGetState(device, &controllerState);

  if (controllerState.Gamepad.wButtons & digitalButtons[button]) {
    return false;
  } else {
    return true;
  }
}

int gamepad_button_count(int device) {
  // GMS gives 16 for my wired 360 controller, i assume every button except
  // the guide button is counted
  return gamepad_is_connected(device) ? 16 : 0;
}

float gamepad_button_value(int device, int button) {
  XINPUT_STATE controllerState = {};

  // Get the state
  DWORD dwResult = XInputGetState(device, &controllerState);

  if (dwResult == ERROR_SUCCESS) {
    switch (button) {
      case gp_shoulderl:
        return (float)controllerState.Gamepad.bLeftTrigger / 255;
      case gp_shoulderr:
        return (float)controllerState.Gamepad.bRightTrigger / 255;
      default:
        break;
    }
  }
  return 0;
}

}
