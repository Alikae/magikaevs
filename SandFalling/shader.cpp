#include "gllib.h"

void check_shaders_compile_errors(unsigned int shader, const char* type)
{
	int success;
	char infoLog[1024];

	if (strcmp(type, "PROGRAM"))
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type, infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type, infoLog);
		}
	}
}

int	create_shader(const char* vertex_path, const char* fragment_path)
{
	char* vertex_code;
	char* fragment_code;
	int	id;

	printf("CREATE SHADER PROGRAM ... ");
	// ensure ifstream objects can throw exceptions:
	vertex_code = read_file(vertex_path);
	fragment_code = read_file(fragment_path);
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_code, NULL);
	glCompileShader(vertex);
	check_shaders_compile_errors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_code, NULL);
	glCompileShader(fragment);
	check_shaders_compile_errors(fragment, "FRAGMENT");
	// shader Program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	check_shaders_compile_errors(id, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	free(vertex_code);
	free(fragment_code);
	printf("DONE\n");
	return (id);
}