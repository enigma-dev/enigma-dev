/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
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
#include <vector>
#include <map>
#include <list>
#include "motion_planning_struct.h"
#include <cmath>
#include <algorithm>
//#include <iostream>
using std::multimap;
using std::pair;
using std::list;

namespace enigma
{
	grid** gridstructarray;
	size_t grid_idmax=0;
}

namespace enigma
{
    grid::grid(unsigned id,int left,int top,int hcells,int vcells,int cellwidth,int cellheight,unsigned threshold,double speed_modifier):
        id(id), left(left), top(top), hcells(hcells), vcells(vcells), cellwidth(cellwidth), cellheight(cellheight), threshold(threshold), speed_modifier(speed_modifier), nodearray()
    {
        gridstructarray[id] = this;
        gridstructarray[id]->nodearray.reserve(hcells*vcells);
        for (size_t i = 0; i < hcells*vcells; i++)
        {
            node node={floor(i / vcells),i % vcells,0,0,0,1};
            gridstructarray[id]->nodearray.push_back(node);
        }

        grid *gr = gridstructarray[id];

        for (size_t i = 0; i < hcells; i++){
            for (size_t c = 0; c < vcells; c++){
            if (i>0){
                gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[(i-1)*vcells+c]); //left
                if (c>0)
                    gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[(i-1)*vcells+c-1]); //top-left
                if (c<vcells-1)
                    gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[(i-1)*vcells+c+1]); //bottom-left
            }
            if (c>0)
                gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[i*vcells+c-1]); //top
            if (i<hcells-1){
                gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[(i+1)*vcells+c]); //right
                if (c>0)
                    gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[(i+1)*vcells+c-1]); //top-right
                if (c<vcells-1)
                    gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[(i+1)*vcells+c+1]); //bottom-bottom
            }
            if (c<vcells-1)
                gr->nodearray[i*vcells+c].neighbor_nodes.push_back(&gr->nodearray[i*vcells+c+1]); //bottom

            }
        }

        if (enigma::grid_idmax < id+1)
          enigma::grid_idmax = id+1;
    }
    grid::~grid() { gridstructarray[id] = NULL; }

    void gridstructarray_reallocate()
    {
        enigma::grid** gridold = gridstructarray;
        gridstructarray = new grid*[enigma::grid_idmax+2];
        for (size_t i = 0; i < enigma::grid_idmax; i++) gridstructarray[i] = gridold[i]; delete[] gridold;
    }

    //Helper functions
    unsigned find_heuristic(const node* n0, const node* n1, bool allow_diag) //Distance from n0 to n1
    {
        if (!allow_diag){
            //std::cout << "Straight " << " H = " << fabs((int)n0->x-(int)n1->x) + fabs((int)n0->y-(int)n1->y) << std::endl;
            return fabs((int)n0->x-(int)n1->x) + fabs((int)n0->y-(int)n1->y);
        }else{
            //std::cout << "Diagn " << " H = " << fmax(fabs((int)n0->x-(int)n1->x) , fabs((int)n0->y-(int)n1->y)) << std::endl;
            return fmax(fabs((int)n0->x-(int)n1->x) , fabs((int)n0->y-(int)n1->y));
        }
    }

    int find_priority(multimap<unsigned,node*> &m, node* value){
        multimap<unsigned,node*>::iterator it;
        for (it = m.begin(); it != m.end(); it++)
            if (it->second == value) return (int)it->first;
        return -1;
    }

    bool check_corners(unsigned id, node* n0, node* n1) //There must be a better way to do this
    {
        //std::cout << "Searching..." << std::endl;
        for (vector<enigma::node*>::iterator it0 = n0->neighbor_nodes.begin(); it0!=n0->neighbor_nodes.end(); ++it0)
        {
            for (vector<enigma::node*>::iterator it1 = n1->neighbor_nodes.begin(); it1!=n1->neighbor_nodes.end(); ++it1)
            {
               // std::cout << (*it0) << " == " << (*it1) << " && it0.cost=" << (*it0)->cost << std::endl;
                if ((*it0)==(*it1) && (*it0)->cost >= gridstructarray[id]->threshold)
                return true;
            }
        }
        return false;
    }

    multimap<unsigned,node*> find_path(unsigned id, node* n0, node* n1, bool allow_diag, bool &status)
    {
        node* current;
        for (size_t i = 0; i < gridstructarray[id]->hcells*gridstructarray[id]->vcells; i++)
        {
            gridstructarray[id]->nodearray[i].F=0;
            gridstructarray[id]->nodearray[i].G=0;
            gridstructarray[id]->nodearray[i].H=0;
            gridstructarray[id]->nodearray[i].came_from=NULL;
        }

        node* start = n0;
        node* destination = n1;
        multimap<unsigned,node*> OPEN;
        list<node*> CLOSED;
        multimap<unsigned,node*> mm_ret;

        if (start == destination)
            return mm_ret;
        start->H = find_heuristic(start,destination,allow_diag);
        start->F = start->H;
        OPEN.insert(pair<unsigned,node*>(start->F,start));

        while (find(CLOSED.begin(), CLOSED.end(), destination) == CLOSED.end() && !OPEN.empty())
        //loop until destination is in the closed list or the open multimap is empty
        {
            //if (OPEN.empty()){break;}
            //multimap<unsigned,node*>::iterator tit;
            /*std::cout << "Begin" << std::endl;
            for ( tit=OPEN.begin() ; tit != OPEN.end(); tit++ )
                std::cout << (*tit).first << " => " << (*tit).second << std::endl;*/

            //this is basically ds_priority_delete_min
            multimap<unsigned,node*>::iterator it = OPEN.begin(), it_check;
            it_check = it++;
            while (it != OPEN.end())
            {
                if ((*it).first < (*it_check).first) {it_check = it;}
                it++;
            }
            current = (*it_check).second;
            OPEN.erase(it_check);
            //std::cout << OPEN.size() << std::endl;
            //std::cout << "End" << std::endl;
            //multimap<unsigned,node*>::iterator tit;
            /*for ( tit=OPEN.begin() ; tit != OPEN.end(); tit++ )
                std::cout << (*tit).first << " => " << (*tit).second << std::endl;*/
            //std::cout << "Ended" << std::endl;

            CLOSED.push_front(current);
            for (vector<enigma::node*>::iterator it = current->neighbor_nodes.begin(); it!=current->neighbor_nodes.end(); ++it)
            { //go trough all the neighbors (should be faster then 8 if cycles like in the init)
                if (!allow_diag && (*it)->x != current->x && (*it)->y != current->y)
                    continue;

                bool cutting = false;
                if (allow_diag == true  && (*it)->x != current->x && (*it)->y != current->y)
                        cutting = check_corners(id, current, (*it));

                if (find(CLOSED.begin(), CLOSED.end(), (*it)) == CLOSED.end() && (*it)->cost<gridstructarray[id]->threshold && cutting == false){
                    if (find_priority(OPEN,(*it)) == -1){ //if the node isn't on the open list
                        (*it)->came_from = current;
                        (*it)->G = current->G + (*it)->cost;
                        if ((*it)->x != current->x && (*it)->y != current->y)
                            (*it)->G += ceil((*it)->cost/2.5); //if it is diagonal increase the move cost
                        (*it)->H = find_heuristic(*it,destination,allow_diag);
                        (*it)->F = (*it)->G + (*it)->H;
                        //std::cout << "Node: " << (*it)->x*gridstructarray[id]->vcells+(*it)->y << " F = " << (*it)->F << " H = " << (*it)->H << " G = " << (*it)->G << std::endl;
                        OPEN.insert(pair<unsigned,node*>((*it)->F,(*it)));

                    }else{ //if the node is already on the open list we just have to see if it is a better path
                        unsigned temp_G = current->G + (*it)->cost;
                        if (temp_G < (*it)->G) {
                            (*it)->came_from = current;
                            (*it)->G = current->G + (*it)->cost;
                            if ((*it)->x != current->x && (*it)->y != current->y)
                                (*it)->G += ceil((*it)->cost/2.5);
                            (*it)->H = find_heuristic(*it,destination,allow_diag);
                            (*it)->F = (*it)->G + (*it)->H;
                        }
                    }
                }
            }
        }
        node *last, *nearest;
        unsigned i = 0;
        status = true;
        if (find(CLOSED.begin(), CLOSED.end(), destination) == CLOSED.end())
        {   //this is for if the destionation can't be found if
            status = false;
            nearest = start;

            list<node*>::iterator it;
            for (it=CLOSED.begin(); it != CLOSED.end(); it++){
                (*it)->H = find_heuristic(*it,destination,allow_diag);
                if ((*it)->H < nearest->H)
                    nearest = *it;
            }
            destination = nearest;
            if (start == destination)
                return mm_ret;
        }

        //mm_ret.insert(pair<unsigned,node*>(i,destination));
        last = destination;
        while (last->came_from != start){
            i += 1;
            mm_ret.insert(pair<unsigned,node*>(i,last->came_from));
            last = last->came_from;
        }

        //delete CLOSED;
        //delete OPEN;
        return mm_ret; //return the multimap containing a list of all the nodes that are in the path.
    }
}
