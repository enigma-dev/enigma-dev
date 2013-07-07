/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#include "../General/OpenGLHeaders.h"
#include "GLd3d.h"
#include "../General/GStextures.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "../General/GLbinding.h"
#include "Widget_Systems/widgets_mandatory.h" // show_error

#include "GLmodel.h"

#include "Universal_System/estring.h"

#include <map>
#include <list>
#include "Universal_System/fileio.h"

using namespace std;

#define GETR(x) ((x & 0x0000FF))/255.0
#define GETG(x) ((x & 0x00FF00)>>8)/255.0
#define GETB(x) ((x & 0xFF0000)>>16)/255.0

#include <floatcomp.h>

class d3d_model
{
    int something;
    list<GLuint> primitive_calls;
    GLuint model;

    public:
    d3d_model(): something(100) {model = glGenLists(1);}
    ~d3d_model() {}

    void clear()
    {
        while (!primitive_calls.empty())
        {
            glDeleteLists(primitive_calls.front(), 1);
            primitive_calls.pop_front();
        }
        primitive_calls.clear();
        glDeleteLists(model, 1);
        model = glGenLists(1);
    }

    void save(string fname)
    {
    }//TODO: format needs to be decided on

    bool load(string fname)  //TODO: this needs to be rewritten properly not using the file_text functions
    {
        using namespace enigma_user;
        int file = file_text_open_read(fname);
        if (file == -1)
            return false;
        something = file_text_read_real(file);
        if (something != 100)
            return false;
        file_text_readln(file);
        file_text_read_real(file);  //don't see the use in this value, it doesn't equal the number of calls left exactly
        file_text_readln(file);
        int kind;
        float v[3], n[3], t[2];
        double col, alpha;
        while (!file_text_eof(file))
        {
            switch (int(file_text_read_real(file)))
            {
                case  0:
                    kind = file_text_read_real(file);
                    model_primitive_begin(kind);
                    break;
                case  1:
                    model_primitive_end();
                    break;
                case  2:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     model_vertex(v);
                    break;
                case  3:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_color(v,col,alpha);
                    break;
                case  4:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     model_vertex_texture(v,t);
                    break;
                case  5:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_texture_color(v,t,col,alpha);
                    break;
                case  6:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     model_vertex_normal(v,n);
                    break;
                case  7:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_normal_color(v,n,col,alpha);
                    break;
                case  8:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     model_vertex_normal_texture(v,n,t);
                    break;
                case  9:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_normal_texture_color(v,n,t,col,alpha);
                    break;
                case  10:
                    model_block(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file), true);
                    break;
                case  11:
                    model_cylinder(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),ftrueGM(file_text_read_real(file)),file_text_read_real(file));
                    break;
                case  12:
                    model_cone(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),ftrueGM(file_text_read_real(file)),file_text_read_real(file));
                    break;
                case  13:
                    model_ellipsoid(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
                case  14:
                    model_wall(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
                case  15:
                    model_floor(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
                default:
                  #if DEBUG_MODE
                  show_error("Error loading model: invalid solid type", false);
                  #endif
                  break;
            }
            file_text_readln(file);
        }
        file_text_close(file);
        return true;
    }

    void draw(double x, double y, double z, int texId)
    {
        texture_reset();
        texture_use(get_texture(texId));
        glPushAttrib(GL_CURRENT_BIT);
        glLoadIdentity();
        glLoadMatrixd(projection_matrix);
        glMultMatrixd(transformation_matrix);
        glTranslatef(x, y, z);
        glCallList(model);
        glLoadIdentity();
        glLoadMatrixd(projection_matrix);
        glMultMatrixd(transformation_matrix);
        glPopAttrib();
    }

    void model_primitive_begin(int kind)
    {
        GLuint index = glGenLists(1);
        primitive_calls.push_back(index);
        glNewList(index, GL_COMPILE);
        glBegin(ptypes_by_id[kind]);
    }

    void model_primitive_end()
    {
        glEnd();
        glEndList();
        glDeleteLists(model, 1);
        model = glGenLists(1);
        glNewList(model, GL_COMPILE);
        list<GLuint>::iterator i;
        for (i = primitive_calls.begin(); i != primitive_calls.end(); i++)
        {
            glCallList(*i);
        }
        glEndList();
    }

    void model_vertex(float v[])
    {
        glVertex3fv(v);
    }

    void model_vertex_color(float v[], int col, double alpha)
    {
        glColor4f(GETR(col), GETG(col), GETB(col), alpha);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }

    void model_vertex_texture(float v[], float t[])
    {
        glTexCoord2fv(t);
        glVertex3fv(v);
    }

