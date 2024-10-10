#include "Graphics_Systems/OpenGL-Common/shader.h"
#include <string>

namespace enigma {
  std::string texColorString = "TexColor = texture2D( en_TexSampler, v_TextureCoord.st ) * v_Color;";
}
