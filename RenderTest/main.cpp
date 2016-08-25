#include "renderutils.h"
#include <cstdio>

void main()
{
	if (!nsfw::initContext("Blah", 800, 600))		
		return;	


	// create some simple geometry
	nsfw::Vertex verts[3] = { { {   0,    .5f,  0,  1} },
							  { { .5f,   -.5f,  0,  1} },
							  { {-.5f,   -.5f,  0,  1} } };

	unsigned      tris[3] = {2,1,0};

	nsfw::Geometry geo = nsfw::makeGeometry(verts,3,tris,3);

	// create a ismple shader
	const char* vertex_shader =
		"#version 150\n"
		"in vec4 position;"
		"void main () { gl_Position = position; } ";

	const char* fragment_shader =
		"#version 150\n"
		"out vec4 outColor;"
		"void main () { outColor = vec4(1.0,0.0,0.0,1.0); } ";

	nsfw::Shader shader = nsfw::makeShader(vertex_shader, fragment_shader);

	// draw it in the update loop
	while (nsfw::stepContext())	
		nsfw::draw(shader, geo);
	
	
	// be sure to free our resources
	nsfw::freeGeometry(geo);
	nsfw::freeShader(shader);

	nsfw::termContext();
}


