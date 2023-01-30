//
// Created by pbialas on 25.09.2020.
//

#include "app.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include "glm/gtc/matrix_transform.hpp"
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
    std::vector<GLushort> indices = {0,3,2 , 1,3,0 , 4,5,6 , 7,8,9 , 10,11,12 , 13,14,15};
    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            0.0f, 0.0f, 0.0f, 0.0f,1.0f,0.0f,//0
            1.0f, 0.0f, 0.0f, 0.0f,1.0f,0.0f,//1
            0.0f, 1.0f, 0.0f, 0.0f,1.0f,0.0f,//2
            1.0f, 1.0f, 0.0f, 0.0f,1.0f,0.0f,//3

            0.0f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,0.0f,0.0f,
            0.5f, 0.5f, 1.0f, 1.0f,0.0f,0.0f,//6

            0.0f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,
            1.0f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,
            0.5f, 0.5f, 1.0f, 0.0f,0.0f,1.0f,//9
            
            1.0f, 1.0f, 0.0f, 0.5f,0.0f,0.5f,
            1.0f, 0.0f, 0.0f, 0.5f,0.0f,0.5f,
            0.5f, 0.5f, 1.0f, 0.5f,0.0f,0.5f,//12

            1.0f, 0.0f, 0.0f, 0.0f,0.5f,0.5f,
            0.0f, 0.0f, 0.0f, 0.0f,0.5f,0.5f,
            0.5f, 0.5f, 1.0f, 0.0f,0.5f,0.5f}; //15
    //Pyramid 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Uniforms
    float strength=0.6;
    float color[3]={0.7,0.6,0.7};

    // PVM
    glm::mat4 PVM_matrix(1.0f);
    glm::mat4 perspective_matrix = glm::perspective(25.0, 1.5, 50.0, 3.0);
    glm::mat4 look_matrix = glm::lookAt(glm::vec3(10.6, -20.0, 6.6), glm::vec3(-0.5, 0.1, 0.0), glm::vec3(0.0, 0.3, -0.25));
    glm::mat4 PVM = PVM_matrix*perspective_matrix*look_matrix;

    GLuint v_buffer_handle_uniforms;
    glGenBuffers(1, &v_buffer_handle_uniforms);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);    
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, v_buffer_handle_uniforms);

    glBufferSubData(GL_UNIFORM_BUFFER, 0,sizeof(float),&strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(float),3*sizeof(float),color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
//////////////////////////////////////////////////

    GLuint v_buffer_handle_uniforms_move;
    glGenBuffers(1, &v_buffer_handle_uniforms_move);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);  
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), &PVM); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle); 
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //buffer of indices
    GLuint v_buffer_handle_indices;
    glGenBuffers(1, &v_buffer_handle_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle_indices);
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
    glDrawElements(GL_TRIANGLES , 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}
