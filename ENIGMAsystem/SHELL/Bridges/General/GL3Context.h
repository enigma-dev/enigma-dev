/** Copyright (C) 2013 Robert B. Colton
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
using namespace enigma_user;

#include <vector>
#include <map>
using std::vector;
using std::map;

//TODO: Replace the fixed function pipeline with shaders

class ContextManager {
private:

protected:
  map< GLenum, GLuint > cacheTextureStates;    /// cached texture stages
  
public:

    vector<gs_scalar> globalVBO_data;
    vector<unsigned int> globalVBO_indices;
    GLuint globalVBO; //The buffer itself
    unsigned int globalVBO_datCount; //Length of the VBO data buffer
    unsigned int globalVBO_verCount; //Number of vertices on this buffer
    unsigned int globalVBO_indCount; //Number of indices
    unsigned int globalVBO_maxBSize; //Maximum buffer size. Buffer will not be regenerated if the current buffer size is smaller than the maximum

ContextManager() {
	glGenBuffersARB(1, &globalVBO);
	
    unsigned int globalVBO_datCount = 0; //Length of the VBO data buffer
    unsigned int globalVBO_verCount = 0; //Number of vertices on this buffer
    unsigned int globalVBO_indCount = 0; //Number of indices
}

~ContextManager() {

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

void BeginShapesBatching() {

}

void EndShapesBatching() {
	if (globalVBO_verCount < 1) { return; }
	//int fbo;
	//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
	//printf("RENDERING THIS - Verts = %i, inds = %i and fbo = %i, data size = %i, index size = %i\n",globalVBO_verCount,globalVBO_indCount,fbo,globalVBO_data.size(),globalVBO_indices.size() );
	//glBindTexture(GL_TEXTURE_2D,0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER, globalVBO);

	// Textures should be clamped when rendering 2D sprites and stuff, so memorize it.
	GLint wrapr, wraps, wrapt;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, &wrapr);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wraps);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &wrapt);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	if (globalVBO_verCount>globalVBO_maxBSize) glBufferDataARB(GL_ARRAY_BUFFER, globalVBO_datCount * sizeof(gs_scalar), &globalVBO_data[0], GL_DYNAMIC_DRAW), globalVBO_maxBSize = globalVBO_verCount;
	else glBufferSubDataARB(GL_ARRAY_BUFFER, 0, globalVBO_datCount * sizeof(gs_scalar), &globalVBO_data[0]);
	glVertexPointer( 2, GL_FLOAT, sizeof(gs_scalar) * 8, NULL );
	glTexCoordPointer( 2, GL_FLOAT, sizeof(gs_scalar) * 8, (void*)(sizeof(gs_scalar) * 2) );
	glColorPointer( 4, GL_FLOAT, sizeof(gs_scalar) * 8, (void*)(sizeof(gs_scalar) * 4) );


	// this sprite batching mechanism does not allow one to apply transformations to sprites or text
	// like is possible with the Direct3D 9 sprite batcher or traditionally in Game Maker.
	//if (d3dZWriteEnable) {
	 // glDepthMask(false);
	//}
	glDrawElements(GL_TRIANGLES, globalVBO_indCount, GL_UNSIGNED_INT, &globalVBO_indices[0] );
	//if (d3dZWriteEnable) {
	 // glDepthMask(true);
	//}

	// And now reset the textures repetition
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);

	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glBindBufferARB(GL_ARRAY_BUFFER, 0);

	globalVBO_datCount = globalVBO_verCount = globalVBO_indCount = 0;
}

void BeginScene() {
	BeginShapesBatching();
}

void EndScene() {
	EndShapesBatching();
}

void SetEnabled(GLenum cap, bool enabled) {
	EndShapesBatching();
	(enabled?glEnable:glDisable)(cap);
}

void glBind() {
	EndShapesBatching();
}

void glActiveTexture() {
	EndShapesBatching();
}

void glReadPixels() {
	EndShapesBatching();
}

void glBindTexture(GLenum target,  GLuint texture) {
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

void glViewport() {
	EndShapesBatching();
}

void glBlendFunc() {
	EndShapesBatching();
}

};

extern ContextManager* oglmgr; // point to our device manager

#endif