    void model_vertex_texture_color(float v[], float t[], int col, double alpha)
    {
        glColor4f(GETR(col), GETG(col), GETB(col), alpha);
        glTexCoord2fv(t);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }

    void model_vertex_normal(float v[], float n[])
    {
        glNormal3fv(n);
        glVertex3fv(v);
    }

    void model_vertex_normal_color(float v[], float n[], int col, double alpha)
    {
        glColor4f(GETR(col), GETG(col), GETB(col), alpha);
        glNormal3fv(n);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }

    void model_vertex_normal_texture(float v[], float n[], float t[])
    {
        glTexCoord2fv(t);
        glNormal3fv(n);
        glVertex3fv(v);
    }

    void model_vertex_normal_texture_color(float v[], float n[], float t[], int col, double alpha)
    {
        glColor4f(GETR(col), GETG(col), GETB(col), alpha);
        glTexCoord2fv(t);
        glNormal3fv(n);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }
    //TODO: Model basic shapes need to be changed to the basic new oens
    void model_block(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed)
    {
        float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y1, z1}, v3[] = {x2, y1, z2},
              v4[] = {x2, y2, z1}, v5[] = {x2, y2, z2}, v6[] = {x1, y2, z1}, v7[] = {x1, y2, z2},
              t0[] = {0, vrep}, t1[] = {0, 0}, t2[] = {hrep, vrep}, t3[] = {hrep, 0},
              t4[] = {hrep*2, vrep}, t5[] = {hrep*2, 0}, t6[] = {hrep*3, vrep}, t7[] = {hrep*3, 0},
              t8[] = {hrep*4, vrep}, t9[] = {hrep*4, 0},
	      n0[] = {-0.5, -0.5, -0.5}, n1[] = {-0.5, -0.5, 0.5}, n2[] = {-0.5, 0.5, -0.5}, n3[] = {-0.5, 0.5, 0.5},
              n4[] = {0.5, 0.5, -0.5}, n5[] = {0.5, 0.5, 0.5}, n6[] = {0.5, -0.5, -0.5}, n7[] = {0.5, -0.5, 0.5};

