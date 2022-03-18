/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
**  Modified 2013 by Josh Ventura                                               **
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
#include <string>
#include <vector>
#include <cmath>
#include <map>
using namespace std;

//#include "Graphics_Systems/OpenGL/OpenGLHeaders.h" //For drawing straight lines
#include "../Paths/pathstruct.h"
#include "libEGMstd.h"
#include "motion_planning_struct.h"
#include "motion_planning.h"
#include "Collision_Systems/General/CSfuncs.h"
#include "Universal_System/scalar.h"

namespace enigma {
	extern size_t grid_idmax;
	extern unsigned bound_texture;
}

namespace enigma_user
{

unsigned mp_grid_create(int left,int top,int hcells,int vcells,int cellwidth,int cellheight, double speed_modifier)
{
    enigma::gridstructarray_reallocate();
    new enigma::grid(enigma::grid_idmax, left, top, hcells, vcells, cellwidth, cellheight, 1, speed_modifier);
    return enigma::grid_idmax-1;
}

void mp_grid_destroy(unsigned id)
{
    delete enigma::gridstructarray[id];
}

unsigned mp_grid_duplicate(unsigned id)
{
    enigma::gridstructarray_reallocate();
    enigma::gridstructarray[enigma::grid_idmax] = new enigma::grid(*enigma::gridstructarray[id]);
    return enigma::grid_idmax-1;
}

void mp_grid_copy(unsigned id, unsigned srcid)
{
    enigma::grid *grid = enigma::gridstructarray[id];
    enigma::grid *sgrid = enigma::gridstructarray[srcid];

    grid->nodearray.clear();
    grid->nodearray.reserve(sgrid->hcells*sgrid->vcells);
    grid->vcells = sgrid->vcells;
    grid->hcells = sgrid->hcells;
    grid->cellwidth = sgrid->cellwidth;
    grid->cellheight = sgrid->cellheight;
    grid->speed_modifier = sgrid->speed_modifier;
    grid->threshold = sgrid->threshold;
    grid->left = sgrid->left;
    grid->top = sgrid->top;
    for (unsigned int i = 0; i < sgrid->hcells*sgrid->vcells; i++)
    {
        enigma::node node(floor(i / sgrid->vcells),i % sgrid->vcells,0,0,0,sgrid->nodearray[i].cost);
        grid->nodearray.push_back(node);
    }

    for (unsigned int i = 0; i < grid->hcells; i++){
        for (unsigned int c = 0; c < grid->vcells; c++){
        if (i>0){
            grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[(i-1)*grid->vcells+c]); //left
            if (c>0)
                grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[(i-1)*grid->vcells+c-1]); //top-left
            if (c<grid->vcells-1)
                grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[(i-1)*grid->vcells+c+1]); //bottom-left
        }
        if (c>0)
            grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[i*grid->vcells+c-1]); //top
        if (i<grid->hcells-1){
            grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[(i+1)*grid->vcells+c]); //right
            if (c>0)
                grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[(i+1)*grid->vcells+c-1]); //top-right
            if (c<grid->vcells-1)
                grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[(i+1)*grid->vcells+c+1]); //bottom-bottom
        }
        if (c<grid->vcells-1)
            grid->nodearray[i*grid->vcells+c].neighbor_nodes.push_back(&grid->nodearray[i*grid->vcells+c+1]); //bottom

        }
    }
}

void mp_grid_clear_all(unsigned id, unsigned cost)
{
    for (vector<enigma::node>::iterator it = enigma::gridstructarray[id]->nodearray.begin(); it!=enigma::gridstructarray[id]->nodearray.end(); ++it)
        (*it).cost = cost;
    enigma::gridstructarray[id]->threshold = cost;
}

void mp_grid_clear_cell(unsigned id,int h,int v, unsigned cost)
{
    enigma::gridstructarray[id]->nodearray[h*enigma::gridstructarray[id]->vcells+v].cost = cost;
    if (enigma::gridstructarray[id]->threshold<cost){enigma::gridstructarray[id]->threshold=cost;}
}

void mp_grid_add_rectangle(unsigned id,double x1,double y1,double x2,double y2, unsigned cost)
{
    enigma::grid *grid = enigma::gridstructarray[id];
    unsigned max_cost=0;
    double x=grid->left, y=grid->top;
    x1-=x, x2-=x, y1-=y, y2-=y;
    unsigned int tx1 = fmax(floor(fmin(x1,x2)/grid->cellwidth),0), tx2 = fmin(ceil((fmin(x1,x2)+fabs(x2-x1))/grid->cellwidth),grid->hcells),
    ty1 = fmax(floor(fmin(y1,y2)/grid->cellheight),0), ty2 = fmin(ceil((fmin(y1,y2)+fabs(y2-y1))/grid->cellheight),grid->vcells);
    for (unsigned int i=tx1; i<tx2; i++){
        for (unsigned int c=ty1; c<ty2; c++){
            if (grid->nodearray[i*grid->vcells+c].cost>max_cost){max_cost=grid->nodearray[i*grid->vcells+c].cost;}
            grid->nodearray[i*grid->vcells+c].cost = cost;
        }
    }
    if (cost>max_cost){max_cost=cost;}
    if (grid->threshold<max_cost){grid->threshold=max_cost;}
}

