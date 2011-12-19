/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
 **  Copyright (C) 2011 Polygone                                                 **
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

inline bool action_if_variable(const variant& variable, const variant& value, int operation) {
    switch (operation)
    {
        case 0: return (variable==value); break;
        case 1: return (variable<value); break;
        case 2: return (variable>value); break;
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

#define action_sound(snd,loop) ((loop?sound_loop:sound_play)(snd))
#define action_if_sound sound_isplaying
#define action_end_sound sound_stop


inline void action_color(const int color) {
	draw_set_color(color);
}

inline bool action_if_number(const int object, const double number, const int operation) {
	switch (operation)
	{
	    case 0: return (instance_number(object) == number); break;
	    case 1:	return (instance_number(object) < number); break;
	    case 2: return (instance_number(object) > number); break;
	    default: return false; //invalid operation
  }
}

inline void action_kill_object() { instance_destroy(); }

inline void action_set_vspeed(const double newvspeed) {
    if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed+=newvspeed;
    } else
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=newvspeed;
}

inline void action_set_hspeed(const double newhspeed) {
	if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed+=newhspeed;
    } else
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed=newhspeed;
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
    if (sides == 0) {return false;}
    return (random(1) < (double)1/sides);
}

inline void action_move_point(const double x, const double y, const double speed) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        move_towards_point(x+inst->x,y+inst->y,speed); } //is speed also relative?
    else move_towards_point(x,y,speed);
}

inline bool action_if(const double x) {
    return x != 0;
}

inline bool action_if_object(const int object, const double xx, const double yy) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        return place_meeting(inst->x+xx,inst->y+yy,object);
    }
    else {
        return place_meeting(xx,yy,object);
    }
    return false;
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

inline bool action_if_empty(const double xx, const double yy, const int objects) {
    if (argument_relative) {
        if (objects == 0)
            return place_free(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+xx,((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+yy);
        else
            return place_empty(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+xx,((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+yy);
    }
    else {
        if (objects == 0)
            return place_free(xx,yy);
        else
            return place_empty(xx,yy);
    }
    return false;
}

inline void action_move_start() {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
	inst->x=inst->xstart;
	inst->y=inst->ystart;
}

inline void action_execute_script(string script,string argument0,string argument1,string argument2,string argument3,string argument4) {}
#define action_execute_script(script,argument0,argument1,argument2,argument3,argument4) script((argument0),(argument1),(argument2),(argument3),(argument4))

inline void action_draw_rectangle(const double x1, const double y1, const double x2, const double y2, const int filled) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle(x1+inst->x,y1+inst->y,x2+inst->x,y2+inst->y,filled);
    }
    else draw_rectangle(x1,y1,x2,y2,filled);
}

inline void action_sprite_set(const double spritep, const  double subimage, const double speed) {
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->sprite_index=spritep;
	if (subimage !=-1) inst->image_index=subimage;
	inst->image_speed=speed;
}

inline void action_draw_text(const string text, const double x, const double y) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(x+inst->x,y+inst->y,text); }
    else {
        draw_text(x,y,text);
    }
}

inline void action_sleep(const double milliseconds, const int redraw) {
    if (redraw) {screen_redraw();}
    sleep(milliseconds/1000);
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

inline void action_another_room(const int roomind, const int transition) {
	//transition_kind=transition;
	room_goto(roomind);
}

inline void action_font(const int font, const int align) {
    draw_set_font(font);
    // draw_set_halign(align);
}


inline void action_wrap(const int direction) {
    switch (direction)
    {
        case 0: move_wrap(1,0,0); break;
        case 1: move_wrap(0,1,0); break;
        case 2: move_wrap(1,1,0); break;
    }
}

inline void action_move_contact(const double direction, const double max_dist, const bool against) {
    move_contact_object(direction, max_dist, all, !against);
}
inline void action_bounce(int precisely, bool against) {
    move_bounce_object(precisely, all, !against);
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

inline void game_restart() { //RELOCATE ME
    room_goto_first();
}
static void show_info() {}  //TEMPORARY FILLER, RELOCATE ME
static inline void action_show_info() {show_info();}

#define action_restart_game game_restart
#define action_message(message) show_message(message)

inline bool action_if_collision(const double x, const double y, const int object) {
  return !action_if_empty(x,y,object); //Already takes argument_relative into account
}


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

inline void action_draw_arrow(const double x1, const double y1, const double x2, const double y2, const double tipSize) {
    draw_arrow(x1, y1, x2, y2, tipSize, 1, false);
}

inline void action_draw_background(const int background, const double x, const double y, const int tiled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        (tiled ? draw_background_tiled : draw_background)(background, inst->x+x, inst->y+y);
    }
    else
      (tiled ? draw_background_tiled : draw_background)(background, x, y);
}

inline void action_draw_ellipse(const double x1, const double y1, const double x2, const double y2, const int filled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, filled);
    }
    else
        draw_ellipse(x1, y1, x2, y2, filled);
}

inline void action_draw_ellipse_gradient(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, false);
    }
    else
        draw_ellipse_color(x1, y1, x2, y2, color1, color2, false);
}

