#ifndef SHADER_HPP
#define SHADER_HPP


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, glm::mat2& value) const;
	void setMat3(const std::string& name, glm::mat3& value) const;
	void setMat4(const std::string& name, glm::mat4& value) const;

	// Getters
	GLuint getProgramId();

private:
	GLuint m_id;						/* Program id */

	const char* m_vertexPath;			/* Vertex shader path */
	const char* m_fragmentPath;			/* Fragment shader path */

	std::string m_vertexFileBuffer;		/* Vertex shader file buffer */
	std::string m_fragmentFileBuffer;	/* Fragment shader file buffer */

	std::ifstream m_vertexFile;			/* Vertex shader file */
	std::ifstream m_fragmentFile;		/* Fragment shader file */

	std::stringstream m_vertexStream;	/* Vertex shader stream */
	std::stringstream m_fragmentStream; /* Fragment shader stream */

	const GLchar* m_vertexCode;			/* Vertex shader code */
	const GLchar* m_fragmentCode;		/* Fragment shader code */

	GLuint m_vertexShader;				/* Vertex shader */
	GLuint m_fragmentShader;			/* Fragment shader */

	GLint m_success;					/* Success integer */
	GLchar m_infoLog[512];				/* Info log buffer */

	int init();
};

#endif