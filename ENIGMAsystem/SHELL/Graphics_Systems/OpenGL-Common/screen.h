namespace enigma {

void gl_screen_init();
void msaa_fbo_blit();

unsigned char* graphics_copy_screen_pixels(int ,int,int,int,bool*);
unsigned char* graphics_copy_screen_pixels(unsigned*, unsigned*, bool*);

} // namespace enigma