        model_primitive_begin(GL_TRIANGLE_STRIP);
        model_vertex_normal_texture(v0,n0,t0);
        model_vertex_normal_texture(v1,n1,t1);
        model_vertex_normal_texture(v6,n2,t2);
        model_vertex_normal_texture(v7,n3,t3);
        model_vertex_normal_texture(v4,n4,t4);
        model_vertex_normal_texture(v5,n5,t5);
        model_vertex_normal_texture(v2,n6,t8);
        model_vertex_normal_texture(v3,n7,t9);
        model_vertex_normal_texture(v0,n0,t6);
        model_vertex_normal_texture(v1,n1,t7);
        model_primitive_end();
        if (closed)
        {
            model_primitive_begin(GL_TRIANGLE_STRIP);
            model_vertex_normal_texture(v0,n4,t0);
            model_vertex_normal_texture(v2,n6,t1);
            model_vertex_normal_texture(v6,n2,t2);
            model_vertex_normal_texture(v4,n0,t3);
            model_primitive_end();
            model_primitive_begin(GL_TRIANGLE_STRIP);
            model_vertex_normal_texture(v1,n1,t0);
            model_vertex_normal_texture(v3,n7,t1);
            model_vertex_normal_texture(v7,n3,t2);
            model_vertex_normal_texture(v5,n5,t3);
            model_primitive_end();
        }
    }

    void model_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
    {
        float v[100][3];
        float t[100][3];
        steps = min(max(steps, 3), 48);
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
        double a, px, py, tp;
        int k;
        model_primitive_begin(GL_TRIANGLE_STRIP);
        a = 0; px = cx+rx; py = cy; tp = 0; k = 0;
        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = z2;
            t[k][0] = tp; t[k][1] = 0;
            model_vertex_texture(v[k],t[k]);
            k++;
            v[k][0] = px; v[k][1] = py; v[k][2] = z1;
            t[k][0] = tp; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        model_primitive_end();
        if (closed)
        {
            model_primitive_begin(GL_TRIANGLE_FAN);
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
            t[k][0] = 0; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++;
            for (int i = 0; i <= steps*2; i+=2)
            {
                model_vertex_texture(v[i+1],t[i]);
            }
            model_primitive_end();

            model_primitive_begin(GL_TRIANGLE_FAN);
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
            t[k][0] = 0; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++;
            for (int i = 0; i <= steps*2; i+=2)
            {
                model_vertex_texture(v[i],t[i]);
            }
            model_primitive_end();
        }
    }

    void model_cone(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
    {
        float v[51][3];
        float t[100][3];
        steps = min(max(steps, 3), 48);
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
        double a, px, py, tp;
        int k = 0;
        model_primitive_begin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
        t[k][0] = 0; t[k][1] = 0;
        model_vertex_texture(v[k],t[k]);
        k++;
        a = 0; px = cx+rx; py = cy; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = z1;
            t[k][0] = tp; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        model_primitive_end();
        if (closed)
        {
            model_primitive_begin(GL_TRIANGLE_FAN);
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
            t[k][0] = 0; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++;
            tp = 0;
        for (int i = steps + 1; i >= 0; i--)
        {
                t[k][0] = tp; t[k][1] = 0;
                model_vertex_texture(v[i],t[k]);
                k++; tp += invstep;
            }
            model_primitive_end();
        }
    }

    void model_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, int steps)
    {
        float v[277][3];
        float t[277][3];
        steps = min(max(steps, 3), 24);
        const int zsteps = ceil(steps/2);
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
        double a, b, px, py, pz, tp, tzp, cosb;
        double cosx[25], siny[25], txp[25];
        a = pr; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            cosx[i] = cos(a)*rx; siny[i] = sin(a)*ry;
            txp[i] = tp;
            a += pr; tp += invstep;
        }
        int k = 0, kk;
        model_primitive_begin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = cz - rz;
        t[k][0] = 0; t[k][1] = vrep;
        model_vertex_texture(v[k],t[k]);
        k++;
        b = qr-M_PI/2;
        cosb = cos(b);
        pz = rz*sin(b);
        tzp = vrep-invstep2;
        px = cx+rx*cosb; py = cy;
        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
            t[k][0] = txp[i]; t[k][1] = tzp;
            model_vertex_texture(v[k],t[k]);
            k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
        }
        model_primitive_end();
        for (int ii = 0; ii < zsteps - 2; ii++)
        {
            b += qr;
            cosb = cos(b);
            pz = rz*sin(b);
            tzp -= invstep2;
            model_primitive_begin(GL_TRIANGLE_STRIP);
            px = cx+rx*cosb; py = cy;
            for (int i = 0; i <= steps; i++)
            {
                kk = k - steps - 1;
                model_vertex_texture(v[kk],t[kk]);
                v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
                t[k][0] = txp[i]; t[k][1] = tzp;
                model_vertex_texture(v[k],t[k]);
                k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
            }
            model_primitive_end();
        }
        model_primitive_begin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = cz + rz;
        t[k][0] = 0; t[k][1] = 0;
        model_vertex_texture(v[k],t[k]);
        k++;
        for (int i = k - steps - 2; i <= k - 2; i++)
        {
            model_vertex_texture(v[i],t[i]);
        }
        model_primitive_end();
    }

    void model_icosahedron()
    {

    }

    void model_torus(double x1, double y1, double z1, double hrep, double vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI = 2*3.14)
    {
        int numc = csteps, numt = tsteps;

        for (int i = 0; i < numc; i++) {
            model_primitive_begin(GL_QUAD_STRIP);
            for (int j = 0; j <= numt; j++) {
                for (int k = 1; k >= 0; k--) {

                    double s = (i + k) % numc + 0.5;
                    double t = j % numt;

                    double x = (radius + tradius * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
                    double y = (radius + tradius * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
                    double z = tradius * sin(s * TWOPI / numc);
		    double u = (i + k) / (float)numc;
		    double v = t / (float)numt;

		    float ver[] = {x1 + x, y1 + y, z1 + z}, uv[] = {u, v};
		    model_vertex_texture(ver, uv);
                }
            }
	    model_primitive_end();
        }
    }

    void model_wall(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
    {
    if ((fequal(x1, x2) && fequal(y1, y2)) || fequal(z1, z2)) {
        return;
    }
    float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y2, z1}, v3[] = {x2, y2, z2},
          t0[] = {0, 0}, t1[] = {0, vrep}, t2[] = {hrep, 0}, t3[] = {hrep, vrep};
    model_primitive_begin(GL_TRIANGLE_STRIP);

    const float xd = x1-x2, yd = y2-y1, zd = z2-z1;
    const float usize = fabs(zd), vsize = hypotf(xd, yd);
    const float uz = zd/usize, vx = xd/vsize, vy = yd/vsize;
    glNormal3f(uz*vy, uz*vx, 0);

    if (x2>x1 || y2>y1) {
      model_vertex_texture(v0,t0);
    } else {
      model_vertex_texture(v3,t3);
    }
    if (x1>x2 || y1>y2) {
      model_vertex_texture(v1,t1);
    } else {
      model_vertex_texture(v2,t2);
    }
    if (x1>x2 || y1>y2) {
      model_vertex_texture(v2,t2);
    } else {
      model_vertex_texture(v1,t1);
    }
    if (x2>x1 || y2>y1) {
      model_vertex_texture(v3,t3);
    } else {
      model_vertex_texture(v0,t0);
    }
    model_primitive_end();
    }

    void model_floor(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
    {
        float v0[] = {x1, y1, z1}, v1[] = {x1, y2, z1}, v2[] = {x2, y1, z2}, v3[] = {x2, y2, z2},
              t0[] = {0, 0}, t1[] = {0, vrep}, t2[] = {hrep, 0}, t3[] = {hrep, vrep};
        model_primitive_begin(GL_TRIANGLE_STRIP);
        model_vertex_texture(v0,t0);
        model_vertex_texture(v1,t1);
        model_vertex_texture(v2,t2);
        model_vertex_texture(v3,t3);
        model_primitive_end();
    }
};

static map<unsigned int, d3d_model> d3d_models;
static unsigned int d3d_models_maxid = 0;

namespace enigma_user
{

unsigned int d3d_model_create()
{
    d3d_models.insert(pair<unsigned int, d3d_model>(d3d_models_maxid++, d3d_model()));
    return d3d_models_maxid-1;
}

void d3d_model_destroy(const unsigned int id)
{
    d3d_models[id].clear();
    d3d_models.erase(d3d_models.find(id));
}

bool d3d_model_exists(const unsigned int id)
{
    return (d3d_models.find(id) != d3d_models.end());
}

void d3d_model_clear(const unsigned int id)
{
    d3d_models[id].clear();
}

void d3d_model_save(const unsigned int id, string fname)
{
    d3d_models[id].save(fname);
}

bool d3d_model_load(const unsigned int id, string fname)
{
    d3d_models[id].clear();
    return d3d_models[id].load(fname);
}

void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId)
{
    d3d_models[id].draw(x, y, z, texId);
}

void d3d_model_primitive_begin(const unsigned int id, int kind)
{
    d3d_models[id].model_primitive_begin(kind);
}

void d3d_model_primitive_end(const unsigned int id)
{
    d3d_models[id].model_primitive_end();
}

void d3d_model_vertex(const unsigned int id, double x, double y, double z)
{
    float v[] = {x, y, z};
    d3d_models[id].model_vertex(v);
}

void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha)
{
    float v[] = {x, y, z};
    d3d_models[id].model_vertex_color(v, col, alpha);
}

void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty)
{
    float v[] = {x, y, z}, t[] = {tx, ty};
    d3d_models[id].model_vertex_texture(v, t);
}

