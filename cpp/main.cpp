#include "MortonDemo.hpp"

#ifdef _MSC_VER // THANKS OBAMA
  #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/** Dependencies:
    OpenGL 3.1 support
    GLFW 3.0.4 or >
    Glew 1.10 or >
    C++11

    Current bugs:
    Depending on the platform vertical bars appear
        when the window is resized horizontally? mystery?
    General fixed width discrepancies between 32 / 64 bit architectures.

    Enhancements:
    Fixed resource management not being exception safe by using RAII.
    Optimized CPU hotspots #2 and #3 by elminating unwanted copies.

    Notes:
    Performance seems to be bound by CPU -> GPU transfer rates.
    Preliminary results show bigalg<>() being slower than search<>().
*/

int main()
{
    MortonDemo demo;
    demo.mainLoop();
}