void mp_grid_add_instances(unsigned id,int obj,bool prec,unsigned cost)
{
    enigma::grid *grid = enigma::gridstructarray[id];
    unsigned max_cost=0;
    double x=grid->left, y=grid->top;
    for (unsigned int i=0; i<grid->hcells; i++){
        for (unsigned int c=0; c<grid->vcells; c++){
            if (grid->nodearray[i*grid->vcells+c].cost>max_cost){max_cost=grid->nodearray[i*grid->vcells+c].cost;}
            if (collision_rectangle(x+i*grid->cellwidth,y+c*grid->cellheight,x+(i+1)*grid->cellwidth,y+(c+1)*grid->cellheight,obj,prec,false)!=-4){
                grid->nodearray[i*grid->vcells+c].cost = cost;
            }
        }
    }
    if (cost>max_cost){max_cost=cost;}
    if (grid->threshold<max_cost){grid->threshold=max_cost;}
}

void mp_grid_reset_threshold(unsigned id)
{
    enigma::grid *grid = enigma::gridstructarray[id];
    unsigned max_cost=0;
    for (vector<enigma::node>::iterator it = grid->nodearray.begin(); it!=grid->nodearray.end(); ++it)
        if ((*it).cost>max_cost){max_cost=(*it).cost;}
    grid->threshold=max_cost;
}

void mp_grid_clear_rectangle(unsigned id,double x1,double y1,double x2,double y2, unsigned cost)
{
    mp_grid_add_rectangle(id,x1,y1,x2,y2,cost);
}

void mp_grid_add_cell(unsigned id,int h,int v, unsigned cost)
{
    unsigned max_cost=enigma::gridstructarray[id]->nodearray[h*enigma::gridstructarray[id]->vcells+v].cost;
    enigma::gridstructarray[id]->nodearray[h*enigma::gridstructarray[id]->vcells+v].cost = cost;
    if (cost>max_cost){max_cost=cost;}
    if (enigma::gridstructarray[id]->threshold<max_cost){enigma::gridstructarray[id]->threshold=max_cost;}
}

unsigned mp_grid_get_cell(unsigned id,int h,int v)
{
    return enigma::gridstructarray[id]->nodearray[h*enigma::gridstructarray[id]->vcells+v].cost;
}

unsigned mp_grid_get_threshold(unsigned id)
{
    return enigma::gridstructarray[id]->threshold;
}

void mp_grid_set_threshold(unsigned id, unsigned value)
{
    enigma::gridstructarray[id]->threshold = value;
}

double mp_grid_get_speed_modifier(unsigned id)
{
    return enigma::gridstructarray[id]->speed_modifier;
}

void mp_grid_set_speed_modifier(unsigned id, double value)
{
    enigma::gridstructarray[id]->speed_modifier = value;
}

bool mp_grid_path(unsigned id,unsigned pathid,double xstart,double ystart,double xgoal,double ygoal,bool allowdiag)
{
    enigma::grid *gr = enigma::gridstructarray[id];
    int vc = int(enigma::gridstructarray[id]->vcells),
    xs = floor((xstart-gr->left)/int(gr->cellwidth)), ys = floor((ystart-gr->top)/int(gr->cellheight)),
    xg = floor((xgoal-gr->left)/int(gr->cellwidth)), yg = floor((ygoal-gr->top)/int(gr->cellheight));
    if (xs<0 or xg<0) return false;
    if (xs>int(gr->hcells)-1 or xg>int(gr->hcells)-1) return false;
    if (ys<0 or yg<0) return false;
    if (ys>int(gr->vcells)-1 or yg>int(gr->vcells)-1) return false;
    
    bool status = true; //status to check if we can reach the destination
    multimap<unsigned,enigma::node*> nodelist = enigma::find_path(id, &gr->nodearray[xs*vc+ys], &gr->nodearray[xg*vc+yg], allowdiag, status);
    enigma::path *path = enigma::pathstructarray[pathid];
    path->pointarray.clear();

    //push the very first point
    enigma::path_point point(xstart,ystart,gr->speed_modifier/double(gr->nodearray[xs*vc+ys].cost));
    path->pointarray.push_back(point);
    multimap<unsigned,enigma::node*>::reverse_iterator it;
    for (it=nodelist.rbegin(); it != nodelist.rend(); it++)
    {
            point = enigma::path_point(gr->left+((*it).second->x+0.5)*gr->cellwidth,gr->top+((*it).second->y+0.5)*gr->cellheight,gr->speed_modifier/double((*it).second->cost));
            path->pointarray.push_back(point);
    }

    //push the very last point if we can reach the destination
    if (status == true){
        point = enigma::path_point(xgoal,ygoal,gr->speed_modifier/double(gr->nodearray[xg*vc+yg].cost));
        path->pointarray.push_back(point);
    } else if (path->pointarray.size()==1) {
        point = enigma::path_point(path->pointarray.back().x,path->pointarray.back().y,gr->speed_modifier/double(gr->nodearray[xg*vc+yg].cost));
        path->pointarray.push_back(point);
    }
    enigma::path_recalculate(pathid);
    return true;
}

}

