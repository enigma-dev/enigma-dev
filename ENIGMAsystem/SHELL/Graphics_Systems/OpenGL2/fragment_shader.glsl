varying vec2 v_TextureCoord;
varying vec4 v_Color;

void main() {
  vec4 TexColor;
  if (en_TexturingEnabled == true) {
    TexColor = texture2D(en_TexSampler, v_TextureCoord.st) * v_Color;
  } else {
    TexColor = v_Color;
  }
  if (en_AlphaTestEnabled == true) {
    if (TexColor.a<=en_AlphaTestValue) discard;
  }
  gl_FragColor = TexColor;
}
