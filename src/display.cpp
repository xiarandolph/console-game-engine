#include <iostream>
#include "display.h"

namespace engine {

Display::Display(HANDLE window, HANDLE input)
 : window_(window), input_(input), write_buffer_(NULL) {
    if (window_ == INVALID_HANDLE_VALUE) {
        std::cerr << "error: invalid display window handle" << std::endl;
        exit(1);
    }

    SetConsoleMode(input_, 0);
    events_ = new INPUT_RECORD[128];

    // disable cursor from showing
    CONSOLE_CURSOR_INFO cci = {1, false};
    if (!SetConsoleCursorInfo(window_, &cci)) {
        std::cerr << "error: SetConsoleCursorInfo()" << std::endl;
        exit(1);
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(window_, &csbi) == 0) {
        std::cerr << "error: GetCSBI() failed" << std::endl;
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
    if (events_)
        delete [] events_;

    write_buffer_ = NULL;
    events_ = NULL;
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

bool Display::poll_event(INPUT_RECORD* event) {
    if (curr_event_num_ < num_events_) {
        *event = events_[curr_event_num_++];
        return true;
    }

    DWORD num_unread = 0;
    if (!GetNumberOfConsoleInputEvents(input_, &num_unread)) {
        std::cerr << "error: Display::poll_event "
            << "GetNumberOfConsoleInputEvents failed" << std::endl;
        exit(1);
    }

    if (num_unread == 0)
        return false;

    if (!ReadConsoleInput(input_, events_, 128, &num_events_)) {
        std::cerr << "error: Display::poll_event "
            << "ReadConsoleInput failed" << std::endl;
        exit(1);
    }
    curr_event_num_ = 0;
    *event = events_[curr_event_num_++];
    return true;
}

void Display::set_point(short x, short y, char c, WORD color) {
    if (y >= height_ || x >= width_) {
        std::cerr << "error: Display::set_point out of bounds ("
            << x << ", " << y << ")" << std::endl;
        return;
    }

    write_buffer_[y*width_ + x].Char.AsciiChar = c;
    write_buffer_[y*width_ + x].Attributes = color;
}

} // namespace engine