inline void action_draw_gradient_hor(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, color1, color2, false);
    }
    else
        draw_rectangle_color(x1, y1, x2, y2, color1, color2, color1, color2, false);
}

inline void action_draw_gradient_vert(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color1, color2, color2, false);
    }
    else
        draw_rectangle_color(x1, y1, x2, y2, color1, color1, color2, color2, false);
}

inline void action_draw_score(const double x, const double y, const string caption) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(x+inst->x,y+inst->y,caption+string(score));
    } else draw_text(x,y,caption+string(score));
}

inline void action_draw_sprite(const int sprite, const double x, const double y, const int subimage) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_sprite(sprite,subimage,x+inst->x,y+inst->y);
    }
    else
        draw_sprite(sprite,subimage,x,y);
}

inline void action_set_health(double value) {
    if (argument_relative) health+= (int)value;
    else health = value;
}

void action_draw_health(const double x1, const double y1, const double x2, const double y2, const double backColor, const int barColor);
void action_draw_health(const double x1, const double y1, const double x2, const double y2, const double backColor, const int barColor) {
  double realbar1, realbar2;
  switch (barColor)
  {
      case 0: realbar1=c_green; realbar2=c_red; break;
      case 1: realbar1=c_white; realbar2=c_black; break;
      case 2: realbar1=c_black; realbar2=c_black; break;
      case 3: realbar1=c_gray; realbar2=c_gray; break;
      case 4: realbar1=c_silver; realbar2=c_silver; break;
      case 5: realbar1=c_white; realbar2=c_white; break;
      case 6: realbar1=c_maroon; realbar2=c_maroon; break;
      case 7: realbar1=c_green; realbar2=c_green; break;
      case 8: realbar1=c_olive; realbar2=c_olive; break;
      case 9: realbar1=c_navy; realbar2=c_navy; break;
      case 10: realbar1=c_purple; realbar2=c_purple; break;
      case 11: realbar1=c_teal; realbar2=c_teal; break;
      case 12: realbar1=c_red; realbar2=c_red; break;
      case 13: realbar1=c_lime; realbar2=c_lime; break;
      case 14: realbar1=c_yellow; realbar2=c_yellow; break;
      case 15: realbar1=c_blue; realbar2=c_blue; break;
      case 16: realbar1=c_fuchsia; realbar2=c_fuchsia; break;
      case 17: realbar1=c_aqua; realbar2=c_aqua; break;
      default: realbar1=c_green; realbar2=c_red;
  }
	if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_healthbar(x1+inst->x, y1+inst->y, x2+inst->x, y2+inst->y, health, backColor, realbar2, realbar1, 1, 1, 1);
	}
	else
        draw_healthbar(x1, y1, x2, y2, health, backColor, realbar2, realbar1, 1, 1, 1);
}

inline void action_draw_life(const double x, const double y, const string caption)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(inst->x+x, inst->y+y, caption + string(lives));
    }
    else
        draw_text(x, y, caption + string(lives));
}

inline void action_draw_life_images(const double x, const double y, const int image) {
    int actualX=x, actualY=y;
    const int width = sprite_get_width(image);

    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        actualX+=inst->x;
        actualY+=inst->y;
    }

    for (int i=0; i<lives; i++)
        draw_sprite(image,-1, actualX+(i*width), actualY);
}

inline void action_draw_line(const double x1, const double y1, const double x2, const double y2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_line(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2);
    }
    else
        draw_line(x1, y1, x2, y2);
}

inline void action_draw_text_transformed(const string text, const double x, const double y, const double horScale, const double verScale, const double angle)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text_transformed(inst->x+x, inst->y+y, text, horScale, verScale, angle);
    }
    else
        draw_text_transformed(x, y, text, horScale, verScale, angle);
}

inline void action_draw_variable(variant variable, const double x, const double y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(inst->x+x, inst->y+y, *variable);
    }
    else
        draw_text(x, y, string(variable));
}

inline bool action_if_health(const double value, const int operation)
{
	switch (operation)
	{
	    case 0: return (health == value); break;
	    case 1:	return (health < value); break;
	    case 2: return (health > value); break;
	    default: return false; //invalid operation
  }
}

inline bool action_if_life(const double value, const int operation)
{
	switch (operation)
	{
	    case 0: return (lives == value); break;
	    case 1:	return (lives < value); break;
	    case 2: return (lives > value); break;
	    default: return false; //invalid operation
    }
}

inline bool action_if_score(const double value, const int operation)
{
	switch (operation)
	{
	    case 0: return (score == value); break;
	    case 1:	return (score < value); break;
	    case 2: return (score > value); break;
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

#include "instance_system_base.h"

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

inline int draw_self()
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    draw_sprite_ext(inst->sprite_index, inst->image_index, inst->x, inst->y, inst->image_xscale, inst->image_yscale, inst->image_angle, inst->image_blend, inst->image_alpha);
    return 0;
}  //actions seemed the best place for this

inline void set_automatic_draw(bool enable)
{
    automatic_redraw = enable;
}
