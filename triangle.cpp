#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include "constants.h"


float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const char *fShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *vShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

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

void loadShader(unsigned int shader, const GLchar* const* shaderSource) {
    glShaderSource(shader, 1, shaderSource, NULL);
    glCompileShader(shader);
    
    int  success;
    // error message buffer
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
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
    
    // load and compile vertex shader
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    loadShader(vShader, &vShaderSource);
    
    // load and compile fragment shader
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadShader(fShader, &fShaderSource);

    // create a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // attach the shaders to the shader program
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    // we no longer need shader objects, so free up that memory
    glDeleteShader(vShader);
    glDeleteShader(fShader);


    
    // create vertex buffer object and store the ID in VBO
    unsigned int VBO;
    // also make a Vertex Array Object, which is used to group
    // vertex attribute data together
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    

    // 1. bind vertex array object
    glBindVertexArray(VAO);
    // 2. move the vertices array into an actual opengl buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell opengl how the vertices are formatted
    // 3 items per vertex, 32 bit floats,
    // normalized = false, then null pointer describing the offset of
    // the data in the array (there is no offset, its at the start)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),
                        (void*)0);
    glEnableVertexAttribArray(0);
    // first VAO complete
    

    // enter window loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // configure color to fill the screen with upon clear
        glClearColor( 0.5f, 0.1f, 0.1f, 1.0f);
        // clear the screen buffer with color
        glClear(GL_COLOR_BUFFER_BIT);

        // RENDERING CODE ----------
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // DRAW THE TRIANGLE FRFR
        // primitive type, starting index, number of vertices to draw
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // END ---------------------

        // moves the buffer that is being drawn to into the window so it
        // actually gets drawn by the graphical environment
        glfwSwapBuffers(window);

        // checks if any events like window resizing or button presses happen
        glfwPollEvents();    
    }
    
    // cleanup glfw's allocated resources
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

	return 0;
}
