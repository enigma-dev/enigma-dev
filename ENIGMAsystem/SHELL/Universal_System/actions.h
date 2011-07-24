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

inline void action_set_caption(const int score, const string scoreCaption, const int lives,string livesCaption, const int health, const string healthCaption) {
    show_score=score;
    caption_score=scoreCaption;
    show_lives=lives;
    caption_lives=livesCaption;
    show_health=0;
    caption_health=healthCaption;
}

inline void action_sound(const int sound, const int loop) {
	if (loop==0) sound_play(sound);
	else sound_loop(sound);
}

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

void action_set_hspeed(const double newhspeed) {
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

inline void move_towards_point (const double point_x, const double point_y, const double newspeed) {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    inst->direction = point_direction ( inst->x,inst->y, (point_x), (point_y) );
    inst->speed = (newspeed);
} //RELOCATE ME

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

    bool argument_relative = false;

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

inline bool place_snapped(int hsnap, int vsnap) {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    return  ((((int) inst->x) % ((int) hsnap) == 0) &&  (((int) inst->y) % ((int) vsnap)==0));
} //RELOCATE ME

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

void action_execute_script(string script,string argument0,string argument1,string argument2,string argument3,string argument4) {}
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

inline bool action_if_sound(const int sound) {return sound_isplaying(sound);}

inline void action_end_sound(const int sound) {
    sound_stop(sound);
}

inline void action_sleep(const double milliseconds, const int redraw) {
    if (redraw) {screen_redraw();}
    sleep(milliseconds/1000);
}

inline void action_current_room(const int transition) {
    //transition_kind=transition;
    room_restart();
}

inline void action_next_room(const int transition) {
    //transition_kind=transition;
    room_goto_next();
}

inline void action_another_room(const int room, const int transition) {
	//transition_kind=transition;
	room_goto(room);
}

inline void action_font(const int font, const int align) {
    draw_set_font(font);
    // draw_set_halign(align);
}

/*
 move_wrap by Polygone
 */
void move_wrap(const bool hor, const bool vert, const double margin)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    if (hor)
    {
        const double wdis = room_width + margin*2;
        if (inst->x < -margin)
        {
            inst->x += wdis*ceil(((-margin) - inst->x)/wdis);
        }
        if (inst->x > room_width + margin)
        {
            inst->x -= wdis*ceil((inst->x - (room_width + margin))/wdis);
        }
    }
    if (vert)
    {
        const double hdis = room_height + margin*2;
        if (inst->y < -margin)
        {
            inst->y += hdis*ceil(((-margin) - inst->y)/hdis);
        }
        if (inst->y > room_height + margin)
        {
            inst->y -= hdis*ceil((inst->y - (room_height + margin))/hdis);
        }
    }
} //RELOCATE ME

inline void action_wrap(const int direction) {
    switch (direction)
    {
        case 0: move_wrap(1,0,0); break;
        case 1: move_wrap(0,1,0); break;
        case 2: move_wrap(1,1,0); break;
    }
}

/*
move contact by Polygone
*/

