#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#ifdef EMSCRIPTEN
static void dispatch_main(void* fp)
{
	std::function<void()>* func = (std::function<void()>*)fp;
	(*func)();
}
#endif

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // Synchronise framerate to monitors refresh rate.
#ifndef EMSCRIPTEN
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
#endif
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push(2);
		layout.Push(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		//shader.SetUniform4f("u_Color", 0.8f, 0.4f, 0.8f, 1.0f);
		shader.Bind();
		Texture texture("res/textures/logo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
#ifdef EMSCRIPTEN
		std::function<void()> mainLoop = [&]() {
#else
		while (!glfwWindowShouldClose(window)) {
#endif		
			/* Render here */
			renderer.Clear();

			shader.Bind();
			//shader.SetUniform4f("u_Color", r, 0.4f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
#ifdef EMSCRIPTEN
		};	emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
		}
#endif
	}

	glfwTerminate();
	return 0;
}