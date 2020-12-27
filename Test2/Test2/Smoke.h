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
#include "ShaderSmoke.h"

#define WIDTH 800
#define HEIGHT 600



/*float randFloat() {
		return ((float)rand() / RAND_MAX);
	}*/

class Smoke
{
	GLuint posBuf[2], velBuf[2];
	GLuint particleArray[2];
	GLuint feedback[2], initVel, startTime[2];
	GLuint drawBuf = 1;
	GLuint renderSub, updateSub;

	int nParticles = 500;

	GLuint smokeTexture;

	ShaderSmoke shader = ShaderSmoke("shaders/smoke.vs", "shaders/smoke.fs");
public:
	
	Smoke() {
		/*glGenBuffers(2, posBuf);
		glGenBuffers(2, velBuf);
		glGenBuffers(2, startTime);
		glGenBuffers(1, &initVel);

		int size = nParticles * 3 * sizeof(GLfloat);
		glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, initVel);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
		glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
		glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);

		// first velocity buffer with random velocities
		glm::vec3 v(0.0f);
		float velocity, theta, phi;
		GLfloat* data = new GLfloat[nParticles * 3];

		for (unsigned int i = 0; i < nParticles; i++) {
			theta = glm::mix(0.0f, glm::pi<float>() / 3.0f, randFloat());
			phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

			v.x = sinf(theta) * cosf(phi);
			v.y = cosf(theta);
			v.z = sinf(theta) * sinf(phi);

			velocity = glm::mix(0.1f, 0.3f, randFloat());
			v = glm::normalize(v) * velocity;

			data[3 * i] = v.x;
			data[3 * i + 1] = v.y;
			data[3 * i + 2] = v.z;
		}

		glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, initVel);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		// first start time 
		GLfloat* tdata = new GLfloat[nParticles];
		float ttime = 0.0f;
		float rate = 0.1f;
		for (int i = 0; i < nParticles; i++) {
			tdata[i] = ttime;
			ttime += rate;
		}
		glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), tdata);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// create vertex arrays for each set of buffers
		glGenVertexArrays(2, particleArray);

		// set up particle array 0
		glBindVertexArray(particleArray[0]);
		glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, initVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		// set up particle array 1
		glBindVertexArray(particleArray[1]);
		glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, initVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);

		// setup the feedback objects
		glGenTransformFeedbacks(2, feedback);

		// feedback 0
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[0]);

		// feedback 1
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[1]);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

		const char* outputNames[] = { "Position", "Velocity", "StartTime" };
		glTransformFeedbackVaryings(shader.Program, 3, outputNames, GL_SEPARATE_ATTRIBS);
		shader.Link();  // before link we need bind feedback var's name

		smokeTexture = loadTexture("textures/smoke.png");
		//float T = glfwGetTime();

		delete[] data;
		delete[] tdata;*/
	}

	void Draw() {
		/*glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glm::mat4 model(1.0f);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, smokeTexture);

		shader.Use();

		// we call in shader needed function in shader
		renderSub = glGetSubroutineIndex(shader.Program, GL_VERTEX_SHADER, "render");
		updateSub = glGetSubroutineIndex(shader.Program, GL_VERTEX_SHADER, "update");
		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);       // update

		// set uniform
		shader.setInt("texture1", 0);
		shader.setFloat("ParticleLifetime", 10.0f);
		glm::vec3 Accel(0.0f, 0.1f, 0.0f);
		shader.setVec3("Accel", Accel);
		shader.setFloat("MinParticleSize", 20.0f);
		shader.setFloat("MaxParticleSize", 500.0f);
		shader.setFloat("Time", glfwGetTime());
		shader.setFloat("H", glfwGetTime() - T);
		T = glfwGetTime();

		// render normal
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(10.0);

		glEnable(GL_RASTERIZER_DISCARD);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		glBeginTransformFeedback(GL_POINTS);
		glBindVertexArray(particleArray[1 - drawBuf]);
		glDrawArrays(GL_POINTS, 0, nParticles);
		glEndTransformFeedback();

		glDisable(GL_RASTERIZER_DISCARD);

		// render 
		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);

		// matrices
		model = glm::mat4(1.0f);
		glm::vec3 pos(3.0f, -0.5f, 2.0f);
		model = glm::translate(model, pos);
		glm::mat4 mv = view * model;
		shader.setMat4("MVP", projection * mv);

		glBindVertexArray(particleArray[drawBuf]);
		glDrawTransformFeedback(GL_POINTS, feedback[drawBuf]);

		// swap buffers
		drawBuf = 1 - drawBuf;

		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);*/
	}

	~Smoke() {
		glDeleteBuffers(1, &initVel);
		glDeleteBuffers(2, posBuf);
		glDeleteBuffers(2, velBuf);
		glDeleteBuffers(2, startTime);
	}
};

