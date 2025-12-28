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
#include "mesh.h"

// todo add chunk add occlusion culling

const unsigned int width = 800;
const unsigned int height = 600;

glm::vec3 unitVertices[8] = {
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), //
	glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), //
	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f), //
	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), //
};

unsigned int faceVertices[6][4] = {
	{ 3, 2, 6, 7 }, //
	{ 1, 0, 4, 5 }, //
	{ 4, 0, 3, 7 }, //
	{ 1, 5, 6, 2 }, //
	// FRONT (0, 1, 2, 3)
	{ 0, 1, 2, 3 }, //
	// BACK (5, 4, 7, 6)
	{ 5, 4, 7, 6 }, //
};

glm::vec3 colors[6] = {
	glm::vec3(1.0f, 0.0f, 0.0f), // red
	glm::vec3(0.0f, 1.0f, 0.0f), // green
	glm::vec3(0.0f, 0.0f, 1.0f), // blue
	glm::vec3(1.0f, 1.0f, 0.0f), // yellow
	glm::vec3(1.0f, 0.0f, 1.0f), // magenta
	glm::vec3(0.0f, 1.0f, 1.0f) // cyan
};

enum Face
{
	TOP = 0x01, // 0000 0001
	BOTTOM = 0x02, // 0000 0010
	LEFT = 0x04, // 0000 0100
	RIGHT = 0x08, // 0000 1000
	FRONT = 0x10, // 0001 0000
	BACK = 0x20 // 0010 0000
};

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow*, int width, int height);

int face_to_index(Face face);
void load_face(glm::vec3 position, Face face, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

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

	Mesh test;

	for(int x = 0; x < 5; x++)
	{
		for(int y = 0; y < 5; y++)
		{
			for(int z = 0; z < 5; z++)
			{
				load_face(glm::vec3(x, y, z), Face::RIGHT, test.vertices, test.indices);
				load_face(glm::vec3(x, y, z), Face::LEFT, test.vertices, test.indices);
				load_face(glm::vec3(x, y, z), Face::TOP, test.vertices, test.indices);
				load_face(glm::vec3(x, y, z), Face::BOTTOM, test.vertices, test.indices);
				load_face(glm::vec3(x, y, z), Face::BACK, test.vertices, test.indices);
				load_face(glm::vec3(x, y, z), Face::FRONT, test.vertices, test.indices);
			}
		}
	}

	test.SetUpMesh();

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	double lastTime = glfwGetTime();

	// main loop
	while(!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		// render
		camera.Inputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		camera.Matrix(shader, "camMatrix");

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		test.Draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = currentTime;
	}

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

int face_to_index(Face face)
{
	switch(face)
	{
		case Face::TOP:
			return 0;
		case Face::BOTTOM:
			return 1;
		case Face::LEFT:
			return 2;
		case Face::RIGHT:
			return 3;
		case Face::FRONT:
			return 4;
		case Face::BACK:
			return 5;
		default:
			return -1; // Or some error code
	}
}

void load_face(glm::vec3 position, Face face, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	unsigned int indexOffset = vertices.size();
	for(int i = 0; i < 4; i++)
	{
		unsigned int vIndex = faceVertices[face_to_index(face)][i];
		Vertex v;
		v.position = unitVertices[vIndex] + position;
		v.color = colors[face_to_index(face)];
		vertices.push_back(v);
	}

	// Append indices for the specified face (two triangles)
	indices.push_back(indexOffset);
	indices.push_back(indexOffset + 1);
	indices.push_back(indexOffset + 2);
	indices.push_back(indexOffset);
	indices.push_back(indexOffset + 2);
	indices.push_back(indexOffset + 3);
}
