#version 330

// Input
in vec3 frag_normal;
in vec2 frag_textcoord;
in vec3 frag_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // Write pixel out color
    out_color = vec4(frag_color, 1);
}
