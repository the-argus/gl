#include <iostream>
#include <cmath>

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell opengl the size of the viewport, which we just created with GLFW
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	Shader basic_3D("shaders/passthrough.vs", "shaders/passthrough.fs");
	Shader light_3D("shaders/passthrough.vs", "shaders/light.fs");
	Shader lighted_3D("shaders/textured.vs", "shaders/lighted.fs");

	// initialize vertex buffer and array objects
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	// bind the VAO and VBO so they start recording the openGL state
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
				 GL_STATIC_DRAW);
	glBindVertexArray(VAO);

	// create vertex attributes for cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightcube_VAO;
	glGenVertexArrays(1, &lightcube_VAO);
	glBindVertexArray(lightcube_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// initialize to identity matrix
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	// perspective projection matrix
	projection = glm::perspective(glm::radians(45.0f),
								  (float)WINDOW_HEIGHT / (float)WINDOW_HEIGHT,
								  0.1f, 100.0f);

	// translations performed on the model
	glm::mat4 cube = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
	cube = glm::rotate(cube, glm::radians(30.0f), glm::vec3(-1.0f, 1.0f, 0.0f));

    glm::vec3 lightPos = glm::vec3(1.5f, 1.0f, -10.0f);
	glm::mat4 lightcube = glm::translate(model, lightPos);
	lightcube = glm::rotate(lightcube, glm::radians(45.0f),
							glm::vec3(1.0f, 1.0f, 0.0f));
	lightcube = glm::scale(lightcube, glm::vec3(0.2f));

	// translations performed on the camera
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));

	// window's update loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        float time = glfwGetTime();
		float radius = 5.0f;
		float s = sin(time) * radius;
		float c = cos(time) * radius;
        glm::vec3 target = glm::vec3(cube[3]);
        // circle target around x and z
		glm::vec3 pos = glm::vec3(target.x + s, target.y, target.z + c);
		view =
			glm::lookAt(pos, target, glm::vec3(0.0f, 1.0f, 0.0f));

		lighted_3D.use();
		lighted_3D.setMat4("projection", projection);
		lighted_3D.setMat4("view", view);
		lighted_3D.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lighted_3D.setFloat("ambientStrength", 0.1f);
        lighted_3D.setFloat("diffuseStrength", 1.0f);
        lighted_3D.setFloat("specularStrength", 0.5f);
        lighted_3D.setInt("shininess", 32);
        lighted_3D.setVec3("viewPos", pos);
        lighted_3D.setVec3("lightPos", lightPos);

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, containerTex);

		// draw cube
		lighted_3D.setMat4("model", cube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// draw lightcube
		light_3D.use();
		light_3D.setMat4("projection", projection);
		light_3D.setMat4("view", view);
		light_3D.setMat4("model", lightcube);
		glBindVertexArray(lightcube_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocation
	glDeleteProgram(basic_3D.ID);
	glDeleteProgram(light_3D.ID);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightcube_VAO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
