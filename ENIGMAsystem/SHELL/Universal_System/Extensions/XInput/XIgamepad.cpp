/** Copyright (C) 2013, 2018 Robert Colton
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

#include "Platforms/General/PFgamepad.h"
#include "Platforms/platforms_mandatory.h"

#include <windows.h>
#include <XInput.h>

namespace {

struct Gamepad {
  XINPUT_STATE state, last_state;
  XINPUT_CAPABILITIES caps;
  DWORD state_result;
  float axis_deadzone = 0.05f, button_threshold = 0.5f;
};

Gamepad gamepads[XUSER_MAX_COUNT];

void process_gamepads() {
  for (size_t i = 0; i < XUSER_MAX_COUNT; ++i) {
    auto& gamepad = gamepads[i];
    gamepad.last_state = gamepad.state;
    gamepad.state_result = XInputGetState(i, &gamepad.state);
    XInputGetCapabilities(i, 0, &gamepad.caps);
  }
}

} // anonymous namespace

namespace enigma {

void extension_xinput_init() {
  for (size_t i = 0; i < XUSER_MAX_COUNT; ++i) {
    auto& gamepad = gamepads[i];
    gamepad.state = {};
  }
  process_gamepads(); // update once for create/game start events
  extension_update_hooks.push_back(process_gamepads);
}

} // namespace enigma

namespace enigma_user {

bool gamepad_is_supported() {
  return true;
}

bool gamepad_is_connected(int num) {
  return (gamepads[num].state_result == ERROR_SUCCESS);
}

int gamepad_get_device_count() {
  return XUSER_MAX_COUNT;
}

string gamepad_get_description(int device) {
  const auto& caps = gamepads[device].caps;

  if (gamepads[device].state_result == ERROR_SUCCESS) {
    //if (caps.Type == XINPUT_DEVTYPE_GAMEPAD) {
      switch (caps.SubType) {
        default:
#ifdef XINPUT_DEVSUBTYPE_UNKNOWN
        case XINPUT_DEVSUBTYPE_UNKNOWN:
          return "Xbox 360 Controller (XInput UNKNOWN)";
#endif
        case XINPUT_DEVSUBTYPE_GAMEPAD:
          return "Xbox 360 Controller (XInput STANDARD GAMEPAD)";
        case XINPUT_DEVSUBTYPE_WHEEL:
          return "Xbox 360 Controller (XInput WHEEL)";
        case XINPUT_DEVSUBTYPE_ARCADE_STICK:
          return "Xbox 360 Controller (XInput ARCADE STICK)";
#ifdef XINPUT_DEVSUBTYPE_FLIGHT_STICK
        case XINPUT_DEVSUBTYPE_FLIGHT_STICK:
          return "Xbox 360 Controller (XInput FLIGHT STICK)";
#endif
        case XINPUT_DEVSUBTYPE_DANCE_PAD:
          return "Xbox 360 Controller (XInput DANCE PAD)";
        case XINPUT_DEVSUBTYPE_GUITAR:
          return "Xbox 360 Controller (XInput GUITAR)";
#ifdef XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE
        case XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE:
          return "Xbox 360 Controller (XInput GUITAR ALTERNATE)";
#endif
#ifdef XINPUT_DEVSUBTYPE_GUITAR_BASS
        case XINPUT_DEVSUBTYPE_GUITAR_BASS:
          return "Xbox 360 Controller (XInput GUITAR BASS)";
#endif
        case XINPUT_DEVSUBTYPE_DRUM_KIT:
          return "Xbox 360 Controller (XInput DRUM KIT)";
#ifdef XINPUT_DEVSUBTYPE_ARCADE_PAD
        case XINPUT_DEVSUBTYPE_ARCADE_PAD:
          return "Xbox 360 Controller (XInput ARCADE PAD)";
#endif
      }
    //}
    //return "Unknown Controller";
  }

  return "";
}

int gamepad_get_battery_type(int device) {
	XINPUT_BATTERY_INFORMATION batteryInformation = {};

    // Get the state
    DWORD Result = XInputGetBatteryInformation(device, XINPUT_DEVTYPE_GAMEPAD, &batteryInformation);
	if (Result == ERROR_SUCCESS) {
        switch (batteryInformation.BatteryType) {
			case BATTERY_TYPE_DISCONNECTED:
				return gp_disconnected;
				break;
			case BATTERY_TYPE_WIRED:
				return gp_wired;
				break;
			case BATTERY_TYPE_ALKALINE:
				return gp_alkaline;
				break;
			case BATTERY_TYPE_NIMH:
				return gp_nimh;
				break;
			case BATTERY_TYPE_UNKNOWN:
				return gp_unknown;
				break;
			default:
				return -2;
		}
    } else {
        return -1;
    }
}

int gamepad_get_battery_charge(int device) {
	XINPUT_BATTERY_INFORMATION batteryInformation = {};

    // Get the state
    DWORD Result = XInputGetBatteryInformation(device, XINPUT_DEVTYPE_GAMEPAD, &batteryInformation);
	if (Result == ERROR_SUCCESS) {
        switch (batteryInformation.BatteryLevel) {
			case BATTERY_LEVEL_EMPTY:
				return gp_empty;
				break;
			case BATTERY_LEVEL_LOW:
				return gp_low;
				break;
			case BATTERY_LEVEL_MEDIUM:
				return gp_medium;
				break;
			case BATTERY_LEVEL_FULL:
				return gp_full;
				break;
			default:
				return -2;
		}
    } else {
        return -1;
    }
}

float gamepad_get_button_threshold(int device) {
  return gamepads[device].button_threshold;
}

void gamepad_set_button_threshold(int device, float threshold) {
  gamepads[device].button_threshold = threshold;
}

void gamepad_set_axis_deadzone(int device, float deadzone) {
  gamepads[device].axis_deadzone = deadzone;
}

void gamepad_set_vibration(int device, float left, float right) {
  XINPUT_VIBRATION vibration = {};

  // Set the Vibration Values
  vibration.wLeftMotorSpeed = left * 65535;
  vibration.wRightMotorSpeed = right * 65535;

  // Vibrate the controller
  XInputSetState(device, &vibration);
}

int gamepad_axis_count(int device) {
  if (gamepads[device].state_result != ERROR_SUCCESS) return 0;
  const auto& caps = gamepads[device].caps;
  int axes = 0;
  if (caps.Gamepad.sThumbLX)
    ++axes;
  if (caps.Gamepad.sThumbRX)
    ++axes;
  if (caps.Gamepad.sThumbLY)
    ++axes;
  if (caps.Gamepad.sThumbRY)
    ++axes;
  return axes;
}

float gamepad_axis_value(int device, int axis) {
	const auto& state = gamepads[device].state;

	float ret;
	switch (axis) {
		case gp_axislh:
			ret = state.Gamepad.sThumbLX;
			break;
		case gp_axislv:
			ret = state.Gamepad.sThumbLY;
			break;
		case gp_axisrh:
			ret = state.Gamepad.sThumbRX;
			break;
		case gp_axisrv:
			ret = state.Gamepad.sThumbRY;
			break;
		default:
			return -1;
	}
	if (ret > 0) {
		return ret / 32767;
	} else {
		return ret / 32768;
	}
}

WORD digitalButtons[20] = {
  XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_LEFT_SHOULDER,
  XINPUT_GAMEPAD_RIGHT_SHOULDER, 0, 0, XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB,
  XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_RIGHT, 0, 0, 0, 0
};

bool gamepad_button_check(int device, int button) {
  const bool is_down = (gamepads[device].state.Gamepad.wButtons & digitalButtons[button]);
  return is_down;
}

bool gamepad_button_check_pressed(int device, int button) {
  const bool is_down = (gamepads[device].state.Gamepad.wButtons & digitalButtons[button]);
  const bool was_down = (gamepads[device].last_state.Gamepad.wButtons & digitalButtons[button]);
  return is_down && !was_down;
}

bool gamepad_button_check_released(int device, int button) {
  const bool is_down = (gamepads[device].state.Gamepad.wButtons & digitalButtons[button]);
  const bool was_down = (gamepads[device].last_state.Gamepad.wButtons & digitalButtons[button]);
  return !is_down && was_down;
}

int gamepad_button_count(int device) {
  if (gamepads[device].state_result != ERROR_SUCCESS) return 0;
  const auto& caps = gamepads[device].caps;
  int buttons = 0;
  for (size_t i = 0; i < 20; ++i) {
    if (caps.Gamepad.wButtons & digitalButtons[i])
      ++buttons;
  }
  if (caps.Gamepad.bLeftTrigger)
    ++buttons;
  if (caps.Gamepad.bRightTrigger)
    ++buttons;
	return buttons;
}

float gamepad_button_value(int device, int button) {
	const auto& state = gamepads[device].state;

	switch (button) {
		case gp_shoulderlb:
			return state.Gamepad.bLeftTrigger / 255;
			break;
		case gp_shoulderrb:
			return state.Gamepad.bRightTrigger / 255;
			break;
		default:
			return 0;
	}
}

}
