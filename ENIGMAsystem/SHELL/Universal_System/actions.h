/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
 **  Copyright (C) 2011 Polygone                                                 **
 **  Copyright (C) 2014 Seth N. Hetu                                             **
 **                                                                              **
 **  This file is a part of the ENIGMA Development Environment.                  **
 **                                                                              **
 **                                                                              **
 **  ENIGMA is free software: you can redistribute it and/or modify it under the **
 **  terms of the GNU General Public License as published by the Free Software   **
 **  Foundation, version 3 of the license or any later version.                  **
 **                                                                              **
 **  This application and its source code is distributed AS-IS, WITHOUT ANY      **
 **  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
 **  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
 **  details.                                                                    **
 **                                                                              **
 **  You should have recieved a copy of the GNU General Public License along     **
 **  with this code. If not, see <http://www.gnu.org/licenses/>                  **
 **                                                                              **
 **  ENIGMA is an environment designed to create games and other programs with a **
 **  high-level, fully compilable language. Developers of ENIGMA or anything     **
 **  associated with ENIGMA are in no way responsible for its users or           **
 **  applications created by its users, or damages caused by the environment     **
 **  or programs made in the environment.                                        **
 **                                                                              **
 \********************************************************************************/

#include "Instances/instance_system_base.h"
#include "lives.h"

#include <stdio.h>

namespace enigma {

    int initialize_everything();
    variant ev_perf(int type, int numb);

} // namespace enigma


namespace enigma_user
{
extern bool argument_relative;

//TODO: Confirm timeline mechanics (e.g., stopping resets position?)
inline void action_set_timeline(int timeline, double position, int startImmediately=0, int loop=0)
{
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_index=timeline;
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_position=position;
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_running = (startImmediately==0);
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_loop = (loop==1);
}

inline void action_set_timeline_position(double position)
{
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_position=position;
}

inline void action_set_timeline_speed(double speed)
{
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_speed=speed;
}

#define action_timeline_set action_set_timeline

inline void action_timeline_pause()
{
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_running = false;
}

inline void action_timeline_start()
{
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_running = true;
}

inline void action_timeline_stop()
{
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_running = false;
  ((enigma::object_timelines*)enigma::instance_event_iterator->inst)->timeline_position=0;
}

inline bool action_if_variable(const variant& variable, const variant& value, int operation) {
    switch (operation)
    {
        case 0: return (variable==value); break;
        case 1: return (variable<value); break;
        case 2: return (variable>value); break;
        case 3: return (variable<=value); break;
        case 4: return (variable>=value); break;
        default: return false; //invalid operation
    }
}

inline void action_move_to(const double xx, const double yy) {
  if (argument_relative) {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+=xx;
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+=yy;
  }
  else {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x=xx;
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y=yy;
  }
}

inline void action_set_score(double newscore) {
    if (argument_relative) score+= (int)newscore;
    else score = (int)newscore;
}

inline void action_set_life(double newlives) {
    if (argument_relative) lives+= (int)newlives;
    else lives = newlives;
}

inline void action_set_caption(const int vscore, const string scoreCaption, const int vlives,string livesCaption, const int vhealth, const string healthCaption) {
    show_score=vscore;
    caption_score=scoreCaption;
    show_lives=vlives;
    caption_lives=livesCaption;
    show_health=vhealth;
    caption_health=healthCaption;
}

inline bool action_if_number(const int object, const double number, const int operation) {
	switch (operation)
	{
	    case 0: return (instance_number(object) == lrint(number)); break;
	    case 1:	return (instance_number(object) < number); break;
	    case 2: return (instance_number(object) > number); break;
      case 3: return (instance_number(object) <= number); break;
      case 4: return (instance_number(object) >= number); break;
	    default: return false; //invalid operation
  }
}

inline void action_kill_object() { instance_destroy(); }

inline void action_set_vspeed(const double newvspeed) {
  if (argument_relative) {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed+=newvspeed;
  } else {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=newvspeed;
  }
}

inline void action_set_hspeed(const double newhspeed) {
  if (argument_relative) {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed+=newhspeed;
  } else {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed=newhspeed;
  }
}

inline void action_set_gravity(const double direction, const double newgravity) {
  if (argument_relative) {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity_direction+=direction;
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity+=newgravity;
  } else {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity_direction=direction;
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity=newgravity;
  }
}

inline void action_set_friction(const double newfriction) {
  if (argument_relative) {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->friction+=newfriction;
  } else {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->friction=newfriction;
  }
}

inline bool action_if_dice(double sides) {
    if (sides < 1) {return false;}
    return (random(sides) < 1);
}

inline void action_move_point(const double x, const double y, const double speed) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        move_towards_point(x+inst->x,y+inst->y,speed); } //is speed also relative?
    else move_towards_point(x,y,speed);
}

inline bool action_if(const double x) {
    return x >= 0.5;
}

inline bool action_if_previous_room()
{
    return room_previous(room) != -1;
}

inline bool action_if_next_room()
{
	return room_next(room) != -1;
}

/*
 Fixed version of IsmAvatars action_move
 */
