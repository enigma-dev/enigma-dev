/** Copyright (C) 2013-2014, Robert B. Colton
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

#ifndef ENIGMA_GL_SAMPLERSTATE_H
#define ENIGMA_GL_SAMPLERSTATE_H

#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"

namespace enigma {
  //NOTE: OpenGL 1 hardware does not support sampler objects, some versions of 2 and usually over 3 do. We use this class
  //to emulate the Direct3D behavior.
  struct Sampler {
    int bound_texture;
    bool wrapu, wrapv, wrapw;
    GLint bordercolor[4], min, mag, maxlevel;
    GLfloat anisotropy, minlod, maxlod;

    Sampler(): bound_texture(-1), wrapu(true), wrapv(true), wrapw(true), min(GL_NEAREST), mag(GL_NEAREST), maxlevel(1000), anisotropy(1), minlod(-1000), maxlod(1000) {
      bordercolor[0] = 0;
      bordercolor[1] = 0;
      bordercolor[2] = 0;
      bordercolor[3] = 0;
    }

    ~Sampler() {

    }

    void ApplyState() {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, minlod);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, maxlod);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxlevel);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapu?GL_REPEAT:GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapv?GL_REPEAT:GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapw?GL_REPEAT:GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
    }

    void CompareAndApply(Sampler* sampler) {
      if (sampler->wrapu != wrapu) {
        sampler->wrapu = wrapu;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapu?GL_REPEAT:GL_CLAMP);
      }
      if (sampler->wrapv != wrapv) {
        sampler->wrapv = wrapv;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapv?GL_REPEAT:GL_CLAMP);
      }
      if (sampler->wrapw != wrapw) {
        sampler->wrapw = wrapw;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapw?GL_REPEAT:GL_CLAMP);
      }

      if (sampler->min != min) {
        sampler->min = min;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
      }
      if (sampler->mag != mag) {
        sampler->mag = mag;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
      }

      if (sampler->bordercolor[0] != bordercolor[0] || sampler->bordercolor[1] != bordercolor[1] ||
          sampler->bordercolor[2] != bordercolor[2] || sampler->bordercolor[3] != bordercolor[3]) {
        sampler->bordercolor[0] = bordercolor[0];
        sampler->bordercolor[1] = bordercolor[1];
        sampler->bordercolor[2] = bordercolor[2];
        sampler->bordercolor[3] = bordercolor[3];
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
      }

      if (sampler->anisotropy != anisotropy) {
        sampler->anisotropy = anisotropy;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
      }

      if (sampler->minlod != minlod) {
        sampler->minlod = minlod;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, minlod);
      }
      if (sampler->maxlod != maxlod) {
        sampler->maxlod = maxlod;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, maxlod);
      }
      if (sampler->maxlevel != maxlevel) {
        sampler->maxlevel = maxlevel;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxlevel);
      }
    }
  };

  extern Sampler samplers[8];
}

#endif // ENIGMA_GL_SAMPLERSTATE_H
