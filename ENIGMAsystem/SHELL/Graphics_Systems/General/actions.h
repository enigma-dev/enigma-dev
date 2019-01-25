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

#include "libEGMstd.h"

#include "Universal_System/scalar.h"
#include "Universal_System/GAME_GLOBALS.h" // TODO: Do away with this sloppy infestation permanently!
#include "Universal_System/instance_system_base.h"
#include "Universal_System/lives.h"

namespace enigma_user
{
extern bool argument_relative;

inline void action_color(const int color) {
	draw_set_color(color);
}

inline void action_draw_rectangle(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const int filled) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle(x1+inst->x,y1+inst->y,x2+inst->x,y2+inst->y,filled);
    }
    else draw_rectangle(x1,y1,x2,y2,filled);
}

inline void action_sprite_set(const double spritep, const  double subimage, const double speed) {
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->sprite_index=spritep;
	if ((int)subimage !=-1) inst->image_index=subimage;
	inst->image_speed=speed;
}

inline void action_draw_text(const string text, const gs_scalar x, const gs_scalar y) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(x+inst->x,y+inst->y,text); }
    else {
        draw_text(x,y,text);
    }
}

inline void action_font(const int font, const int align) {
    draw_set_font(font);
    // draw_set_halign(align);
}

#define action_message(message) show_message(message)

inline void action_draw_arrow(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const gs_scalar tipSize) {
    draw_arrow(x1, y1, x2, y2, tipSize, 1, false);
}

inline void action_draw_background(const int background, const gs_scalar x, const gs_scalar y, const int tiled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        (tiled ? draw_background_tiled : draw_background)(background, inst->x+x, inst->y+y,16777215,1);
    }
    else
      (tiled ? draw_background_tiled : draw_background)(background, x, y,16777215,1);
}

inline void action_draw_ellipse(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const int filled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, filled);
    }
    else
        draw_ellipse(x1, y1, x2, y2, filled);
}

inline void action_draw_ellipse_gradient(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, false);
    }
    else
        draw_ellipse_color(x1, y1, x2, y2, color1, color2, false);
}

inline void action_draw_gradient_hor(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, color1, color2, false);
    }
    else
        draw_rectangle_color(x1, y1, x2, y2, color1, color2, color1, color2, false);
}

inline void action_draw_gradient_vert(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color1, color2, color2, false);
    }
    else
        draw_rectangle_color(x1, y1, x2, y2, color1, color1, color2, color2, false);
}

inline void action_draw_score(const gs_scalar x, const gs_scalar y, const string caption) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(x+inst->x,y+inst->y,caption+string(score));
    } else draw_text(x,y,caption+string(score));
}

inline void action_draw_sprite(const int sprite, const gs_scalar x, const gs_scalar y, const int subimage) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_sprite(sprite,subimage,x+inst->x,y+inst->y);
    }
    else
        draw_sprite(sprite,subimage,x,y);
}

void action_draw_health(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const double backColor, const int barColor);
void action_draw_health(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2, const double backColor, const int barColor) {
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
        draw_healthbar(x1+inst->x, y1+inst->y, x2+inst->x, y2+inst->y, health, backColor, realbar2, realbar1, 0, 1, 1);
	}
	else
        draw_healthbar(x1, y1, x2, y2, health, backColor, realbar2, realbar1, 0, 1, 1);
}

inline void action_draw_life(const gs_scalar x, const gs_scalar y, const string caption)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(inst->x+x, inst->y+y, caption + string(lives));
    }
    else
        draw_text(x, y, caption + string(lives));
}

inline void action_draw_life_images(const gs_scalar x, const gs_scalar y, const int image) {
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

inline void action_draw_line(const gs_scalar x1, const gs_scalar y1, const gs_scalar x2, const gs_scalar y2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_line(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2);
    }
    else
        draw_line(x1, y1, x2, y2);
}

inline void action_draw_text_transformed(const string text, const gs_scalar x, const gs_scalar y, const gs_scalar horScale, const gs_scalar verScale, const double angle)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text_transformed(inst->x+x, inst->y+y, text, horScale, verScale, angle);
    }
    else
        draw_text_transformed(x, y, text, horScale, verScale, angle);
}

inline void action_draw_variable(variant variable, const gs_scalar x, const gs_scalar y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(inst->x+x, inst->y+y, *variable);
    }
    else
        draw_text(x, y, string(variable));
}

inline bool action_replace_sprite(int ind, std::string filename, int imgnumb)
{
    return sprite_replace(ind, filename, imgnumb, true, false, 0, 0, true);
}

inline bool action_replace_background(int ind, std::string filename)
{
    return background_replace(ind, filename, true, false, true, true);
}

inline int draw_self()
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    draw_sprite_ext(inst->sprite_index, inst->image_index, inst->x, inst->y, inst->image_xscale, inst->image_yscale, inst->image_angle, inst->image_blend, inst->image_alpha);
    return 0;
}  //actions seemed the best place for this

inline int action_draw_self()
{
    return draw_self();
}

inline void action_sprite_transform(gs_scalar xscale, gs_scalar yscale, double angle, int mirror)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    inst->image_xscale = (mirror==1 || mirror==3)?-xscale:xscale;
    inst->image_yscale = (mirror==2 || mirror==3)?-yscale:yscale;
    inst->image_angle = angle;
}

inline void action_sprite_color(int color, double alpha)
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    inst->image_blend = color;
    inst->image_alpha = alpha;
}

inline void set_automatic_draw(bool enable)
{
    automatic_redraw = enable;
}

inline void draw_enable_drawevent(bool enable)
{
    automatic_redraw = enable;
}

}
