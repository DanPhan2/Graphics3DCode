#version 410
 in vec4 b_vertex_color;
layout(location=0) out vec4 vFragColor;

void main() {
    vFragColor = b_vertex_color;
}
