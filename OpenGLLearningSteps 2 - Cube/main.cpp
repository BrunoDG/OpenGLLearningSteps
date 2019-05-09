// Include standard headers
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

	// An Array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// This will idenfity our vertex buffer
	GLuint vertexBuffer;
	// Generate 1 buffer, put the resulting identifier in vertexBuffer
	glGenBuffers(1, &vertexBuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// Give our vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Create abd Compile our GLSL program from the shaders
	GLuint programID = LoadShaders("shaders/SimpleTransform.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

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

		// Draw the triangle, finally!
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		
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