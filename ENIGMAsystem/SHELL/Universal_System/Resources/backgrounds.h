#include <string>

namespace enigma_user
{
int background_add(std::string filename, bool transparent = false, bool smooth = false, bool preload = true, bool mipmap = false);
int background_create_color(unsigned w, unsigned h, int col, bool preload = true);
bool background_replace(int back, std::string filename, bool transparent = false, bool smooth = false, bool preload = true, bool free_texture = true, bool mipmap = false);
void background_save(int back, std::string fname);
void background_delete(int back, bool free_texture = true);
int background_duplicate(int back);
void background_assign(int back, int copy_background, bool free_texture = true);
bool background_exists(int back);
void background_set_alpha_from_background(int back, int copy_background, bool free_texture = true);
int background_get_texture(int backId);
int background_get_width(int backId);
int background_get_height(int backId);
var background_get_uvs(int backId);

// Used for testing
bool background_textures_equal(int id1, int id2);
uint32_t background_get_pixel(int bkg, unsigned x, unsigned y);

} //namespace enigma_user