double move_contact_object(double angle, double dist, const int object, const bool solid_only = false)
{
    const double DMIN = 0.000001, DMAX = 1000000;
    const double contact_distance = DMIN;
    if (dist <= 0)
    {
        dist = DMAX;
    }
    angle = ((angle mod 360) + 360) mod 360;
    const double rad_angle = degtorad(angle);
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    const int quad = int(angle/90.0);
    for (enigma::inst_iter *it = enigma::fetch_inst_iter_by_int(object); it != NULL; it = it->next)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)it->inst;
        if (inst2->id == inst1->id || solid_only && !inst2->solid) {continue;}

        if (inst2->x + inst2->bbox_right >= inst1->x + inst1->bbox_left && inst2->y + inst2->bbox_bottom >= inst1->y + inst1->bbox_top && inst2->x + inst2->bbox_left <= inst1->x + inst1->bbox_right && inst2->y + inst2->bbox_top <= inst1->y + inst1->bbox_bottom)
        {
            dist = 0;
            break;
        }

        switch (quad)
        {
            case 0:
            if ((inst2->x + inst2->bbox_left > inst1->x + inst1->bbox_right || inst1->y + inst1->bbox_top > inst2->y + inst2->bbox_bottom) &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_right, inst1->y + inst1->bbox_bottom, inst2->x + inst2->bbox_left, inst2->y + inst2->bbox_top)) >= 0  &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_left, inst1->y + inst1->bbox_top, inst2->x + inst2->bbox_right, inst2->y + inst2->bbox_bottom)) <= 0)
            {
                if (direction_difference(angle, point_direction(inst1->x + inst1->bbox_right, inst1->y + inst1->bbox_top, inst2->x + inst2->bbox_left, inst2->y + inst2->bbox_bottom)) > 0)
                {
                    dist = min(dist, (inst1->y + inst1->bbox_top - (inst2->y + inst2->bbox_bottom) - contact_distance)/sin(rad_angle));
                }
                else
                {
                    dist = min(dist, (inst2->x + inst2->bbox_left - (inst1->x + inst1->bbox_right) - contact_distance)/cos(rad_angle));
                }
            }
            break;
            case 1:
            if ((inst1->x + inst1->bbox_left > inst2->x + inst2->bbox_right || inst1->y + inst1->bbox_top > inst2->y + inst2->bbox_bottom) &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_left, inst1->y + inst1->bbox_bottom, inst2->x + inst2->bbox_right, inst2->y + inst2->bbox_top)) <= 0  &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_right, inst1->y + inst1->bbox_top, inst2->x + inst2->bbox_left, inst2->y + inst2->bbox_bottom)) >= 0)
            {
                if (direction_difference(angle, point_direction(inst1->x + inst1->bbox_left, inst1->y + inst1->bbox_top, inst2->x + inst2->bbox_right, inst2->y + inst2->bbox_bottom)) > 0)
                {
                    dist = min(dist, (inst2->x + inst2->bbox_right - (inst1->x + inst1->bbox_left) + contact_distance)/cos(rad_angle));
                }
                else
                {
                    dist = min(dist, (inst1->y + inst1->bbox_top - (inst2->y + inst2->bbox_bottom) - contact_distance)/sin(rad_angle));
                }
            }
            break;
            case 2:
            if ((inst1->x + inst1->bbox_left > inst2->x + inst2->bbox_right || inst2->y + inst2->bbox_top > inst1->y + inst1->bbox_bottom) &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_right, inst1->y + inst1->bbox_bottom, inst2->x + inst2->bbox_left, inst2->y + inst2->bbox_top)) <= 0  &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_left, inst1->y + inst1->bbox_top, inst2->x + inst2->bbox_right, inst2->y + inst2->bbox_bottom)) >= 0)
            {
                if (direction_difference(angle, point_direction(inst1->x + inst1->bbox_left, inst1->y + inst1->bbox_bottom, inst2->x + inst2->bbox_right, inst2->y + inst2->bbox_top)) > 0)
                {
                    dist = min(dist, (inst1->y + inst1->bbox_bottom - (inst2->y + inst2->bbox_top) + contact_distance)/sin(rad_angle));
                }
                else
                {
                    dist = min(dist, (inst2->x + inst2->bbox_right - (inst1->x + inst1->bbox_left) + contact_distance)/cos(rad_angle));
                }
            }
            break;
            case 3:
            if ((inst2->x + inst2->bbox_left > inst1->x + inst1->bbox_right || inst2->y + inst2->bbox_top > inst1->y + inst1->bbox_bottom) &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_right, inst1->y + inst1->bbox_top, inst2->x + inst2->bbox_left, inst2->y + inst2->bbox_bottom)) <= 0  &&
            direction_difference(angle, point_direction(inst1->x + inst1->bbox_left, inst1->y + inst1->bbox_bottom, inst2->x + inst2->bbox_right, inst2->y + inst2->bbox_top)) >= 0)
            {
                if (direction_difference(angle, point_direction(inst1->x + inst1->bbox_right, inst1->y + inst1->bbox_bottom, inst2->x + inst2->bbox_left, inst2->y + inst2->bbox_top)) > 0)
                {
                    dist = min(dist, (inst2->x + inst2->bbox_left - (inst1->x + inst1->bbox_right) - contact_distance)/cos(rad_angle));
                }
                else
                {
                    dist = min(dist, (inst1->y + inst1->bbox_bottom - (inst2->y + inst2->bbox_top) + contact_distance)/sin(rad_angle));
                }
            }
            break;
        }
    }
    inst1->x += cos(rad_angle)*dist;
    inst1->y -= sin(rad_angle)*dist;
    return dist;
}

inline int move_contact_all(const double direction, const double speed)
{
    return move_contact_object(direction, speed, all);
}

