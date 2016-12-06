//
// Created by bahuljain on 12/2/16.
//

#include <linmath.h>

#ifndef OPENGLRENDER_HELPER_H
#define OPENGLRENDER_HELPER_H

#endif //OPENGLRENDER_HELPER_H

const float deg_to_rad = (3.1415926f / 180.0f);

// three helper functions for the vec4 class:
void vecproduct(vec4 &res, const vec4 &v1, const vec4 &v2) {
    for (int i = 0; i < 4; ++i)
        res[i] = v1[i] * v2[i];
}

void vecset(vec4 &res, const vec4 &v1) {
    for (int i = 0; i < 4; ++i)
        res[i] = v1[i];
}

void vecclear(vec4 &res) {
    for (int i = 0; i < 4; ++i)
        res[i] = 0.0;
}