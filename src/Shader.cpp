#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) :
	m_vertexPath(vertexPath),
	m_fragmentPath(fragmentPath)
{
	this->init();
}

Shader::~Shader()
{
	// --
}

int Shader::init()
{
	// Ensure ifstream objects can throw exceptions
	this->m_vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	this->m_fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open files
		this->m_vertexFile.open(this->m_vertexPath);
		this->m_fragmentFile.open(this->m_fragmentPath);

		// Read files buffer contents into stream
		this->m_vertexStream << this->m_vertexFile.rdbuf();
		this->m_fragmentStream << this->m_fragmentFile.rdbuf();

		// Close file handlers
		this->m_vertexFile.close();
		this->m_fragmentFile.close();

		// Convert stream into string
		this->m_vertexFileBuffer = m_vertexStream.str();
		this->m_fragmentFileBuffer = m_fragmentStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Shader file not successfully read." << std::endl;

		return 1;
	}

	// Convert std::string to const char*
	this->m_vertexCode = m_vertexFileBuffer.c_str();
	this->m_fragmentCode = m_fragmentFileBuffer.c_str();

	// Vertex shader
	this->m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->m_vertexShader, 1, &m_vertexCode, NULL);
	glCompileShader(this->m_vertexShader);

	glGetShaderiv(this->m_vertexShader, GL_COMPILE_STATUS, &this->m_success);
	if (!this->m_success) {
		glGetShaderInfoLog(this->m_vertexShader, 512, NULL, this->m_infoLog);
		std::cout << "Vertex shader compilation failed" << std::endl << this->m_infoLog << std::endl;

		return 1;
	}

	// Fragment shader
	this->m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->m_fragmentShader, 1, &m_fragmentCode, NULL);
	glCompileShader(this->m_fragmentShader);

	glGetShaderiv(this->m_fragmentShader, GL_COMPILE_STATUS, &this->m_success);
	if (!this->m_success) {
		glGetShaderInfoLog(this->m_fragmentShader, 512, NULL, this->m_infoLog);
		std::cout << "Fragment shader compilation failed" << std::endl << this->m_infoLog << std::endl;

		return 1;
	}

	// Program shader
	this->m_id = glCreateProgram();
	glAttachShader(this->m_id, this->m_vertexShader);
	glAttachShader(this->m_id, this->m_fragmentShader);
	glLinkProgram(this->m_id);

	glGetShaderiv(this->m_id, GL_LINK_STATUS, &this->m_success);
	if (!this->m_success) {
		glGetProgramInfoLog(this->m_id, 512, NULL, this->m_infoLog);
		std::cout << "Program shader linking failed" << std::endl << this->m_infoLog << std::endl;

		return 1;
	}

	// Delete shaders
	glDeleteShader(this->m_vertexShader);
	glDeleteShader(this->m_fragmentShader);

	return 0;
}

void Shader::use()
{
	glUseProgram(this->m_id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->m_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->m_id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(this->m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->m_id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(this->m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->m_id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(this->m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->m_id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, glm::mat2& value) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->m_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, glm::mat3& value) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->m_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->m_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

GLuint Shader::getProgramId()
{
	return this->m_id;
}