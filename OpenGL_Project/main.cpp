#include <glad/glad.h>
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
#include"Textures.h"
//#include "stb_image.h"
#include<array>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool isMove(GLFWwindow* window, float* arr);
std::array<float, 4> mouse();
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Game", NULL, NULL);
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
	std::array <float, 4> mice2{ 0,1,0,1 };
	int ID_mice = glGetUniformLocation(ourShader.getID(), "mouse");

	constexpr int sizex = 8, sizey = 1;
	vector<Sphere> vec;
	{
		vec.resize(4);//float radius=1, float x=0, float y=0, float z=0, float r=0, float g=0, float b=0, float a=1

			vec[0] = Sphere{100,14,100,39,0,3,8,1};
			vec[1] = Sphere{50,14,-100,-39,9,3,2,1};
			vec[2] = Sphere{ 10,14,-100,200,0,3,0,1 };
			vec[3] = Sphere{ 45,-14,100,309,3,3,3,1 };/**/
	}
	vector<Box> vec2(1);//(float sizex=0, float sizey=0, float sizez=0, float x=0, float y=0, float z=0, float a1=0, float b1=0, float c1=0
	vec2[0] = Box{ 7,8,900,100,18,27,1 / 4,1 / 5.1 / 6 };
	Textures tex(ourShader, vec2, vec);
	tex.makeOut();
	tex.send();

	while (!glfwWindowShouldClose(window))
	{
		// Обработка ввода
		processInput(window);

		// Рендеринг
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		mice2 = mouse();
		glUniform4f(ID_mice, mice2[0], mice2[1], mice2[2], mice2[3]);
		tex.sendData();

		if (isMove(window, moveCrd))
		{
			glUniform3f(ID_coords, moveCrd[0], moveCrd[1], moveCrd[2]);
		}
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