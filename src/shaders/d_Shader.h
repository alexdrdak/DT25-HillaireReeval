#pragma once

#include "../d_global.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

class d_Shader {
public:
	d_Shader(const size_t _attribCount, const int _mode);
	~d_Shader() = default;

	GLuint d_ShaderHandle;

	void d_SetUniformMatrix4v(const std::string& _target, glm::mat4 _val);
	void d_SetUniform3v(const std::string& _target, float _val1, float _val2, float _val3);
	void d_SetUniform3f(const std::string& _target, glm::vec3 _val);
};