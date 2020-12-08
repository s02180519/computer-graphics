#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL\glew.h> 
#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;
class Shader
{
public:

	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const char* geometryPath = NULL);
	void checkCompileErrors(GLuint shader, std::string type);


	void Use();
	void setVec3(const char*, float, float, float);
	void setFloat(const char*, float);
	void setInt(const char*, int);
	void setVec3(const char*, glm::vec3);
	void setMat4(const char*, glm::mat4);
};





const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;


class Camera
{
public:glm::vec3 Position;
	   glm::vec3 Front;
	   glm::vec3 Up;
	   glm::vec3 Right;
	   glm::vec3 WorldUp;

	   GLfloat Yaw;
	   GLfloat Pitch;

	   GLfloat MovementSpeed;
	   GLfloat MouseSensitivity;


	   Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	   Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	   glm::mat4 GetViewMatrix();

	   void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);


private:

	void updateCameraVectors();
};



GLuint loadTexture(char const* path);
GLFWwindow* CreateWindow(GLFWkeyfun, GLFWcursorposfun, const char* s = "LearnOpenGL", int width = 800, int height = 600);

struct Scene
{
	GLuint planeVAO = 0;
	GLuint planeVBO = 0;

	GLuint floorTexture;
	GLuint floorSpecular;

	Scene()
	{
		floorTexture = loadTexture("textures/wood.png");
		floorSpecular = loadTexture("textures/wood_specular.jpg");
	}


	void renderFloor();

	void renderScene(int, Shader&, Camera&);
};