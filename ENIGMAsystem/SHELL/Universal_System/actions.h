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
//#include <string>

using namespace std;

void action_another_room(int room, int transition) {
	//transition_kind=transition;
	room_goto(room);
} 
/*void action_bounce(int precisely, int against) {}
void action_cd_pause() {}
void action_cd_play(double firstTrack,double lastTrack) {}
void action_cd_playing() {}
void action_cd_present() {}
void action_cd_resume() {}
void action_cd_stop() {}
void action_change_object(int changeInto, int performEvents) {}
*/
void action_color(int color) {
	draw_set_color(color);
}
void action_create_object(int object, double x, double y) {
	instance_create(x, y, object);
}
/*void action_create_object_motion(int object,double x, double y,double speed, double direction) {}
 
void action_create_object_random(int object1, int object2, int object3,int object4, double x, double y) {
 instance_create(x,y,choose(object1,object2,object3,object4));
 }
 */
void action_current_room(int transition) {
	//transition_kind=0;
	room_restart();
}
/*void action_draw_arrow(double x1, double y1, double x2, double y2, double tipSize) {}
void action_draw_background(int background,double x, double y, int tiled) {}
*/
void action_draw_ellipse(double x1, double y1, double x2, double y2, int filled) {
	draw_ellipse(x1, y1, x2, y2, filled);
}
void action_draw_ellipse_gradient(double x1,double y1,double x2,double y2, int color1, int color2) {
	//draw_ellipse_color(x1, y1, x2, y2, color1, color2, false);
}
void action_draw_gradient_hor(double x1,double y1,double x2,double y2,double color1,double color2) {
	draw_rectangle_color(x1, y1, x2, y2, color1, color2, color1, color2, false);
}
void action_draw_gradient_vert(double x1,double y1,double x2,double y2,double color1,double color2) {
	draw_rectangle_color(x1, y1, x2, y2, color1, color1, color2, color2, false);
}
void action_draw_health(double x1,double y1,double x2,double y2,double backColor,double barColor) {
	//draw_healthbar(x1, y1, x2, y2, health, backColor, 0, 0, 1, 1, 1);
}
/*void action_draw_life(double x,double y, string caption) {}
void action_draw_life_images(double x,double y, int image) {}
void action_draw_line(double x1,double y1,double x2,double y2) {}
void action_draw_rectangle(double x1,double y1,double x2,double y2,int filled) {}
void action_draw_score(double x,double y, string caption) {}
void action_draw_sprite(int sprite,double x,double y, int subimage) {}
void action_draw_text(string text,double x,double y) {}
void action_draw_text_transformed(string text,double x,double y,double horScale,double verScale, double angle) {}
void action_draw_variable(string variable,double x,double y) {}
void action_effect(int type,double x,double y,double size,double color,int where) {}
*/
void action_end_game() {
//game_end();
}
void action_end_sound(int sound) {
	//sound_stop(sound);
}
/*void action_execute_script(string script,double argument0,double argument1,double argument2,double argument3,double argument4) {}
void action_font(int font,int align) {}
*/
void action_fullscreen(int action) {
//action : switch, window, fullscreen (need to find out exact numbers, just guessed)
/*	if (action ==0) {window_set_fullscreen(!window_get_fullscreen());}
	if (action ==1) {window_set_fullscreen(false);}
	if (action ==2) {window_set_fullscreen(true);}*/
}
/*void action_highscore_clear() {}
void action_highscore_show(int background,int border,double newColor,double otherColor,string font) {}
void action_inherited() { event_inherited(); }
*/
void action_kill_object() { instance_destroy(); }
/*void action_kill_position(double x,double y) {}
void action_linear_step(double x,double y,double speed,int stopAt) {}
void action_load_game(string filename) {}
*/
void action_message(string message) {
	show_message(message);
}
/*void action_move(string directions,double speed) {}
void action_move_contact(double direction,double maximum, int against) {}
void action_move_point(double x,double y,double speed) {}
void action_move_random(double snapHor,double snapVer) {} */

void action_move_start() {
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x=((enigma::object_graphics*)enigma::instance_event_iterator->inst)->xstart;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y=((enigma::object_graphics*)enigma::instance_event_iterator->inst)->ystart;
}

void action_move_to(double xx,double yy) {
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x=xx;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y=yy;
}


