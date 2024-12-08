#version 330

// Input
in vec3 frag_color1;
in vec3 frag_color2;
in float frag_noise;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // Color the fragment using a mix of colors and a noise.
    vec3 final_color = mix(frag_color1, frag_color2, frag_noise);
    out_color = vec4(final_color, 1);
}
