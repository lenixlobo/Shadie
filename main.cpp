//Dragon shading

#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cmath>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include<glm\glm\glm.hpp>
#include<glm\glm\gtc\matrix_transform.hpp>
#include<glm\glm\gtc\type_ptr.hpp>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int scr_width = 800;
const unsigned int scr_height = 600;

//lighting 
glm::vec3 lightPos(1.2f,1.0f,2.0f);

bool firstMouse = true;
//camera
Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;


float deltaTime = 0.0f;
float last_Frame = 0.0f;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(scr_width, scr_height, "Drag0n", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to Create Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed  to init GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	Shader dragShader("shaders/drag.vert", "shaders/drag.frag");
	Shader LightShader("shaders/lamp.vert","shaders/lamp.frag");

	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<int> elements;
	load_obj("model/dragon.obj", vertices, normals, elements);

	
	float *drag_vertices;
	int *dragon_indices;
	float *dragon_normal;

	drag_vertices = new float[3*elements.size()];
	//dragon_indices = new int[elements.size()];
	dragon_normal = new float[3*normals.size()];
	int vertexPtr = 0;
	for (unsigned int i = 0;i < elements.size();i++) {
		drag_vertices[vertexPtr++] = vertices[elements[i]].x;
		drag_vertices[vertexPtr++] = vertices[elements[i]].y;
		drag_vertices[vertexPtr++] = vertices[elements[i]].z;
	}

	int normal_ptr=0;
	for (unsigned int i = 0;i < normals.size();i++) {
		dragon_normal[normal_ptr++] = normals[elements[i]].x;
		dragon_normal[normal_ptr++] = normals[elements[i]].y;
		dragon_normal[normal_ptr++] = normals[elements[i]].z;
	}
	
	unsigned int VBO, VAO;
	unsigned int d_normal;


	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1,&d_normal);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, 3*elements.size()*sizeof(float),drag_vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, d_normal);
	glBufferData(GL_ARRAY_BUFFER, 3*normals.size()*sizeof(float), dragon_normal, GL_STATIC_DRAW);


	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(1);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLfloat lampvertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f };

	unsigned int lamp_vao, lamp_vbo;
	glGenVertexArrays(1,&lamp_vao);
	glGenBuffers(1,&lamp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,lamp_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(lampvertices),lampvertices,GL_STATIC_DRAW);
	glBindVertexArray(lamp_vao);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	std::cout << "Begining drawing" << std::endl;
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - last_Frame;
		last_Frame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		dragShader.use();

		dragShader.setVec3("objectColor",1.0f,0.5f,0.3f);
		dragShader.setVec3("lightColor",1.0f,1.0f,1.0f);
		dragShader.setVec3("lightPos",lightPos);

		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
		dragShader.setMat4("projection",projection);

		glm::mat4 view = camera.get_view_matrix();
		dragShader.setMat4("view",view);

		glm::mat4 model = glm::mat4(1.0f);
		
		dragShader.setMat4("model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0,elements.size());


		LightShader.use();
		LightShader.setMat4("projection", projection);
		LightShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		LightShader.setMat4("model", model);
		glBindVertexArray(lamp_vao);
		glDrawArrays(GL_TRIANGLES,0,36);
		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;

	
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	float cameraSpeed = 0.10f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACK, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
}

//mouse  callback
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;//IMP: reversed cause y coords vary from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
//scroll callback
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	camera.process_mouse_scroll(yoffset);
}