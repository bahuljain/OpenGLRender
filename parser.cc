//
// Created by bahuljain on 11/26/16.
//
#include "include/parser.h"

#include <fstream>
#include <sstream>

using namespace std;

// Given the name of a wavefront (OBJ) file that consists JUST of
// vertices, triangles, and comments, read it into the tris and verts
// vectors.
//
// tris is a vector of ints that is 3*n long, where n is the number of
// triangles. The ith triangle has vertex indexes 3*i, 3*i+1, and 3*i+2.
//
// The ith triangle has vertices:
//
//     verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2],
//     verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2],
//     verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]
//
// given in counterclockwise order with respect to the surface normal
//
// If you are using the supplied Parser class, you should probably make this
// a method on it: Parser::read_wavefront_file().
//
void read_wavefront_file(
        const char *file,
        vector<int> &tris,
        vector<float> &verts) {

    // clear out the tris and verts vectors:
    tris.clear();
    verts.clear();

    ifstream in(file);
    char buffer[1025];
    string cmd;

    for (int line = 1; in.good(); line++) {
        in.getline(buffer, 1024);
        buffer[in.gcount()] = 0;

        cmd = "";

        istringstream iss(buffer);

        iss >> cmd;

        if (cmd[0] == '#' or cmd.empty()) {
            // ignore comments or blank lines
            continue;
        } else if (cmd == "v") {
            // got a vertex:

            // read in the parameters:
            double pa, pb, pc;
            iss >> pa >> pb >> pc;

            verts.push_back(pa);
            verts.push_back(pb);
            verts.push_back(pc);
        } else if (cmd == "f") {
            // got a face (triangle)

            // read in the parameters:
            int i, j, k;
            iss >> i >> j >> k;

            // vertex numbers in OBJ files start with 1, but in C++ array
            // indices start with 0, so we're shifting everything down by
            // 1
            tris.push_back(i - 1);
            tris.push_back(j - 1);
            tris.push_back(k - 1);
        } else {
            cerr << "Parser error: invalid command at line " << line
                 << endl;
        }

    }

    in.close();

    //   std::cout << "found this many tris, verts: " << tris.size () / 3.0 << "  " << verts.size () / 3.0 << std::endl;
}

int parseObjFile(string filename, vector<float> &vertices,
                 vector<Vector> &vertex_norms) {
    vector<int> tris;
    vector<float> verts;

    read_wavefront_file(filename.c_str(), tris, verts);
    Vector vertex_normals[verts.size()/3];

    for (unsigned int i = 0; i < verts.size() / 3; i++)
        vertex_normals[i] = {0, 0, 0};

    for (unsigned int i = 0; i < tris.size() / 3; i++) {
        int v1, v2, v3;
        float x1, y1, z1, x2, y2, z2, x3, y3, z3;
        Vector e1, e2, n;

        v1 = tris[3 * i];
        v2 = tris[3 * i + 1];
        v3 = tris[3 * i + 2];

        x1 = verts[3 * v1];
        y1 = verts[3 * v1 + 1];
        z1 = verts[3 * v1 + 2];
        Vector p1 (x1, y1, z1);

        x2 = verts[3 * v2];
        y2 = verts[3 * v2 + 1];
        z2 = verts[3 * v2 + 2];
        Vector p2 (x2, y2, z2);

        x3 = verts[3 * v3];
        y3 = verts[3 * v3 + 1];
        z3 = verts[3 * v3 + 2];
        Vector p3 (x3, y3, z3);

        e1 = p2.plus(-p1);
        e2 = p3.plus(-p1);
        n = e1.cross(e2).norm();

        vertex_normals[v1].plusEq(n);
        vertex_normals[v2].plusEq(n);
        vertex_normals[v3].plusEq(n);

        vertices.push_back(x1);
        vertices.push_back(y1);
        vertices.push_back(z1);
        vertices.push_back(x2);
        vertices.push_back(y2);
        vertices.push_back(z2);
        vertices.push_back(x3);
        vertices.push_back(y3);
        vertices.push_back(z3);
    }

    for (unsigned int i = 0; i < verts.size() / 3; i++) {
        if (!vertex_normals[i].equals(Vector(0, 0, 0)))
            vertex_normals[i] = vertex_normals[i].norm();
    }

    for (unsigned int i = 0; i < tris.size() / 3; i++) {
        vertex_norms.push_back(vertex_normals[tris[3 * i]]);
        vertex_norms.push_back(vertex_normals[tris[3 * i + 1]]);
        vertex_norms.push_back(vertex_normals[tris[3 * i + 2]]);
    }

    return (int) tris.size();
}
