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

//#include <string>
//#include <string.h>
#include <vector>
#include <math.h>
//#include <iostream>
#include <float.h> //maxiumum values for certain datatypes. Useful for minx = DBL_MAX
//using namespace std;
//#include "../Graphics_Systems/graphics_mandatory.h"
//#include "../libEGMstd.h"

#include "pathstruct.h"


namespace enigma {
	path** pathstructarray;
	extern size_t path_idmax;
}


namespace enigma
{
    static inline double llen(const path_point* p0, const path_point* p1)
    {
        double x0 = (p1->x-p0->x), y0 = (p1->y-p0->y);
        return sqrt(x0*x0+y0*y0);
    }

    static inline double blen(const path_point* p0, const path_point* p1, const path_point* p2)
    {
      /*if (p0 == p1) return llen(p0,p1); else if (p1 == p2) return llen(p1,p2);
      double ax,bx,ay,by;
      //std::cout << "p0=" << p0 << " p1=" <<  p1 << " p2=" << p2 << std::endl;
      ax = p0->x - 2*p1->x + p2->x;
      ay = p0->y - 2*p1->y + p2->y;
      bx = 2*p1->x - 2*p0->x;
      by = 2*p1->y - 2*p0->y;
      float A = 4*(ax*ax + ay*ay);
      float B = 4*(ax*bx + ay*by);
      float C = bx*bx + by*by;

      float Sabc = 2*sqrt(A+B+C);
      float A_2 = sqrt(A);
      float A_32 = 2*A*A_2;
      float C_2 = 2*sqrt(C);
      float BA = B/A_2;
      float BAC2 = BA+C_2;
      BAC2 = BAC2==0?1:BAC2;
      std::cout << "A_2=" << A_2 << " BA+C_2=" <<  BAC2 << " 4*A_32" << 4*A_32 << std::endl;

      return ( A_32*Sabc + A_2*B*(Sabc-C_2) + (4*C*A-B*B)*log( (2*A_2+BA+Sabc)/(BAC2)))/(4*A_32);*/
      double x1=p0->x, y1=p0->y,
            x2=p1->x, y2=p1->y,
            x3=p2->x, y3=p2->y,
            length=0,lx=0.5*(x1+x2), ly=0.5*(y1+y2),x,y,dx,dy;

      double t=0;
      for (int i=0; i<=20; i++){
        x = 0.5 * (((x1 - 2 * x2 + x3) * t + 2 * x2 - 2 * x1) * t + x1 + x2);
        y = 0.5 * (((y1 - 2 * y2 + y3) * t + 2 * y2 - 2 * y1) * t + y1 + y2);
        length += hypot(x-lx,y-ly);
        lx = x, ly = y;
        //std::cout << "i: " << i << " t: " << t << "length: " << length << " x: " << x << " y: " << x << " dx: " << dx << " dy: " << dy << std::endl;
        t += 0.05;
      }
      return length;
    };

    path::path(unsigned pathid, bool smooth, bool closed, int precision, unsigned pointcount):
        id(pathid), precision(precision), smooth(smooth), closed(closed), pointarray(), total_length(0)
    {
        pathstructarray[pathid] = this;
        pathstructarray[pathid]->pointarray.reserve(pointcount);
        if (enigma::path_idmax < pathid+1)
          enigma::path_idmax = pathid+1;
    }
    path::~path() { pathstructarray[id] = NULL; }

    void path_add_point(unsigned pathid, double x, double y, double speed)
    {
        path_point point={x,y,speed};
        pathstructarray[pathid]->pointarray.push_back(point);
    }

    void path_recalculate(unsigned pathid)
    {
        path* const pth = pathstructarray[pathid];
        if (!pth) return;
        pth->total_length = 0; pth->pointoffset.clear();
        if (!pth->pointarray.size()) return;

        const size_t pc = pth->pointarray.size();
        const path_point& start = pth->closed ? pth->pointarray[pc-1] : pth->pointarray[0];
        const path_point& end  =  pth->closed ? pth->pointarray[0] : pth->pointarray[pc-1];
        const path_point& beforestart  =  pth->closed ? pth->pointarray[pc-2] : pth->pointarray[0];
        const path_point& afterend  =  pth->closed ? pth->pointarray[1] : pth->pointarray[pc-1];
        double minx=DBL_MAX,miny=DBL_MAX,maxx=-DBL_MAX,maxy=-DBL_MAX;
        for (size_t i = 0; i < pc; i++)
        {
          const double len =
          (pth->smooth) ?
            //blen(i==0 ? &beforestart : (i==1 ? &start : &pth->pointarray[i-2]), i==0 ? &start : &pth->pointarray[i-1], &pth->pointarray[i])
            //blen(&pth->pointarray[i], i==pc-1 ? &end : &pth->pointarray[i+1], i==pc-2 ? &afterend : (i==pc-1 ? &end : &pth->pointarray[i+2]))
            blen(i==0 ? &start : &pth->pointarray[i-1], &pth->pointarray[i], i+1==pc ? &end : &pth->pointarray[i+1])
            //blen((i==0 || i==1) ? &start : &pth->pointarray[i-2], (i==0) ? &start : &pth->pointarray[i-1], i+1==pc ? &end : &pth->pointarray[i])
          :
            llen(i==0 ? &start : &pth->pointarray[i-1], &pth->pointarray[i]);
          pth->pointarray[i].length = len;
          pth->total_length += len;

          minx = pth->pointarray[i].x<minx ? pth->pointarray[i].x : minx;
          miny = pth->pointarray[i].y<miny ? pth->pointarray[i].y : miny;
          maxx = pth->pointarray[i].x>maxx ? pth->pointarray[i].x : maxx;
          maxy = pth->pointarray[i].y>maxy ? pth->pointarray[i].y : maxy;
        }

        pth->centerx = minx + (maxx-minx)/2;
        pth->centery = miny + (maxy-miny)/2;

        double position = 0;
        for (size_t i = 0; i < pc; i++)
        {
          //if (pth->pointarray[i].length) {
            pth->pointoffset[position/pth->total_length] = i;
            position += pth->pointarray[i].length;
            //std::cout << "Position: " << position << " i: " << i << " length: " << pth->pointarray[i].length << std::endl;
          //}
        }
        //std::cout << "size of pointoffset: " << pth->pointoffset.size() << std::endl;
        //for (ppi_t i = pth->pointoffset.begin(); i != pth->pointoffset.end(); i++) std::cout << i->first << "=>" << i->second << std::endl;
    }

