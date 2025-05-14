#include "d_Shader.h"

struct d_shaderstruct {
	std::string d_debug_path;
	std::string d_contents;
	GLenum d_type;
};

d_Shader::d_Shader(const size_t _attribCount, const int _mode)
{

	std::string d_header = "#version 430 core\n";
	d_header += (_mode == 2) ? "#define D_SSBO\n" : "";

	std::string d_attribs = "";
	for (int i = 0; i < _attribCount; i++) {
		d_attribs += "layout(location=" + std::to_string(i) + ") in vec3 d_attr" + std::to_string(i) + ";\n";
	}

	std::string d_body = 
		"#ifdef D_SSBO\n "
			"layout(binding = 4, std430) readonly buffer ssbo1 { \n"
			"vec3 d_instance[];\n"
			"};\n"
			"vec3 d_offsetPos = d_instance[gl_InstanceID];\n"
		"#else\n"
			"layout(location = 3) in vec3 d_instance;\n"
			"vec3 d_offsetPos = d_instance;\n"
		"#endif\n";

	d_body = (_mode > 0) ? d_body : "";

	std::string d_main = "void main() {\n gl_Position = vec4(";

	if (_mode > 0) // d_instance vertAttr
		d_main += "d_offsetPos+";

	for (int i = 0; i < _attribCount-1; i++) {
		d_main += "d_attr" + std::to_string(i)+"+";
	}
	d_main += "d_attr" + std::to_string(_attribCount-1) + ", 1.0);\n}\n";


	std::string d_vertShaderContent = d_header + d_attribs + d_body + d_main;

	//std::cout << d_vertShaderContent << "\n\n\n" << std::endl;

	d_ShaderHandle = glCreateProgram();

	GLuint d_tempHandle = glCreateShader(GL_VERTEX_SHADER);
	const char* temp_src = d_vertShaderContent.c_str();

	glShaderSource(d_tempHandle, 1, &temp_src, nullptr);
	glCompileShader(d_tempHandle);

	int compileSuccess;
	glGetShaderiv(d_tempHandle, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess) {
		char infoLog[512];
		glGetShaderInfoLog(d_tempHandle, 512, NULL, infoLog);
		d_Logger::info("ERROR::SHADER::COMPILATION_FAILED: " + std::string(infoLog));
		d_Logger::info("Shader: " + d_vertShaderContent);
	}
	else {
		d_Logger::info(".VERT SHADER OK.");
	}

	std::string d_fragShaderContent =
		"#version 430 core\n"
		"out vec4 color;\n" 
			"void main() {\n"
			"color = vec4(vec3(1.0), 1.0);\n"
		"}";
	
	glAttachShader(d_ShaderHandle, d_tempHandle);

	d_tempHandle = glCreateShader(GL_FRAGMENT_SHADER);
	temp_src = d_fragShaderContent.c_str();

	glShaderSource(d_tempHandle, 1, &temp_src, nullptr);
	glCompileShader(d_tempHandle);

	glGetShaderiv(d_tempHandle, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess) {
		char infoLog[512];
		glGetShaderInfoLog(d_tempHandle, 512, NULL, infoLog);
		d_Logger::info("ERROR::SHADER::COMPILATION_FAILED: " + std::string(infoLog));
		d_Logger::info("Shader: " + d_fragShaderContent);
	}
	else {
		d_Logger::info(".FRAG SHADER OK.");
	}

	glAttachShader(d_ShaderHandle, d_tempHandle);

	glLinkProgram(d_ShaderHandle);
	int success;
	glGetProgramiv(d_ShaderHandle, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(d_ShaderHandle, 512, NULL, infoLog);
		d_Logger::err("ERROR::SHADER_PRORGAM::COMPILATION_FAILED: " + std::string(infoLog));
		return;
	}
	glValidateProgram(d_ShaderHandle);
	d_Logger::info("SUCCESS::SHADER_PRORGAM::COMPILATION_OK");
}

/*
* Never forget to glUseProgam() before use.
*/
void d_Shader::d_SetUniformMatrix4v(const std::string& _target, glm::mat4 _val) {
	//std::cout << glGetUniformLocation(d_ShaderHandle, _target.c_str()) << std::endl;
	glUniformMatrix4fv(glGetUniformLocation(d_ShaderHandle, _target.c_str()), 1, GL_FALSE, glm::value_ptr(_val));
}

void d_Shader::d_SetUniform3v(const std::string& _target, float _val1, float _val2, float _val3) {	
	//std::cout << glGetUniformLocation(d_ShaderHandle, _target.c_str()) << std::endl;
	glUniform3f(glGetUniformLocation(d_ShaderHandle, _target.c_str()), _val1, _val2, _val3);
}

void d_Shader::d_SetUniform3f(const std::string& _target, glm::vec3 _val) {	
	//std::cout << glGetUniformLocation(d_ShaderHandle, _target.c_str()) << std::endl;
	glUniform3f(glGetUniformLocation(d_ShaderHandle, _target.c_str()), _val[0], _val[1], _val[2]);
}