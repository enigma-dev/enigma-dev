#include "Graphics_Systems/OpenGL-Common/screen.h"
#include "OpenGLHeaders.h"
#include "Platforms/General/PFwindow.h"

namespace enigma {

GLuint msaa_fbo = 0;

void gl_screen_init() {
  //TODO: This never reports higher than 8, but display_aa should be 14 if 2,4,and 8 are supported and 8 only when only 8 is supported
  glGetIntegerv(GL_MAX_SAMPLES_EXT, &enigma_user::display_aa);
}

void msaa_fbo_blit() {
  if (!GLEW_EXT_framebuffer_object || !msaa_fbo) return;
  GLint fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::msaa_fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  //TODO: Change the code below to fix this to size properly to views
  glBlitFramebuffer(0, 0, enigma_user::window_get_region_width_scaled(), enigma_user::window_get_region_height_scaled(),
                    0, 0, enigma_user::window_get_region_width_scaled(), enigma_user::window_get_region_height_scaled(),
					GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  // glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
}

} // namespace enigma

namespace enigma_user {

void display_reset(int samples, bool vsync) {
  set_synchronization(vsync);

  if (!GLEW_EXT_framebuffer_object) return;

  GLint fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);

  GLuint ColorBufferID, DepthBufferID;

  // Cleanup the multi-sampler fbo if turning off multi-sampling
  if (samples == 0) {
    if (enigma::msaa_fbo != 0) {
      glDeleteFramebuffers(1, &enigma::msaa_fbo);
      enigma::msaa_fbo = 0;
    }
    return;
  }

  //TODO: Change the code below to fix this to size properly to views
  // If we don't already have a multi-sample fbo then create one
  if (enigma::msaa_fbo == 0) {
    glGenFramebuffersEXT(1, &enigma::msaa_fbo);
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, enigma::msaa_fbo);
  // Now make a multi-sample color buffer
  glGenRenderbuffersEXT(1, &ColorBufferID);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, ColorBufferID);
  glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, GL_RGBA8, window_get_region_width(), window_get_region_height());
  // We also need a depth buffer
  glGenRenderbuffersEXT(1, &DepthBufferID);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, DepthBufferID);
  glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, GL_DEPTH_COMPONENT24, window_get_region_width(), window_get_region_height());
  // Attach the render buffers to the multi-sampler fbo
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, ColorBufferID);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, DepthBufferID);
}

} // namespace enigma_user
