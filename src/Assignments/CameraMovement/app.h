//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/gl.h"
#include "camera.h"
#include "camera_controler.h"

class SimpleShapeApplication : public xe::Application
{
public:
//Camera
    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() const { return camera_; }

    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override; 
    
    void scroll_callback(double xoffset, double yoffset) override;
//Camera Movement
    void set_controler(CameraControler *controler) { controler_ = controler; }
    void cursor_position_callback(double x, double y);
    void mouse_button_callback(int button, int action, int mods);

private:
    GLuint vao_;

    glm::mat4 M_; 

    GLuint u_pvm_buffer_;

    Camera *camera_;

    CameraControler *controler_;
};