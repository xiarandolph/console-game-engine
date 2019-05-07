#include <iostream>
#include "display.h"

int main(int argc, char** argv) {
    engine::Display disp;

    INPUT_RECORD event;
    int x = 5, y = 5;

    // main loop
    const int TPS = 25; // ticks per second
    const int SKIP_TICKS = 1000 / TPS;
    const int MAX_FRAMESKIP = 5;
    
    DWORD next_game_tick = GetTickCount();
    int loops;
    float interpolation;

    bool running = true;
    // game loop with fixed game rate but varying fps (fps as fast as possible)
    while (running) {
        loops = 0;
        while (GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) {
            // game.update();
            
            while (disp.poll_event(&event)) {
                switch (event.EventType) {
                case KEY_EVENT:
                    if (event.Event.KeyEvent.bKeyDown) {
                        switch (event.Event.KeyEvent.uChar.AsciiChar) {
                        case 'w':
                            --y;
                            break;
                        case 'a':
                            --x;
                            break;
                        case 's':
                            ++y;
                            break;
                        case 'd':
                            ++x;
                            break;
                        }
                    }
                    break;
                }
            }
            
            next_game_tick += SKIP_TICKS;
            loops++;
        }
        
        // update game with interpolation
        //interpolation = float(GetTickCount() + SKIP_TICKS - next_game_tick) / SKIP_TICKS;
        //game.render(interpolation);

        disp.clear(' ');
        disp.set_point(x, y, '#');
        disp.render();
    }
    
    return 0;
}
