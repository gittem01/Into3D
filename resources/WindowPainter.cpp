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
        if (releaseQueue[i-2]){
            mouseData[i] = 0;
            releaseQueue[i-2] = false;
        }
    }
    smoothMousePos[2] = 0;
}

void WindowPainter::looper() { 
    this->io = ImGui::GetIO();

    int x, y;
    glfwGetWindowSize(window, &x, &y);

    this->clearMouseData();

    imRender();
    
    glfwPollEvents();

    for (int i = 0; i < 3; i++) {
        if (!io.MouseDownOwned[i]) {
            short pos = i + 2;
            if (io.MouseClicked[i]){
                this->mouseData[pos] = 2;
            }
            else if (io.MouseReleased[i]){
                releaseQueue[i] = true;
            }
        }
    }

    glfwSwapInterval(1);

    glfwSwapBuffers(window);

    smoothMousePos[0] += smoothMouseDiff[0] * moveSmooth;
    smoothMousePos[1] += smoothMouseDiff[1] * moveSmooth;
    
    smoothMousePos[2] += smoothMouseDiff[2] * scrollSmooth;

    smoothDiff[0] = smoothMouseDiff[0] * moveSmooth;
    smoothDiff[1] = smoothMouseDiff[1] * moveSmooth;

    smoothMouseDiff[0] -= smoothDiff[0];
    smoothMouseDiff[1] -= smoothDiff[1];

    smoothMouseDiff[2] -= smoothMouseDiff[2] * scrollSmooth;
}

void WindowPainter::imRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Text("FPS : %f", io.Framerate);

    ImGui::Checkbox("Enable Gravity", &enableGravity);

    ImGui::Combo("Camera Type", &current_pos, cameraTypes, 3, IM_ARRAYSIZE(cameraTypes));

    ImGui::SliderFloat("Camera speed", &cameraSpeed, 0.05f, 1.0f, "%.3f", 1.0f);
    ImGui::SliderFloat("Movement smoothness", &keySmoothness, 0.05f, 1.0f, "%.3f", 1.0f);
    ImGui::SliderFloat("Camera sensitivity", &cameraSensitivity, 0.05f, 1.0f, "%.3f", 1.0f);
    ImGui::SliderFloat("Camera smoothness", &moveSmooth, 0.05f, 1.0f, "%.3f", 1.0f);
    ImGui::SliderFloat("Scroll smoothness", &scrollSmooth, 0.05f, 1.0f, "%.3f", 1.0f);

    ImGui::BeginTabBar("Info");

    ImGui::TextWrapped(
        "Camera movement in WALKER camera mode:\n"
        "WASD are same as any other game q : camera up, e : camera down\n"
        "shift slows the camera down, ctrl speeds up\n"
        "third and first person modes are same as any other game\n"
        "WASD movement space jump"
    );

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glm::vec2 winSize(1600, 900);

    window = glfwCreateWindow(winSize.x, winSize.y, "Painted Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Window creation error\n");
        std::exit(-1);
    }

    float x;
    glfwGetWindowContentScale(window, &x, NULL);
    dpiScaling = x;

    glfwSetWindowSize(window, winSize.x / dpiScaling, winSize.y / dpiScaling);

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

    glViewport(0, 0, winSize.x, winSize.y);

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
}

void WindowPainter::mouseEventCallback(GLFWwindow* window, double xpos, double ypos)
{
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
    xpos *= thisClass->dpiScaling;
    ypos *= thisClass->dpiScaling;

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

void WindowPainter::windowSizeEventCallback(GLFWwindow* window, int width, int height) {
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);

    glViewport(0, 0, width * thisClass->dpiScaling, height * thisClass->dpiScaling);
}