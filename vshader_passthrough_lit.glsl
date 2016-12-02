#version 120

uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

uniform vec4 viewer;

// input transform from the host program, for an orthographic projection (see
// the class notes and the code that makes this projection to verify!).
// a rotation around the world Y (up) axis has already been applied on the host.
uniform mat4 MVP;


// we are going to be getting color values, per-vertex, from the main
// program running on the cpu (and that are then stored in the VBO on the
// card. this color, called "vCol", is just going to be passed through
// to the fragment shader, which will intrpolate its value per-fragment
// amongst the 3 vertex colors on the triangle that fragment is a part of.
//
// on mac you may have to say: "attribute" instead of "in":
attribute vec4 vCol;

// we are going to be getting an attribute from the main program, named
// "vPos", one for each vertex.
//
// on mac you may have to say: "attribute vec4 vPosition;" instead of "in...":
attribute vec4 vPos;
attribute vec4 vNorm;

// our shader outputs one color value per vertex: we will send it to the
// fragment shader, and along the way we want the device to interpolate
// the per-vertex values:
varying vec4 color;

void main()
{
    vec4 ambient_color = material_ambient * light_ambient;
    vec4 diffuse_product = light_diffuse * material_diffuse;
    vec4 spec_product = light_specular * material_specular;

    vec4 light_vec = normalize(light_position - vPos);

    float dd1 = max(0, dot(light_vec, vNorm));
    vec4 diffuse_color = diffuse_product * dd1;

    vec4 view_vec = normalize(viewer - vPos);
    vec4 bisector = normalize(light_vec + view_vec);

    float dd2 = max(0, dot(bisector, vNorm));
    vec4 specular_color = spec_product * pow(dd2, material_shininess);
    vec4 final_color = ambient_color + diffuse_color + specular_color;
    final_color[3] = 1.0;

    gl_Position = MVP * vPos;
    color = final_color;
}


