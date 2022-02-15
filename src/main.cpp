#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/model.h"
#include "graphics/light.h"
#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/camera.h"

#define win_height 480
#define win_width 800

using namespace std;

Camera Camera::cam(glm::vec3(0.0f, 0.0f, 3.0f));

double previous = 0.0f; 

bool torch = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
	cout << "WSAD - move" << endl;
	cout << "Hold F - torch" << endl;

	int success;
	char infoLog[512];

	glfwInit();	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Lighmapping", NULL, NULL);
	if (window == NULL) {
		cout << "Could not open window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, win_width, win_height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
	
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  1.0f, -2.0f),
		glm::vec3(-2.0f, -2.0f, -3.0f),
		glm::vec3(-2.0f, -1.0f, -4.0f)		
	};

	Cube cubes[4];
	for (unsigned int i = 0; i < 4; i++) {
		cubes[i] = Cube(cubePositions[i], glm::vec3(1.5f));
		cubes[i].init();
	}

	DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.5f) };

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.25f,  0.2f,  1.2f),
		glm::vec3(1.3f, -3.3f, -3.0f),
		glm::vec3(-2.0f,  2.0f, -5.0f),
		glm::vec3(0.0f,  0.0f, -2.0f)
	};
	Lamp lamps[4];
	for (unsigned int i = 0; i < 4; i++) {
		lamps[i] = Lamp(glm::vec3(1.0f),
			glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f),
			1.0f, 0.07f, 0.032f,
			pointLightPositions[i], glm::vec3(0.5f));
		lamps[i].init();
	}

	SpotLight s = {
		Camera::cam.pos, Camera::cam.cameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.032f,
		glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)
	};

	

	while (!glfwWindowShouldClose(window)) {
		double current = glfwGetTime();
		double diff = current - previous;		

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
			Camera::cam.updatePos(CameraDirection::FORWARD, diff);
		
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
			Camera::cam.updatePos(CameraDirection::BACKWARD, diff);
		
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
			Camera::cam.updatePos(CameraDirection::RIGHT, diff);
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Camera::cam.updatePos(CameraDirection::LEFT, diff);		

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);		

		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) torch = true;
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) torch = false;

		glClearColor(.8f, 1.0f, .8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.activate();

		shader.set3Float("viewPos", Camera::cam.pos);

		dirLight.render(shader);

		for (unsigned int i = 0; i < 4; i++) {
			lamps[i].pointLight.render(shader, i);
		}
		shader.setInt("noPointLights", 4);

		if (torch) {
			s.position = Camera::cam.pos;
			s.direction = Camera::cam.cameraFront;
			s.render(shader, 0);
			shader.setInt("noSpotLights", 1);
		}
		else {
			shader.setInt("noSpotLights", 0);
		}

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = Camera::cam.getViewMatrix();
		projection = glm::perspective(
			glm::radians(Camera::cam.zoom), 
			(float)win_width / win_height, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		for (unsigned int i = 0; i < 4; i++) {
			cubes[i].render(shader);
		}

		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);

		for (unsigned int i = 0; i < 4; i++) {
			lamps[i].render(lampShader);
		}

		previous = current;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}	


	for (unsigned int i = 0; i < 4; i++) {
		cubes[i].cleanup();
	}
	for (unsigned int i = 0; i < 4; i++) {
		lamps[i].cleanup();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
