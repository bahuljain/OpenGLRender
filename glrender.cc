#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "include/parser.h"

typedef vec4 point4;
typedef vec4 color4;

// source provided for function to load and compile shaders
GLuint InitShader(const char *vertexShaderFile,
                  const char *fragmentShaderFile);

// viewer's position, for lighting calculations
//point4 viewer = {0.0, 0.0, -10.0f, 1.0};

// light & material definitions, again for lighting calculations:
point4 light_position = {100.0, 100.0, 100.0f, 1.0};
color4 light_ambient = {0.2, 0.2, 0.2, 1.0};
color4 light_diffuse = {1.0, 1.0, 1.0, 1.0};
color4 light_specular = {1.0, 1.0, 1.0, 1.0};

color4 material_ambient = {1.0, 0.0, 1.0, 1.0};
color4 material_diffuse = {1.0, 0.8, 0.0, 1.0};
color4 material_specular = {1.0, 0.8, 0.0, 1.0};
float material_shininess = 100.0;

// "names" for the various buffers, shaders, programs etc:
GLuint vertex_buffer, program;
GLint p_location, v_location, vpos_location, vnorm_location, viewer_location;
GLint ld_location, ls_location, la_location, lp_location;
GLint md_location, ms_location, ma_location, mshiny_location;

float theta = 0.0f;  // angle with the z axis
float phi = 90.0;    // angle with the Y axis
float r = 10.0f;      // distance of viewer from origin
float posx = 0.0f;   // translation along X
float posy = 0.0f;   // translation along Y

// transform th
// e triangle's vertex data and Put it into the points array.
// also, compute the normals at each vertex, and put that into the norms array.
void tri(vec4 vertices[], point4 points[], vec4 norms[], int NumVertices) {
    for (int k = 0; k < NumVertices / 3; k++) {
        vecset(points[3 * k + 0], vertices[3 * k + 0]);
        vecset(points[3 * k + 1], vertices[3 * k + 1]);
        vecset(points[3 * k + 2], vertices[3 * k + 2]);

        // compute the lighting at each vertex, then set it as the color there:

        vec4 e1, e2, n;
        vec4_sub(e1, points[3 * k + 1], points[3 * k]);
        vec4_sub(e2, points[3 * k + 2], points[3 * k]);
        vec4_mul_cross(n, e1, e2);
        n[3] = 0.f; // cross product in 4d sets this to 1.0, which we do not want
        vec4_norm(n, n);

        vecset(norms[3 * k + 0], n);
        vecset(norms[3 * k + 1], n);
        vecset(norms[3 * k + 2], n);
    }
}

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        r = fminf(50, r + 0.5f);

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        r = fmaxf(3, r - 0.5f);
}

void init(int points_size, int colors_size) {

    // set up vertex buffer object - this will be memory on the GPU where
    // we are going to store our vertex data (that is currently in the "points"
    // array)
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);


    // specify that its part of a VAO, what its size is, and where the
    // data is located, and finally a "hint" about how we are going to use
    // the data (the driver will put it in a good memory location, hopefully)
    glBufferData(GL_ARRAY_BUFFER, points_size + colors_size,
                 NULL, GL_DYNAMIC_DRAW);

    // load in these two shaders...  (note: InitShader is defined in the
    // accompanying initshader.c code).
    // the shaders themselves must be text glsl files in the same directory
    // as we are running this program:
    program = InitShader("vshader_passthrough_lit.glsl",
                         "fshader_passthrough_lit.glsl");

    // ...and set them to be active
    glUseProgram(program);

    // get acces to the various things we will be sending to the shaders:
    v_location = glGetUniformLocation(program, "V");
    p_location = glGetUniformLocation(program, "P");
    vpos_location = glGetAttribLocation(program, "vPos");
    vnorm_location = glGetAttribLocation(program, "vNorm");
    ld_location = glGetUniformLocation(program, "light_diffuse");
    ls_location = glGetUniformLocation(program, "light_specular");
    la_location = glGetUniformLocation(program, "light_ambient");
    lp_location = glGetUniformLocation(program, "light_position");
    md_location = glGetUniformLocation(program, "material_diffuse");
    ms_location = glGetUniformLocation(program, "material_specular");
    ma_location = glGetUniformLocation(program, "material_ambient");
    mshiny_location = glGetUniformLocation(program, "material_shininess");
    viewer_location = glGetUniformLocation(program, "viewer");

    glUniform4fv(ld_location, 1, (const GLfloat *) light_diffuse);
    glUniform4fv(ls_location, 1, (const GLfloat *) light_specular);
    glUniform4fv(la_location, 1, (const GLfloat *) light_ambient);
    glUniform4fv(lp_location, 1, (const GLfloat *) light_position);
    glUniform4fv(md_location, 1, (const GLfloat *) material_diffuse);
    glUniform4fv(ms_location, 1, (const GLfloat *) material_specular);
    glUniform4fv(ma_location, 1, (const GLfloat *) material_ambient);
    glUniform1f(mshiny_location, material_shininess);

    glEnableVertexAttribArray(vpos_location);

    // the vPosition attribute is a series of 4-vecs of floats, starting at the
    // beginning of the buffer
    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE,
                          0, (void *) (0));

    glEnableVertexAttribArray(vnorm_location);
    glVertexAttribPointer(vnorm_location, 4, GL_FLOAT, GL_FALSE,
                          0, (void *) (long) points_size);
}

