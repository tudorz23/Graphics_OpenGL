#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_textcoord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 Color1;
uniform vec3 Color2;


// Output
out vec3 frag_normal;
out vec2 frag_textcoord;
out vec3 frag_color;


float random (in vec2 arg)
{
    return fract(sin(dot(arg.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}


float noise(in vec2 coord)
{
    vec2 i = floor(coord);
    vec2 f = fract(coord);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    //vec2 u = f * f * (3.0 - 2.0 * f);
    vec2 u = smoothstep(0.,1.,f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}


void main()
{
    // Send output to fragment shader
    frag_normal = v_normal;
    frag_textcoord = v_textcoord;
    frag_color = v_color;

    vec2 param = vec2(v_position.x, v_position.z);
    float noise_val = noise(param * 0.3f);

    vec3 final_position = vec3(v_position.x, noise_val * 0.2, v_position.z);

//    vec3 color1 = vec3(0.78f, 0.05f, 0.94f);
//    vec3 color1 = vec3(0.82, 0.647, 0.376);
//    vec3 color2 = vec3(0.216, 0.149, 0.612);
//    vec3 color2 = vec3(0.42, 0.439, 0.306);
//    vec3 color2 = vec3(0.639, 0.631, 0.078);

    frag_color = mix(Color1, Color2, noise_val);

    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(final_position, 1.0);
}
