Tower Defense - Build Instructions
==================================
Requirements: CMake 3.15+ and a C++17 compiler. All libraries (GLFW, GLAD, GLM, stb, miniaudio)
are included in third_party/ - nothing else needs to be installed. OpenGL 3.3 core.

macOS (Terminal, or open the folder in VS Code with the CMake Tools extension):
    xcode-select --install          (once, if you have no compiler)
    brew install cmake              (once)
    cmake -S . -B build
    cmake --build build
    ./build/TowerDefense

macOS with Xcode:
    cmake -S . -B build-xcode -G Xcode
    open build-xcode/TowerDefense.xcodeproj

Windows (Visual Studio 2022 or newer, "Desktop development with C++"):
    File > Open > Folder... and select this folder, then run TowerDefense.
  or from a terminal:
    cmake -S . -B build
    cmake --build build --config Release
    build\Release\TowerDefense.exe

Controls: Esc pauses/quits.
