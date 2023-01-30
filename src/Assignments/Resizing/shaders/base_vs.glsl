#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 a_vertex_color;
out vec4 b_vertex_color;
    layout(std140, binding=1) uniform Transformations {
        mat4 PVM;
    };

void main() {
    b_vertex_color = a_vertex_color;
    gl_Position =  PVM * a_vertex_position;
}
