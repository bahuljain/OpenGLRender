#version 120

uniform bool frag;

uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

uniform vec4 viewer;

uniform mat4 V;
uniform mat4 P;

// in
attribute vec4 vPos;
attribute vec4 vNorm;

// out
varying vec4 color;
varying vec4 vNormf;
varying vec4 light_vecf;
varying vec4 view_vecf;

void main()
{
    vec4 light_vec = normalize(light_position - vPos);
    vec4 view_vec = normalize(viewer - vPos);

    vec4 final_color = vec4(0, 0, 0, 1.0);

    if (!frag) {
        vec4 ambient_color = material_ambient * light_ambient;
        vec4 diffuse_product = light_diffuse * material_diffuse;
        vec4 spec_product = light_specular * material_specular;

        float dd1 = max(0, dot(light_vec, vNorm));
        vec4 diffuse_color = diffuse_product * dd1;

        vec4 bisector = normalize(light_vec + view_vec);

        float dd2 = max(0, dot(bisector, vNorm));
        vec4 specular_color = spec_product * pow(dd2, material_shininess);
        final_color = ambient_color + diffuse_color + specular_color;
        final_color[3] = 1.0;
    }

    gl_Position = P * V * vPos;
    view_vecf = view_vec;
    light_vecf = light_vec;
    vNormf = vNorm;
    color = final_color;
}


