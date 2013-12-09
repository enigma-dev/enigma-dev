/******************************************************************************

 ENIGMA
 Copyright (C) 2008-2013 Enigma Strike Force

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#ifndef ERT_OBJECT_HPP_
#define ERT_OBJECT_HPP_

#include "ert/real.hpp"
#include "ert/property.hpp"

namespace ert {
  struct object {
    object(unsigned long id, real_t x, real_t y);
    virtual ~object();
    
    const unsigned long id;
    const real_t xstart;
    const real_t ystart;
    
    real_t x;
    real_t y;
    real_t xprevious;
    real_t yprevious;
    
    virtual unsigned long get_object_index() = 0;
    property_ro<object, unsigned long, &object::get_object_index> object_index();
    
    virtual bool object_is_solid() = 0;
    virtual bool object_is_visible() = 0;
    virtual bool object_is_persistent() = 0;
    virtual real_t object_depth() = 0;
    virtual real_t object_sprite_index() = 0;
    virtual real_t object_mask_index() = 0;
    
    struct object_properties {
      bool solid;
      bool visible;
      bool persistent;
      real_t depth;
      real_t sprite_index;
      real_t mask_index;
      real_t image_alpha;
      real_t image_angle;
      real_t image_blend;
      real_t image_index;
      real_t image_speed;
      real_t image_xscale;
      real_t image_yscale;
      real_t friction;
      real_t hfriction;
      real_t vfriction;
      real_t gravity;
      real_t hgravity;
      real_t vgravity;
      real_t gravity_direction;
      real_t hspeed;
      real_t vspeed;
      real_t speed;
      real_t direction;
    } properties;
    
    bool get_solid();
    void set_solid(bool);
    property<object, bool, &object::get_solid, &object::set_solid> solid();
    
    bool get_visible();
    void set_visible(bool);
    property<object, bool, &object::get_visible, &object::set_visible> visible();
    
    bool get_persistent();
    void set_persistent(bool);
    property<object, bool, &object::get_persistent, &object::set_persistent> persistent();
    
    real_t get_depth();
    void set_depth(real_t);
    property<object, real_t, &object::get_depth, &object::set_depth> depth();
    
    real_t get_sprite_index();
    void set_sprite_index(real_t);
    property<object, real_t, &object::get_sprite_index, &object::set_sprite_index> sprite_index();
    
    real_t get_sprite_width();
    property_ro<object, real_t, &object::get_sprite_width> sprite_width();
    
    real_t get_sprite_height();
    property_ro<object, real_t, &object::get_sprite_height> sprite_height();
    
    real_t get_sprite_xoffset();
    property_ro<object, real_t, &object::get_sprite_xoffset> sprite_xoffset();
    
    real_t get_sprite_yoffset();
    property_ro<object, real_t, &object::get_sprite_yoffset> sprite_yoffset();
    
    real_t get_image_alpha();
    void set_image_alpha(real_t);
    property<object, real_t, &object::get_image_alpha, &object::set_image_alpha> image_alpha();
    
    real_t get_image_angle();
    void set_image_angle(real_t);
    property<object, real_t, &object::get_image_angle, &object::set_image_angle> image_angle();
    
    real_t get_image_blend();
    void set_image_blend(real_t);
    property<object, real_t, &object::get_image_blend, &object::set_image_blend> image_blend();
    
    real_t get_image_index();
    void set_image_index(real_t);
    property<object, real_t, &object::get_image_index, &object::set_image_index> image_index();
    
    real_t get_image_number();
    property_ro<object, real_t, &object::get_image_number> image_number();
    
    real_t get_image_speed();
    void set_image_speed(real_t);
    property<object, real_t, &object::get_image_speed, &object::set_image_speed> image_speed();
    
    real_t get_image_xscale();
    void set_image_xscale(real_t);
    property<object, real_t, &object::get_image_xscale, &object::set_image_xscale> image_xscale();
    
    real_t get_image_yscale();
    void set_image_yscale(real_t);
    property<object, real_t, &object::get_image_yscale, &object::set_image_yscale> image_yscale();
    
    real_t get_mask_index();
    void set_mask_index(real_t);
    property<object, real_t, &object::get_mask_index, &object::set_mask_index> mask_index();
    
    real_t get_bbox_bottom();
    property_ro<object, real_t, &object::get_bbox_bottom> bbox_bottom();
    
    real_t get_bbox_left();
    property_ro<object, real_t, &object::get_bbox_left> bbox_left();
    
    real_t get_bbox_right();
    property_ro<object, real_t, &object::get_bbox_right> bbox_right();
    
    real_t get_bbox_top();
    property_ro<object, real_t, &object::get_bbox_top> bbox_top();
    
    real_t get_direction();
    void set_direction(real_t);
    void update_direction();
    property<object, real_t, &object::get_direction, &object::set_direction> direction();
    
    real_t get_friction();
    void set_friction(real_t);
    void update_friction();
    property<object, real_t, &object::get_friction, &object::set_friction> friction();
    
    real_t get_gravity();
    void set_gravity(real_t);
    void update_gravity();
    property<object, real_t, &object::get_gravity, &object::set_gravity> gravity();
    
    real_t get_gravity_direction();
    void set_gravity_direction(real_t);
    property<object, real_t, &object::get_gravity_direction, &object::set_gravity_direction> gravity_direction();
    
    real_t get_hspeed();
    void set_hspeed(real_t);
    property<object, real_t, &object::get_hspeed, &object::set_hspeed> hspeed();
    
    real_t get_vspeed();
    void set_vspeed(real_t);
    property<object, real_t, &object::get_vspeed, &object::set_vspeed> vspeed();
    
    real_t get_speed();
    void set_speed(real_t);
    void update_speed();
    property<object, real_t, &object::get_speed, &object::set_speed> speed();
  };
}

#endif // ERT_OBJECT_HPP_
