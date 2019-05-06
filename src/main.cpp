#include <iostream>
#include "display.h"

int main(int argc, char** argv) {
    engine::Display disp;

    INPUT_RECORD event;
    int x = 5, y = 5;

    // main loop
    while (true) {
        while (disp.poll_event(&event)) {
            switch (event.EventType) {
            case KEY_EVENT:
                x++;
                break;
            }
        }
        disp.clear(' ');
        disp.set_point(x, y, '#');
        disp.render();
    }
    
    return 0;
}
