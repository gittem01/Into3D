#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <glm/glm.hpp>

class WindowPainter
{
public:
	int* mouseData = (int*)calloc(7, sizeof(int));
	int* keyData = (int*)calloc(512, sizeof(int));
	ImGuiIO io;
	int* lastMousePos = (int*)calloc(2, sizeof(int));
	glm::vec2 lastWinPos, lastWinSize;
	GLFWwindow* window;
	float currentColor[4] = {1, 1, 1, 1};
	float currentDepth = 0;
	bool isPicking, drawMode, grid;
	const char* tripletDist[8] = { "2", "3", "4", "5", "6", "7", "8", "9" };
	float bounciness = 0.2f;
	int tripletNum = 0;
	int numOfTriplets = 0;
	int timeStep = 60;
	int posIteration = 10;
	int velocityIteration = 10;
	bool enableGravity = false;
	bool showLines = true;
	bool fullScreen = false;
	bool isClear = false;

	WindowPainter();

	void massInit();
	void looper();
	void clearMouseData();
	void imRender();
	static void mouseEventCallback(GLFWwindow*, double, double);
	static void buttonEventCallback(GLFWwindow*, int, int, int);
	static void scrollEventCallback(GLFWwindow*, double, double);
	static void glfwKeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowSizeEventCallback(GLFWwindow*, int, int);
	static void glfwWindowFocusCallback(GLFWwindow*, int);
};