//
// Created by pbialas on 25.09.2020.
//

#include "app.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"


void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }
    std::vector<GLushort> indices = {0, 2, 1, 5, 6, 3, 3, 4,5};
    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,
            0.5f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,
            0.0f, 0.5f, 0.0f, 1.0f,0.0f,0.0f,
            -0.5f, 0.0f, 0.0f, 0.0f,1.0f,0.0f,
            0.5f, 0.0f, 0.0f, 0.0f,1.0f,0.0f,
            0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,
            0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,
            -0.5f, 0.0f, 0.0f, 0.0f,1.0f,0.0f}; 

    
    float strength=0.6;
    float color[3]={0.7,0.6,0.7};



    GLuint v_buffer_handle_uniforms;
    glGenBuffers(1, &v_buffer_handle_uniforms);
    
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms);

    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);    

    glBufferSubData(GL_UNIFORM_BUFFER, 0,sizeof(float),&strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(float),3*sizeof(float),color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
//////////////////////////////////////////////////

    float theta = 1.0*glm::pi<float>()/6.0f;
    auto cs = std::cos(theta);
    auto ss = std::sin(theta);  
    glm::mat2 rot{cs,ss,-ss,cs};
    glm::vec2 trans{0.0,  -0.25};
    glm::vec2 scale{0.5, 0.5};
    GLuint v_buffer_handle_uniforms_move;
    glGenBuffers(1, &v_buffer_handle_uniforms_move);
    
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);

    glBufferData(GL_UNIFORM_BUFFER, 12 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);    

//skala transform, rotacja
    glBufferSubData(GL_UNIFORM_BUFFER, 0,2*sizeof(float),&scale);
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(float),2*sizeof(float),&trans);
    glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(float),4*sizeof(float),&rot[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 8*sizeof(float),4*sizeof(float),&rot[1]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle); 
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering

    //buffer of indices
    GLuint v_buffer_handle_indices;
    glGenBuffers(1, &v_buffer_handle_indices);
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle_indices));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle_indices);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
 
    
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));
    

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBufferBase(GL_UNIFORM_BUFFER,0,v_buffer_handle_uniforms);
    glBindBufferBase(GL_UNIFORM_BUFFER,1,v_buffer_handle_uniforms_move);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto[w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    //glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawElements(GL_TRIANGLES , 9, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
