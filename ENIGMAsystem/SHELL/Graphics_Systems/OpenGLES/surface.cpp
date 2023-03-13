#include "Graphics_Systems/OpenGL-Common/surface_impl.h"
#include "Graphics_Systems/OpenGL-Common/textures_impl.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/graphics_mandatory.h"

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h" // for show_error
#endif

namespace enigma_user{

void surface_add_colorbuffer(int id, int index, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(enigma::RawImage(nullptr, surf.width, surf.height), false, nullptr, nullptr, internalFormat, format, type);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, enigma::get_texture_peer(texture), 0);
  glBindFramebuffer(GL_FRAMEBUFFER, enigma::bound_framebuffer);
}

void surface_add_depthbuffer(int id, int internalFormat, unsigned format, unsigned type){
  get_surface(surf,id);
  glBindFramebuffer(GL_FRAMEBUFFER, surf.fbo);
  int texture = enigma::graphics_create_texture_custom(enigma::RawImage(nullptr, surf.width, surf.height), false, nullptr, nullptr, internalFormat, format, type);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, enigma::get_texture_peer(texture), 0);
  glBindFramebuffer(GL_FRAMEBUFFER, enigma::bound_framebuffer);
}

void surface_set_target(int id)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surf,id);
  //This fixes several consecutive surface_set_target() calls without surface_reset_target.
  if (enigma::bound_framebuffer != 0) { d3d_transform_stack_pop(); d3d_projection_stack_pop();}
  enigma::bound_framebuffer = surf.fbo;
  glBindFramebuffer(GL_FRAMEBUFFER, surf.fbo); //bind it
  d3d_transform_stack_push();
  d3d_projection_stack_push();
  glViewport(0, 0, surf.width, surf.height);
  glScissor(0, 0, surf.width, surf.height);
  d3d_set_projection_ortho(0, surf.height, surf.width, -surf.height, 0);
}

void surface_reset_target(void)
{
  draw_batch_flush(batch_flush_deferred);
  enigma::bound_framebuffer = 0;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  d3d_transform_stack_pop();
  d3d_projection_stack_pop();
  screen_reset_viewport();
}

void surface_free(int id)
{
  get_surface(surf,id);
  if (enigma::bound_framebuffer == surf.fbo) glBindFramebuffer(GL_FRAMEBUFFER, enigma::bound_framebuffer=0);
  enigma::graphics_delete_texture(surf.texture);
  if (surf.write_only == true){
    if (surf.has_depth_buffer == true) { glDeleteRenderbuffers(1, &surf.depth_buffer); }
    if (surf.has_stencil_buffer == true) { glDeleteRenderbuffers(1, &surf.stencil_buffer); }
  }else if (surf.has_depth_buffer == true){
    enigma::graphics_delete_texture(surf.depth_buffer);
  }
  glDeleteFramebuffers(1, &surf.fbo);
  delete enigma::surfaces[id];
}

}// namespace enigma_user
