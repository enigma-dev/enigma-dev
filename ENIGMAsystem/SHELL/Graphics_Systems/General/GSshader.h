#ifndef ENIGMA_GS_SHADER_H
#define ENIGMA_GS_SHADER_H

#include "Universal_System/var4.h"

namespace enigma {

extern const char* shader_version;
extern const char* vertex_prefix;
extern const char* vertex_body;
extern const char* fragment_prefix;
extern const char* fragment_body;

} //namespace enigma

namespace enigma_user {

void shader_set(int id);
void shader_reset();
int shader_current();
bool shader_is_compiled();
int shader_get_uniform(int program, string name);
int shader_get_sampler_index(int program, string name);
void shader_set_uniform_f(int location, float v0);
void shader_set_uniform_f(int location, float v0, float v1);
void shader_set_uniform_f(int location, float v0, float v1, float v2);
void shader_set_uniform_f(int location, float v0, float v1, float v2, float v3);
void shader_set_uniform_f_array(int location, const var& array);
void shader_set_uniform_i(int location, int v0);
void shader_set_uniform_i(int location, int v0, int v1);
void shader_set_uniform_i(int location, int v0, int v1, int v2);
void shader_set_uniform_i(int location, int v0, int v1, int v2, int v3);
void shader_set_uniform_i_array(int location, const var& array);
void shader_set_uniform_matrix(int location);
void shader_set_uniform_matrix_array(int location, const var& array);

}

#endif //ENIGMA_GS_SHADER_H