    void pathstructarray_reallocate()
    {
        enigma::path** pathold = pathstructarray;
        pathstructarray = new path*[enigma::path_idmax+2];
        for (size_t i = 0; i < enigma::path_idmax; i++) pathstructarray[i] = pathold[i]; delete[] pathold;
    }

    /// Returns an iterator to the point in @param path whose segment covers @param position.
    inline ppi_t path_point_iterator_at(path* pth, double position) {
      return --pth->pointoffset.upper_bound(position);
    }
    inline int path_point_index_at(path* pth, double position) {
      return path_point_iterator_at(pth, position)->second;
    }

    void path_getXY(path *pth, double &x, double &y, double position)
    {
      if (!pth) return;
      if (!pth->pointarray.size()) return;
      ppi_t ppi = path_point_iterator_at(pth, position);
      double t = position - ppi->first;
      t /= double(pth->pointarray[ppi->second].length) / pth->total_length;
      int sid = ppi->second;
      double x1, x2, x3, y1, y2, y3;
      //std::cout << "t on path: " << position << " first: " << ppi->first << " second: " << ppi->second << " Sid: " << sid << " delta:" << t << std::endl;
      const size_t pc = pth->pointarray.size();
      const path_point& start = pth->closed ? pth->pointarray[pc-1] : pth->pointarray[0];
      const path_point& end  =  pth->closed ? pth->pointarray[0] : pth->pointarray[pc-1];
      if (sid==0)
          x1=start.x, y1=start.y;
      else
          x1=pth->pointarray[sid-1].x, y1=pth->pointarray[sid-1].y;

      x2=pth->pointarray[sid].x, y2=pth->pointarray[sid].y;

        //std::cout<<pth->pointarray.size()<<std::endl;
      if (pth->pointarray.size()==1){x=x1; y=y1; return;}
      else if (pth->pointarray.size()==2 && x1==x2 && y1==y2){x=x1; y=y1; return;}

      if (pth->smooth && (pth->pointarray.size()>2 || pth->closed)){
          if (sid+1==pc)
              x3=end.x, y3=end.y;
          else
              x3=pth->pointarray[sid+1].x, y3=pth->pointarray[sid+1].y;
          x = 0.5 * (((x1 - 2 * x2 + x3) * t + 2 * x2 - 2 * x1) * t + x1 + x2);
          y = 0.5 * (((y1 - 2 * y2 + y3) * t + 2 * y2 - 2 * y1) * t + y1 + y2);
      }else{
          x = x1+(x2-x1)*t;
          y = y1+(y2-y1)*t;
      }
    }

    void path_getspeed(path *pth, double &speed, double position)
    {
      if (!pth) return;
      if (!pth->pointarray.size()) return;
      ppi_t ppi = path_point_iterator_at(pth, position);
      double t = position - ppi->first;
      t /= double(pth->pointarray[ppi->second].length) / pth->total_length;
      int sid = ppi->second;
      double x1, x2, x3;
      //std::cout << "t on path: " << position << " first: " << ppi->first << " Sid: " << sid << " delta:" << t << std::endl;
      const size_t pc = pth->pointarray.size();
      const path_point& start = pth->closed ? pth->pointarray[pc-1] : pth->pointarray[0];
      const path_point& end  =  pth->closed ? pth->pointarray[0] : pth->pointarray[pc-1];
      if (sid==0)
          x1=start.speed;
      else
          x1=pth->pointarray[sid-1].speed;
      x2=pth->pointarray[sid].speed;
      if (pth->smooth){
          if (sid+1==pc)
              x3=end.speed;
          else
              x3=pth->pointarray[sid+1].speed;
          speed = 0.5 * (((x1 - 2 * x2 + x3) * t + 2 * x2 - 2 * x1) * t + x1 + x2);
      }else{
          speed = x1+(x2-x1)*t;
      }
    }

    /// Allocates and zero-fills the path array at game start
    void paths_init()
    {
        pathstructarray = new path*[enigma::path_idmax+1];
        for (unsigned i = 0; i < enigma::path_idmax; i++)
            pathstructarray[i] = NULL;
    }
}
