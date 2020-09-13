#include "texture.h"

#include "Widget_Systems/widgets_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/image_formats.h"

namespace enigma {
  
void Texture::init() {
  if (pxdata != nullptr) {
    if (channels == 1) {
      unsigned char* rgba = mono_to_rgba(pxdata, width, height);
      ID = enigma::graphics_create_texture(enigma::RawImage(rgba, width, height), false);
      delete[] pxdata;
    } else ID = enigma::graphics_create_texture(enigma::RawImage(pxdata, width, height), false);
    
  } else DEBUG_MESSAGE("Error: trying to intialize texture from empty pixel data", MESSAGE_TYPE::M_ERROR);
}

}
