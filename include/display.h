#ifndef ENGINE_DISPLAY_H
#define ENGINE_DISPLAY_H

#include "windows.h"

namespace engine {

class Display {
public:
    Display(HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE));
    ~Display();

    void render();
    void clear(char c) {clear(c, color_);}
    void clear(char c, WORD color);

private:
    HANDLE window_;             // output console
    short width_, height_;      // console size

    SMALL_RECT write_area_;     // render area
    CHAR_INFO* write_buffer_;   // chars to display
    
    WORD color_ = 0x07;         // default text color
};

} // namespace engine

#endif
