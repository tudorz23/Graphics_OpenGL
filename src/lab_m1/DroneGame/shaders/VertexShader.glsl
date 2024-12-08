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
out vec3 frag_color1;
out vec3 frag_color2;
out float frag_noise;


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

    vec2 u = smoothstep(0.,1.,f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}


void main()
{
    vec2 param = vec2(v_position.x, v_position.z);
    float noise_val = noise(param);

    vec3 final_position = vec3(v_position.x, noise_val * 0.25, v_position.z);

    // Send output to fragment shader.
    frag_color1 = Color1;
    frag_color2 = Color2;
    frag_noise = noise_val;

    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(final_position, 1.0);
}