void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha)
{
    float v[] = {x, y, z}, t[] = {tx, ty};
    d3d_models[id].model_vertex_texture_color(v, t, col, alpha);
}

void d3d_model_vertex_normal(const unsigned int id, double x, double y, double z, double nx, double ny, double nz)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz};
    d3d_models[id].model_vertex_normal(v, n);
}

void d3d_model_vertex_normal_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, int col, double alpha)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz};
    d3d_models[id].model_vertex_normal_color(v, n, col, alpha);
}

void d3d_model_vertex_normal_texture(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz}, t[] = {tx, ty};
    d3d_models[id].model_vertex_normal_texture(v, n, t);
}

void d3d_model_vertex_normal_texture_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz}, t[] = {tx, ty};
    d3d_models[id].model_vertex_normal_texture_color(v, n, t, col, alpha);
}

void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed)
{
    d3d_models[id].model_block(x1, y1, z1, x2, y2, z2, hrep, vrep, closed);
}

void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
{
    d3d_models[id].model_cylinder(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
{
    d3d_models[id].model_cone(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, int steps)
{
    d3d_models[id].model_ellipsoid(x1, y1, z1, x2, y2, z2, hrep, vrep, steps);
}

void d3d_model_icosahedron(const unsigned int id)
{
    d3d_models[id].model_icosahedron();
}

void d3d_model_torus(const unsigned int id, double x1, double y1, double z1, double hrep, double vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI)
{
    d3d_models[id].model_torus(x1, y1, z1, hrep, vrep, csteps, tsteps, radius, tradius, TWOPI);
}

void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
{
    d3d_models[id].model_wall(x1, y1, z1, x2, y2, z2, hrep, vrep);
}

void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
{
    d3d_models[id].model_floor(x1, y1, z1, x2, y2, z2, hrep, vrep);
}

}