static void mouse_move_rotate(GLFWwindow *window, double x, double y) {

    static int lastx = x;// keep track of where the mouse was last:

    int amntX = (int) (x - lastx);
    if (amntX != 0) {
        theta += amntX;
        if (theta > 360.0) theta -= 360.0;
        if (theta < 0.0) theta += 360.0;

        lastx = (int) x;
    }

    static int lasty = y;// keep track of where the mouse was last:

    int amntY = (int) (y - lasty);
    if (amntY != 0) {
        phi += amntY;
        if (phi > 175.0) phi = 175.0;
        if (phi < 5.0) phi = 5.0;

        lasty = (int) y;
    }

    //  std::cout << theta << std::endl;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "usage: ./simple2 filename.obj" << std::endl;
        exit(EXIT_FAILURE);
    }

    vector<float> verts;
    const int NumVertices = parseObjFile(argv[1], verts);

    vec4 vertices[NumVertices];

    for (int i = 0; i < NumVertices; i++) {
        vertices[i][0] = verts[3 * i];
        vertices[i][1] = verts[3 * i + 1];
        vertices[i][2] = verts[3 * i + 2];
        vertices[i][3] = 1;
    }

    // we will copy our transformed points to here:
    point4 points[NumVertices];

    // and we will store the norms, per face per vertex, here. since there is
    // only 1 triangle, with 3 vertices, there will just be 3 here:
    vec4 norms[NumVertices];

    // if there are errors, call this routine:
    glfwSetErrorCallback(error_callback);

    // start up GLFW:
    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);

    // for more modern version of OpenGL:
    //  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(640, 480, "hello triangle transform!", NULL,
                              NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    // call only once: demo for rotation:
    glfwSetCursorPosCallback(window, mouse_move_rotate);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    init(sizeof(points), sizeof(norms));

    // enable z-buffering if you are loading inmore than one triangle -
    // this keeps things closer to the screen drawn in front!
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // tri() will multiply the points by ctm, and figure out the lighting too
    tri(&vertices[0], &points[0], &norms[0], NumVertices);

    // tell the VBO to get the data from the points and norms arrays:
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(norms),
                    norms);

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        mat4x4 p, v;

        // in case the  window viewport size changed, we will need to adjust the
        // projection:
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);

        // clear the window (with white) and clear the z-buffer
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 up = {0, 1.0f, 0};
        vec3 center = {0, 0, 0};


        float x = r * sinf(theta * deg_to_rad) * sinf(phi * deg_to_rad);
        float y = r * cosf(phi * deg_to_rad);
        float z = r * cosf(theta * deg_to_rad) * sinf(phi * deg_to_rad);

        vec3 eye = {x, y ,z};

        vec4 viewer = {x, y, z, 1.0f};
        glUniform4fv(viewer_location, 1, (const GLfloat *) viewer);

        mat4x4_look_at(v, eye, center, up);
        mat4x4_perspective(p, 30 * deg_to_rad, ratio, 0.1f, 100);

        // send that projection to the device, where the shader will apply it:
        glUniformMatrix4fv(p_location, 1, GL_FALSE, (const GLfloat *) p);
        glUniformMatrix4fv(v_location, 1, GL_FALSE, (const GLfloat *) v);

        glDrawArrays(GL_TRIANGLES, 0, NumVertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
