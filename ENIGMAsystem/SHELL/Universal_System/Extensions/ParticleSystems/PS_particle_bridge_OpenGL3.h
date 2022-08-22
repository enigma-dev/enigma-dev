/** Copyright (C) 2008-2013 Josh Ventura, forthevin, Robert B. Colton
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

#ifndef ENIGMA_PS_PARTICLE_BRIDGE_OPENGL3_H
#define ENIGMA_PS_PARTICLE_BRIDGE_OPENGL3_H

#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GLTextureStruct.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "PS_particle_instance.h"
#include "PS_particle_sprites.h"
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#include <iostream>

#include "PS_particle_system.h"
#include "PS_particle.h"

namespace enigma {
  namespace particle_bridge {
    GLuint load_and_compile_shader(const char* src, GLenum shader_type) {

      // Compile the shader.
      GLuint shader = glCreateShader(shader_type);
      glShaderSource(shader, 1, &src, NULL); // Expects null-terminated.
      glCompileShader(shader);

      // Check the result of the compilation.
      GLint test;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
      if (!test) {
        const size_t buffer_size = 512;
        char buffer[buffer_size];
        glGetShaderInfoLog(shader, buffer_size, NULL, buffer);
        DEBUG_MESSAGE(std::string("Shader compilation failed with this message: ") + buffer, MESSAGE_TYPE::M_ERROR);
        exit(-1);
      }
      return shader;
    }
    GLuint vao;
    GLuint shader_program;
    void initialize_particle_bridge() {

      int major_version;
      glGetIntegerv(GL_MAJOR_VERSION, &major_version);
      if (major_version < 3) {
        int minor_version;
        glGetIntegerv(GL_MINOR_VERSION, &minor_version);
        DEBUG_MESSAGE("OpenGL 3.0 or more must be supported, but a smaller version number was found: " + std::to_sring(major_version) + "." + std::to_string(minor_version), MESSAGE_TYPE::M_ERROR);
        exit(-1);
      }

      glGenVertexArrays(1, &vao); // Allocate vertex array 1.
      glBindVertexArray(vao);

      // Load and compile the vertex and fragment shaders.

      const char* vertex_source = // TODO: Use raw literals from C++11 to write the shader instead.
"#version 130\n" // GLSL 1.30 matches OpenGL 3.0.
"\n"
"in vec2 position;\n"
"in vec4 input_color;\n"
"in vec2 tex_coord;\n"
"out vec4 transfer_color;\n"
"out vec2 transfer_tex;\n"
"uniform mat4 trans_mat;\n"
"uniform mat4 proj_mat;\n"
"\n"
"void main() {\n"
"  gl_Position = proj_mat*trans_mat*vec4(position.x, position.y, 0.0, 1.0);\n"
"  transfer_color = input_color;\n"
"  transfer_tex = tex_coord;\n"
"}";

      GLuint vertex_shader = load_and_compile_shader(vertex_source, GL_VERTEX_SHADER);

      const char* fragment_source = // TODO: Use raw literals from C++11 to write the shader instead.
"#version 130\n" // GLSL 1.30 matches OpenGL 3.0.
"\n"
"in vec4 transfer_color;\n"
"in vec2 transfer_tex;\n"
"out vec4 out_color;\n"
"\n"
"uniform sampler2D texture_sampler;\n"
"\n"
"void main() {\n"
"  out_color = texture(texture_sampler, transfer_tex);\n"
"  out_color = vec4(transfer_color.x*out_color.x, transfer_color.y*out_color.y, transfer_color.z*out_color.z, transfer_color.w*out_color.w);\n"
"}";

      GLuint fragment_shader = load_and_compile_shader(fragment_source, GL_FRAGMENT_SHADER);

      // Attach the above shader to a program.
      shader_program = glCreateProgram();
      glAttachShader(shader_program, vertex_shader);
      glAttachShader(shader_program, fragment_shader);

      // Flag the shaders for deletion.
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);

      // Link and use the program.
      glLinkProgram(shader_program);
      glUseProgram(shader_program);
    }
    double wiggle;
    int subimage_index;
    double x_offset;
    double y_offset;

  void draw_particles(std::vector<particle_instance>& pi_list, bool oldtonew, double a_wiggle, int a_subimage_index,
      double a_x_offset, double a_y_offset) {
      using namespace enigma::particle_bridge;
      wiggle = a_wiggle;
      subimage_index = a_subimage_index;
      x_offset = a_x_offset;
      y_offset = a_y_offset;

      glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT); // Attrib push 1.

      if (pi_list.size() > 0) {
        glBindVertexArray(vao); // Bind vertex array.
        glUseProgram(shader_program); // Bind shader program.

        // Transfer data to shaders.

        const unsigned int pi_list_size = pi_list.size();

        std::vector<GLfloat> points;
        points.reserve(pi_list_size*2);

        std::vector<GLfloat> colors;
        colors.reserve(pi_list_size*3);

        std::vector<GLfloat> texcoords;
        texcoords.reserve(pi_list_size*2);

        std::vector<GLuint> indices;
        indices.reserve(pi_list_size*6);

        std::vector<int> texture_indices;
        std::deque<bool> blend_adds;

        for (unsigned int i = 0; i < pi_list_size; i++) {

          particle_instance& pi = pi_list[i];
          double x, y;
          int color = pi.color;
          int alpha = pi.alpha;
          double xscale, yscale;
          double rot;
          double width, height;
          double pi_x_offset, pi_y_offset;
          double tbx, tby, tbw, tbh;
          if (pi.pt->alive) {
            particle_type* pt = pi.pt;
            x = pi.x;
            y = pi.y;
            double size = std::max(0.0, pi.size + pt->size_wiggle*particle_system::get_wiggle_result(pi.size_wiggle_offset, wiggle));
            xscale = pt->xscale*size;
            yscale = pt->yscale*size;
            double rot_degrees = pi.angle + pt->ang_wiggle*particle_system::get_wiggle_result(pi.ang_wiggle_offset, wiggle);
            if (pt->ang_relative) {
              rot_degrees += pi.direction;
            }
            rot = rot_degrees*M_PI/180.0;
            blend_adds.push_back(pt->blend_additive);

            if (!pt->is_particle_sprite && enigma_user::sprite_exists(pt->sprite_id)) {
              int sprite_id = pt->sprite_id;
              int subimg;
              if (!pt->sprite_animated) {
                subimg = pi.sprite_subimageindex_initial;
              }
              else {
                const sprite *const spr = spritestructarray[pt->sprite_id];
                const int subimage_count = spr->subcount;
                if (pt->sprite_stretched) {
                  subimg = int(subimage_count*(1.0 - 1.0*pi.life_current/pi.life_start));
                  subimg = subimg >= subimage_count ? subimage_count - 1 : subimg;
                  subimg = subimg % subimage_count;
                }
                else {
                  subimg = (subimage_index + pi.sprite_subimageindex_initial) % subimage_count;
                }
              }
              const enigma::sprite *const spr2d = enigma::spritestructarray[sprite_id];
              const int usi = subimg % spr2d->subcount;
              width = spr2d->width;
              height = spr2d->height;
              pi_x_offset = spr2d->xoffset;
              pi_y_offset = spr2d->yoffset;
              tbx = spr2d->texturexarray[usi];
              tby = spr2d->textureyarray[usi];
              tbw = spr2d->texturewarray[usi];
              tbh = spr2d->textureharray[usi];
              texture_indices.push_back(spr2d->texturearray[usi]);
            }
            else {
              particle_sprite* ps;
              if (!pt->is_particle_sprite) {
                ps = get_particle_sprite(pt_sh_pixel);
              }
              else {
                ps = pt->part_sprite;
              }
              width = ps->width;
              height = ps->height;
              pi_x_offset = ps->width/2.0;
              pi_y_offset = ps->height/2.0;
              tbx = 1.0;
              tby = 1.0;
              tbw = 0.0;
              tbh = 0.0;
              texture_indices.push_back(ps->texture);
            }
          }
          else {
            double size = pi.size;
            if (size <= 0) size = 0;

            particle_sprite* ps = get_particle_sprite(pt_sh_pixel);
            texture_indices.push_back(ps->texture);
            blend_adds.push_back(false);

            rot = pi.angle*M_PI/180.0;

            x = round(pi.x), y = round(pi.y);
            xscale = size, yscale = size;
            pi_x_offset = ps->width/2.0;
            pi_y_offset = ps->height/2.0;
            width = ps->width, height = ps->height;
            tbx = 1, tby = 1, tbw  = 0, tbh = 0;
          }

          const double
          w = width*xscale, h = height*yscale,
          wsinrot = w*sin(rot), wcosrot = w*cos(rot);

          // Particle system offset.
          x += x_offset;
          y += y_offset;

          double ulcx = x - xscale * pi_x_offset * cos(rot) + yscale * pi_y_offset * cos(M_PI/2+rot);
          double ulcy = y + xscale * pi_x_offset * sin(rot) - yscale * pi_y_offset * sin(M_PI/2+rot);

          double t1x = tbx, t1y = tby;
          double v1x = ulcx, v1y = ulcy;
          double t2x = tbx+tbw, t2y = tby;
          double v2x = ulcx + wcosrot, v2y = ulcy - wsinrot;

          const double mpr = 3*M_PI/2 + rot;
          ulcx += h * cos(mpr);
          ulcy -= h * sin(mpr);

          double t3x = tbx, t3y = tby+tbh;
          double v3x = ulcx, v3y = ulcy;
          double t4x = tbx+tbw, t4y = tby+tbh;
          double v4x = ulcx + wcosrot, v4y = ulcy - wsinrot;

          points.push_back(v1x);
          points.push_back(v1y);
          points.push_back(v2x);
          points.push_back(v2y);
          points.push_back(v3x);
          points.push_back(v3y);
          points.push_back(v4x);
          points.push_back(v4y);
          const GLfloat r = COL_GET_R(color)/255.0, g = COL_GET_G(color)/255.0, b = COL_GET_B(color)/255.0;
          const GLfloat a = alpha/255.0;
          colors.push_back(r);
          colors.push_back(g);
          colors.push_back(b);
          colors.push_back(a);
          colors.push_back(r);
          colors.push_back(g);
          colors.push_back(b);
          colors.push_back(a);
          colors.push_back(r);
          colors.push_back(g);
          colors.push_back(b);
          colors.push_back(a);
          colors.push_back(r);
          colors.push_back(g);
          colors.push_back(b);
          colors.push_back(a);
          texcoords.push_back(t1x);
          texcoords.push_back(t1y);
          texcoords.push_back(t2x);
          texcoords.push_back(t2y);
          texcoords.push_back(t3x);
          texcoords.push_back(t3y);
          texcoords.push_back(t4x);
          texcoords.push_back(t4y);
          unsigned int i4 = i*4;
          if (!oldtonew) {
            i4 = (pi_list_size - 1 - i)*4;
          }
          indices.push_back(i4 + 0);
          indices.push_back(i4 + 1);
          indices.push_back(i4 + 2);
          indices.push_back(i4 + 1);
          indices.push_back(i4 + 2);
          indices.push_back(i4 + 3);
        }

        std::vector<std::pair<int,int> > switch_offset_instcount;
        {
          bool init = true;
          int prev_texture_index = -1;
          bool prev_blend_add = false;
          int curr_texture_index = -1;
          bool curr_blend_add = false;
          int switch_offset = 0;
          int switch_count = 0;
          for (unsigned int loop_i = 0; loop_i  < pi_list.size(); loop_i ++) {
            unsigned int i = loop_i;
            if (!oldtonew) {
              i = pi_list_size - 1 - loop_i;
            }

            curr_texture_index = texture_indices[i];
            curr_blend_add = blend_adds[i];

            if (init) {
              init = false;
            }
            else {
              if (prev_blend_add != curr_blend_add || prev_texture_index != curr_texture_index) {
                int new_offset = switch_offset;
                if (!oldtonew) {
                  new_offset = pi_list_size - switch_offset - 1;
                }
                switch_offset_instcount.push_back(std::pair<int,int>(new_offset, switch_count));
                switch_offset = i;
                switch_count = 0;
              }
            }
            switch_count++;
            prev_texture_index = curr_texture_index;
            prev_blend_add = curr_blend_add;
          }
          int new_offset = switch_offset;
          if (!oldtonew) {
            new_offset = pi_list_size - switch_offset - 1;
          }
          switch_offset_instcount.push_back(std::pair<int,int>(new_offset, switch_count));
        }

        const unsigned int points_bsize = sizeof(GLfloat)*points.size();
        const unsigned int colors_bsize = sizeof(GLfloat)*colors.size();
        const unsigned int texcoords_bsize = sizeof(GLfloat)*texcoords.size();
        const unsigned int total_bsize = points_bsize + colors_bsize + texcoords_bsize;

        GLuint vbo;
        glGenBuffers(1, &vbo); // Allocate vertex buffer array 1.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, total_bsize, NULL, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, points_bsize, points.data());
        glBufferSubData(GL_ARRAY_BUFFER, points_bsize, colors_bsize, colors.data());
        glBufferSubData(GL_ARRAY_BUFFER, points_bsize + colors_bsize, texcoords_bsize, texcoords.data());

        GLint position_attribute = glGetAttribLocation(shader_program, "position");
        glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(position_attribute);

        GLint color_attribute = glGetAttribLocation(shader_program, "input_color");
        glVertexAttribPointer(color_attribute, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)points_bsize);
        glEnableVertexAttribArray(color_attribute);

        GLint texcoord_attribute = glGetAttribLocation(shader_program, "tex_coord");
        glVertexAttribPointer(texcoord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(points_bsize + colors_bsize));
        glEnableVertexAttribArray(texcoord_attribute);

        GLuint eab;
        glGenBuffers(1, &eab); // Allocate element array buffer 1.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

        GLint transform_matrix_location = glGetUniformLocation(shader_program, "trans_mat");
        glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, mv_matrix.Transpose());

        GLint projection_matrix_location = glGetUniformLocation(shader_program, "proj_mat");
        glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, projection_matrix.Transpose());

        // Draw.

        for (unsigned i = 0; i < switch_offset_instcount.size(); i++) {
          int switch_offset = switch_offset_instcount[i].first;
          int switch_count = switch_offset_instcount[i].second;

          int actual_index = switch_offset;
          if (!oldtonew) {
            actual_index = pi_list_size - switch_offset - 1;
          }
          enigma_user::texture_set(textureStructs[texture_indices[actual_index]]->gltex);
          if (blend_adds[actual_index]) {
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
          }
          else {
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
          }

          glDrawElements(GL_TRIANGLES, switch_count*6, GL_UNSIGNED_INT, (GLvoid*)(0 + switch_offset*6*sizeof(GLuint))); // 6 vertices per particle instance.
        }

        // Cleanup.

        glDeleteBuffers(1, &eab); // Deallocate element array buffer 1.
        glDeleteBuffers(1, &vbo); // Deallocate vertex buffer array 1.

        glBindVertexArray(0); // Unbind vertex array.
        glUseProgram(0); // Unbind shader program.
      }

      glPopAttrib(); // Attrib pop 1.
    }
  }
}

#endif // ENIGMA_PS_PARTICLE_BRIDGE_OPENGL3_H
