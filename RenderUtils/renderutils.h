#pragma once
#include "renderobjects.h"
#include "Vertex.h"

namespace nsfw
{
	bool initContext(const char *title, int width, int height);
	bool stepContext();
	bool termContext();

	Geometry makeGeometry(const Vertex * vert, size_t vcount, const unsigned * tris, size_t tcount);
	void freeGeometry(Geometry &geo);

	Shader makeShader(const char *vert, const char *frag);
	void freeShader(Shader &shader);

	// crenderutils.h
	void draw(const Shader &shader, const Geometry &geo);
}