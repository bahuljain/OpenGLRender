#version 120

// the simplest fragment shader: get the color (from the vertex shader -
// actually interpolated from values specified in the vertex shader - and
// just pass them through to the render:
// 
// on the mac, you may need to say "varying vec4 color;" instead of this:
//varying vec4 color;


// in GLSL 150, use layouts for this:
// layout (location = 0) in color

uniform bool frag;

uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;


// in later GLSL, use "in", and "out" respectively

// for lit:
varying vec4 vNormf;
varying vec4 light_vecf;
varying vec4 view_vecf;
varying vec4 color;

void main() 
{ 
  // "gl_FragColor" is already defined for us - it's the one thing you have
  // to set in the fragment shader:

    if (frag) {
        vec4 ambient_color = material_ambient * light_ambient;
        vec4 diffuse_product = light_diffuse * material_diffuse;
        vec4 spec_product = light_specular * material_specular;

        float dd1 = max(0, dot(light_vecf, vNormf));
        vec4 diffuse_color = diffuse_product * dd1;

        vec4 bisector = normalize(light_vecf + view_vecf);

        float dd2 = max(0, dot(bisector, vNormf));
        vec4 specular_color = spec_product * pow(dd2, material_shininess);
        vec4 final_color = ambient_color + diffuse_color + specular_color;
        final_color[3] = 1.0;

        gl_FragColor = final_color;
    }
    else
        gl_FragColor = color;
}

