#ifndef _FBO_H_
#define _FBO_H_

#include "Globals.h"

typedef unsigned int GLuint;

class FBO {
public:
	FBO() {};
	~FBO();

	bool Create(uint width, uint height);
	void Bind();
	void Unbind();
	void Delete();

	GLuint GetTexture() const;

public:
	GLuint fbo_id = 0;
	GLuint color_id = 0;
	GLuint depth_id = 0;

	uint width, height;
};


#endif // !_FBO_H_