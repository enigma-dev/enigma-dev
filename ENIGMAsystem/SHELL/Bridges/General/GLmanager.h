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

#include "OpenGLHeaders.h"
#include "Graphics_Systems/OpenGL-OpenGLES/GLprofiler.h"
#include "Graphics_Systems/General/GSmodel.h"

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
bool hasdrawn;
int shapes_d3d_model;
int shapes_d3d_texture;
enigma::GPUProfiler gpuprof;

ContextManager() {
	hasdrawn = false;
	shapes_d3d_model = -1;
	shapes_d3d_texture = -1;
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

void BeginScene() {
	last_depth = 0;
	// Reapply the stored render states and what not
	RestoreState();
}

void EndScene() {
  gpuprof.end_frame();
}

void SetEnabled(GLenum cap, bool enabled) {
	(enabled?glEnable:glDisable)(cap);
}

void Bind() {

}

void ActiveTexture() {

}

void ReadPixels() {

}

// Function no longer used since introduction of sampler states for compatibility with Studio.
void BindTexture(GLenum target,  GLuint texture) {
  if (bound_tex != texture) {
      glBindTexture(target, bound_tex = texture);
  }
  return;
  // Update the texture state cache
  // If the return value is 'true', the command must be forwarded to OpenGL
  map< GLenum, GLuint >::iterator it = cacheTextureStates.find( target );
  if ( cacheTextureStates.end() == it ) {
    cacheTextureStates.insert( map< GLenum, GLuint >::value_type(target, texture) );
    glBindTexture(target, texture);
  }
  if( it->second == texture )
      return;
  it->second = texture;
  glBindTexture(target, texture);
}

void ResetTextureStates() {
	// loop through and check if any of the texture states opengl has set do not match the ones we've cached
}

GLuint GetBoundTexture() { //This is used for cases when there are texture coordinates provided, but texture is 0 (like d3d_model_block)
	return bound_tex;
}

};

extern ContextManager* oglmgr; // point to our device manager

#endif
