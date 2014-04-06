#include "MortonDemo.hpp"

#ifdef _MSC_VER // THANKS OBAMA
  #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/** Dependencies:
    OpenGL 3.1 support
    GLFW 3.0.4 or >
    Glew 1.10 or >
    C++11
*/

int main()
{
    MortonDemo demo;
    demo.mainLoop();
}
