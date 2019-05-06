#include <iostream>
#include "display.h"

namespace engine {

Display::Display(HANDLE window) : window_(window), write_buffer_(NULL) {
    if (window_ == INVALID_HANDLE_VALUE) {
        std::cerr << "error: invalid display window handle" << std::endl;
        exit(1);
    }
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(window_, &csbi) == 0) {
        std::cerr << "error: get csbi failed" << std::endl;
        exit(1);
    }

    // add 1 for rounding down, another 1 to width for the scroll bar
    width_ = csbi.srWindow.Right - csbi.srWindow.Left + 2;
    height_ = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    // disable scrolling by setting the height
    csbi.dwSize.X = width_;
    csbi.dwSize.Y = height_;
    SetConsoleScreenBufferSize(window_, csbi.dwSize);

    // disable horizontal scroll bar
    ++csbi.srWindow.Right;
    SetConsoleWindowInfo(window_, true, &csbi.srWindow);

    write_area_ = {0, 0, width_, height_};
    write_buffer_ = new CHAR_INFO[width_ * height_];
}

Display::~Display() {
    //free(write_buffer);
    if (write_buffer_)
        delete [] write_buffer_;
    write_buffer_ = NULL;
}

void Display::render() {
    WriteConsoleOutput(window_, write_buffer_, {width_, height_}, {0, 0}, &write_area_);
}

void Display::clear(char c, WORD color) {
    for (short i = 0; i < width_ * height_; i++) {
        write_buffer_[i].Char.AsciiChar = c;
        write_buffer_[i].Attributes = color;
    }
}

} // namespace engine
