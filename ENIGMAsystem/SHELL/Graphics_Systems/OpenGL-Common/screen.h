namespace enigma {

void gl_screen_init();
void msaa_fbo_blit();


// these declarations/signatures are made for the opengl-common,and gles
unsigned char* graphics_copy_screen_pixels(int ,int,int,int,bool*);
unsigned char* graphics_copy_screen_pixels(unsigned*, unsigned*, bool*);






} // namespace enigma
