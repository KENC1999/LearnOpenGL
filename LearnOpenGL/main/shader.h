#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


class Shader
{
public:
	// 程序ID
	unsigned int ID;

	// 构造器读取并构建着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用/激活程序
	void use();
	// uniform工具函数
	void set(const std::string &name, bool value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void set(const std::string &name, int value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void set(const std::string &name, float value) const{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) 
{
	string vertex_code;
	string fragment_code;
	ifstream v_file;
	ifstream f_file;
	v_file.exceptions(ifstream::failbit|ifstream::badbit);
	f_file.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		v_file.open(vertexPath);
		f_file.open(fragmentPath);
		stringstream v_stream, f_stream;
		v_stream << v_file.rdbuf();
		f_stream << f_file.rdbuf();
		v_file.close();
		f_file.close();
		vertex_code = v_stream.str();
		fragment_code = f_stream.str();
	}
	catch (ifstream::failure& e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	const char* vShaderCode = vertex_code.c_str();
	const char* fShaderCode = fragment_code.c_str();
	unsigned int vertex, fragment;
	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	
	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	//shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	//delete shader
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type){
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success){
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::"<<type<<"::COMPILATION_FAILED\n" << infoLog <<"\n----------"<< std::endl;
		}
	}
	else {
		glGetShaderiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK::COMPILATION_FAILED\n" << infoLog << "\n----------" << std::endl;
		}
	}
}




#endif