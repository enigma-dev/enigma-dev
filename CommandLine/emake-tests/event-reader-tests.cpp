#include "event_reader/event_parser.h"
#include <gtest/gtest.h>

#include <sstream>
#include <fstream>

constexpr char kTestEvents[] = R"yaml(
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
    Name: "Collision %1"
    Description: "Instance is colliding (overlapping) with another object."
    Group: "Collisions"
    Type: Parameterized
    Parameters:
      - Object
    SuperCheck: "instance_number(%1)"
    SubCheck: "enigma::place_meeting_inst(x,y,%1)"
    Prefix: "{"
    Suffix: "}"

  BeginStep:
    Name: "Begin Step"
    Description: "The first step event executed each frame."
    Group: "Step"
    Constant: "begin step constant code"

  Step:
    Name: "Step"
    Group: "Step"
    Description: "Standard step event executed after begin step and input handling."
    Type: Special
    Constant: "step constant code"
    SubCheck: "true"

  EndStep:
    Name: "End Step"
    Description: "The last of the step events."
    Group: "Step"
    Type: Special
    Constant: "end step constant code"
    SubCheck: "true"

  Keyboard:
    Name: "Keyboard %1"
    Description: "A key was pressed or is being held."
    Group: "Input"
    Type: Parameterized
    Parameters:
      - Key
    SuperCheck: "keyboard_check(%1)"

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
  2:  # Alarms. GM only supports ten.
    Parameterized: Alarm[%1]  # Number <1-10>
  3:  # Step events.
    Specialized:
      Cases:
        0: Step
        1: BeginStep
        2: EndStep
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
)yaml";

TEST(EventReaderTest, RealEventFileIsReadable) {
  std::ifstream file("events.ey");
  EventData events(ParseEventFile(file));
  Event ev = events.DecodeEventString("Create");
  EXPECT_TRUE(ev.IsValid());
}

TEST(EventReaderTest, ReadsEventFile) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  EXPECT_EQ(events.get_event(2, 0).TrueFunctionName(), "alarm_0");
  EXPECT_EQ(events.get_event(2, 12).TrueFunctionName(), "alarm_12");
  EXPECT_EQ(events.get_event(4, 0).TrueFunctionName(), "collision_0");
  EXPECT_EQ(events.get_event(4, 1000).TrueFunctionName(), "collision_1000");
  EXPECT_EQ(events.get_event(6, 24).TrueFunctionName(), "joystick_1_button_4");
  EXPECT_TRUE(events.get_event(2, 0).HasSubCheck());
  EXPECT_TRUE(events.get_event(2, 8).HasSubCheck());
  EXPECT_TRUE(events.get_event(3, 0).HasSubCheck());
  EXPECT_FALSE(events.get_event(3, 1).HasSubCheck());
  EXPECT_TRUE(events.get_event(3, 2).HasSubCheck());
}


TEST(EventReaderTest, KeyboardUp) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.DecodeEventString("Keyboard[Up]");
  EXPECT_EQ(ev.HumanName(), "Keyboard vk_up");
  EXPECT_EQ(ev.IdString(), "Keyboard[Up]");
  EXPECT_EQ(ev.BaseFunctionName(), "keyboard");
  EXPECT_EQ(ev.TrueFunctionName(), "keyboard_Up");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 5);
  EXPECT_EQ(events.reverse_get_event(ev).id, 38);
  EXPECT_FALSE(ev.HasSubCheck());
  EXPECT_FALSE(ev.HasSubCheckExpression());
  EXPECT_FALSE(ev.HasSubCheckFunction());
  EXPECT_TRUE(ev.HasSuperCheck());
  EXPECT_TRUE(ev.HasSuperCheckExpression());
  EXPECT_EQ(ev.event->super_check(), "keyboard_check(%1)");
  EXPECT_EQ(ev.SuperCheckExpression(), "keyboard_check(vk_up)");
}

