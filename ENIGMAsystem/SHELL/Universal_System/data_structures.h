/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2011 Alasdair Morrison <tgmg@g-java.com>                      **
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

unsigned int ds_grid_create(const unsigned int w, const unsigned int h);
void ds_grid_destroy(const unsigned int id);
void ds_grid_clear(const unsigned int id, const variant val);
void ds_grid_copy(const unsigned int id, const unsigned int source);
void ds_grid_resize(const unsigned int id, const unsigned int w, const unsigned int h);
unsigned int ds_grid_width(const unsigned int id);
unsigned int ds_grid_height(const unsigned int id);
void ds_grid_set(const unsigned int id, const unsigned int x, const unsigned int y, const variant val);
void ds_grid_add(const unsigned int id, const unsigned int x, const unsigned int y, const variant val);
void ds_grid_multiply(const unsigned int id, const unsigned int x, const unsigned int y, const double val);
void ds_grid_set_region(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
void ds_grid_add_region(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
void ds_grid_multiply_region(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const double val);
void ds_grid_set_disk(const unsigned int id, const double x, const double y, const double r, const variant val);
void ds_grid_add_disk(const unsigned int id, const double x, const double y, const double r, const variant val);
void ds_grid_multiply_disk(const unsigned int id, const double x, const double y, const double r, const double val);
void ds_grid_set_grid_region(const unsigned int id, const unsigned int source, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int xpos, const unsigned int ypos);
void ds_grid_add_grid_region(const unsigned int id, const unsigned int source, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int xpos, const unsigned int ypos);
void ds_grid_multiply_grid_region(const unsigned int id, const unsigned int source, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int xpos, const unsigned int ypos);
variant ds_grid_get(const unsigned int id, const unsigned int x, const unsigned int y);
variant ds_grid_get_sum(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_max(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_min(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_mean(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_disk_sum(const unsigned int id, const double x, const double y, const double r);
variant ds_grid_get_disk_max(const unsigned int id, const double x, const double y, const double r);
variant ds_grid_get_disk_min(const unsigned int id, const double x, const double y, const double r);
variant ds_grid_get_disk_mean(const unsigned int id, const double x, const double y, const double r);
bool ds_grid_value_exists(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
int ds_grid_value_x(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
int ds_grid_value_y(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
bool ds_grid_value_disk_exists(const unsigned int id, const double x, const double y, const double r, const variant val);
bool ds_grid_value_disk_x(const unsigned int id, const double x, const double y, const double r, const variant val);
bool ds_grid_value_disk_y(const unsigned int id, const double x, const double y, const double r, const variant val);
void ds_grid_shuffle(const unsigned int id);
bool ds_grid_exists(const unsigned int id);
unsigned int ds_grid_duplicate(const unsigned int source);

unsigned int ds_map_create();
void ds_map_destroy(const unsigned int id);
void ds_map_clear(const unsigned int id);
void ds_map_copy(const unsigned int id, const unsigned int source);
unsigned int ds_map_size(const unsigned int id);
bool ds_map_empty(const unsigned int id);
void ds_map_add(const unsigned int id, const variant key, const variant val);
void ds_map_replace(const unsigned int id, const variant key, const variant val);
void ds_map_delete(const unsigned int id, const variant key);
void ds_map_delete(const unsigned int id, const variant first, const variant last);
bool ds_map_exists(const unsigned int id, const variant key);
variant ds_map_find_value(const unsigned int id, const variant key);
variant ds_map_find_previous(const unsigned int id, const variant key);
variant ds_map_find_next(const unsigned int id, const variant key);
variant ds_map_find_first(const unsigned int id);
variant ds_map_find_last(const unsigned int id);
bool ds_map_exists(const unsigned int id);
unsigned int ds_map_duplicate(const unsigned int source);

unsigned int ds_list_create();
void ds_list_destroy(const unsigned int id);
void ds_list_clear(const unsigned int id);
void ds_list_copy(const unsigned int id, const unsigned int source);
unsigned int ds_list_size(const unsigned int id);
bool ds_list_empty(const unsigned int id);
void ds_list_add(const unsigned int id, const variant val);
void ds_list_insert(const unsigned int id, const unsigned int pos, const variant val);
void ds_list_replace(const unsigned int id, const unsigned int pos, const variant val);
void ds_list_delete(const unsigned int id, const unsigned int pos);
void ds_list_delete(const unsigned int id, const unsigned int first, const unsigned int last);
int ds_list_find_index(const unsigned int id, const variant val);
variant ds_list_find_value(const unsigned int id, const unsigned int pos);
void ds_list_sort(const unsigned int id, const bool ascend);
void ds_list_shuffle(const unsigned int id);
bool ds_list_exists(const unsigned int id);
unsigned int ds_list_duplicate(const unsigned int source);

unsigned int ds_priority_create();
void ds_priority_destroy(const unsigned int id);
void ds_priority_clear(const unsigned int id);
void ds_priority_copy(const unsigned int id, const unsigned int source);
unsigned int ds_priority_size(const unsigned int id);
bool ds_priority_empty(const unsigned int id);
void ds_priority_add(const unsigned int id, const variant val, const variant prio);
void ds_priority_change_priority(const unsigned int id, const variant val, const variant prio);
variant ds_priority_find_priority(const unsigned int id, const variant val);
void ds_priority_delete_value(const unsigned int id, const variant val);
bool ds_priority_value_exists(const unsigned int id, const variant val);
variant ds_priority_delete_min(const unsigned int id);
variant ds_priority_find_min(const unsigned int id);
variant ds_priority_delete_max(const unsigned int id);
variant ds_priority_find_max(const unsigned int id);
bool ds_priority_exists(const unsigned int id);
unsigned int ds_priority_duplicate(const unsigned int source);

unsigned int ds_queue_create();
void ds_queue_destroy(const unsigned int id);
void ds_queue_clear(const unsigned int id);
void ds_queue_copy(const unsigned int id, const unsigned int source);
unsigned int ds_queue_size(const unsigned int id);
bool ds_queue_empty(const unsigned int id);
void ds_queue_enqueue(const unsigned int id, const variant val);
variant ds_queue_dequeue(const unsigned int id);
variant ds_queue_head(const unsigned int id);
variant ds_queue_tail(const unsigned int id);
bool ds_queue_exists(const unsigned int id);
unsigned int ds_queue_duplicate(const unsigned int source);

unsigned int ds_stack_create();
void ds_stack_destroy(const unsigned int id);
void ds_stack_clear(const unsigned int id);
void ds_stack_copy(const unsigned int id, const unsigned int source);
unsigned int ds_stack_size(const unsigned int id);
bool ds_stack_empty(const unsigned int id);
void ds_stack_push(const unsigned int id, const variant val);
variant ds_stack_pop(const unsigned int id);
variant ds_stack_top(const unsigned int id);
bool ds_stack_exists(const unsigned int id);
unsigned int ds_stack_duplicate(const unsigned int source);
