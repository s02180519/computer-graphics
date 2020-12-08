#include "MyLib.h"
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath != NULL)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");


	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");


	unsigned int geometry;
	if (geometryPath != NULL)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	if (geometryPath != NULL)
		glAttachShader(Program, geometry);
	glLinkProgram(Program);
	checkCompileErrors(Program, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != NULL)
		glDeleteShader(geometry);

}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
}

void Shader::Use()
{
	glUseProgram(this->Program);
}

void Shader::setVec3(const char* s, float a, float b, float c)
{
	GLint sLoc = glGetUniformLocation(Program, s);
	glUniform3f(sLoc, a, b, c);
}

void Shader::setVec3(const char* s, glm::vec3 a)
{
	GLint sLoc = glGetUniformLocation(Program, s);
	glUniform3f(sLoc, a.x, a.y, a.z);
}

void Shader::setFloat(const char* s, float a)
{
	GLint sLoc = glGetUniformLocation(Program, s);
	glUniform1f(sLoc, a);
}

void Shader::setInt(const char* s, int a)
{
	GLint sLoc = glGetUniformLocation(Program, s);
	glUniform1i(sLoc, a);
}

void Shader::setMat4(const char* s, glm::mat4 a)
{
	GLint sLoc = glGetUniformLocation(Program, s);
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(a));
}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY)
{
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY)
{
	this->Position = glm::vec3(posX, posY, posZ);
	this->WorldUp = glm::vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}



void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	this->updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);

	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

GLuint loadTexture(char const* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);


	int nrComponents, width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, &nrComponents, 0);
	if (image)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SOIL_free_image_data(image);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		SOIL_free_image_data(image);
	}

	return textureID;
}



void Scene::renderFloor()
{
	if (planeVAO == 0)
	{
		float planeVertices[] = {
			// positions               //Normals         // texture 
			 5.0f, -0.51f,  5.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
			-5.0f, -0.51f,  5.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
			-5.0f, -0.51f,  -5.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

			 5.0f, -0.51f,  5.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
			-5.0f, -0.51f,  -5.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
			 5.0f, -0.51f,  -5.0f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f
		};

		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);


		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


void Scene::renderScene(int withmap, Shader& ourShader, Camera& camera)
{

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	model = glm::translate(model, glm::vec3(-1.5f, -2.61f, -1.5f));
	ourShader.setMat4("model", model);

	if (!withmap)
	{

		ourShader.Use();
		ourShader.setInt("material.diffuse", 0);
		ourShader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);

		ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);


		ourShader.setFloat("light.constant", 1.0f);
		ourShader.setFloat("light.linear", 0.09f);
		ourShader.setFloat("light.quadratic", 0.032f);


		ourShader.setVec3("viewPos", camera.Position);
		view = camera.GetViewMatrix();

		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, floorSpecular);
	}
	renderFloor();
}

GLFWwindow* CreateWindow(GLFWkeyfun key, GLFWcursorposfun mouse_callback, const char* s, int width, int height)
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(width, height, s, nullptr, nullptr);
	glfwMakeContextCurrent(window);


	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;

	glewInit();


	glViewport(0, 0, width, height);
	return window;
}