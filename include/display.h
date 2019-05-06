#ifndef ENGINE_DISPLAY_H
#define ENGINE_DISPLAY_H

#include "windows.h"

namespace engine {

class Display {
public:
    Display(const HANDLE& window = GetStdHandle(STD_OUTPUT_HANDLE));
    ~Display();

    void render();
    void clear(char c);

    void resize(short width, short height);

private:
    HANDLE window_;              // output console
    short width_, height_;    // console size

    SMALL_RECT write_area_;      // render area
    CHAR_INFO* write_buffer_;    // chars to display

};

} // namespace engine

#endif
