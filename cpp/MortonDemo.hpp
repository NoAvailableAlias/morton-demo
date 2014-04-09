#ifndef MORTON_DEMO_HPP
#define MORTON_DEMO_HPP

#include "DemoState.hpp"

#include <memory>

// ApEk,
// NoAvailableAlias
// this code is public domain

struct GLFWwindow;

class MortonDemo
{
    DemoState state;
    std::shared_ptr<void> clean; // RAII cleanup code

    void drawBackground() const;
    void drawHatchArea() const;
    void drawSearched() const;
    void draw2dRange() const;
    void demoDoDraw() const;

    void displayFps() const;

    void windowSizeIndirect(GLFWwindow*, int, int);
    void cursorButtonIndirect(GLFWwindow*, int, int, int);
    void cursorPositionIndirect(GLFWwindow*, double, double);

    static void windowSizeCallback(GLFWwindow*, int, int);
    static void cursorButtonCallback(GLFWwindow*, int, int, int);
    static void cursorPositionCallback(GLFWwindow*, double, double);

    void initializeDemo();

    public:

    void mainLoop();
};

#endif // MORTON_DEMO_HPP
