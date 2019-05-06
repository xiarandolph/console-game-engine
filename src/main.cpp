#include <iostream>
#include "display.h"

int main(int argc, char** argv) {
    engine::Display disp;
    
    disp.clear('a');
    disp.render();
    
    int a;
    std::cin >> a;
    
    return 0;
}