TEST(EventReaderTest, EndStep) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.DecodeEventString("EndStep");
  EXPECT_EQ(ev.HumanName(), "End Step");
  EXPECT_EQ(ev.IdString(), "EndStep");
  EXPECT_EQ(ev.BaseFunctionName(), "endstep");
  EXPECT_EQ(ev.TrueFunctionName(), "endstep");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 3);
  EXPECT_EQ(events.reverse_get_event(ev).id, 2);
}

TEST(EventReaderTest, EndStepById) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.get_event(3, 2);
  EXPECT_EQ(ev.HumanName(), "End Step");
  EXPECT_EQ(ev.IdString(), "EndStep");
  EXPECT_EQ(ev.BaseFunctionName(), "endstep");
  EXPECT_EQ(ev.TrueFunctionName(), "endstep");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 3);
  EXPECT_EQ(events.reverse_get_event(ev).id, 2);
}

TEST(EventReaderTest, Collision) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.DecodeEventString("Collision[1337]");
  EXPECT_EQ(ev.HumanName(), "Collision 1337");
  EXPECT_EQ(ev.IdString(), "Collision[1337]");
  EXPECT_EQ(ev.BaseFunctionName(), "collision");
  EXPECT_EQ(ev.TrueFunctionName(), "collision_1337");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 4);
  EXPECT_EQ(events.reverse_get_event(ev).id, 1337);
}

TEST(EventReaderTest, CollisionById) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.get_event(4, 1337);
  EXPECT_EQ(ev.HumanName(), "Collision 1337");
  EXPECT_EQ(ev.IdString(), "Collision[1337]");
  EXPECT_EQ(ev.BaseFunctionName(), "collision");
  EXPECT_EQ(ev.TrueFunctionName(), "collision_1337");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 4);
  EXPECT_EQ(events.reverse_get_event(ev).id, 1337);
}

TEST(EventReaderTest, Alarm) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.DecodeEventString("Alarm[12]");
  EXPECT_EQ(ev.HumanName(), "Alarm 12");
  EXPECT_EQ(ev.IdString(), "Alarm[12]");
  EXPECT_EQ(ev.BaseFunctionName(), "alarm");
  EXPECT_EQ(ev.TrueFunctionName(), "alarm_12");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 2);
  EXPECT_EQ(events.reverse_get_event(ev).id, 12);
}

TEST(EventReaderTest, AlarmById) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.get_event(2, 12);
  EXPECT_EQ(ev.HumanName(), "Alarm 12");
  EXPECT_EQ(ev.IdString(), "Alarm[12]");
  EXPECT_EQ(ev.BaseFunctionName(), "alarm");
  EXPECT_EQ(ev.TrueFunctionName(), "alarm_12");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 2);
  EXPECT_EQ(events.reverse_get_event(ev).id, 12);
}

TEST(EventReaderTest, Joystick) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.DecodeEventString("JoystickButton[1][2]");
  EXPECT_EQ(ev.HumanName(), "Joystick 1 Button 2");
  EXPECT_EQ(ev.IdString(), "Joystick[1]Button[2]");
  EXPECT_EQ(ev.BaseFunctionName(), "joystickbutton");
  EXPECT_EQ(ev.TrueFunctionName(), "joystick_1_button_2");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 6);
  EXPECT_EQ(events.reverse_get_event(ev).id, 22);
}

TEST(EventReaderTest, CaseInsensitivity) {
  std::istringstream str(kTestEvents);
  EventData events(ParseEventFile(str));
  Event ev = events.DecodeEventString("JoyStickButton[1][2]");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 6);
  EXPECT_EQ(events.reverse_get_event(ev).id, 22);
  EXPECT_EQ(ev.IdString(), "Joystick[1]Button[2]");
  ev = events.DecodeEventString("AlArM[1]");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 2);
  EXPECT_EQ(events.reverse_get_event(ev).id, 1);
  EXPECT_EQ(ev.IdString(), "Alarm[1]");
  ev = events.DecodeEventString("kEyBoArD[lEFt]");
  EXPECT_EQ(events.reverse_get_event(ev).mid, 5);
  EXPECT_EQ(events.reverse_get_event(ev).id, 37);
  EXPECT_EQ(ev.IdString(), "Keyboard[Left]");
}
