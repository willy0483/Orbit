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
#include "camera.h"

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
		// COORDINATES      // COLORS           // TexCoord  //NORMALS
		-0.5f, 0.0f, 0.5f,	0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f,	-1.0f, 0.0f, // Bottom side
		-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 5.0f, 0.0f,	-1.0f, 0.0f, // Bottom side
		0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 5.0f, 0.0f,	-1.0f, 0.0f, // Bottom side
		0.5f,  0.0f, 0.5f,	0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f,	-1.0f, 0.0f, // Bottom side

		-0.5f, 0.0f, 0.5f,	0.83f, 0.70f, 0.44f, 0.0f, 0.0f, -0.8f, 0.5f,  0.0f, // Left Side
		-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, -0.8f, 0.5f,  0.0f, // Left Side
		0.0f,  0.8f, 0.0f,	0.92f, 0.86f, 0.76f, 2.5f, 5.0f, -0.8f, 0.5f,  0.0f, // Left Side

		-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f,	0.5f,  -0.8f, // Non-facing side
		0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f,	0.5f,  -0.8f, // Non-facing side
		0.0f,  0.8f, 0.0f,	0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f,	0.5f,  -0.8f, // Non-facing side

		0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.8f,	0.5f,  0.0f, // Right side
		0.5f,  0.0f, 0.5f,	0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.8f,	0.5f,  0.0f, // Right side
		0.0f,  0.8f, 0.0f,	0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.8f,	0.5f,  0.0f, // Right side

		0.5f,  0.0f, 0.5f,	0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f,	0.5f,  0.8f, // Facing side
		-0.5f, 0.0f, 0.5f,	0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f,	0.5f,  0.8f, // Facing side
		0.0f,  0.8f, 0.0f,	0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f,	0.5f,  0.8f // Facing side
	};

	unsigned int indices[] = {
		0,	1,	2, // Bottom side
		0,	2,	3, // Bottom side
		4,	6,	5, // Left side
		7,	9,	8, // Non-facing side
		10, 12, 11, // Right side
		13, 15, 14 // Facing side
	};

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	// generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind - data
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// attributes
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// texture

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texture_width;
	int texture_height;
	int nrChannels;

	unsigned char* data = stbi_load("texture/wall.jpg", &texture_width, &texture_height, &nrChannels, 0);
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	float speed = 2.0f;
	float radius = 1.0f;
	float angle = 0.0f;
	glm::vec3 position;

	double lastTime = glfwGetTime();

	// main loop
	while(!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render
		shader.use();

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		camera.Matrix(shader, "camMatrix");

		position.x = std::cos(angle) * radius;
		position.z = std::sin(angle) * radius;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, -0.5f, position.z));
		// model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

		angle += speed * deltaTime;

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = currentTime;
	}

	// delete objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
