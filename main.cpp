#include <iostream>
#include <cmath>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "constants.h"
#include "shader.h"

// cube vertices
#include "cube.h"

float triangle_vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

float nocolor_vertices[] = {
    // positions            // texcoords
     0.5f,  0.5f, 0.0f,     1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,     1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f  // top left
};

float vertices[] = {
    // position             // vertex color     // texcoords
     0.5f,  0.5f, 0.0f,     0.5f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,     0.0f, 0.5f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,     0.5f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 0.5f,   0.0f, 1.0f  // top left
};

unsigned int indices[] = { // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

    
    // global configuration
    glEnable(GL_DEPTH_TEST);  
    

    
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
    Shader yellow_shader(VERTEX_OFFSET, FRAG_YELLOW);
    Shader time_shader(VERTEX_TEX_OFFSET, FRAG_TIME_TEXTURED);
    Shader col_pos_shader(VERTEX_COL_POS, FRAG_COL_POS);
    // shader that draws with offset + a texture
    Shader tex_pos_shader(VERTEX_TEX_OFFSET, FRAG_TEX);

    Shader main_shader(VERTEX_FULL, FRAG_FULL);

    // the big boy
    Shader main_shader_3D(VERTEX_TEXTURE_3D, FRAG_TEX);

    // 1. bind vertex array object
    glBindVertexArray(VAO);
    // 2. move the vertices array into an actual opengl buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell opengl how the vertices are formatted
    // 5 items per vertex, 32 bit floats,
    // normalized = false, then null pointer describing the offset of
    // the data in the array (there is no offset, its at the start)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
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

    // third for color changing rectangle

    unsigned int rVBO;
    unsigned int rVAO;
    glGenBuffers(1, &rVBO);
    glGenVertexArrays(1, &rVAO);

    glBindVertexArray(rVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nocolor_vertices),
            nocolor_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),
            (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
            (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // cube cube cube cube cube cube
    unsigned int cVBO;
    unsigned int cVAO;
    glGenBuffers(1, &cVBO);
    glGenVertexArrays(1, &cVAO);

    glBindVertexArray(cVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices),
            cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),
            (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
            (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // TEXTURES -----------------------
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // allocate opengl texture object
    unsigned int jaypehg;
    glGenTextures(1, &jaypehg);
    glBindTexture(GL_TEXTURE_2D, jaypehg);

    // load textures from file (nrChannels == number of color channels)
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height,
            &nrChannels, 0);
    if (data) {
        // generate tex2d at currently bound texture using data char pointer
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    // free up memory
    stbi_image_free(data);

    // enter window loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // configure color to fill the screen with upon clear
        glClearColor( 0.5f, 0.1f, 0.1f, 1.0f);
        // clear the screen buffer with color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // RENDERING CODE ----------
        
        // uniform time
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue*2) / 2.0f);
        float redValue = (cos(timeValue*2) / 2.0f);
        float blueValue = (sin(timeValue*2 + 1.6f) / 2.0f);
        
        // draw rectangle with changing color
        
        // time_shader.use();

        // // uniforms
        // time_shader.setVec3("vOffset", -redValue, -greenValue, -blueValue);
        // time_shader.setVec4("timeColor", redValue,
                // greenValue, blueValue, 0.5f);
        // // texture
        // glBindTexture(GL_TEXTURE_2D, jaypehg);
        // // rectangle EBO
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // // vertices
        // glBindVertexArray(rVAO);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);
        

        // box
        // main_shader.use();

        // glBindTexture(GL_TEXTURE_2D, jaypehg);
        // glBindVertexArray(VAO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);
        
        // draw yellow triangle
        yellow_shader.use();

        // calculate a transform
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(
                    0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -10.0f));
        // send the transformed matrix into a uniform called "transform"
        yellow_shader.setMat4("transform", trans);
 
        yellow_shader.setVec3("vOffset", redValue, greenValue, blueValue);

        glBindVertexArray(tVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0); 


        // 3d cube!! ===================

        main_shader_3D.use();
        
        // plane rotation matrix
        glm::mat4 model = glm::mat4(1.0f);
        // rotate the moving rectangle backwards in 3d space
        // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        // view matrix (move worldspace away from camera/ negative z)
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
        // PROJECTION MATRIX BAYBEE
        glm::mat4 projection;
        // documentation: FOV, aspect ratio, near, far.
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
 
        // dynamic uniform sending
        main_shader_3D.setMat4("projection", projection);
        main_shader_3D.setMat4("view", view);
        // int modelLoc = glGetUniformLocation(main_shader_3D.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // int viewLoc = glGetUniformLocation(main_shader_3D.ID, "view");
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // int projectionLoc = glGetUniformLocation(main_shader_3D.ID, "projection");
        // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindTexture(GL_TEXTURE_2D, jaypehg);
        glBindVertexArray(cVAO);

        for (   unsigned int i = 0;
                i < sizeof(cubePositions)/sizeof(cubePositions[0]);
                i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model,
                    (float)glfwGetTime() * glm::radians(angle),
                    glm::vec3(0.5f, 1.0f, 0.0f));  
            main_shader_3D.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
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
