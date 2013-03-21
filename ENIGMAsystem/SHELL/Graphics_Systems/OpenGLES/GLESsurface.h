/** Copyright (C) 2008-2013 Josh Ventura, Dave "biggoron"
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

/**Surface functions***********************************************************\

int surface_create(double width, double height)
int surface_set_target(int id)
int surface_reset_target(void)
int surface_destroy(int id)
int draw_surface(double id, double x, double y)

\******************************************************************************/


int surface_create(double width, double height);
int surface_set_target(int id);
int surface_reset_target(void);
int surface_destroy(int id);
int draw_surface(double id, double x, double y);
