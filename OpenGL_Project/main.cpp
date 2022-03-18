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
//#include <time.h>

#include "shader.h"
#include "Matrix.h"
#include "Sphere.h"
#include"Textures.h"
#include "Materials.h"
#include "Polygon.h"
//#include "stb_image.h"
#include<array>
#include <iostream>
#include <C:\Users\shishkin_i\Downloads\Pump_Impellar (1).cpp>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool isMove(GLFWwindow* window, float* arr);
void sendWindowP(GLFWwindow*,ShProg&);
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
	Shader sh1("shader3.vert");
	Shader sh2("shader4.frag");
	ShProg ourShader{ sh1,sh2 };
	ourShader.use();
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
	int time_GLSL = glGetUniformLocation(ourShader.getID(), "time");

	constexpr int sizex = 8, sizey = 1;
	vector<Sphere> vec;
	{
		//float radius=1, float x=0, float y=0, float z=0, float r=0, float g=0, float b=0, float a=1

		vec.push_back(Sphere{ 100,14,100,39,0,3,8 });
		vec[vec.size() - 1].setMat(2, 100);
		vec.push_back(Sphere{ 50,14,-100,-39,9,3,2 });
		vec[vec.size() - 1].setMat(1, 100);
		vec.push_back(Sphere{ 10,14,-100,200,0,3,0 });
		vec[vec.size() - 1].setMat(1, 100);
		vec.push_back(Sphere{ 45,-14,100,309,3,3,3 });/**/
		vec[vec.size() - 1].setMat(1, 100);
		vec.push_back(Sphere{ 45,1000,1000,1000,3,3,3 });/**/
		vec[vec.size() - 1].setMat(1, 100);
		vec.push_back(Sphere{ 45,1000,1000,-1000,3,3,3 });/**/
		vec[vec.size() - 1].setMat(1, 100);
		vec.push_back(Sphere{ 45,1000,-1000,1000,3,3,3 });/**/
		vec[vec.size() - 1].setMat(1, 100);

	}
	vector<Box> vec2;//(float sizex=0, float sizey=0, float sizez=0, float x=0, float y=0, float z=0, float a1=0, float b1=0, float c1=0
	{
		vec2.push_back(Box{ 80,8,900,100, 18,27,1 / 1,1 / 1, 1 / 1 });
		vec2[vec2.size() - 1].setMat(1, 100);
		vec2.push_back(Box{ 7,80,90,10,7,50,1 / 9,1 / 1,1 / 8 });
		vec2[vec2.size() - 1].setMat(0, 100);
		vec2.push_back(Box{ 1,2000,2000,0,0,0,0,0,0});
		vec2[vec2.size() - 1].setMat(3, 100);
	}
	vector<Triangle> vec3;
	{
		vec3.reserve(2);
		vec3.push_back(Triangle{ -1000,1000,-1000, -1000,1000,1000,  -1000,-1000,1000 });
		vec3.push_back(Triangle{ 1000,1000,-1000, 1000,1000,1000,  1000,-1000,1000 });
		vec3[0].setMat(2, 100);
		vec3[1].setMat(1, 100);
		for (long i = 0; i < 3420; ++i)
		{
			long j = i * 9;
			float* q = &vertexTriangleArray[j];
			vec3.push_back(Triangle(q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8]));
			vec3[i].setMat(1, 100);
		}
	}
	vector<Material1> mats;
	{
		//mats.reserve(2);
		mats.push_back(Material1{ 0,0,200,1,0.1 });
		mats.push_back(Material1{ 0,1,0 });
		mats.push_back(Material1{ 1,0,0 });
		mats.push_back(Material1{ 1,2,3 });
	}
	Textures tx{ ourShader };
	vector <float> hans;
	vector <long> hans2;
	vector <int> hans3;
	long long matBeg = 0;
	{
		for (auto& j : vec)
		{
			const auto& q{ j.getArr() };
			for (auto z : q)hans.push_back(z);
			hans2.push_back(j.materialID);
			hans3.push_back(j.materialClass);
		}
		tx.addData(8, vec.size(), 1, &hans[0]);
		tx.addMaterials(matBeg, &hans2[0], &hans3[0]);
	}
	{
		hans.clear();
		hans2.clear();
		hans3.clear();
		for (auto& j : vec2)
		{
			const auto& q = j.get();
			for (auto z : q)hans.push_back(z);
			hans2.push_back(j.materialID);
			hans3.push_back(j.materialClass);
		}
		tx.addData(12, vec2.size(), 2, &hans[0]);
		tx.addMaterials(matBeg, &hans2[0], &hans3[0]);
	}
	{
		hans.clear();
		hans2.clear();
		hans3.clear();
		for (auto& j : vec3)
		{
			const auto& q = j.get();
			for (auto z : q)hans.push_back(z);
			hans2.push_back(j.materialID);
			hans3.push_back(j.materialClass);
		}
		tx.addData(12, vec3.size(), 3, &hans[0]);
		tx.addMaterials(matBeg, &hans2[0], &hans3[0]);
	}
	{
		hans.clear();
		for (auto& j : mats)
		{ 
			const auto& q = j.getData();
			for (auto z : q)hans.push_back(z);
		}
		tx.addData(8, mats.size(), 100, &hans[0]);
		tx.addMaterials(matBeg, &hans2[0], &hans3[0]);//случайное наполнение
	}
	tx.compressData(0);
	tx.createTexes();
	tx.compressData();
	while (!glfwWindowShouldClose(window))
	{
		// Обработка ввода
		processInput(window);
		//__rdtsc();
		// Рендеринг
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		mice2 = mouse();
		sendWindowP(window, ourShader);
		glUniform4f(ID_mice, mice2[0], mice2[1], mice2[2], mice2[3]);
		float time_now = __rdtsc();
		//time_now = time_now / 1e14;
		//cout << time_now << endl;
		glUniform1f(time_GLSL, time_now);
		tx.send();
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

void sendWindowP(GLFWwindow* window,ShProg& ourSdr)
{
	static bool isFirst{ 1 };
	static int ID_coords;
	if (isFirst)
	{
		isFirst = 0;
		ID_coords = glGetUniformLocation(ourSdr.getID(), "u_resolution");

	}
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glUniform2i(ID_coords, width, height);

}