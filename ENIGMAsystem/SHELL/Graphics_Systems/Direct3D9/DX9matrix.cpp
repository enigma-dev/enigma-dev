/** Copyright (C) 2013-2014 Robert B. Colton
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
#include "Bridges/General/DX9Context.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GStextures.h"

#include "../General/GSmodel.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

#include "Graphics_Systems/General/GScolor_macros.h"

namespace enigma_user
{

void d3d_set_perspective(bool enable)
{
  D3DXMATRIX matProjection;
  if (enable) {
    D3DXMatrixPerspectiveFovLH(&matProjection, 45, -view_wview[view_current] / (double)view_hview[view_current], 32000, -32000);
  } else {
    D3DXMatrixPerspectiveFovLH(&matProjection, 0, 1, 0, 1);
  }
  //set projection matrix
  d3dmgr->SetTransform(D3DTS_PROJECTION, &matProjection);
  // Unverified note: Perspective not the same as in GM when turning off perspective and using d3d projection
  // Unverified note: GM has some sort of dodgy behaviour where this function doesn't affect anything when calling after d3d_set_projection_ext
  // See also OpenGL3/GL3d3d.cpp Direct3D9/DX9d3d.cpp OpenGL1/GLd3d.cpp
}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup)
{
	D3DXVECTOR3 vEyePt( xfrom, yfrom, zfrom );
	D3DXVECTOR3 vLookatPt( xto, yto, zto );
	D3DXVECTOR3 vUpVec( xup, yup, zup );

	// Get D3DX to fill in the matrix values
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

	// Set our view matrix
	d3dmgr->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIX matProj;

	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian(45), view_wview[view_current] / (double)view_hview[view_current], 1.0f, 32000.0f );
	d3dmgr->SetTransform( D3DTS_PROJECTION, &matProj );
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
{
	D3DXVECTOR3 vEyePt( xfrom, yfrom, zfrom );
	D3DXVECTOR3 vLookatPt( xto, yto, zto );
	D3DXVECTOR3 vUpVec( xup, yup, zup );

	// Get D3DX to fill in the matrix values
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

	// Set our view matrix
	d3dmgr->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIX matProj;

	D3DXMatrixPerspectiveFovLH( &matProj, gs_angle_to_radians(angle), aspect, znear, zfar );
	d3dmgr->SetTransform( D3DTS_PROJECTION, &matProj );
}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  // This fixes font glyph edge artifacting and vertical scroll gaps
  // seen by mostly NVIDIA GPU users.  Rounds x and y and adds +0.01 offset.
  // This will prevent the fix from being negated through moving projections
  // and fractional coordinates.
  x = round(x) + 0.01f; y = round(y) + 0.01f;
  D3DXMATRIX matRotZ, mat1Trans, mat2Trans, matScale;

  // Translate so the center is at 0,0
  D3DXMatrixTranslation(&mat1Trans, -x-width/2.0, -y-height/2.0, 0);

	// Rotate around the center
	D3DXMatrixRotationZ( &matRotZ, gs_angle_to_radians(angle) );        // Roll

  // Move back
  //D3DXMatrixTranslation(&mat2Trans, +x+width/2.0, +y+height/2.0, 0); //Offset back
  //D3DXMatrixTranslation(&mat3Trans, -x, -y-height, 0); //Move the view
  // Or do both of them together:
	D3DXMatrixTranslation(&mat2Trans, width/2.0, height/2.0-height, 0);
  // I don't get why the view moving is done here though. It should be possible in D3DXMatrixOrthoOffCenterLH instead,
  // like we do in GL - H.G.

 	D3DXMatrixScaling(&matScale, 1, -1, 1);

	// Calculate our world matrix by multiplying the above (in the correct order)
	D3DXMATRIX matView=mat1Trans*matRotZ*mat2Trans*matScale;

	// Set the matrix to be applied to anything we render from now on
	d3dmgr->SetTransform( D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixOrthoOffCenterLH(&matProjection,
							0,
							(FLOAT)width,
							0,
							(FLOAT)height,
							32000.0f,    // the near view-plane
							-32000.0f);    // the far view-plane

	d3dmgr->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  D3DXMATRIX matView, matRotZ;    // the projection transform matrix

  D3DXMatrixOrthoOffCenterLH(&matView,
    (FLOAT)x,
    (FLOAT)x+width,
    (FLOAT)y,
    (FLOAT)y+height,
    -32000.0f,    // the near view-plane
    32000.0f);    // the far view-plane

  // Initialize rotation matrix
  D3DXMatrixRotationZ( &matRotZ, gs_angle_to_radians(angle) );
  matView *= matRotZ;

  // Set the matrix to be applied to anything we render from now on
  d3dmgr->SetTransform( D3DTS_VIEW, &matView);

  D3DXMATRIX matProj;    // the projection transform matrix
  D3DXMatrixPerspectiveFovLH(&matProj,
    D3DXToRadian(60), width/height, 0.1, 32000);    // the far view-plane

  d3dmgr->SetTransform(D3DTS_PROJECTION, &matProj);    // set the projection transform
}

D3DXMATRIX matWorld;
D3DXMATRIX matNull;

// ***** TRANSFORMATIONS BEGIN *****
void d3d_transform_set_identity()
{
	matWorld = matNull;
	D3DXMatrixIdentity( &matWorld );
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
	D3DXMATRIX matTranslate;    // a matrix to store the translation information

	// build a matrix to move the model
	// store it to matTranslate
	D3DXMatrixTranslation(&matTranslate, xt, yt, zt);

	matWorld *= matTranslate;

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);

}

void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
	D3DXMATRIX matScale;

	// build a matrix to double the size of the model
	// store it to matScale
	D3DXMatrixScaling(&matScale, xs, ys, zs);

	matWorld *= matScale;

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_add_rotation_x(gs_scalar angle)
{
	D3DXMATRIX matRot;

	// build a matrix to rotate the model by so many radians
	D3DXMatrixRotationX(&matRot, gs_angle_to_radians(-angle));

	matWorld *= matRot;

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_add_rotation_y(gs_scalar angle)
{
//D3DXMatrixIdentity( &matWorld );
	D3DXMATRIX matRot;

	// build a matrix to rotate the model by so many radians
	D3DXMatrixRotationY(&matRot, gs_angle_to_radians(-angle));

	matWorld *= matRot;

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_add_rotation_z(gs_scalar angle)
{
	D3DXMATRIX matRot;

	// build a matrix to rotate the model by so many radians
	D3DXMatrixRotationZ(&matRot, gs_angle_to_radians(-angle));

	matWorld *= matRot;

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
	D3DXMATRIX matRot;

	// build a matrix to rotate the model by so many radians
	angle = D3DXToRadian(-angle);
	D3DXMatrixRotationYawPitchRoll(&matRot, y * angle, x * angle, z * angle);

	matWorld *= matRot;

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
	// build a matrix to move the model
	// store it to matTranslate
	D3DXMatrixTranslation(&matWorld, xt, yt, zt);

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
	// build a matrix to double the size of the model
	// store it to matScale
	D3DXMatrixScaling(&matWorld, xs, ys, zs);

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_set_rotation_x(double angle)
{
	// build a matrix to rotate the model by so many radians
	D3DXMatrixRotationX(&matWorld, gs_angle_to_radians(-angle));

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_set_rotation_y(gs_scalar angle)
{
	// build a matrix to rotate the model by so many radians
	D3DXMatrixRotationY(&matWorld, gs_angle_to_radians(-angle));

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_set_rotation_z(gs_scalar angle)
{
	D3DXMatrixIdentity( &matWorld );
	// build a matrix to rotate the model by so many radians
	D3DXMatrixRotationZ(&matWorld, gs_angle_to_radians(-angle));

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
	// build a matrix to rotate the model by so many radians
	angle = gs_angle_to_radians(-angle);
	D3DXMatrixRotationYawPitchRoll(&matWorld, y * angle, x * angle, z * angle);

	// tell Direct3D about our matrix
	d3dmgr->SetTransform(D3DTS_WORLD, &matWorld);
}

}

#include <stack>
stack<bool> trans_stack;
int trans_stack_size = 0;

namespace enigma_user
{

bool d3d_transform_stack_push()
{

}

bool d3d_transform_stack_pop()
{

}

void d3d_transform_stack_clear()
{

}

bool d3d_transform_stack_empty()
{
    return (trans_stack_size == 0);
}

bool d3d_transform_stack_top()
{

}

bool d3d_transform_stack_disgard()
{
    if (trans_stack_size == 0) return false;
    trans_stack.push(0);
    trans_stack_size--;
    return true;
}

}
