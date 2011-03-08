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
	if (operation == 1) //less than
		return (instance_number(object) < number);
	if (operation == 2) //greater than
		return (instance_number(object) > number);
	return 0;
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
	if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_healthbar(x1+inst->x, y1+inst->y, x2+inst->x, y2+inst->y, health, backColor, barColor, barColor, 1, 1, 1);
	} else
        draw_healthbar(x1, y1, x2, y2, health, backColor, barColor, barColor, 1, 1, 1);
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