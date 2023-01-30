#version 420
in vec4 b_vertex_color;
layout(location=0) out vec4 vFragColor;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color; 
   };


void main() {
    vFragColor.a = b_vertex_color.a;
    vFragColor.rgb = strength*color*b_vertex_color.rgb;
}
