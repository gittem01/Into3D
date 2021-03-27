#include "WindowPainter.h"

WindowPainter::WindowPainter() {
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
    if (lastMousePos[0] == mouseData[0] && lastMousePos[1] == mouseData[1]) {
        mouseData[6] = 0;
    }
    else {
        mouseData[6] = 1;
    }
}

void WindowPainter::looper() { 
    this->io = ImGui::GetIO();
    int x, y, xpos, ypos;
    glfwGetWindowSize(window, &x, &y);
    glfwGetWindowPos(window, &xpos, &ypos);

    if (fullScreen) {
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), xpos, ypos, 1920, 1080, 60);
        glViewport(0, 0, 1920, 1080);
    }
    else{
        if (x == 1920 && y == 1080) {
            x = 1600; y = 900;
        }
        if (xpos == 0 && ypos == 0) {
            xpos = 160; ypos = 90;
        }
        glfwSetWindowMonitor(window, 0, xpos, ypos, x, y, 60);
        glViewport(0, 0, x, y);
    }

    this->clearMouseData();
    lastMousePos[0] = mouseData[0];
    lastMousePos[1] = mouseData[1];

    /* Render here */
    imRender();

    for (int i = 0; i < 3; i++) {
        if (!io.MouseDownOwned[i]) {
            short pos = i + 2;
            if (io.MouseClicked[i])
                this->mouseData[pos] = 2;
            else if (io.MouseReleased[i])
                this->mouseData[pos] = 0;
        }
    }
    
    /* Swap front and back buffers */
    glfwSwapInterval(1);

    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

void WindowPainter::imRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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
}

void WindowPainter::buttonEventCallback(GLFWwindow* window, int button, int action, int mods) {
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
}

void WindowPainter::scrollEventCallback(GLFWwindow* window, double xoffset, double yoffset) {
    WindowPainter* thisClass = (WindowPainter*)glfwGetWindowUserPointer(window);
    thisClass->mouseData[5] = (int)yoffset;
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