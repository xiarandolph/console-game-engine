#include <iostream>
#include "display.h"

namespace engine {

Display::Display(const HANDLE& window) : window_(window) {
    if (window_ == INVALID_HANDLE_VALUE) {
        std::cerr << "error: invalid display window handle" << std::endl;
        exit(1);
    }
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(window_, &csbi) == 0) {
        std::cerr << "error: get csbi failed" << std::endl;
        exit(1);
    }

    // add 1 because of rounding down
    width_ = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height_ = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    write_area_ = {0, 0, width_, height_};
    write_buffer_ = new CHAR_INFO[width_ * height_];
    
    // TODO: check if window size can be set to exactly fit the desired size
    //resize(width_, height_);
}

Display::~Display() {
    //free(write_buffer);
    if (write_buffer_)
        delete [] write_buffer_;
    write_buffer_ = NULL;
}

void Display::resize(short width, short height) {
    std::cerr << "Display::resize() DOESNT WORK, FIX OR DONT USE" << std::endl;
    exit(1);

    if (write_buffer_)
        delete [] write_buffer_;
    
    width_ = width;
    height_ = height;
    write_area_ = {0, 0, width_, height_};
    
    std::cout << width_ << " " << height_ << '\n';
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(window_, &csbi) == 0) {
        std::cerr << "error: get csbi failed" << std::endl;
        exit(1);
    }
    
    std::cerr << csbi.dwMaximumWindowSize.X  << " " << csbi.dwMaximumWindowSize.Y << '\n';
    

    if (!SetConsoleWindowInfo(window_, true, &write_area_)) {
        std::cerr << "error: resize() SetConsoleWindowInfo failed\n" << std::endl;
        exit(1);
    }

    //write_buffer_ = (CHAR_INFO*) calloc(width_ * height_, sizeof(CHAR_INFO*));
    write_buffer_ = new CHAR_INFO[width_ * height_];
}

void Display::render() {
    WriteConsoleOutput(window_, write_buffer_, {width_, height_}, {0, 0}, &write_area_);
}

void Display::clear(char c) {
    for (short i = 0; i < width_ * height_; i++) {
        write_buffer_[i].Char.AsciiChar = c;
        write_buffer_[i].Attributes = 0x0F;  // color
    }
}

} // namespace engine