#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolors.h"

#include "Universal_System/var4.h"

namespace enigma_user
{

void mp_grid_draw_neighbours(unsigned int id, unsigned int h, unsigned int v, unsigned int mode)
{
    enigma::grid *grid = enigma::gridstructarray[id];
    if (h>grid->hcells-1) return;
    if (v>grid->vcells-1) return;
    draw_primitive_begin(8);
    unsigned int vc = enigma::gridstructarray[id]->vcells;
    for (vector<enigma::node*>::iterator it = enigma::gridstructarray[id]->nodearray[h*vc+v].neighbor_nodes.begin(); it!=enigma::gridstructarray[id]->nodearray[h*vc+v].neighbor_nodes.end(); ++it){
        draw_vertex_color(grid->left+(*it)->x*grid->cellwidth,grid->top+(*it)->y*grid->cellheight,0x0000FF,(mode==0?0.5:1.0));
        draw_vertex_color(grid->left+((*it)->x+1)*grid->cellwidth,grid->top+(*it)->y*grid->cellheight,0x0000FF,(mode==0?0.5:1.0));
        draw_vertex_color(grid->left+((*it)->x+1)*grid->cellwidth,grid->top+((*it)->y+1)*grid->cellheight,0x0000FF,(mode==0?0.5:1.0));
        draw_vertex_color(grid->left+(*it)->x*grid->cellwidth,grid->top+((*it)->y+1)*grid->cellheight,0x0000FF,(mode==0?0.5:1.0));
    }
    draw_primitive_end();
    if (mode==1){
        int tc = draw_get_color();
        draw_set_color_rgba(255,255,255,1);
        for (vector<enigma::node*>::iterator it = enigma::gridstructarray[id]->nodearray[h*vc+v].neighbor_nodes.begin(); it!=enigma::gridstructarray[id]->nodearray[h*vc+v].neighbor_nodes.end(); ++it){
            draw_text(((*it)->x+0.5)*grid->cellwidth,((*it)->y+0.5)*grid->cellheight,(*it)->x*grid->vcells+(*it)->y);
        }
        draw_set_color(tc);
    }
}

void mp_grid_draw(unsigned int id, unsigned int mode, unsigned int color_mode)
{
    enigma::grid *grid = enigma::gridstructarray[id];
    draw_primitive_begin(8);
    int x=grid->left, y=grid->top;
    for (unsigned int i=0; i<grid->hcells; i++){
        for (unsigned int c=0; c<grid->vcells; c++){
            int col;
            if (color_mode==0){
                if (grid->nodearray[i*grid->vcells+c].cost < grid->threshold)
                    col = 0x008000;
                else
                    col = 0x0000FF;
            }else{
                col = merge_color(0x008000, 0x0000FF, (float)grid->nodearray[i*grid->vcells+c].cost/(float)color_mode);
            }

            draw_vertex_color(x+i*grid->cellwidth,y+c*grid->cellheight,col,1.0);
            draw_vertex_color(x+(i+1)*grid->cellwidth,y+c*grid->cellheight,col,1.0);
            draw_vertex_color(x+(i+1)*grid->cellwidth,y+(c+1)*grid->cellheight,col,1.0);
            draw_vertex_color(x+i*grid->cellwidth,y+(c+1)*grid->cellheight,col,1.0);
        }
    }
    draw_primitive_end();

    if (mode){
        int tc = draw_get_color();
        draw_set_color_rgba(255,255,255,1);
        if (mode==1){ //draw cost
            for (unsigned int i=0; i<grid->hcells; i++){
                for (unsigned int c=0; c<grid->vcells; c++){
                    draw_text(x+(i+0.5)*grid->cellwidth,y+(c+0.5)*grid->cellheight,grid->nodearray[i*grid->vcells+c].cost);
                }
            }
        }else{ //draw coords
            for (unsigned int i=0; i<grid->hcells; i++){
                for (unsigned int c=0; c<grid->vcells; c++){
                    draw_text(x+(i+0.5)*grid->cellwidth,y+(c+0.5)*grid->cellheight,toString(evariant(grid->nodearray[i*grid->vcells+c].x)) + ":" + toString(evariant(grid->nodearray[i*grid->vcells+c].y)));
                }
            }
        }
        draw_set_color(tc);
    }
}

}
