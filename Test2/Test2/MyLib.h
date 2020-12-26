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

#include <map>

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Shader
{
public:

	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);


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
const GLfloat ZOOM = 45.0f;


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
	   float Zoom;

	   Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	   Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);


	   glm::mat4 GetViewMatrix();


	   void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);


	   void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	   void ProcessMouseScroll(float yoffset);
private:

	void updateCameraVectors();
};

class Square
{
public:
	float Vertices[36];
	glm::vec3 Center;
	Square(float*);
	Square& operator=(const Square& ob);
};

GLuint loadTexture(char const* path);
GLuint loadCubemap(vector<std::string> faces);
GLFWwindow* CreateWindow(GLFWkeyfun, GLFWcursorposfun, const char* s = "LearnOpenGL", int width = 800, int height = 600);

struct Scene
{
	GLuint skyboxVAO = 0;
	GLuint skyboxVBO = 0;

	GLuint planeVAO = 0;
	GLuint planeVBO = 0;

	GLuint simple_cubeVAO = 0;
	GLuint simple_cubeVBO = 0;

	GLuint transparentVAO = 0;
	GLuint transparentVBO = 0;

	GLuint reflectCubeVAO = 0;
	GLuint reflectCubeVBO = 0;

	GLuint refractCubeVAO = 0;
	GLuint refractCubeVBO = 0;

	GLuint floorTexture;
	GLuint SkyboxTexture;
	GLuint cubeTexture;
	GLuint transparentTexture;


	glm::vec3 cubePositions[2] = {
	glm::vec3(-1.0f, 0.0f, -1.0f),
	glm::vec3(2.0f, 0.0f, 0.0f)
	};
	
	vector<glm::vec3> transparentPositions{
		glm::vec3(-2.21f, 0.0f, -0.01f),
		glm::vec3(2.0f, 0.0f, 1.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};
	
	Scene()
	{
		vector<std::string> faces
		{
			"textures/right.jpg",
			"textures/left.jpg",
			"textures/top.jpg",
			"textures/bottom.jpg",
			"textures/front.jpg",
			"textures/back.jpg"
		};
		
		transparentTexture = loadTexture("textures/glass.png");
		floorTexture = loadTexture("textures/floor.jpg");
		cubeTexture = loadTexture("textures/cube.jpg");
		SkyboxTexture = loadCubemap(faces);

	}

	void renderSkybox();
	void renderFloor();
	void renderSimpleCube();
	void renderTransparent();
	void renderReflectCube();
	void renderRefractCube();
	void renderScene( Shader&, Shader&, Shader&, Shader&, Camera&, GLfloat deltaTime);
};