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

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
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



enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
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

	   void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	   void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);


private:

	void updateCameraVectors();
};



GLuint loadTexture(char const* path);
GLuint loadCubemap(vector<std::string> faces);
GLFWwindow* CreateWindow(GLFWkeyfun, GLFWcursorposfun, const char* s = "LearnOpenGL", int width = 800, int height = 600);

struct Scene
{
	GLuint skyboxVAO = 0;
	GLuint skyboxVBO = 0;

	GLuint cubeVAO = 0;
	GLuint cubeVBO = 0;

	GLuint planeVAO = 0;
	GLuint planeVBO = 0;

	GLuint lightVAO = 0;

	GLuint floorTexture;
	GLuint floorSpecular;
	GLuint cubemapTexture;
	GLuint diffuseMap;
	GLuint specularMap;

	glm::vec3 cubePositions[9] = {
	glm::vec3(-2.5f,  -2.61f,  -1.2f),
	glm::vec3(-1.0f, -2.61f, -1.2f),
	glm::vec3(-1.7f, -1.61f, -1.1f),
	glm::vec3(0.3f, -2.61f, -0.5f),
	glm::vec3(-0.3f,  -1.61f, -0.9f),
	glm::vec3(-0.85f,  -0.61f, -1.0f),
	glm::vec3(1.0f, -0.61f, -1.5f),
	glm::vec3(1.0f,  -1.61f, -1.5f),
	glm::vec3(1.0f, -2.61f, -1.5f)
	};

	GLfloat angle[9] = { 0.0f, 0.3f, -0.5f, -0.2f, 0.7f, 0.5f, 0.5f, -0.2f,-0.7f};

	Scene()
	{
		vector<std::string> faces
		{
			"textures/right.png",
			"textures/left.png",
			"textures/top.png",
			"textures/bottom.png",
			"textures/front.png",
			"textures/back.png"
		};
		floorTexture = loadTexture("textures/wood.png");
		floorSpecular = loadTexture("textures/wood_specular.jpg");
		cubemapTexture = loadCubemap(faces);
		diffuseMap = loadTexture("textures/container2.png");
		specularMap = loadTexture("textures/container2_specular.png");
	}



	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);


	void renderCube();
	void renderFloor();
	void renderLamp();
	void renderSkybox();
	void renderScene(int, Shader&, Shader&, Shader&, Camera&);
};