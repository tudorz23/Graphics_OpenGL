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


// Output
out vec3 frag_normal;
out vec2 frag_textcoord;
out vec3 frag_color;


void main()
{
    // Send output to fragment shader
    frag_normal = v_normal;
    frag_textcoord = v_textcoord;
    frag_color = v_color;

    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
