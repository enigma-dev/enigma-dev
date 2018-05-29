#include "GSprimitives.h"

namespace enigma_user {

int draw_primitive_count(int kind, int vertex_count) {
  size_t primitive_count = 0;
  switch (kind) {
    case pr_pointlist: primitive_count = vertex_count; break;
    case pr_linelist: primitive_count = vertex_count / 2; break;
    case pr_linestrip: primitive_count = vertex_count - 1; break;
    case pr_trianglelist: primitive_count = vertex_count / 3; break;
    case pr_trianglestrip: primitive_count = vertex_count - 2; break;
    case pr_trianglefan: primitive_count = vertex_count - 2; break;
  }
  if (primitive_count < 0) primitive_count = 0;
  return primitive_count;
}

}
