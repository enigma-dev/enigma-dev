/** Copyright (C) 2013-2014 Robert B. Colton, Harijs Grinbergs
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

#ifndef OPENGL3CONTEXTMANAGER
#define OPENGL3CONTEXTMANAGER

#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSmodel.h"
#include "Graphics_Systems/OpenGL3/GL3profiler.h"

#include <vector>
#include <map>
using std::vector;
using std::map;

class ContextManager {
private:
GLuint bound_tex;

protected:
  map< GLenum, GLuint > cacheTextureStates;    /// cached texture stages

public:

float last_depth;
int last_stride;
bool hasdrawn;
int shapes_d3d_model;
int shapes_d3d_texture;
enigma::GPUProfiler gpuprof;

ContextManager() {
	hasdrawn = false;
	shapes_d3d_model = -1;
	shapes_d3d_texture = -1;
	last_stride = -1;
	last_depth = 0.0f;
	bound_tex = 0;
}

~ContextManager() {

}

float GetDepth() {
	return last_depth;
}

int GetShapesModel() {
	return shapes_d3d_model;
}

//TODO: Write this method so that for debugging purposes we can dump the entire render state to a text file.
void DumpState() {

}

//TODO: Write this method so that we can serialize and save the entire render state to be reloaded
void SaveState() {

}

//TODO: Write this method so that we can read and restore a previously saved render state
void LoadState() {

}

//TODO: Write this method so that it applies its cached render states to the graphics handle
void RestoreState() {

}

void BeginShapesBatching(int texId) {
	if (shapes_d3d_model == -1) {
		shapes_d3d_model = enigma_user::d3d_model_create(enigma_user::model_stream);
		last_stride = -1;
	} else if (texId != shapes_d3d_texture || (enigma_user::d3d_model_get_stride(shapes_d3d_model) != last_stride && last_stride != -1)) {
		last_stride = -1;
		if (!hasdrawn) {
			enigma_user::d3d_model_draw(shapes_d3d_model, shapes_d3d_texture);
			enigma_user::d3d_model_clear(shapes_d3d_model);
		}
	} else {
		last_stride = enigma_user::d3d_model_get_stride(shapes_d3d_model);
	}
	hasdrawn = false;
	shapes_d3d_texture = texId;
}

void EndShapesBatching() {
	if (hasdrawn || shapes_d3d_model == -1) { return; }
	hasdrawn = true;
	enigma_user::d3d_model_draw(shapes_d3d_model, shapes_d3d_texture);
	enigma_user::d3d_model_clear(shapes_d3d_model);

	shapes_d3d_texture = -1;
	last_stride = -1;
  last_depth -= 1;
}

void BeginScene() {
	last_depth = 0;
	// Reapply the stored render states and what not
	RestoreState();
}

void EndScene() {
  gpuprof.end_frame();
  EndShapesBatching();
}

void SetEnabled(GLenum cap, bool enabled) {
	EndShapesBatching();
	(enabled?glEnable:glDisable)(cap);
}

void Bind() {
	EndShapesBatching();
}

void ActiveTexture() {
	EndShapesBatching();
}

void ReadPixels() {
	EndShapesBatching();
}

// Function no longer used since introduction of sampler states for compatibility with Studio.
void BindTexture(GLenum target,  GLuint texture) {
  if (bound_tex != texture) {
      //EndShapesBatching();
      glBindTexture(target, bound_tex = texture);
  }
  return;
  // Update the texture state cache
  // If the return value is 'true', the command must be forwarded to OpenGL
  map< GLenum, GLuint >::iterator it = cacheTextureStates.find( target );
  if ( cacheTextureStates.end() == it ) {
    cacheTextureStates.insert( map< GLenum, GLuint >::value_type(target, texture) );
    EndShapesBatching();
    glBindTexture(target, texture);
  }
  if( it->second == texture )
      return;
  it->second = texture;
  EndShapesBatching();
  glBindTexture(target, texture);
}

void ResetTextureStates() {
	// loop through and check if any of the texture states opengl has set do not match the ones we've cached
}

void Viewport() {
	EndShapesBatching();
}

void Transformation() { //Used when calling 3d transformations (translation, scaling etc.)
	EndShapesBatching();
}

void BlendFunc() { //Used when calling blend functions
	EndShapesBatching();
}

void ColorFunc() { //Used when calling color functions
	if (shapes_d3d_model != -1){
		if (enigma_user::d3d_model_has_color(shapes_d3d_model) == false){
			EndShapesBatching();
		}
	}
}

void Lighting() { //Used when lighting is enabled/disabled
	EndShapesBatching();
}

GLuint GetBoundTexture() { //This is used for cases when there are texture coordinates provided, but texture is 0 (like d3d_model_block)
	return bound_tex;
}

};

extern ContextManager* oglmgr; // point to our device manager

#endif
