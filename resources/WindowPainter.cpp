#include "WindowPainter.h"

WindowPainter::WindowPainter() {
    smoothMousePos[0] = INT_MIN; smoothMousePos[1] = INT_MIN;
    lastMousePos[0] = INT_MIN; lastMousePos[1] = INT_MIN;
    this->massInit();
    this->io = ImGui::GetIO();
}

void WindowPainter::clearMouseData() {
    this->mouseData[5] = 0;
    for (int i = 2; i < 5; i++) {
        if (this->mouseData[i] == 2) {
            this->mouseData[i] = 1;
        }
    }
    smoothMousePos[2] = 0;
}

void WindowPainter::looper() { 
    this->io = ImGui::GetIO();
    int x, y, xpos, ypos;
    glfwGetWindowSize(window, &x, &y);
    glfwGetWindowPos(window, &xpos, &ypos);

    this->clearMouseData();

    for (int i = 0; i < 3; i++) {
        if (!io.MouseDownOwned[i]) {
            short pos = i + 2;
            if (io.MouseClicked[i]){
                this->mouseData[pos] = 2;
            }
            else if (io.MouseReleased[i])
                this->mouseData[pos] = 0;
        }
    }

    // for (char i=2; i<5; i++){
    //     if (mouseData[i]){
    //         cursorOutFunc(x, y);
    //         break;
    //     }
    // }

    imRender();

    glfwSwapInterval(1);

    glfwSwapBuffers(window);

    glfwPollEvents();

    smoothMousePos[0] += smoothMouseDiff[0] * moveSmooth;
    smoothMousePos[1] += smoothMouseDiff[1] * moveSmooth;
    
    smoothMousePos[2] += smoothMouseDiff[2] * scrollSmooth;

    smoothDiff[0] = smoothMouseDiff[0] * moveSmooth;
    smoothDiff[1] = smoothMouseDiff[1] * moveSmooth;

    smoothMouseDiff[0] -= smoothDiff[0];
    smoothMouseDiff[1] -= smoothDiff[1];

    smoothMouseDiff[2] -= smoothMouseDiff[2] * scrollSmooth;
}

void WindowPainter::cursorOutFunc(int width, int height) {
	if (this->mouseData[0] > width - 3) {
		glfwSetCursorPos(window, 2, this->mouseData[1]);
		this->smoothMouseDiff[0] += width - 3;
	}
	else if (this->mouseData[0] < 2) {
		glfwSetCursorPos(window, width - 3, this->mouseData[1]);
		this->smoothMouseDiff[0] -= width - 3;
	}

	if (this->mouseData[1] > height - 3) {
		glfwSetCursorPos(window, this->mouseData[0], 2);
		this->smoothMouseDiff[1] += height - 3;
	}
	else if (this->mouseData[1] < 2) {
		glfwSetCursorPos(window, this->mouseData[0], height - 3);
		this->smoothMouseDiff[1] -= height - 3;
	}
}

void WindowPainter::imRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Checkbox("Enable Gravity", &enableGravity);

    ImGui::Combo("Camera Type", &current_pos, cameraTypes, 3, IM_ARRAYSIZE(cameraTypes));

    ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.05f, 1.0f, "%.3f", 1.0f);
    ImGui::SliderFloat("Camera Sensitivity", &cameraSensitivity, 0.05f, 1.0f, "%.3f", 1.0f);
    ImGui::SliderFloat("Camera smoothness", &moveSmooth, 0.05f, 1.0f, "%.3f", 1.0f);
    ImGui::SliderFloat("Scroll smoothness", &scrollSmooth, 0.05f, 1.0f, "%.3f", 1.0f);

    ImGui::BeginTabBar("Info");

    ImGui::TextWrapped("Camera movement in WALKER camera mode:\n"
        "WASD are same as any other game q : camera up, e : camera down\n"
        "shift slows the camera down, ctrl speeds up\n"
        "third and first person modes are same as any other game\n"
        "WASD movement space jump");

    ImGui::EndTabBar();


    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WindowPainter::massInit() {
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (!glfwInit()){
        printf("GLFW initialization error\n");
        std::exit(-1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(1600, 900, "Painted Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Window creation error\n");
        std::exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(this->window, WindowPainter::mouseEventCallback);
    glfwSetMouseButtonCallback(this->window, WindowPainter::buttonEventCallback);
    glfwSetScrollCallback(this->window, WindowPainter::scrollEventCallback);
    glfwSetWindowFocusCallback(this->window, WindowPainter::glfwWindowFocusCallback);
    glfwSetKeyCallback(this->window, WindowPainter::glfwKeyEventCallback);
    glfwSetWindowSizeCallback(this->window, WindowPainter::windowSizeEventCallback);
    glfwSetWindowUserPointer(window, this);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        std::exit(-1);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, 1600, 900);

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
}

void WindowPainter::mouseEventCallback(GLFWwindow* window, double xpos, double ypos)
{
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
    thisClass->mouseData[0] = (int)xpos;
    thisClass->mouseData[1] = (int)ypos;
    
    if (thisClass->smoothMousePos[0] == INT_MIN){
        thisClass->smoothMousePos[0] = xpos;
        thisClass->smoothMousePos[1] = ypos;
    }

    if (thisClass->mouseData[3] && thisClass->lastMousePos[0] != INT_MIN){
        thisClass->smoothMouseDiff[0] += xpos - thisClass->lastMousePos[0];
        thisClass->smoothMouseDiff[1] += ypos - thisClass->lastMousePos[1];
    }

    thisClass->lastMousePos[0] = (int)xpos;
    thisClass->lastMousePos[1] = (int)ypos;
}

void WindowPainter::buttonEventCallback(GLFWwindow* window, int button, int action, int mods) {
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
    
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT){
        thisClass->smoothMousePos[0] = thisClass->mouseData[0];
        thisClass->smoothMousePos[1] = thisClass->mouseData[1];

        thisClass->smoothMouseDiff[0] = 0;
        thisClass->smoothMouseDiff[1] = 0;
    }
}

void WindowPainter::scrollEventCallback(GLFWwindow* window, double xoffset, double yoffset) {
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
    thisClass->mouseData[5] = (int)yoffset;
    thisClass->smoothMouseDiff[2] += yoffset;
}

void WindowPainter::glfwKeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        thisClass->keyData[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        thisClass->keyData[key] = false;
    }
}

void WindowPainter::glfwWindowFocusCallback(GLFWwindow* window, int isFocused) {
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
    if (!isFocused) {
        thisClass->clearMouseData();
    }
}

void WindowPainter::windowSizeEventCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}