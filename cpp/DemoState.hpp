#ifndef DEMO_STATE_HPP
#define DEMO_STATE_HPP

#include "Point.hpp"

#include <cstdint>
#include <vector>
#include <array>

// ApEk,
// NoAvailableAlias
// this code is public domain

using Color = std::array<float, 3>;

struct GLFWwindow;

class DemoState
{
    void updateBackground();
    void updateHatchArea();
    void updateSearched();

    public:

    template <typename T> struct VboWrapper
    {
        std::uint32_t vboID;
        std::vector<T> buffer;

        std::size_t size() const
        {
            return buffer.size() * sizeof(T);
        }
    };

    Point pressPos;
    Point cursorPos;
    Point releasePos;
    Point windowSize = {{ 512, 512 }};

    bool keyPressed;
    bool keyChanged;

    GLFWwindow* window;

    VboWrapper<Color> backgroundColors;
    VboWrapper<Point> backgroundPoints;

    VboWrapper<Color> hatchAreaColors;
    VboWrapper<Point> hatchAreaPoints;

    VboWrapper<Color> searchedColors;
    VboWrapper<Point> searchedPoints;

    void resetState();
    void updateTick();
};

#endif // DEMO_STATE_HPP
