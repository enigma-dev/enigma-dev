/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu
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
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>

#include <floatcomp.h>

using namespace std;

struct posi { // Homogenous point.
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;
    posi(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar w1) : x(x1), y(y1), z(z1), w(w1){}
};

namespace enigma_user
{
	void d3d_clear_depth(double value){}
	void d3d_set_software_vertex_processing(bool software){}

	void d3d_start()
	{
		enigma::d3dMode = true;
		enigma::d3dPerspective = true;
		enigma::d3dHidden = true;
		enigma::d3dZWriteEnable = true;
		enigma::d3dCulling = rs_none;
	}

	void d3d_end()
	{
		enigma::d3dMode = false;
		enigma::d3dPerspective = false;
		enigma::d3dHidden = false;
		enigma::d3dZWriteEnable = false;
		enigma::d3dCulling = rs_none;
	}


	// disabling hidden surface removal in means there is no depth buffer
	void d3d_set_hidden(bool enable)
	{
		enigma::d3dHidden = enable;
	}

	// disabling zwriting can let you turn off testing for a single model, for instance
	// to fix cutout billboards such as trees the alpha pixels on their edges may not depth sort
	// properly particle effects are usually drawn with zwriting disabled because of this as well
	void d3d_set_zwriteenable(bool enable)
	{
		enigma::d3dZWriteEnable = enable;
	}

	void d3d_set_culling(int mode)
	{
		enigma::d3dCulling = mode;
	}
}
