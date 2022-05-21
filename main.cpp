#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "constants.h"
#include "cube.h"
#include "shader.h"
#include "stb_image.h"

// function called whenever a user resizes the window
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
	// initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window using glfw
	GLFWwindow *window =
		glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	// handle failure
	if (window == NULL) {
		std::cout << "Failed to initialize GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell opengl the size of the viewport, which we just created with GLFW
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	Shader basic_3D =
		Shader("shaders/passthrough.vs", "shaders/passthrough.fs");

	// initialize vertex buffer and array objects
	unsigned int VBO;
	unsigned int VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	// bind the VAO and VBO so they start recording the openGL state
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
				 GL_STATIC_DRAW);

	// create vertex attributes for cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)3);
	glEnableVertexAttribArray(1);

	// unbind
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
					GL_REPEAT); // set texture wrapping to GL_REPEAT (default
								// wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture for the cube
	int width, height, nrChannels;
	unsigned char *data =
		stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
		return -1;
	}
	// register with opengl
	unsigned int containerTex;
	glGenTextures(1, &containerTex);
	glBindTexture(GL_TEXTURE_2D, containerTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	// initialize to identitiy matrix
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	// perspective projection matrix
	projection = glm::perspective(glm::radians(45.0f),
								  (float)WINDOW_HEIGHT / (float)WINDOW_HEIGHT,
								  0.1f, 100.0f);

	// translations performed on the model
	glm::mat4 cube = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
	glm::mat4 lightcube = glm::translate(model, glm::vec3(1.5f, 1.0f, -10.0f));
	lightcube = glm::rotate(lightcube, glm::radians(45.0f),
							glm::vec3(1.0f, 1.0f, 0.0f));

	// translations performed on the camera
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));

	// window's update loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, containerTex);
		basic_3D.use();
		basic_3D.setMat4("projection", projection);
		basic_3D.setMat4("view", view);

		glBindVertexArray(VAO);
		
        // draw cube
        basic_3D.setMat4("model", cube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw lightcube
        basic_3D.setMat4("model", lightcube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocation
	glDeleteProgram(basic_3D.ID);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
