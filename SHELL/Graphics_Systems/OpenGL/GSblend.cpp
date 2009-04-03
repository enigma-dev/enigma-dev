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

#include <GL/gl.h>

int draw_set_blend_mode(int mode)
{
    switch (mode)
    {
        case 0: //bm_normal
             glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
             return 0;
        case 1: //bm_add
             glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
             return 0;
        case 2: //bm_max
             glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
             return 0;
        case 3: //bm_subtract
             glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
             return 0;
        default:
             glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
             return 0;
    }
}

namespace enigma
{
    int blendequivs[]={ GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,
                        GL_ONE_MINUS_SRC_ALPHA,GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,
                        GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA_SATURATE };
}

int draw_set_blend_mode_ext(double src,double dest)
{
    int source,dst;
    source=enigma::blendequivs[((int)src-1) % 10];
    dst=enigma::blendequivs[((int)dest-1) % 10];

    glBlendFunc(source,dst);

    return 0;
}
