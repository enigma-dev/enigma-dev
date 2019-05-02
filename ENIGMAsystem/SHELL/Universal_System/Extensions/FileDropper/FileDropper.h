#include <string>

using std::string;

namespace enigma_user {

bool file_dnd_get_enabled();
void file_dnd_set_enabled(bool enable);
string file_dnd_get_files();
void file_dnd_set_files(string pattern, bool allowfiles, bool allowdirs, bool allowdups);
void file_dnd_add_files(string files);
void file_dnd_remove_files(string files);

}
