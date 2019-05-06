#ifndef ENGINE_DISPLAY_H
#define ENGINE_DISPLAY_H

#include "windows.h"

namespace engine {

class Display {
public:
    Display(HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE),
            HANDLE input = GetStdHandle(STD_INPUT_HANDLE));
    ~Display();

    void render();
    void clear(char c) {clear(c, color_);}
    void clear(char c, WORD color);

    void set_point(short x, short y, char c) {set_point(x, y, c, color_);}
    void set_point(short x, short y, char c, WORD color);

    bool poll_event(INPUT_RECORD* event);

private:
    HANDLE window_;             // output console
    HANDLE input_;              // input console
    short width_, height_;      // console size

    SMALL_RECT write_area_;     // render area
    CHAR_INFO* write_buffer_;   // chars to display
    INPUT_RECORD* events_;      // array of events to be polled
    DWORD num_events_ = 0;      // number of events in the array
    DWORD curr_event_num_ = 0;  // num events polled already
    
    WORD color_ = 0x07;         // default text color
};

} // namespace engine

#endif
