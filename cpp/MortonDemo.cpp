#include "MortonDemo.hpp"
#include "Point.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>
#include <chrono>
#include <memory>
#include <string>
#include <thread>

#ifndef _MSC_VER
  #include <unistd.h> // MinGW workaround
#endif

// ApEk,
// NoAvailableAlias
// this code is public domain

using Micro = std::chrono::microseconds;

void MortonDemo::drawBackground() const
{
    GLint cid = state.backgroundColors.vboID;
    GLint pid = state.backgroundPoints.vboID;
    auto size = state.backgroundPoints.buffer.size();

    glBindBuffer(GL_ARRAY_BUFFER, pid);
    glVertexPointer(2, GL_INT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, cid);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_POINTS, 0, size);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void MortonDemo::drawHatchArea() const
{
    GLint cid = state.hatchAreaColors.vboID;
    GLint pid = state.hatchAreaPoints.vboID;
    auto size = state.hatchAreaPoints.buffer.size();

    glBindBuffer(GL_ARRAY_BUFFER, pid);
    glVertexPointer(2, GL_INT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, cid);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_POINTS, 0, size);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void MortonDemo::drawSearched() const
{
    GLint cid = state.searchedColors.vboID;
    GLint pid = state.searchedPoints.vboID;
    auto size = state.searchedPoints.buffer.size();

    glBindBuffer(GL_ARRAY_BUFFER, pid);
    glVertexPointer(2, GL_INT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, cid);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_POINTS, 0, size);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void MortonDemo::draw2dRange() const
{
    Point const& releasePos = state.releasePos;
    Point const& cursorPos = state.cursorPos;
    Point const& pressPos = state.pressPos;

    bool keyPressed = state.keyPressed;

    GLint x1 = keyPressed ? cursorPos[0] : pressPos[0];
    GLint y1 = keyPressed ? cursorPos[1] : pressPos[1];
    GLint x2 = releasePos[0];
    GLint y2 = releasePos[1];

    glBegin(GL_LINE_LOOP);
    {
        glColor3f(0.0f, 0.5f, 0.0f);

        glVertex2i(x1, y1);
        glVertex2i(x2, y1);
        glVertex2i(x2, y2);
        glVertex2i(x1, y2);
    }
    glEnd();
}
void MortonDemo::demoDoDraw() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    {
        drawBackground(); // GL_STATIC_DRAW
        drawHatchArea(); // GL_DYNAMIC_DRAW
        drawSearched(); // GL_DYNAMIC_DRAW
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    draw2dRange();
}

void MortonDemo::displayFps() const
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> delta;
    static int fps;

    if (1000000 < std::chrono::duration_cast<Micro>
        (std::chrono::high_resolution_clock::now() - delta).count())
    {
        std::string title("Morton Order Demo : ");
        title += std::to_string(fps) + " FPS";
        glfwSetWindowTitle(state.window, title.c_str());
        delta = std::chrono::high_resolution_clock::now();
        fps = 0;
    }
    ++fps;
}

void MortonDemo::windowSizeIndirect(GLFWwindow* w, int width, int height)
{
    state.windowSize[0] = static_cast<PointElement>(width);
    state.windowSize[1] = static_cast<PointElement>(height);

    state.resetState();

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}
void MortonDemo::cursorButtonIndirect(GLFWwindow* w, int button, int action, int mods)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_1: // create a 2d range
        if (action == GLFW_PRESS)
        {
            state.pressPos = state.cursorPos;
            state.releasePos = state.cursorPos;
            state.keyPressed = true;
        }
        else
        {
            state.releasePos = state.cursorPos;
            state.keyPressed = false;
        }
    break;
        case GLFW_MOUSE_BUTTON_2: // toggle bigmin alg
        if (action == GLFW_PRESS)
        {
            state.bigminFlag = !state.bigminFlag;
        }
    break;
        case GLFW_MOUSE_BUTTON_4: // reset window size
        if (action == GLFW_PRESS)
        {
            windowSizeCallback(state.window, 512, 512);
            glfwSetWindowSize(state.window, 512, 512);
        }
    }
    state.keyChanged = true;
}
void MortonDemo::cursorPositionIndirect(GLFWwindow* w, double x, double y)
{
    state.cursorPos[0] = static_cast<PointElement>(x);
    state.cursorPos[1] = static_cast<PointElement>(y);
    state.keyChanged = true;
}

