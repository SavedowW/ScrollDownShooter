# ScrollDownShooter
Simple scroll-down shooter on C++/SDL2
## Building
To build, you need to have cmake installed on your system. Assuming that you are in the root directory, use:
```
mkdir build
cd build
cmake -A Win32 -DCMAKE_INSTALL_PREFIX=<SDL2 folder>..
cmake --build .
```
`<SDL2 folder>` is a folder that contains /include/ and /lib/x86/ with all `SDL2`, `SDL_image`, `SDL_mixer`, `SDL_ttf` and `SDL_gfx` stuff. It is not required if you already have SDL installed on your system.

`-A Win32` (probably different on unix) is required because `SDL_gfx` can be compiled only as x32 library. If i am wrong, feel free to use x64 version.
