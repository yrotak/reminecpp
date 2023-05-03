#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* path, GLenum type) :
	m_path(path),
	m_type(type)
{
	this->init();
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->m_id);
}

int Texture::init()
{
	// Generate texture
	glGenTextures(1, &this->m_id);
	glBindTexture(this->m_type, this->m_id);

	glTexParameteri(this->m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(this->m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(this->m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(this->m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(1);

	this->m_data = stbi_load(this->m_path, &this->m_width, &this->m_height, &this->m_nrComponents, 0);

	if (this->m_data) {
		switch (this->m_nrComponents) {
		case 1:
			this->m_format = GL_RED;
			break;
		case 3:
			this->m_format = GL_RGB;
			break;
		case 4:
			this->m_format = GL_RGBA;
			break;
		default:
			break;
		}

		glTexImage2D(this->m_type, 0, this->m_format, this->m_width, this->m_height, 0, this->m_format, GL_UNSIGNED_BYTE, this->m_data);
		glGenerateMipmap(this->m_type);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		return 1;
	}

	glActiveTexture(0);
	glBindTexture(this->m_type, 0);
	stbi_image_free(this->m_data);

	return 0;
}

void Texture::bind(GLint textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(this->m_type, this->m_id);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->m_type, 0);
}

int Texture::getWidth()
{
	return this->m_width;
}

int Texture::getHeight()
{
	return this->m_height;
}

GLenum Texture::getType()
{
	return this->m_type;
}

GLenum Texture::getFormat()
{
	return this->m_format;
}