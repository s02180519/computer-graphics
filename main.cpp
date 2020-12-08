#define GLEW_STATIC
#include <GL\glew.h>

#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "MyLib.h"
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




using namespace std;

GLfloat lastX = 400, lastY = 300;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
bool firstMouse = true;

GLfloat blur = 7;
bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}



int main()
{

	////////////////////////////////////Опции OPENGL//////////////////////////////////////////////
	GLFWwindow* window = CreateWindow(key_callback, mouse_callback);

	Scene scene;

	//////////////////////////////Шейдеры//////////////////////////////////////////////////////////

	Shader ourShader("VertexShaderSource.vert", "FragmentShaderSource.frag");


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	

		ourShader.Use();
		ourShader.setInt("depthMap", 2);
		glActiveTexture(GL_TEXTURE2);

		scene.renderScene(0, ourShader, camera);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}