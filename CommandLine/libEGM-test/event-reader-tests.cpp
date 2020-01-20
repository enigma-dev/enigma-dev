#include "event_reader/event_parser.h"
#include <gtest/gtest.h>

#include <sstream>

constexpr char kTestEvents[] = R"(
Events:
  Alarm:
    Name: "Alarm %1"
    Type: Parameterized
    Parameters:
      - integer
    Group: Alarm
    SubCheck: |
      {
        alarm[%1] = (int) alarm[%1];
        return alarm[%1] < 0 ? false : !((int)--alarm[%1]);
      }

  Collision:
    Name: "Colision %1"
    Description: "Instance is colliding (overlapping) with another object."
    Group: "Collisions"
    Type: Parameterized
    Parameters:
      - Object
    SuperCheck: |
      instance_number(%1)
    SubCheck: |
      enigma::place_meeting_inst(x,y,%1)
    Prefix: "{"
    Suffix: "}"

  Keyboard:
    Name: "Keyboard %1"
    Description: "A key was pressed or is being held."
    Group: "Input"
    Type: Parameterized
    Parameters:
      - Key
    SuperCheck: |
      keyboard_check(%1)

  Joystick.Button:
    Name: "Joystick %1 Button %2"
    Description: "A joystick button is pressed or held"
    Group: "Input"
    Type: Parameterized
    Parameters:
      - Integer
      - JoystickButton

Aliases:
  Key:
    Left:        {value: 37,  spelling: "vk_left"}
    Right:       {value: 39,  spelling: "vk_right"}
    Up:          {value: 38,  spelling: "vk_up"}
    Down:        {value: 40,  spelling: "vk_down"}
    A:  {value: 65, spelling: "'A'"}
    B:  {value: 66, spelling: "'B'"}

  JoystickButton:
    Left:   {value: 0, spelling: "js_left"}
    Right:  {value: 1, spelling: "js_right"}
    Up:     {value: 2, spelling: "js_up"}
    Down:   {value: 3, spelling: "js_down"}

GameMakerEventMappings:
  0:  # Instance construct event.
    Single: Create
  2:  # Alarms. GM only supports ten.
    Parameterized: Alarm[%1]  # Number <1-10>
  4:  # Collision events.
    Parameterized: Collision[%1]  # <Object>
  5:  # Keyboard events.
    Parameterized: Keyboard[%1]   # <Key>
  6:  # The "Mouse" group, which somehow inherited joysticks.
    Specialized:
      Cases:
        21: Joystick[1]Button[1]
        22: Joystick[1]Button[2]
        23: Joystick[1]Button[3]
        24: Joystick[1]Button[4]
        25: Joystick[1]Button[5]
        26: Joystick[1]Button[6]
        27: Joystick[1]Button[7]
        28: Joystick[1]Button[8]
        36: Joystick[2]Button[1]
        37: Joystick[2]Button[2]
        38: Joystick[2]Button[3]
        39: Joystick[2]Button[4]
        40: Joystick[2]Button[5]
        41: Joystick[2]Button[6]
        42: Joystick[2]Button[7]
        43: Joystick[2]Button[8]
)";

TEST(EventReaderTest, ReadsEventFile) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.DecodeEventString("Keyboard[Up]");
  EXPECT_EQ(ev.HumanName(), "Keyboard vk_up");
  EXPECT_EQ(ev.IdString(), "Keyboard[Up]");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 5);
  EXPECT_EQ(events.reverse_get_event(ev).id, 38);
  EXPECT_FALSE(ev.HasSubCheck());
  EXPECT_FALSE(ev.HasSubCheckExpression());
  EXPECT_FALSE(ev.HasSubCheckFunction());
  // EXPECT_TRUE(ev.HasSuperCheck());
  // EXPECT_TRUE(ev.HasSuperCheckExpression());
  // EXPECT_EQ(ev.event->super_check(), "keyboard_check(%1)");
  // EXPECT_EQ(ev.SuperCheckExpression(), "keyboard_check(vk_up)");
}
