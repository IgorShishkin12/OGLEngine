﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include<iomanip>
//#define STB_IMAGE_IMPLEMENTATION //<-- хз костыль какойто заводской
//#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <math.h>

#include "shader.h"
#include "Matrix.h"
#include "Sphere.h"
//#include "stb_image.h"
#include<array>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool isMove(GLFWwindow* window, float* arr);
std::array<float, 4> mouse();
array <float, 7> camera(GLFWwindow* window);
// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: инициализация и конфигурирование    
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw: создание окна
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: загрузка всех указателей на OpenGL-функции
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Компилирование нашей шейдерной программы
	Shader sh1("shader3.vs");
	Shader sh2("shader4.fs");
	ShProg ourShader{ sh1,sh2 };

	// Указание вершин (и буфера(ов)) и настройка вершинных атрибутов
	float vertices[] = {
		// координаты        // цвета            // текстурные координаты
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // верхняя правая вершина
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // нижняя правая вершина
	   -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // нижняя левая вершина
	   -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // верхняя левая вершина 
	};




	unsigned int indices[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Цветовые атрибуты
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Атрибуты текстурных координат
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	float moveCrd[3]{ 0.0,0.0,0.0 };
	int ID_coords = glGetUniformLocation(ourShader.getID(), "me1");
	std::array <float, 7> mice2{ 0,0,0,0,1,0,1 };
	int ID_mice = glGetUniformLocation(ourShader.getID(), "mouse");

	int ID_sizeSph = glGetUniformLocation(ourShader.getID(), "sizeSph");

	unsigned int ID_texDataSph;
	glGenTextures(1, &ID_texDataSph);
	glBindTexture(GL_TEXTURE_2D, ID_texDataSph);
	constexpr int sizex = 8, sizey = 1;
	glUniform2i(ID_sizeSph, sizex, sizey);
	vector<Sphere> vec;
	{
		vec.resize(sizex*sizey/2);//float radius=1, float x=0, float y=0, float z=0, float r=0, float g=0, float b=0, float a=1

			vec[0] = Sphere{ 100,14,100,39,0,3,8,1 };
			vec[1] = Sphere{ 50,14,100,39,9,3,2,1 };
			vec[2] = Sphere{ 10,14,-100,200,0,3,2,1 };
			vec[3] = Sphere{ 45,-14,10,309,3,3,3,1 };
	}
	//cout << setprecision(50);
	//cout << vec[0].radius;
	vector <std::array<float, 8>> toTex;
	for (int i = 0; i < sizex * sizey/2; ++i)
	{
		toTex.push_back(vec[i].getArr());
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//см ниже
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//пиксель на всей области одного цвета
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, sizex , sizey, 0, GL_RGBA,GL_FLOAT,&toTex[0][0]);                  //gl_rgba32f ненормализует
	//все о тексстурах: обычно цвета нормализуются, в gl_rgba32f не нормализуются
	// параметры:https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
	//  texelFetch позволяет передавать в аргументах значения как для массива
	// 
	// Цикл рендеринга

	while (!glfwWindowShouldClose(window))
	{
		// Обработка ввода
		processInput(window);

		// Рендеринг
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Рендеринг ящика
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		mice2 = camera(window);
			//std::cout << mice2[0][0]<< "\t" << mice2[0][1]<< "\t" << mice2[1][0]<<"\t" << mice2[1][1]<< "\t" << 0.00005<<std::endl;
		glUniform4f(ID_mice, mice2[3], mice2[4], mice2[5], mice2[6]);
		glUniform3f(ID_coords, mice2[0], mice2[1], mice2[2]);
		glUniform2i(ID_sizeSph, sizex, sizey);

		// glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	// glfw: завершение, освобождение всех выделенных ранее GLFW-реурсов
	glfwTerminate();
	return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


std::array<float, 4> mouse()
{
	static std::array<float, 2> arr{ 0,0 };
		POINT p;
		static POINT pPast{ 0,0 };
		if (GetCursorPos(&p))
		{
			if (p.x != pPast.x || p.y != pPast.y)
			{
				POINT difference{ p.x - pPast.x,p.y - pPast.y };
				arr[0] += -difference.x / 100.0;
				arr[1] += -difference.y / 100.0;
				pPast.x = p.x;
				pPast.y = p.y;
			}
		}
		return { sin(arr[1]), cos(arr[1]), sin(arr[0]), cos(arr[0]) };
}


array <float, 7> camera(GLFWwindow *window)
{
	static bool flag1385{ 1 };
	static VectorF position{ 3 }, px{ 3 }, py{ 3 }, pz{ 3 };
	if(flag1385)
	{
		position.get(1) = 0;
		position.get(2) = 0;
		position.get(3) = 0;
		px = position;
		py = position;
		pz = position;
		px.get(1) = 1;
		py.get(2) = 1;
		pz.get(3) = 1;
		flag1385 = 0;
	}

	static std::array<float, 2> arr{ 0,0 };
	POINT p;
	static POINT pPast{ 0,0 };
	static pair<float,float> pPast1{ 0,0 };
	if (GetCursorPos(&p))
	{
		if (p.x != pPast.x || p.y != pPast.y)
		{
			POINT difference{ p.x - pPast.x,p.y - pPast.y };
			MatrixF mx1 = changeRMx3('x', difference.x / 100.0);
			MatrixF tmx = changeRMx3('y',difference.y/100.0);
			tmx = tmx * mx1;
			MatrixF tme = px * tmx;
			px = tme;
			tme = py * tmx;
			py = tme;
			tme = pz * tmx;
			pz = tme;

			pPast.x = p.x;
			pPast.y = p.y;
			pPast1.first += p.x/100.0;
			pPast1.second += p.y/100.0;
		}
	}
	static float oim[3];
	oim[0] = 0;
	oim[1] = 0;
	oim[3] = 0;
	if (isMove(window, oim))
	{
		MatrixF sPlus = 1*position + oim[0] * py + oim[1] * pz + oim[3] * px;
		position = sPlus;
	}
	return array<float, 7>{position.get(1), position.get(2), position.get(3), sin(pPast1.first), cos(pPast1.first), sin(pPast1.second), cos(pPast1.second) };



}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Убеждаемся, что окно просмотра соответствует новым размерам окна.
	// Обратите внимание, что высота и ширина будут значительно больше, чем указано, на Retina-дисплеях
	glViewport(0, 0, width, height);
}
bool isMove(GLFWwindow *window, float* arr)
{
	bool flag1 = 0;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		flag1 = true;
		++arr[1];
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		flag1 = true;
	   -- arr[1];
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		flag1 = true;
		++arr[0];
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		flag1 = true;
		--arr[0];
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		flag1 = true;
		++arr[2];
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		flag1 = true;
		--arr[2];
	}
	return flag1;

}