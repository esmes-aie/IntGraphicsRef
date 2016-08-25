#pragma once

namespace nsfw
{
	struct Geometry { size_t vao, vbo, ibo, size; };

	// crenderutils.h
	struct Shader   { size_t handle; };

	Shader makeShader(const char *vert, const char *frag);
	void freeShader(Shader &shader);
}