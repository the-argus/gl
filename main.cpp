#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "constants.h"
#include "shader.h"

float triangle_vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

float vertices[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
};

unsigned int indices[] = { // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
};

void move2DVertices(GLFWwindow* window, float (&verts)[], int move_x, int move_y) {
    int subdex = 0;
    int size = (int)(sizeof(*verts)/sizeof(float));
    float scale_x;
    float scale_y;
    int w_x;
    int w_y;
    glfwGetWindowSize(window, &w_x, &w_y);
    scale_x = move_x / scale_x;
    scale_y = move_y / scale_y;
    for (int vert = 0; vert < size; vert++) {
        switch (subdex) {
            case 0:
                continue;
                break;
            case 1:
                verts[vert] += scale_x;
                break;
            case 2:
                verts[vert] += scale_y;
                break;
            default:
                std::cout << "error" << std::endl;
        }

        subdex %= 3;
    }
}

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

    int xin = 0;
    int yin = 0;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        xin += 1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        xin -= 1;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        yin += 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        yin -= 1;

    move2DVertices(window, triangle_vertices, xin, yin);

}

void loadShader(unsigned int shader, const GLchar* const* shaderSource)
{
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
            WINDOW_TITLE, NULL, NULL);

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
    

    
    // create vertex buffer object and store the ID in VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // also make a Vertex Array Object, which is used to group
    // vertex attribute data together
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // EBO for drawing vertices in certain order
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    // bind indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
            GL_STATIC_DRAW);

    // shader program objects
    Shader yellow_shader(VERTEX_PASSTHROUGH, FRAG_YELLOW);
    Shader time_shader(VERTEX_PASSTHROUGH, FRAG_TIMER);

    // 1. bind vertex array object
    glBindVertexArray(VAO);
    // 2. move the vertices array into an actual opengl buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell opengl how the vertices are formatted
    // 3 items per vertex, 32 bit floats,
    // normalized = false, then null pointer describing the offset of
    // the data in the array (there is no offset, its at the start)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),
                        (void*)0);
    glEnableVertexAttribArray(0);
    
    // first VAO complete, now make a second one for yellow triangle

    unsigned int tVBO;
    unsigned int tVAO;
    glGenBuffers(1, &tVBO);
    glGenVertexArrays(1, &tVAO);
    
    glBindVertexArray(tVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices),
            triangle_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),
                        (void*)0);
    glEnableVertexAttribArray(0); 

    // enter window loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // configure color to fill the screen with upon clear
        glClearColor( 0.5f, 0.1f, 0.1f, 1.0f);
        // clear the screen buffer with color
        glClear(GL_COLOR_BUFFER_BIT);

        // RENDERING CODE ----------
        
        // uniform time
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue*2) / 2.0f) + 0.5f;
        float redValue = (cos(timeValue*2) / 2.0f) + 0.5f;
        float blueValue = (sin(timeValue*2 + 1.6f) / 2.0f) + 0.5f;
        
        time_shader.use();

        time_shader.setVec4("timeColor", redValue,
                greenValue, blueValue, 1.0f);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        //DRAW RECTANGLE
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // unbind vertex array
        glBindVertexArray(0);
        
        // draw yellow triangle
        yellow_shader.use();
        glBindVertexArray(tVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        // END ---------------------

        // moves the buffer that is being drawn to into the window so it
        // actually gets drawn by the graphical environment
        glfwSwapBuffers(window);

        // checks if any events like window resizing or
        // button presses happen
        glfwPollEvents();    
    }
    
    // cleanup glfw's allocated resources
    glDeleteBuffers(1, &VBO);
    glfwTerminate();


	return 0;
}
