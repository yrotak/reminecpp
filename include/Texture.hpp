#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "stdafx.h"

class Texture {
public:
	Texture(const char* path, GLenum type);
	~Texture();

	void bind(GLint textureUnit);
	void unbind();

	// Getters
	int getWidth();
	int getHeight();
	GLenum getType();
	GLenum getFormat();

private:
	GLuint m_id;			/* Texture id */

	const char* m_path;		/* Texture path */
	GLenum m_type;			/* Texture type */
	int m_width;			/* Texture width */
	int m_height;			/* Texture height */
	int m_nrComponents;		/* Texture nrChannels */
	GLenum m_format;		/* Texture format */
	unsigned char* m_data;	/* Texture data */

	int init();
};
#endif