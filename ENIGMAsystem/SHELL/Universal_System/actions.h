 /*
 *  actions.h
 *  EnigmaXcode
 *
 *  Created by Alasdair Morrison on 08/09/2010.
 *  Copyright 2010 EnigmaDev. All rights reserved.
 *
 */
#include <string>

using namespace std;

/*void action_another_room(int room, int transition) {} 
void action_bounce(int precisely, int against) {}
void action_cd_pause() {}
void action_cd_play(double firstTrack,double lastTrack) {}
void action_cd_playing() {}
void action_cd_present() {}
void action_cd_resume() {}
void action_cd_stop() {}
void action_change_object(int changeInto, int performEvents) {}
void action_color(double color) {}
void action_create_object(int object, double x, double y) {}
void action_create_object_motion(int object,double x, double y,double speed, double direction) {}
void action_create_object_random(int object1, int object2, int object3,int object4, double x, double y) {}
void action_current_room(int transition) {}
void action_draw_arrow(double x1, double y1, double x2, double y2, double tipSize) {}
void action_draw_background(int background,double x, double y, int tiled) {}
void action_draw_ellipse(double x1, double y1, double x2, double y2, int filled) {}
void action_draw_ellipse_gradient(double x1,double y1,double x2,double y2, double color1, double color2) {}
void action_draw_gradient_hor(double x1,double y1,double x2,double y2,double color1,double color2) {}
void action_draw_gradient_vert(double x1,double y1,double x2,double y2,double color1,double color2) {}
void action_draw_health(double x1,double y1,double x2,double y2,double backColor,double barColor) {}
void action_draw_life(double x,double y, string caption) {}
void action_draw_life_images(double x,double y, int image) {}
void action_draw_line(double x1,double y1,double x2,double y2) {}
void action_draw_rectangle(double x1,double y1,double x2,double y2,int filled) {}
void action_draw_score(double x,double y, string caption) {}
void action_draw_sprite(int sprite,double x,double y, int subimage) {}
void action_draw_text(string text,double x,double y) {}
void action_draw_text_transformed(string text,double x,double y,double horScale,double verScale, double angle) {}
void action_draw_variable(string variable,double x,double y) {}
void action_effect(int type,double x,double y,double size,double color,int where) {}
void action_end_game() {}
void action_end_sound(int sound) {}
void action_execute_script(string script,double argument0,double argument1,double argument2,double argument3,double argument4) {}
void action_font(int font,int align) {}
void action_fullscreen(int action) {}
void action_highscore_clear() {}
void action_highscore_show(int background,int border,double newColor,double otherColor,string font) {}
void action_inherited() {}
void action_kill_object() {}
void action_kill_position(double x,double y) {}
void action_linear_step(double x,double y,double speed,int stopAt) {}
void action_load_game(string filename) {}
void action_message(string message) {}
void action_move(string directions,double speed) {}
void action_move_contact(double direction,double maximum, int against) {}
void action_move_point(double x,double y,double speed) {}
void action_move_random(double snapHor,double snapVer) {}
void action_move_start() {}
void action_move_to(double x,double y) {}
void action_next_room(int transition) {}
void action_partemit_burst(int emitter,int type,double amount) {}
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
void action_previous_room(int transition) {}
void action_replace_background(int background,string filename) {}
void action_replace_sound(int sound, string filename) {}
void action_replace_sprite(int sprite,string filename,int images) {}
void action_restart_game() {}
void action_reverse_xdir() {}
void action_reverse_ydir() {}
void action_save_game(string filename) {}
void action_set_alarm(int steps,int alarm) {}
void action_set_caption(int score,string scoreCaption,int lives,string livesCaption, int health, string healthCaption) {}
void action_set_cursor(int sprite, int cursor) {}
void action_set_friction(double friction) {}
void action_set_gravity(double direction,double gravity) {}
void action_set_health(double value) {}
void action_set_hspeed(double hspeed) {}
void action_set_life(double lives) {}
void action_set_motion(double direction,double speed) {}
void action_set_score(double score) {}
void action_set_timeline(int timeline, double position) {}
void action_set_timeline_position(double position) {}
void action_set_vspeed(double vspeed) {}
void action_show_info() {}
void action_show_video(string filename,int fullscreen,int loop) {}
void action_sleep(double milliseconds,int redraw) {}
void action_snap(double hor,double vert) {}
void action_snapshot(string filename) {}
*/
void action_sound(int sound,int loop) {
	if (loop==0) sound_play(sound);
	else sound_loop(sound);
}
/*
void action_sprite_color(double color, double  opacity) {}

void action_sprite_set(double spritep,double subimage, double speed) {}

void action_sprite_transform(double hscale, double vscale, double rotation, int mirror) {}

void action_webpage(string address) {}

void action_wrap(int direction) {}


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
