#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include "constants.h"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// create callback function which gets called whenever the window gets
// resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // one liner key handling
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();

	// set the window hints to reflect version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	// create GLFW window object
	// not really sure what the nulls are for
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
            "OpenGL", NULL, NULL);

	// catch initialization failure and cleanup
	if (window == NULL)
	{
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return -1;
	}

	// if success, set the current opengl context to be this window
	glfwMakeContextCurrent(window);
	
	// now initialize GLAD
	// glfwGetProcAddress accounts for which OS we are compiling for and gives
    // the correct function for loading GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}

	// our first openGL function
	// tells opengl the size of the rendering window (make it the same size as
    // the window glfw just created)
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // tell glfw to bind that func to resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // enter window loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering code
        // ...
        
        // configure color to fill the screen with upon clear
        glClearColor( 0.5f, 0.1f, 0.1f, 1.0f);
        // clear the screen buffer with color
        glClear(GL_COLOR_BUFFER_BIT);

        // moves the buffer that is being drawn to into the window so it
        // actually gets drawn by the graphical environment
        glfwSwapBuffers(window);

        // checks if any events like window resizing or button presses happen
        glfwPollEvents();    
    }
    
    // cleanup glfw's allocated resources
    glfwTerminate();

	return 0;
}