void MortonDemo::windowSizeCallback(GLFWwindow* w, int width, int height)
{
    MortonDemo* self = static_cast<MortonDemo*>(glfwGetWindowUserPointer(w));
    self->windowSizeIndirect(w, width, height);
}
void MortonDemo::cursorButtonCallback(GLFWwindow* w, int button, int action, int mods)
{
    MortonDemo* self = static_cast<MortonDemo*>(glfwGetWindowUserPointer(w));
    self->cursorButtonIndirect(w, button, action, mods);
}
void MortonDemo::cursorPositionCallback(GLFWwindow* w, double x, double y)
{
    MortonDemo* self = static_cast<MortonDemo*>(glfwGetWindowUserPointer(w));
    self->cursorPositionIndirect(w, x, y);
}

void MortonDemo::initializeDemo()
{
    Point& windowSize = state.windowSize;

    if (!glfwInit())
    {
        throw std::runtime_error("glfwInit failed");
    }
    state.window = glfwCreateWindow(
        windowSize[0], windowSize[1], "", NULL, NULL);

    if (state.window == nullptr)
    {
        throw std::runtime_error("glfwCreateWindow failed");
    }
    glfwSwapInterval(1);
    glfwMakeContextCurrent(state.window);

    if (auto fail = glewInit())
    {
        std::cerr << glewGetErrorString(fail) << std::endl;

        throw std::runtime_error("glewInit failed");
    }
    glGenBuffers(1, &state.backgroundColors.vboID);
    glGenBuffers(1, &state.backgroundPoints.vboID);

    glGenBuffers(1, &state.hatchAreaColors.vboID);
    glGenBuffers(1, &state.hatchAreaPoints.vboID);

    glGenBuffers(1, &state.searchedColors.vboID);
    glGenBuffers(1, &state.searchedPoints.vboID);

    clean = std::shared_ptr<void>(this, [](void* ptr) // RAII :D
    {
        MortonDemo* self = static_cast<MortonDemo*>(ptr);

        glDeleteBuffers(1, &self->state.backgroundColors.vboID);
        glDeleteBuffers(1, &self->state.backgroundPoints.vboID);

        glDeleteBuffers(1, &self->state.hatchAreaColors.vboID);
        glDeleteBuffers(1, &self->state.hatchAreaPoints.vboID);

        glDeleteBuffers(1, &self->state.searchedColors.vboID);
        glDeleteBuffers(1, &self->state.searchedPoints.vboID);

        glfwTerminate();
    });
    glfwSetWindowUserPointer(state.window, this);
    glfwSetCursorPosCallback(state.window, cursorPositionCallback);
    glfwSetWindowSizeCallback(state.window, windowSizeCallback);
    glfwSetMouseButtonCallback(state.window, cursorButtonCallback);

    windowSizeCallback(state.window, windowSize[0], windowSize[1]);
}

void MortonDemo::mainLoop()
{
    try
    {
        initializeDemo();

        while (!glfwWindowShouldClose(state.window))
        {
            displayFps();

            if (state.keyChanged || state.keyPressed)
            {
                glClear(GL_COLOR_BUFFER_BIT);

                state.updateTick();
                demoDoDraw();

                glfwSwapBuffers(state.window);
            }
            else // yield cpu resources
            {
#ifdef _MSC_VER
                std::this_thread::sleep_for(Micro(10000));
#else
                usleep(10000); // MinGW workaround
#endif
            }
            glfwPollEvents();
        }
    }
    catch (std::runtime_error const& error)
    {
        std::cerr << error.what() << std::endl;

        glfwTerminate();
    }
    catch (std::exception const& error)
    {
        std::cerr << error.what() << std::endl;
    }
}
