#define MATRIX_VIEW                                    0
#define MATRIX_PROJECTION                              1
#define MATRIX_WORLD                                   2
#define MATRIX_WORLD_VIEW                              3
#define MATRIX_WORLD_VIEW_PROJECTION                   4
#define MATRICES_MAX                                   5

uniform mat4 transform_matrix[MATRICES_MAX];
#define gm_Matrices transform_matrix
#define modelMatrix transform_matrix[MATRIX_WORLD]
#define modelViewMatrix transform_matrix[MATRIX_WORLD_VIEW]
#define projectionMatrix transform_matrix[MATRIX_PROJECTION]
#define viewMatrix transform_matrix[MATRIX_VIEW]
#define modelViewProjectionMatrix transform_matrix[MATRIX_WORLD_VIEW_PROJECTION]
#define in_Colour in_Color

uniform mat3 normalMatrix;
#line 0
