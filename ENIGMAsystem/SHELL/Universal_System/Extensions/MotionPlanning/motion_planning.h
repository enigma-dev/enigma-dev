/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Gr�nbergs                                         **
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

namespace enigma_user {
unsigned mp_grid_create(int left,int top,int hcells,int vcells,int cellwidth,int cellheight, double speed_modifier = 1);
void mp_grid_destroy(unsigned id);
unsigned mp_grid_duplicate(unsigned id);
void mp_grid_copy(unsigned id, unsigned srcid);
unsigned mp_grid_get_cell(unsigned id,int h,int v);
void mp_grid_draw(unsigned id, unsigned mode = 0, unsigned color_mode = 0);
void mp_grid_draw_neighbours(unsigned int id, unsigned int h, unsigned int v, unsigned int mode = 0);
bool mp_grid_path(unsigned id,unsigned path,double xstart,double ystart,double xgoal,double ygoal,bool allowdiag);
void mp_grid_clear_all(unsigned id, unsigned cost = 1);
void mp_grid_clear_cell(unsigned id,int h,int v, unsigned cost = 1);
void mp_grid_clear_rectangle(unsigned id,double x1,double y1,double x2,double y2, unsigned cost = 1);
void mp_grid_add_rectangle(unsigned id,double x1,double y1,double x2,double y2, unsigned cost = 50000);
void mp_grid_add_cell(unsigned id,int h,int v, unsigned cost = 50000);
void mp_grid_add_instances(unsigned id,int obj,bool prec, unsigned cost = 50000);
unsigned mp_grid_get_threshold(unsigned id);
void mp_grid_set_threshold(unsigned id, unsigned threshold = 1);
void mp_grid_reset_threshold(unsigned id);
double mp_grid_get_speed_modifier(unsigned id);
void mp_grid_set_speed_modifier(unsigned id, double value);
}

