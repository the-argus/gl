#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

int main() {
    glfwInit();

    // set the window hints to reflect version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return 0;
}