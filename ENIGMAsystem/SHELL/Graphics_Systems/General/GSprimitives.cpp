#include "GSprimitives.h"

namespace enigma_user {

unsigned draw_primitive_count(int kind, unsigned vertex_count) {
  switch (kind) {
    case pr_pointlist: return vertex_count;
    case pr_linelist: return vertex_count / 2;
    case pr_linestrip: if (vertex_count > 1) return vertex_count - 1; break;
    case pr_trianglelist: return vertex_count / 3;
    case pr_trianglestrip: if (vertex_count > 2) return vertex_count - 2; break;
    case pr_trianglefan: if (vertex_count > 2) return vertex_count - 2; break;
  }
  return 0;
}

}
