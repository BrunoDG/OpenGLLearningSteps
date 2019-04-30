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

//Include GLM
#include <glm/glm.hpp>

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

	do 
	{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flilckering, so it's there nonetheless
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	
	// Check if thw ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}