inline int move_contact_solid(const double direction, const double speed)
{
    return move_contact_object(direction, speed, all, true);
}

inline int move_contact(const double direction, const double speed)
{
    return move_contact_object(direction, speed, all);
} //RELOCATE US

inline void action_move_contact(const double direction, const double speed, const int against)
{
    if (against == 0)
    {
        move_contact_object(direction, speed, all, true);
    }
    else if (against == 1)
    {
        move_contact_object(direction, speed, all, false);
    }
}

/* move bounce by Polygone */

int move_bounce_object(const bool adv, const int object, const bool solid_only = false)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    for (enigma::inst_iter *it = enigma::fetch_inst_iter_by_int(object); it != NULL; it = it->next)
    {
        const enigma::object_collisions* inst2 = (enigma::object_collisions*)it->inst;
        if (inst2->id == inst1->id || solid_only && !inst2->solid) {continue;}

        if (inst1->x + inst1->bbox_left <= inst2->x + inst2->bbox_right && inst1->y + inst1->bbox_top <= inst2->y + inst2->bbox_bottom && inst1->x + inst1->bbox_right >= inst2->x + inst2->bbox_left && inst1->y + inst1->bbox_bottom >= inst2->y + inst2->bbox_top)
        {
            if (inst1->x - inst1->hspeed + inst1->bbox_left > inst2->x + inst2->bbox_right || inst1->x - inst1->hspeed + inst1->bbox_right < inst2->x + inst2->bbox_left)
            {
                inst1->hspeed *= -1;
            }
            if (inst1->y - inst1->vspeed + inst1->bbox_top > inst2->x + inst2->bbox_bottom || inst1->y - inst1->vspeed + inst1->bbox_bottom < inst2->x + inst2->bbox_top)
            {
                inst1->vspeed *= -1;
            }
            inst1->x += inst1->hspeed;
            inst1->y += inst1->vspeed;
            return inst2->id;
        }
        if (inst1->x + inst1->hspeed + inst1->bbox_left <= inst2->x + inst2->bbox_right && inst1->y + inst1->vspeed + inst1->bbox_top <= inst2->y + inst2->bbox_bottom && inst1->x + inst1->hspeed + inst1->bbox_right >= inst2->x + inst2->bbox_left && inst1->y + inst1->vspeed + inst1->bbox_bottom >= inst2->y + inst2->bbox_top)
        {
            if (inst1->x + inst1->bbox_left > inst2->x + inst2->bbox_right || inst1->x + inst1->bbox_right < inst2->x + inst2->bbox_left)
            {
                inst1->hspeed *= -1;
            }
            if (inst1->y + inst1->bbox_top > inst2->y + inst2->bbox_bottom || inst1->y + inst1->bbox_bottom < inst2->y + inst2->bbox_top)
            {
                inst1->vspeed *= -1;
            }
            return inst2->id;
        }
    }
    return -4;
}

inline int move_bounce_all(const bool adv)
{
    return move_bounce_object(adv, all, false);
}

inline int move_bounce_solid(const bool adv)
{
    return move_bounce_object(adv, all, true);
}

inline int move_bounce(const bool adv)
{
    return move_bounce_object(adv, all, false);
} //RELOCATE US

inline void action_bounce(int precisely, int against)
{
    if (against == 0)
    {
        move_bounce_object(precisely, all, true);
    }
    else if (against == 1)
    {
        move_bounce_object(precisely, all, false);
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

void motion_set(int dir, double newspeed) //RELOCATE ME
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->direction=dir;
    inst->speed=newspeed;
}
//#define motion_set(newdirection,newspeed) {direction=(newdirection); speed=(newspeed);}

void motion_add(double newdirection, double newspeed) //RELOCATE ME
{
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);

    inst->hspeed+= (newspeed) * cos(degtorad((newdirection)));
    inst->vspeed-= (newspeed) * sin(degtorad((newdirection)));
}
//#define motion_add(newdirection,newspeed) hspeed+= (newspeed) * cos(degtorad((newdirection))); vspeed-= (newspeed) * sin(degtorad((newdirection)));

inline void game_restart() { //RELOCATE ME
    room_goto_first();
}

void action_restart_game();
#define action_restart_game game_restart


inline bool action_if_collision(const double x, const double y, const int object) {
        return !action_if_empty(x,y,object); //Already takes argument_relative into account
}

inline void action_message(string message) {
show_message(message);
}

