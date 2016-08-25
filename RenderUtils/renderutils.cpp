#define GLEW_STATIC
#include "glew\glew.h"
#include "glfw\glfw3.h"
#include <cstdio>
#include "renderutils.h"

namespace nsfw
{
	static GLFWwindow *window = nullptr;

#ifdef _DEBUG
void APIENTRY nsfwGLerrorCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam)
{
	fprintf(stderr,"%s\n", message);
	if (severity == GL_DEBUG_SEVERITY_HIGH)
		abort();
}
#endif

bool initContext(const char * title, int width, int height)
{
	if (!glfwInit())
	{
		fprintf(stderr, "GLFW failed to initialize.");
		return false;
	}
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		fprintf(stderr, "Window failed to initialize.");
		glfwTerminate();
		return false;
	}
	
	glfwMakeContextCurrent(window);
	
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Glew failed to initialize.");
		return false;
	}
	
	glClearColor(1.f,1.f,1.f,1.f);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);

#ifdef _DEBUG
	// Enable the debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//GLDEBUGPROC t = &nsfwGLerrorCallback;
	glDebugMessageCallback(nsfwGLerrorCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
#endif
	return true;
}

bool stepContext()
{		
	glfwPollEvents();
	glfwSwapBuffers(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return !glfwWindowShouldClose(window);
}

bool termContext()
{
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
	return true;
}

Geometry makeGeometry(const Vertex * verts, size_t vcount, const unsigned * tris, size_t tcount)
{
	Geometry retval;
	
	retval.size = tcount;
	glGenBuffers(1, &retval.vbo);		// Vertex Buffer Object - Used to define the vertices.
	glGenBuffers(1, &retval.ibo);		// Index Buffer Object  - Used to define the triangles (groups of vertices based on the vertex index).
	glGenVertexArrays(1, &retval.vao);	// Vertex Array Object  - Discrete grouping of IBO and VBO

	glBindVertexArray(retval.vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo);

	glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(Vertex), verts, GL_STATIC_DRAW);	// VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tcount * sizeof(unsigned), tris, GL_STATIC_DRAW);	// IBO
																							// VAO
	glEnableVertexAttribArray(0);	// position

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return retval;
}

void freeGeometry(Geometry &geometry)
{
	glDeleteBuffers(1, &geometry.ibo);
	glDeleteBuffers(1, &geometry.vbo);
	glDeleteVertexArrays(1, &geometry.vao);

	geometry = { 0,0,0,0 };
}

// crenderutils.h
Shader makeShader(const char * vert, const char * frag)
{
	Shader retval;

	// define
	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	// assign the source
	glShaderSource(vs, 1, &vert, NULL);
	glShaderSource(fs, 1, &frag, NULL);

	// compile
	glCompileShader(vs);
	glCompileShader(fs);

	// link
	glAttachShader(retval.handle, fs);
	glAttachShader(retval.handle, vs);
	glLinkProgram(retval.handle);

	// once the program is linked,
	// individual shaders aren't needed.
	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

void freeShader(Shader & shader)
{
	glDeleteProgram(shader.handle);
	shader = { 0 }; // prevent reuse.
}

// crenderutils.cpp
void draw(const Shader & shader, const Geometry & geo)
{
	glUseProgram(shader.handle);
	glBindVertexArray(geo.vao);

	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}
}