inline void action_move(const char dir[9], int argspeed) {
    int dirs[9] = {  225, 270, 315, 180, -1, 0, 135, 90, 45 };
    int chosendirs[9];
    int choices = 0;
    for (int i = 0; i < 9; i++)
        if (dir[i] == '1') chosendirs[choices++] = dirs[i];
    if (choices == 0) return;
    choices = int(random(choices)); //choices is now chosen

    //We use rval.d for efficiency, so hspeed/vspeed aren't set twice.
    const double newdir =
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->direction.rval.d = chosendirs[choices];
    if (argument_relative)
        argspeed += ((enigma::object_planar*)enigma::instance_event_iterator->inst)->speed;
    const double newspd =
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->speed.rval.d = chosendirs[choices] == -1 ? 0 : argspeed;
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->hspeed.rval.d = newspd * cos(degtorad(newdir));
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->vspeed.rval.d = -newspd * sin(degtorad(newdir));
}

inline void action_reverse_xdir() {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed=-((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed;
}

inline void action_reverse_ydir() {
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=-((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed;
}

inline bool action_if_aligned(const double snapHor, const double snapVer) {
	return place_snapped(snapHor, snapVer);
}

inline void action_move_start() {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
	inst->x=inst->xstart;
	inst->y=inst->ystart;
}

inline void action_execute_script(string script,string argument0,string argument1,string argument2,string argument3,string argument4) {}
#define action_execute_script(script,argument0,argument1,argument2,argument3,argument4) script((argument0),(argument1),(argument2),(argument3),(argument4))

inline void action_sleep(const double milliseconds, const int redraw) {
    if (redraw) {screen_redraw();}
    sleep(milliseconds);
}

inline void action_current_room(const int transition) {
    //transition_kind=transition;
    room_restart();
}

inline void action_previous_room(const int transition) {
    //transition_kind=transition;
    room_goto_previous();
}

inline void action_next_room(const int transition) {
    //transition_kind=transition;
    room_goto_next();
}

//NOTE: The second parameter for the transition should default to 0.
//Studio implemented this action with the same LIB id (224) but removed the transition option
//because they deprecated room transitions.
inline void action_another_room(const int roomind, const int transition=0) {
	//transition_kind=transition;
	room_goto(roomind);
}

inline void action_wrap(const int direction) {
    switch (direction)
    {
         case 0: move_wrap(1,0,0); break;
         case 1: move_wrap(0,1,0); break;
default: case 2: move_wrap(1,1,0); break;
    }
}

inline void action_set_motion(const double dir, const double nspeed) {
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    if (argument_relative) {
        inst->hspeed+= (nspeed) * cos(degtorad((dir)));
        inst->vspeed-= (nspeed) * sin(degtorad((dir)));
    }
    else  {
        inst->direction=dir;
        inst->speed=nspeed;
    }
}

#define action_message(message) show_message(message)
#define action_if_sound sound_isplaying
#define action_end_sound sound_stop
//#define exit return 0; //NOTE: This is intentionally disabled; we handle it elsewhere.
#define globalvar global var

inline void action_create_object(const int object, const double x, const double y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        instance_create(inst->x+x, inst->y+y, object);
    }
    else
        instance_create(x, y, object);
}

void action_create_object_random(const int object1, const int object2, const int object3, const int object4, const double x, const double y);
void action_create_object_random(const int object1, const int object2, const int object3, const int object4, const double x, const double y)
{
    int obj_ar[4], obj_num = 0;
    if (object1 != -1)
        obj_ar[obj_num++] = object1;
    if (object2 != -1)
        obj_ar[obj_num++] = object2;
    if (object3 != -1)
        obj_ar[obj_num++] = object3;
    if (object4 != -1)
        obj_ar[obj_num++] = object4;
    const int object = obj_ar[int(random(obj_num))];
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        instance_create(inst->x+x, inst->y+y, object);
    }
    else
        instance_create(x, y, object);
}

inline void action_set_health(double value) {
    if (argument_relative) health+= (int)value;
    else health = value;
}

#define _V_EPSILON 1e-8
inline bool action_if_health(const double value, const int operation)
{
	switch (operation)
	{
	    case 0: return (fabs(health - value) < _V_EPSILON); break;
	    case 1: return (health < value); break;
	    case 2: return (health > value); break;
      case 3: return (health <= value); break;
      case 4: return (health >= value); break;
	    default: return false; //invalid operation
  }
}

inline bool action_if_life(const double value, const int operation)
{
	switch (operation)
	{
	    case 0: return (fabs(lives - value) < _V_EPSILON); break;
	    case 1: return (lives < value); break;
	    case 2: return (lives > value); break;
      case 3: return (lives <= value); break;
      case 4: return (lives >= value); break;
	    default: return false; //invalid operation
    }
}

inline bool action_if_score(const double value, const int operation)
{
	switch (operation)
	{
	    case 0: return (fabs(score - value) < _V_EPSILON); break;
	    case 1: return (score < value); break;
	    case 2: return (score > value); break;
      case 3: return (score <= value); break;
      case 4: return (score >= value); break;
	    default: return false; //invalid operation
  }
}

inline bool action_if_mouse(const int button) {
	return mouse_check_button(button);
}

inline void action_move_random(const double snapHor, const double snapVer) {
    move_random(snapHor, snapVer);
}

inline void action_snap(const double hor, const double vert) {
    move_snap(hor, vert);
}

inline void action_highscore_clear() {
    highscore_clear();
}

inline void action_create_object_motion(int object, double x, double y, double speed, double direction)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        enigma::object_planar* const ii = ((enigma::object_planar*)enigma::fetch_instance_by_int(instance_create(inst->x + x, inst->y + y, object)));
        ii->speed.rval.d = speed;
        ii->direction = direction;
    }
    else
    {
        enigma::object_planar* const ii = ((enigma::object_planar*)enigma::fetch_instance_by_int(instance_create(x, y, object)));
        ii->speed.rval.d = speed;
        ii->direction = direction;
    }
}