void action_next_room(int transition) {
//transition_kind=transition;
	room_goto_next();
}
/*void action_partemit_burst(int emitter,int type,double amount) {}
void action_partemit_create(int emitter,int shape,double xleft,double xright,double ytop,double ybottom) {}
void action_partemit_destroy(int emitter) {}
void action_partemit_stream(int emitter,int type,int amount) {}
void action_partsyst_clear() {}
void action_partsyst_create(double depth) {}
void action_partsyst_destroy() {}
void action_parttype_color(int type,int shape,double color1,double color2,double startalpha,double endalpha) {}
void action_parttype_create(int type,int shape,int sprite, double minsize,double maxsize, double increment) {}
void action_parttype_gravity(int type,double amount,double direction) {}
void action_parttype_life(int type,double minlife,double maxlife) {}
void action_parttype_secondary(int primary,int stepType,double stepCount,int deathType,double deathCount) {}
void action_parttype_speed(int type,double minSpeed,double maxSpeed,double minDir,double maxDir,double friction) {}
void action_path(int path,double speed,int atEnd,double position) {}
void action_path_end() {}
void action_path_position(double position) {}
void action_path_speed(double speed) {}
void action_potential_step(double x,double y,double speed,int avoid) {}
*/
void action_previous_room(int transition) {
	//transition_kind=transition;
	//room_goto_previous();
}
/*
void action_replace_background(int background,string filename) {}
void action_replace_sound(int sound, string filename) {}
void action_replace_sprite(int sprite,string filename,int images) {}
void action_restart_game() { game_restart(); } */

void action_reverse_xdir() {
((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed=-((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed;
}
void action_reverse_ydir() { 
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=-((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed;
}

/*void action_save_game(string filename) {}*/

void action_set_alarm(int steps,int alarmno) {
	//((enigma::object_graphics*)enigma::instance_event_iterator->inst)->alarm[alarmno]=steps;
}
/*
void action_set_caption(int score,string scoreCaption,int lives,string livesCaption, int health, string healthCaption) {
 show_score=score;
 caption_score=scoreCaption;
 show_lives=lives;
 caption_lives=livesCaption;
 show_health=0;
 caption_health=healthCaption;
 }
void action_set_cursor(int sprite, int cursor) { cursor_sprite=sprite;}*/

void action_set_friction(double newfriction) {
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->friction=newfriction;
}

void action_set_gravity(double direction,double newgravity) { 
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity_direction=direction;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity=newgravity;
}

//void action_set_health(double value) { health=value;}

void action_set_hspeed(double newhspeed) {
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed=newhspeed;
}

//void action_set_life(double newlives) {lives = newlives; } 

void action_set_motion(double newdirection,double newspeed) {
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->direction=newdirection;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->speed=newspeed;
}

//extern int score;
//void action_set_score(double newscore) { score = (int)newscore;}

/*
 void action_set_timeline(int timeline, double position) {}
void action_set_timeline_position(double position) {}
*/
 void action_set_vspeed(double newvspeed) {
	 ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=newvspeed;
 }
/* void action_show_info() {}
void action_show_video(string filename,int fullscreen,int loop) { splash_show_video(filename,fullscreen,loop); } */
void action_sleep(double milliseconds,int redraw) {
	sleep(milliseconds);
}

/* void action_snap(double hor,double vert) { move_snap(hor,vert); }
 */
void action_snapshot(string filename) { screen_save(filename); }

void action_sound(int sound,int loop) {
	if (loop==0) sound_play(sound);
	else sound_loop(sound);
}

void action_sprite_color(double color, double  opacity) {
	//image_blend=color; image_alpha = opacity;
	//((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_blend=color;
	//((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_alpha=opacity;
	
	
}

void action_sprite_set(double spritep,double subimage, double speed) {
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->sprite_index=spritep;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index=subimage;
	//((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_speed=speed;
}


void action_sprite_transform(double hscale, double vscale, double rotation, int mirror) {
	//image_xscale=hscale; image_yscale=vscale; image_angle=rotation; imageyscale=-1;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_xscale=hscale;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_yscale=vscale;
	((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_angle=rotation;
	//need to do mirror
	
}

/*
void action_webpage(string address) {}

void action_wrap(int direction) { move_wrap(0,0,0);}


///
// if functions need to actually be functions
///

//void action_if(x) (x)
bool action_if(bool expression) {return expression; }
bool action_if_aligned(double snapHor, double snapVer) { return 0;}
bool action_if_collision(double x, double y, int objects) {return 0;}
bool action_if_dice(int sides) {return 0;}
bool action_if_empty(double x, double y,int object) {return 0;}
bool action_if_health(double value, int operation) {return 0;}
bool action_if_life(double value, int operation) {return 0;}
bool action_if_mouse(int button) {return 0;}
bool action_if_next_room() {return 0;}
bool action_if_number(int object, double number, int operation) {return 0;}
bool action_if_object(int object, double x, double y) {return 0;}
bool action_if_previous_room() {return 0;}
bool action_if_question(string question) {return 0;}
bool action_if_score(double value, int operation) {return 0;}
bool action_if_sound(int sound) {return 0;}
bool action_if_variable(var variable, double value, int operation) {return 0;}
 */
