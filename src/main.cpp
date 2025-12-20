#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>

#include "shader.h"

const unsigned int width = 800;
const unsigned int height = 600;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow*, int width, int height);

int main()
{
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Orbit", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader shader("src/Shaders/default.vert", "src/Shaders/default.frag");

	float vertices[] = {
		-0.5f, -0.5, 0.0f, // bottom left
		0.5f,  -0.5, 0.0f, // bottom right
		0.0f,  0.5,	 0.0f, // top
	};

	unsigned int VAO;
	unsigned int VBO;

	// generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind - data
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// attributes
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// main loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render
		shader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// delete shaders;
	shader.deleteProgram();

	// clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}
