/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
\*********************************************************************************/

/**Instance functions**********************************************************\

int instance_create(ARG x,ARG2 y,ARG3 object)
int instance_destroy()
bool instance_exists(ARG obj)
int instance_find(ARG obj,ARG2 n)
int instance_number(ARG obj)
int instance_position(ARG x,ARG2 y,ARG3 obj)
int instance_nearest(ARG x,ARG2 y,ARG3 obj)
int instance_furthest(ARG x,ARG2 y,ARG3 obj)


\******************************************************************************/

namespace enigma
{
int destroycalls=0,createcalls=0;
}

int instance_create(double x,double y,int object)
{
    int a=enigma::maxid;
    enigma::maxid++;

    enigma::newinst_id=a;
    enigma::newinst_obj=object;
    enigma::newinst_x=x;
    enigma::newinst_y=y;

    switch (object)
    {
       #include "../Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h"
       default:
            #if SHOWERRORS
            show_error("Object does not exist...",0);
            #endif
            return -1;
    }

    return a;
}

int instance_destroy(double id)
{
    enigma::cleanups[enigma::cleancount++]=(int)id;

    enigma::instancecount--;
    instance_count--;

    return 0;
}

int instance_destroy()
{
    #if SHOWERRORS
    show_error("Nothing to destroy.",0);
    #endif
    return -1;
}

bool instance_exists(int obj)
{
     int instance=(int) obj;
     if (instance>100000)
     {
        return (enigma::instance_list.find(instance) != enigma::instance_list.end());
     }
     else if (instance>=0)
     {
       for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
       {
           if ((*enigma::instance_iterator).second->object_index==instance)
           {
              return 1;
           }
       }
       return 0;
     }
     else
     {
         switch (instance)
         {
            case -1:
                 return (enigma::instance_list.find((int)((*enigma::instance_iterator).second->id)) != enigma::instance_list.end());
                 break;
            case -3:
                 return (enigma::instance_list.size()>0);
                 break;
            default:
                 return 0;
                 break;
         }
     }
     return 0;
}

int instance_find(double obj,double n)
{
   int objind=(int)obj;
   int num=(int)n;
   int nth=0;
   for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
   {
       if ((*enigma::instance_iterator).second->object_index==objind)
       {
          nth++;
          if (nth>num)
          return (int) (*enigma::instance_iterator).second->id;
       }
   }
   return -4;
}
int instance_number(double obj)
{
   int objind=(int)obj;
   int n=0;
   for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
   {
       if ((*enigma::instance_iterator).second->object_index==objind)
       {
          n++;
       }
   }
   return n;
}


int instance_position(double x,double y,double obj)
{
    int objind=(int) obj;
    int xl,yl;
    int xc=(int)x,yc=(int)y;

    for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
    if (((*enigma::instance_iterator).second->object_index==objind) || objind==-3)
    {
       xl=(int)(*enigma::instance_iterator).second->x;
       yl=(int)(*enigma::instance_iterator).second->y;

       if (xl==xc && yl==yc)
       return (int)(*enigma::instance_iterator).second->id;
    }
    }
    return -4;
}

int instance_nearest(double x,double y,double obj)
{
    double dist_lowest=-1,retid=-4;
    int objind=(int) obj;
    int xl,yl;
    double dstclc;

    for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
      if ((*enigma::instance_iterator).second->object_index==objind)
      {
         xl=(int)((*enigma::instance_iterator).second->x-x);
         yl=(int)((*enigma::instance_iterator).second->y-y);
         dstclc=sqrt(xl*xl+yl*yl);
         if (dstclc<dist_lowest || dist_lowest==-1)
         {
            dist_lowest=dstclc;
            retid=(*enigma::instance_iterator).second->id;
         }
      }
    }

    return (int)retid;
}
int instance_furthest(double x,double y,double obj)
{
    double dist_highest=0,retid=0;
    bool found=0;
    int objind=(int) obj;
    int xl,yl;
    double dstclc;

    for (enigma::instance_iterator=enigma::instance_list.begin(); enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
    if (((*enigma::instance_iterator).second->object_index==objind) || objind==-3)
    {
       xl=(int)((*enigma::instance_iterator).second->x-x);
       yl=(int)((*enigma::instance_iterator).second->y-y);
       dstclc=sqrt(xl*xl+yl*yl);
       if (dstclc>dist_highest || found==0)
       {
          dist_highest=dstclc;
          retid=(*enigma::instance_iterator).second->id;
          found=1;
       }
    }
    }

    return (int)retid;
}


//int instance_place(x,y,obj)
//int instance_copy(performevent)
//instance_change(obj,perf)
