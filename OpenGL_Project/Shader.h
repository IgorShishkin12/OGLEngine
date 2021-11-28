#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <direct.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class Shader
{
	string m_path;
	ifstream m_file;
	string m_shader;
	string m_libDir;
	char m_type;
	int m_id=0;

public:
	/*инициация перегружена*(нет)
*можно ввести полный путь или если файл в той же директории то
*можно ввести его имя и расширение
*
 */
	Shader(string Path)
		: m_path{ Path }
	{
		if (Path.find('/') == string::npos || Path.find('\\') == string::npos)
		{

			char current_work_dir[FILENAME_MAX];
			_getcwd(current_work_dir, sizeof(current_work_dir));
			string t_path = Path;
			Path = current_work_dir;
			/******напишу путь ручеуми если что закоментить*******************/
			Path = "C:\\Users\\shishkin_i\\source\\repos\\OpenGL_Project\\OpenGL_Project";
			/*************************************************************/

			Path += "\\" + t_path;
			//cout<<Path<<"\n";
			m_path = Path;
		}
		m_file.open(m_path);
		string buffer;
		while (getline(m_file, buffer))
		{
			m_shader += buffer + "\n";
		}
		m_file.close();
		m_shader += "\0";
		//cout<<m_shader;
		m_type = m_path[m_path.size() - 2]; //.vs for vertex shader, .fs for fragment shader
		if (m_type == 'v')
		{
			vertexShaderInit();
		}
		else/* if (m_type == 'f')*/
		{
			fragmentShaderInit();
		}
	}
	int getID()
	{
		return m_id;
	}

private:
	void vertexShaderInit()
	{
		const char* time_ch = m_shader.c_str();
		m_id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_id, 1, &time_ch, NULL);
		glCompileShader(m_id);
		checkCompileErrors();
	}

	void fragmentShaderInit()
	{
		const char* time_ch = m_shader.c_str();
		m_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_id, 1, &time_ch, NULL);
		glCompileShader(m_id);
		checkCompileErrors();
	}

	void checkCompileErrors()
	{
		int success;
		char infoLog[1024];
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_id, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << m_type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
};

class ShProg
{
	int ID;

public:
	ShProg(Shader &vertex, Shader &fragment)// vertex,fragment
	{

		ID = glCreateProgram();
		glAttachShader(ID, vertex.getID());
		glAttachShader(ID, fragment.getID());
		glLinkProgram(ID);
		checkCompileErrors();
	}

	void use()
	{
		glUseProgram(ID);
	}

	// Полезные uniform-функции
	void setBool(const string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	int getID()
	{
		return ID;
	}


private:
	void checkCompileErrors()
	{
		int success;
		char infoLog[1024];

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM" << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}


};