void show_info() {}  //TEMPORARY FILLER, RELOCATE ME
inline void action_show_info() {show_info();}

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

inline void action_create_object_random(const int object1, const int object2, const int object3, const int object4, const double x, const double y)
{
    int obj_ar[4], obj_num = 0;
    if (object1 != -1)
    {
        obj_ar[obj_num] = object1;
        obj_num += 1;
    }
    if (object2 != -1)
    {
        obj_ar[obj_num] = object2;
        obj_num += 1;
    }
    if (object3 != -1)
    {
        obj_ar[obj_num] = object3;
        obj_num += 1;
    }
    if (object4 != -1)
    {
        obj_ar[obj_num] = object4;
        obj_num += 1;
    }
    const int object = obj_ar[int(random(obj_num))];
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        instance_create(inst->x+x, inst->y+y, object);
    }
    else
        instance_create(x, y, object);
}

inline void action_draw_arrow(const double x1, const double y1, const double x2, const double y2, const double tipSize)
{
    draw_arrow(x1, y1, x2, y2, tipSize, 1, false);
}

inline void action_draw_background(const int background, const double x, const double y, const int tiled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        if (tiled == 0)
        {
            draw_background(background, inst->x+x, inst->y+y);
        }
        else
        {
            draw_background_tiled(background, inst->x+x, inst->y+y);
        }
    }
    else
    {
        if (tiled == 0)
        {
            draw_background(background, x, y);
        }
        else
        {
            draw_background_tiled(background, x, y);
        }
    }
}

inline void action_draw_ellipse(const double x1, const double y1, const double x2, const double y2, const int filled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, filled);
    }
    else
    {
        draw_ellipse(x1, y1, x2, y2, filled);
    }
}

inline void action_draw_ellipse_gradient(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, false);
    }
    else
    {
        draw_ellipse_color(x1, y1, x2, y2, color1, color2, false);
    }
}

inline void action_draw_gradient_hor(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, color1, color2, false);
    }
    else
    {
        draw_rectangle_color(x1, y1, x2, y2, color1, color2, color1, color2, false);
    }
}

inline void action_draw_gradient_vert(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color1, color2, color2, false);
    }
    else
    {
        draw_rectangle_color(x1, y1, x2, y2, color1, color1, color2, color2, false);
    }
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

inline void action_draw_health(const double x1, const double y1, const double x2, const double y2, const double backColor, const int barColor) {
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
	} else
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
    {
        draw_text(x, y, caption + string(lives));
    }
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
    {
        draw_line(x1, y1, x2, y2);
    }
}

inline void action_draw_text_transformed(const string text, const double x, const double y, const double horScale, const double verScale, const double angle)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text_transformed(inst->x+x, inst->y+y, text, horScale, verScale, angle);
    }
    else
    {
        draw_text_transformed(x, y, text, horScale, verScale, angle);
    }
}

inline void action_draw_variable(variant variable, const double x, const double y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(inst->x+x, inst->y+y, *variable);
    }
    else
    {
        draw_text(x, y, string(variable));
    }
}


inline void action_end_game()
{
    game_end();
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

inline bool action_if_mouse(const int button)
{
	return mouse_check_button(button);
}

inline void action_move_random(const double snapHor, const double snapVer)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    const int mask = ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->sprite_index;   //CHANGE TO MASK INDEX!!!
    const double x1 = sprite_get_xoffset(mask), y1 = sprite_get_yoffset(mask), x2 = room_width - sprite_get_width(mask) + sprite_get_xoffset(mask), y2 = room_height - sprite_get_height(mask) + sprite_get_yoffset(mask);
    if (snapHor == 0)
    {
        inst->x = x1 + random(x2 - x1);
    }
    else
    {
        inst->x = x1 + floor(random(x2 - x1)/snapHor)*snapHor;
    }
    if (snapVer == 0)
    {
        inst->y = y1 + random(y2 - y1);
    }
    else
    {
        inst->y = y1 + floor(random(y2 - y1)/snapVer)*snapVer;
    }
}   //NOTE MASK INDEX CHANGE!!!

void move_snap(const double hsnap, const double vsnap)
{
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    if (hsnap != 0)
    {
        inst->x = round(inst->x/hsnap)*hsnap;
    }
    if (vsnap != 0)
    {
        inst->y = round(inst->y/vsnap)*vsnap;
    }
}

inline void action_snap(const double hor, const double vert)
{
    move_snap(hor, vert);
}
