#include "DemoState.hpp"
#include "Point.hpp"

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <vector>

// ApEk,
// NoAvailableAlias
// this code is public domain

static float toRange(std::size_t X, std::size_t A, std::size_t B, float C, float D)
{
    return (X - A) / (float)(B - A) * (D - C) + C;
}
bool operator< (Point const& lhs, Point const& rhs)
{
    return lhs[0] < rhs[0] and lhs[1] < rhs[1];
}
bool operator> (Point const& lhs, Point const& rhs)
{
    return lhs[0] > rhs[0] and lhs[1] > rhs[1];
}
template <typename Iterator> struct RangeHelper
{
    Iterator lhs;
    Iterator rhs;

    Iterator cbegin() const { return lhs; }
    Iterator begin() const { return lhs; }
    Iterator cend() const { return rhs; }
    Iterator end() const { return rhs; }
};
template <typename Iterator>
static RangeHelper<Iterator> Range(Iterator lhs, Iterator rhs)
{
    if (lhs <= rhs) return { lhs, rhs }; return { rhs, lhs };
}
template <typename PT>
static void search(PT Min,
                   PT Max,
                   std::vector<PT> const& points,
                   std::vector<PT>& missed,
                   std::vector<PT>& retval)
{
    PT min = { std::min(Min[0], Max[0]), std::min(Min[1], Max[1]) };
    PT max = { std::max(Min[0], Max[0]), std::max(Min[1], Max[1]) };

    auto begin = points.cbegin();
    auto end   = points.cend();

    auto start = std::lower_bound(begin, end, min, morton<PT>());
    auto stop  = std::upper_bound(start, end, max, morton<PT>());

    for (PT point : Range(start, stop))
    {
        if (point > min && point < max)
        {
            retval.emplace_back(point);
        }
        else
        {
            missed.emplace_back(point);
        }
    }
}

void DemoState::updateBackground()
{
    auto xMax = windowSize[0];
    auto yMax = windowSize[1];
    auto sMax = xMax * yMax;

    backgroundColors.buffer.resize(sMax);
    backgroundPoints.buffer.resize(sMax);

    for (std::size_t x = 0; x < xMax; ++x)
    {
        for (std::size_t y = 0; y < yMax; ++y)
        {
            auto i = x * yMax + y;
            backgroundPoints.buffer[i] = Point{{ x, y }};
            backgroundColors.buffer[i] = Color{{ 0.0f, 0.f,
                toRange(i, 0, sMax, 0.2f, 1.0f) }};
        }
    }
    std::sort(backgroundPoints.buffer.begin(),
        backgroundPoints.buffer.end(), morton<Point>());

    glBindBuffer(GL_ARRAY_BUFFER, backgroundPoints.vboID);
    glBufferData(GL_ARRAY_BUFFER, backgroundPoints.size(),
        backgroundPoints.buffer.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, backgroundColors.vboID);
    glBufferData(GL_ARRAY_BUFFER, backgroundColors.size(),
        backgroundColors.buffer.data(), GL_STATIC_DRAW);
}
void DemoState::updateHatchArea()
{
    glBindBuffer(GL_ARRAY_BUFFER, hatchAreaPoints.vboID);
    glBufferData(GL_ARRAY_BUFFER, hatchAreaPoints.size(),
        hatchAreaPoints.buffer.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, hatchAreaColors.vboID);
    glBufferData(GL_ARRAY_BUFFER, hatchAreaColors.size(),
        hatchAreaColors.buffer.data(), GL_DYNAMIC_DRAW);
}
void DemoState::updateSearched()
{
    glBindBuffer(GL_ARRAY_BUFFER, searchedPoints.vboID);
    glBufferData(GL_ARRAY_BUFFER, searchedPoints.size(),
        searchedPoints.buffer.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, searchedColors.vboID);
    glBufferData(GL_ARRAY_BUFFER, searchedColors.size(),
        searchedColors.buffer.data(), GL_DYNAMIC_DRAW);
}

void DemoState::resetState()
{
    pressPos = {};
    releasePos = {};
    keyChanged = true;
    updateBackground();

    hatchAreaColors.buffer.clear();
    hatchAreaPoints.buffer.clear();
    searchedColors.buffer.clear();
    searchedPoints.buffer.clear();
}
void DemoState::updateTick()
{
    Point const& min = keyPressed ? cursorPos : pressPos;
    Point const& max = releasePos;

    hatchAreaColors.buffer.clear();
    hatchAreaPoints.buffer.clear();
    searchedColors.buffer.clear();
    searchedPoints.buffer.clear();

    search<Point>(min, max, backgroundPoints.buffer,
                            hatchAreaPoints.buffer,
                            searchedPoints.buffer);
    {
        std::size_t size = hatchAreaPoints.buffer.size();
        std::size_t i = 0;

        hatchAreaColors.buffer.resize(size);

        for (Color& color : hatchAreaColors.buffer)
        {
            color = Color{{ toRange(i++, 0, size, 0.1f, 1.0f), 0.0f, 0.0f }};
        }
    }
    {
        std::size_t size = searchedPoints.buffer.size();
        std::size_t i = 0;

        searchedColors.buffer.resize(size);

        for (Color& color : searchedColors.buffer)
        {
            color = Color{{ 0.0f, toRange(i++, 0, size, 0.1f, 1.0f), 0.0f }};
        }
    }
    updateHatchArea();
    updateSearched();

    keyChanged = false;
}
