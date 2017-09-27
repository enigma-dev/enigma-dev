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
#include "../General/GSd3d.h"
#include "../General/GSmatrix.h"
#include "../General/GSmath.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include <stack>

//using namespace std;

#include <floatcomp.h>

std::stack<enigma::Matrix4> trans_stack;
std::stack<enigma::Matrix4> proj_stack;
std::stack<enigma::Matrix4> view_stack;

namespace enigma
{
    #define degtorad(x) x*(M_PI/180.0)

    //These are going to be modified by the user via functions
    enigma::Matrix4 projection_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), view_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), model_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

    //This is for GL1
    enigma::Matrix4 mv_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

    //This is the user can access it
    enigma::Matrix4 mvp_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
}
namespace enigma_user
{
	void d3d_transform_set_identity()
	{
		enigma::model_matrix.init_identity();
		enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
	{
		enigma::model_matrix.translate(xt, yt, zt);
		enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
	{
		enigma::model_matrix.scale(xs, ys, zs);
		enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_rotation_x(gs_scalar angle)
	{
		enigma::model_matrix.rotate_x(-angle);
		enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_rotation_y(gs_scalar angle)
	{
		enigma::model_matrix.rotate_y(-angle);
		enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_rotation_z(gs_scalar angle)
	{
			enigma::model_matrix.rotate_z(-angle);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}
	void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
	{
			enigma::model_matrix.rotate(-angle,x,y,z);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_rotation(gs_scalar x, gs_scalar y, gs_scalar z)
	{
			enigma::model_matrix.rotate(x,y,z);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_look_at(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
	{
			enigma::Matrix4 m;
			m.init_look_at_transform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
			enigma::model_matrix = m*enigma::model_matrix;
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
	{
			enigma::model_matrix.init_translation_transform(xt, yt, zt);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
	{
			enigma::model_matrix.init_scale_transform(xs, ys, zs);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}
	void d3d_transform_set_rotation_x(gs_scalar angle)
	{
			enigma::model_matrix.init_rotate_x_transform(degtorad(-angle));
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_rotation_y(gs_scalar angle)
	{
			enigma::model_matrix.init_rotate_y_transform(degtorad(-angle));
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_rotation_z(gs_scalar angle)
	{
			enigma::model_matrix.init_rotate_z_transform(degtorad(-angle));
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
	{
			enigma::model_matrix.init_identity();
			enigma::model_matrix.rotate(-angle, x, y, z);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_rotation(gs_scalar x, gs_scalar y, gs_scalar z)
	{
			enigma::model_matrix.init_identity();
			enigma::model_matrix.rotate(x,y,z);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_look_at(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
	{
			enigma::model_matrix.init_look_at_transform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_set_array(const gs_scalar *matrix)
	{
			enigma::model_matrix = enigma::Matrix4(matrix);
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	void d3d_transform_add_array(const gs_scalar *matrix)
	{
			enigma::Matrix4 m(matrix);
			enigma::model_matrix = m*enigma::model_matrix;
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	gs_scalar * d3d_transform_get_array_pointer(){
			return enigma::model_matrix;
	}

	var d3d_transform_get_array(){
			var mm;
			mm(3,3) = 0;
			mm(0,0) = enigma::model_matrix(0,0), mm(0,1) = enigma::model_matrix(0,1), mm(0,2) = enigma::model_matrix(0,2), mm(0,3) = enigma::model_matrix(0,3),
			mm(1,0) = enigma::model_matrix(1,0), mm(1,1) = enigma::model_matrix(1,1), mm(1,2) = enigma::model_matrix(1,2), mm(1,3) = enigma::model_matrix(1,3),
			mm(2,0) = enigma::model_matrix(2,0), mm(2,1) = enigma::model_matrix(2,1), mm(2,2) = enigma::model_matrix(2,2), mm(2,3) = enigma::model_matrix(2,3),
			mm(3,0) = enigma::model_matrix(3,0), mm(3,1) = enigma::model_matrix(3,1), mm(3,2) = enigma::model_matrix(3,2), mm(3,3) = enigma::model_matrix(3,3);
			return mm;
	}

	void d3d_projection_set_array(const gs_scalar *matrix)
	{
		enigma::projection_matrix = enigma::Matrix4(matrix);
	}

	void d3d_projection_add_array(const gs_scalar *matrix)
	{
		enigma::Matrix4 m(matrix);
		enigma::projection_matrix = m*enigma::projection_matrix;
	}

	gs_scalar * d3d_projection_get_array_pointer(){
		return enigma::projection_matrix;
	}

	var d3d_projection_get_array(){
		var pm;
		pm(3,3) = 0;
		pm(0,0) = enigma::projection_matrix(0,0), pm(0,1) = enigma::projection_matrix(0,1), pm(0,2) = enigma::projection_matrix(0,2), pm(0,3) = enigma::projection_matrix(0,3),
		pm(1,0) = enigma::projection_matrix(1,0), pm(1,1) = enigma::projection_matrix(1,1), pm(1,2) = enigma::projection_matrix(1,2), pm(1,3) = enigma::projection_matrix(1,3),
		pm(2,0) = enigma::projection_matrix(2,0), pm(2,1) = enigma::projection_matrix(2,1), pm(2,2) = enigma::projection_matrix(2,2), pm(2,3) = enigma::projection_matrix(2,3),
		pm(3,0) = enigma::projection_matrix(3,0), pm(3,1) = enigma::projection_matrix(3,1), pm(3,2) = enigma::projection_matrix(3,2), pm(3,3) = enigma::projection_matrix(3,3);
		return pm;
	}

	gs_scalar * d3d_view_get_array_pointer(){
		return enigma::view_matrix;
	}

	var d3d_view_get_array(){
		var pm;
		pm(3,3) = 0;
		pm(0,0) = enigma::view_matrix(0,0), pm(0,1) = enigma::view_matrix(0,1), pm(0,2) = enigma::view_matrix(0,2), pm(0,3) = enigma::view_matrix(0,3),
		pm(1,0) = enigma::view_matrix(1,0), pm(1,1) = enigma::view_matrix(1,1), pm(1,2) = enigma::view_matrix(1,2), pm(1,3) = enigma::view_matrix(1,3),
		pm(2,0) = enigma::view_matrix(2,0), pm(2,1) = enigma::view_matrix(2,1), pm(2,2) = enigma::view_matrix(2,2), pm(2,3) = enigma::view_matrix(2,3),
		pm(3,0) = enigma::view_matrix(3,0), pm(3,1) = enigma::view_matrix(3,1), pm(3,2) = enigma::view_matrix(3,2), pm(3,3) = enigma::view_matrix(3,3);
		return pm;
	}

	gs_scalar * d3d_transformation_get_mv(){
		return enigma::mv_matrix;
	}

	gs_scalar * d3d_transformation_get_mvp(){
		enigma::mvp_matrix = enigma::projection_matrix * enigma::mv_matrix;
		return enigma::mvp_matrix;
	}

	bool d3d_transform_stack_push(){
		trans_stack.push(enigma::model_matrix);
		return true;
	}
	bool d3d_transform_stack_pop(){
		if (trans_stack.size() == 0) return false;
		enigma::model_matrix = trans_stack.top();
		trans_stack.pop();
		enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
		return true;
	}

	void d3d_transform_stack_clear()
	{
			trans_stack = std::stack<enigma::Matrix4>();
			enigma::model_matrix.init_identity();
			enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	bool d3d_transform_stack_empty()
	{
		return (trans_stack.size() == 0);
	}

	bool d3d_transform_stack_top()
	{
		if (trans_stack.size() == 0) return false;
		enigma::model_matrix = trans_stack.top();
		enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
		return true;
	}

	bool d3d_transform_stack_discard()
	{
		if (trans_stack.size() == 0) return false;
		trans_stack.pop();
		return true;
	}

	bool d3d_projection_stack_push()
	{
	    //if (proj_stack.size() == 31) return false; //This is a GM limitation that ENIGMA doesn't have
	    proj_stack.push(enigma::projection_matrix);
	    view_stack.push(enigma::view_matrix);
	    return true;
	}

	bool d3d_projection_stack_pop()
	{
	    if (proj_stack.size() == 0) return false;
	    enigma::projection_matrix = proj_stack.top();
	    enigma::view_matrix = view_stack.top();
	    enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	    proj_stack.pop();
	    view_stack.pop();
	    return true;
	}

	void d3d_projection_stack_clear()
	{
	    proj_stack = std::stack<enigma::Matrix4>();
	    view_stack = std::stack<enigma::Matrix4>();
	    enigma::projection_matrix.init_identity();
	    enigma::view_matrix.init_identity();
	    enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	}

	bool d3d_projection_stack_empty()
	{
	    return (proj_stack.size() == 0);
	}

	bool d3d_projection_stack_top()
	{
	    if (proj_stack.size() == 0) return false;
	    enigma::projection_matrix = proj_stack.top();
	    enigma::view_matrix = view_stack.top();
	    enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
	    return true;
	}

	bool d3d_projection_stack_discard()
	{
	    if (proj_stack.size() == 0) return false;
	    proj_stack.pop();
	    view_stack.pop();
	    return true;
	}
}