inline void action_fullscreen(int action)
{
    switch (action)
    {
        case 0: default:
            window_set_fullscreen(!window_get_fullscreen());
            break;
        case 1:
            window_set_fullscreen(false);
            break;
        case 2:
            window_set_fullscreen(true);
            break;
    }
}

/* Temp location fpr event perform stuff */ //TODO: Move

enum
{
    ev_create       = 0,
    ev_destroy      = 1,
    ev_alarm        = 2,
    ev_step         = 3,
    ev_collision    = 4,
    ev_keyboard     = 5,
    ev_mouse        = 6,
    ev_other        = 7,
    ev_draw         = 8,
    ev_keypress     = 9,
    ev_keyrelease   = 10,
    ev_trigger      = 11
};

enum
{
    ev_left_button              = 0,
    ev_right_button             = 1,
    ev_middle_button            = 2,
    ev_no_button                = 3,
    ev_left_press               = 4,
    ev_right_press              = 5,
    ev_middle_press             = 6,
    ev_left_release             = 7,
    ev_right_release            = 8,
    ev_middle_release           = 9,
    ev_mouse_enter              = 10,
    ev_mouse_leave              = 11,
    ev_joystick1_left           = 16,
    ev_joystick1_right          = 17,
    ev_joystick1_up             = 18,
    ev_joystick1_down           = 19,
    ev_joystick1_button1        = 21,
    ev_joystick1_button2        = 22,
    ev_joystick1_button3        = 23,
    ev_joystick1_button4        = 24,
    ev_joystick1_button5        = 25,
    ev_joystick1_button6        = 26,
    ev_joystick1_button7        = 27,
    ev_joystick1_button8        = 28,
    ev_joystick2_left           = 31,
    ev_joystick2_right          = 32,
    ev_joystick2_up             = 33,
    ev_joystick2_down           = 34,
    ev_joystick2_button1        = 36,
    ev_joystick2_button2        = 37,
    ev_joystick2_button3        = 38,
    ev_joystick2_button4        = 39,
    ev_joystick2_button5        = 40,
    ev_joystick2_button6        = 41,
    ev_joystick2_button7        = 42,
    ev_joystick2_button8        = 43,
    ev_global_left_button       = 50,
    ev_global_right_button      = 51,
    ev_global_middle_button     = 52,
    ev_global_left_press        = 53,
    ev_global_right_press       = 54,
    ev_global_middle_press      = 55,
    ev_global_left_release      = 56,
    ev_global_right_release     = 57,
    ev_global_middle_release    = 58,
    ev_mouse_wheel_up           = 60,
    ev_mouse_wheel_down         = 61,
	ev_gui                      = 64
};

enum
{
    ev_outside          = 0,
    ev_boundary         = 1,
    ev_game_start       = 2,
    ev_game_end         = 3,
    ev_room_start       = 4,
    ev_room_end         = 5,
    ev_no_more_lives    = 6,
    ev_animation_end    = 7,
    ev_end_of_path      = 8,
    ev_no_more_health   = 9,
    ev_user0            = 10,
    ev_user1            = 11,
    ev_user2            = 12,
    ev_user3            = 13,
    ev_user4            = 14,
    ev_user5            = 15,
    ev_user6            = 16,
    ev_user7            = 17,
    ev_user8            = 18,
    ev_user9            = 19,
    ev_user10           = 20,
    ev_user11           = 21,
    ev_user12           = 22,
    ev_user13           = 23,
    ev_user14           = 24,
    ev_user15           = 25,
    ev_close_button     = 30
};

enum
{
    ev_step_normal  = 0,
    ev_step_begin   = 1,
    ev_step_end     = 2
};

inline variant event_perform(int type, int numb) {
    return enigma::ev_perf(type, numb);
}

inline variant event_user(int numb) {
    return event_perform(enigma_user::ev_other, numb + enigma_user::ev_user0);
}

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
#endif

inline void event_inherited() {
  DEBUG_MESSAGE("Event inherited called on an object that has no event to inherit.", MESSAGE_TYPE::M_ERROR);
}

}

#define action_inherited event_inherited
