#include "FBO.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"


FBO::~FBO()
{
	Delete();
}

bool FBO::Create(uint width, uint height)
{
	bool ret = true;

	//Generate Buffers

	if (glGenFramebuffers == 0)
		LOG("Error, glGenFramebuffers not present!");
	glGenFramebuffers(1, &fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

	//Create Color attachement
	glGenTextures(1, &color_id);
	glBindTexture(GL_TEXTURE_2D, color_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_id, 0);

	//Create Depth attachement
	glGenRenderbuffers(1, &depth_id);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_id);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		LOG("Framebuffer created succesfully!");
		ret = false;
	}

	this->width = width;
	this->height = height;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return ret;
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
}

void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FBO::Delete()
{
	glDeleteFramebuffers(1, &fbo_id);
	glDeleteTextures(1, &color_id);
	glDeleteTextures(1, &depth_id);
}

GLuint FBO::GetTexture() const
{
	return fbo_id;
}
