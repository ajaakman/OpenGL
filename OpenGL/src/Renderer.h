#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define GLCall(x) GLClearError();\
	x;\
	GLLogCall(#x, __FILE__, __LINE__)

extern void GLClearError();
extern bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};