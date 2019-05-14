﻿// Include standard headers
// A thing to remember: always include the "opengl32.lib" on the linker, otherwise OpenGL functions won't work.
#include <stdio.h>
#include <stdlib.h>

// Include GLEW, but always before gl.h and glfw3.h
// Another thing to remember: When installing GLEW, you should copy the "glew32.dll" to the "%SystemRoot%/system32" folder,
// otherwise, GLEW won't work.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include glm
#include <glm/glm.hpp>
// Before including the transform lib from glm/gtx, since it's an experimental feature, you need to define the experimental use as enabled.
#define GLM_ENABLE_EXPERIMENTAL
// Include the matrix transformations lib from glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//include the shader loading function
#include "common/Shader.hpp"

GLFWwindow* window; // (In the Accompanying source code, this variable is global for simplicity)

int main()
{
	// Initializing GLFW
	glewExperimental = true; // This is needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Creating the first window
	glfwWindowHint(GLFW_SAMPLES, 4); // I'm using 4x Antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // I want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // This indicates that the OpenGL version is 3.3. If MAJOR was 4 and MINOR 6, I'd be using OpenGL 4.6
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // This should not be needed, it's just to make Mac OS happy
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Since I'm not using old OpenGL, I'll use Core Profile

	// Open a Window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 1", NULL, NULL); // To create a window use this method and pass width, height and window label. The other two params are null
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials \n");
		glfwTerminate(); // Kills the current instance of GLFW
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set the background as a dark blue color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// This is the Vertex Array Object, which will identify our vertex array
	GLuint VertexArrayId;
	// Generate 1 array, put the resulting identifier in vertexArray
	glGenVertexArrays(1, &VertexArrayId);
	// Bind to the 'vertexarray' array'
	glBindVertexArray(VertexArrayId);

	// Our Vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2 = 12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f, // triangle 3 : begin
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, // triangle 3 : end
		1.0f, 1.0f, -1.0f, // triangle 4: begin
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f, // triangle 4 : end
		-1.0f, -1.0f, -1.0f, // triangle 5 : begin
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 5 : end
		1.0f, -1.0f, 1.0f, // triangle 6 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, // triangle 6 : end
		-1.0f, 1.0f, 1.0f, // triangle 7 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, // triangle 7 : end
		1.0f, 1.0f, 1.0f, // triangle 8 : begin
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, // triangle 8 : end
		1.0f, -1.0f, -1.0f, // triangle 9 : begin
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, // triangle 9 : end
		1.0f, 1.0f, 1.0f, // triangle 10 : begin
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 10 : end
		1.0f, 1.0f, 1.0f, // triangle 11 : begin
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 11 : end
		1.0f, 1.0f, 1.0f, // triangle 12 : begin
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f // triangle 12 : end
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	// This will idenfity our vertex buffer
	GLuint vertexBuffer;
	// Generate 1 buffer, put the resulting identifier in vertexBuffer
	glGenBuffers(1, &vertexBuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// Give our vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Create abd Compile our GLSL program from the shaders
	GLuint programID = LoadShaders("shaders/TransformVertexShader.vertexshader", "shaders/ColorFragmentShader.fragmentshader");

	// translate the matrix
	//glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
	//glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
	//glm::vec4 transformedVector = myMatrix * myVector; // guess the result

	// Identity Matrix
	//glm::mat4 myIdentityMatrix = glm::mat4(1.0f);

	// Scaling Matrix
	//glm::mat4 myScalingMatrix = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));

	// Rotation Matrix
	//glm::vec3 myRotationAxis;
	//glm::rotate(angle_in_degrees, myRotationAxis);

	// Order of multiplication on the sentence below: Scaling -> Rotation -> Translation.
	//TransformedVector = TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector;

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	do
	{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flilckering, so it's there nonetheless
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1st attribute buffer: vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,			// attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// is it normalized? if yes, then GL_TRUE. Otherwise, GL_FALSE
			0,			// stride
			(void*)0	// array buffer offset
		);

		// 2nd attribute buffer: colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,			// attribute. No particular reason for 1, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
		);

		// Draw the triangle, finally!
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 12*3 vertices -> 12 triangles -> 6 squares
		
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Check if thw ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and Shader
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayId);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}