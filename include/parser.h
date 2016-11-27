//
// Created by bahuljain on 11/26/16.
//

#include <vector>
#include <iostream>
#include <linmath.h>

using namespace std;

#ifndef OPENGLRENDER_PARSER_H
#define OPENGLRENDER_PARSER_H

#endif //OPENGLRENDER_PARSER_H

void read_wavefront_file(
        const char *file,
        std::vector<int> &tris,
        std::vector<float> &verts);

int parseObjFile(string filename, vector<float> &vertices);