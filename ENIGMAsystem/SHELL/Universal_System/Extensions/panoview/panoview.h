#include <string>

namespace enigma {

void panoview_init();

} // namespace enigma

namespace enigma_user {

void panorama_create(std::string panorama, std::string defcursor);
double panorama_get_hangle();
void panorama_set_hangle(double hangle);
double panorama_get_vangle();
void panorama_set_vangle(double vangle);

} // namespace enigma_user
