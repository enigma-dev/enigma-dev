/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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

/*
bool action_if_variable(const variant& variable, const variant& value, int operation);
void action_move_to(double xx,double yy);
void action_create_object(int object, double x, double y);
void action_set_score(double newscore);
void action_set_life(double newlives);
void action_sound(int sound,int loop);
void action_color(int color);
bool action_if_number(int object, double number, int operation);
void action_kill_object();
void action_set_vspeed(double newvspeed);
void action_draw_score(double x,double y, string caption);
void action_draw_sprite(int sprite,double x,double y, int subimage);
void action_set_health(double value);
void action_draw_health(double x1,double y1,double x2,double y2,double backColor,double barColor);
void action_draw_life_images(double x,double y, int image);
bool action_if_dice(int sides);
void action_move_point(double x,double y,double speed);

//Relocate
void move_towards_point ( double point_x, double point_y, double newspeed );*/

inline bool action_if_variable(const variant& variable, const variant& value, int operation) {
    if (operation==0) return (variable==value);
    else if (operation==1) return (variable<value);
    else if (operation==2) return (variable>value);
    else return 0; //invalid operation
}

inline void action_move_to(double xx,double yy) {
    if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+=xx;
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+=yy;
    }
    else {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x=xx;
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y=yy;
	}
}

inline void action_create_object(int object, double x, double y) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        instance_create(x+inst->x, y+inst->y, object);
    }
    else instance_create(x, y, object);
}

inline void action_set_score(double newscore) { 
    if (argument_relative) score+= (int)newscore;
    else score = (int)newscore;
}

inline void action_set_life(double newlives) {
    if (argument_relative) lives+= (int)newlives;
    else lives = newlives; 
} 

inline void action_set_caption(int score,string scoreCaption,int lives,string livesCaption, int health, string healthCaption) {
    show_score=score;
    caption_score=scoreCaption;
    show_lives=lives;
    caption_lives=livesCaption;
    show_health=0;
    caption_health=healthCaption;
}

inline void action_sound(int sound,int loop) {
	if (loop==0) sound_play(sound);
	else sound_loop(sound);
}

inline void action_color(int color) {
	draw_set_color(color);
}

inline bool action_if_number(int object, double number, int operation) {
	if (operation == 0) //equal to
		return (instance_number(object) == number);
	else if (operation == 1) //less than
		return (instance_number(object) < number);
	else if (operation == 2) //greater than
		return (instance_number(object) > number);
	 else return 0; //invalid operation
}

inline void action_kill_object() { instance_destroy(); }

inline void action_set_vspeed(double newvspeed) {
    if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed+=newvspeed;
    } else
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=newvspeed;
}

inline void action_draw_score(double x,double y, string caption) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(x+inst->x,y+inst->y,caption+string(score));
    } else draw_text(x,y,caption+string(score));
}

inline void action_draw_sprite(int sprite,double x,double y, int subimage) {
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

inline void action_draw_health(double x1,double y1,double x2,double y2,double backColor,double barColor) {
   // printf("barColor: %f",barColor);
    double realbar1, realbar2;
    if (barColor==0) { realbar1=c_green; realbar2=c_red;  //green to red
    }
    else if (barColor==1) { realbar1=c_white; realbar2=c_black;  //white to black
    }
    else if (barColor==2) { realbar1=c_black; realbar2=c_black;  //white to black
    }
    else if (barColor==3) { realbar1=c_gray; realbar2=c_gray;  
    }
    else if (barColor==4) { realbar1=c_silver; realbar2=c_silver;  
    }
    else if (barColor==5) { realbar1=c_white; realbar2=c_white;  
    }
    else if (barColor==6) { realbar1=c_maroon; realbar2=c_maroon;  
    }
    else if (barColor==7) { realbar1=c_green; realbar2=c_green; 
    }
    else if (barColor==8) { realbar1=c_olive; realbar2=c_olive;  
    }
    else if (barColor==9) { realbar1=c_navy; realbar2=c_navy;  
    }
    else if (barColor==10) { realbar1=c_purple; realbar2=c_purple;  
    }
    else if (barColor==11) { realbar1=c_teal; realbar2=c_teal;  
    }
    else if (barColor==12) { realbar1=c_red; realbar2=c_red;  
    }
    else if (barColor==13) { realbar1=c_lime; realbar2=c_lime;  
    }
    else if (barColor==14) { realbar1=c_yellow; realbar2=c_yellow;  
    }
    else if (barColor==15) { realbar1=c_blue; realbar2=c_blue;  
    }
    else if (barColor==16) { realbar1=c_fuchsia; realbar2=c_fuchsia;  
    }
    else if (barColor==17) { realbar1=c_aqua; realbar2=c_aqua; 
    }
    else {
        //default
        realbar1=c_green; realbar2=c_red;
    }
	if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_healthbar(x1+inst->x, y1+inst->y, x2+inst->x, y2+inst->y, health, backColor, realbar2, realbar1, 1, 1, 1);
	} else
        draw_healthbar(x1, y1, x2, y2, health, backColor, realbar2, realbar1, 1, 1, 1);
}

inline void action_draw_life_images(double x,double y, int image) {
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

/*inline bool action_if_dice(int sides) {
    if (sides!=0)
        return !(random(1) < 1/sides);
    else return false;
}*/

inline bool action_if_dice(int sides) {return !(floor(random(sides)));}

inline void move_towards_point ( double point_x, double point_y, double newspeed ) {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
    inst->direction = point_direction ( inst->x,inst->y, (point_x), (point_y) );
    inst->speed = (newspeed);
} //RELOCATE ME

inline void action_move_point(double x,double y,double speed) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        move_towards_point(x+inst->x,y+inst->y,speed); } //is speed also relative?
    else move_towards_point(x,y,speed);
}


inline bool action_if(double x) { 
    return x >= .5; 
}

inline bool action_if_object(int object, double xx, double yy) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        return place_meeting(inst->x+xx,inst->y+yy,object);
    }
    else {
        return place_meeting(xx,yy,object);
    }
    return 0;
}

inline bool action_if_next_room() {
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

inline bool action_if_aligned(double snapHor, double snapVer) { 
	return place_snapped(snapHor, snapVer);
}

inline bool action_if_empty(double xx, double yy, int objects) {
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
    return 0;
}

inline void action_move_start() {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
	inst->x=inst->xstart;
	inst->y=inst->ystart;
}

void action_execute_script(string script,string argument0,string argument1,string argument2,string argument3,string argument4) {}
#define action_execute_script(script,argument0,argument1,argument2,argument3,argument4) script((argument0),(argument1),(argument2),(argument3),(argument4))

inline void action_draw_rectangle(double x1,double y1,double x2,double y2,int filled) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle(x1+inst->x,y1+inst->y,x2+inst->x,y2+inst->y,filled);
    }
    else draw_rectangle(x1,y1,x2,y2,filled);
}

void action_set_alarm(int steps,int alarmno);
#define action_set_alarm(steps,alarmno) if (argument_relative) alarm[(alarmno)] += (steps); else alarm[(alarmno)] = (steps);

void action_sprite_set(double spritep,double subimage, double speed) {
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->sprite_index=spritep;
	if (subimage !=-1) inst->image_index=subimage;
	inst->image_speed=speed;
}
//#define action_sprite_set(spritep,subimg, imgspeed) sprite_index=(spritep); if (subimg !=-1) {image_index=subimg;} image_speed=imgspeed;



