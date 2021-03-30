#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/glm.hpp>

class WindowPainter
{
public:
	int* mouseData = (int*)calloc(7, sizeof(int));
	int* keyData = (int*)calloc(512, sizeof(int));

	float cameraSpeed = 0.5f;
	float cameraSensitivity = 0.5f;
	float* smoothMousePos = (float*)calloc(3, sizeof(float));
	float* smoothMouseDiff = (float*)calloc(3, sizeof(float)); // Mouse x, y, scroll diffs
	float moveSmooth = 0.1f; float scrollSmooth = 0.1f;
	float* smoothDiff = (float*)calloc(2, sizeof(float));

	ImGuiIO io;
	int* lastMousePos = (int*)calloc(2, sizeof(int));
	glm::vec2 lastWinPos, lastWinSize;
	GLFWwindow* window;
	bool enableGravity = true;
	const char* cameraTypes[3] = { "WALKER", "THIRD", "FIRST" };
	int current_pos = 0;

	WindowPainter();

	void massInit();
	void looper();
	void cursorOutFunc(int width, int height);
	void clearMouseData();
	void imRender();
	static void mouseEventCallback(GLFWwindow*, double, double);
	static void buttonEventCallback(GLFWwindow*, int, int, int);
	static void scrollEventCallback(GLFWwindow*, double, double);
	static void glfwKeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowSizeEventCallback(GLFWwindow*, int, int);
	static void glfwWindowFocusCallback(GLFWwindow*, int);
};