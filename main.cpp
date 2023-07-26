#include <iostream>
#include "Graphics.h"
int main() {
    Graphics *g = new Graphics(500, 500);
    g->colorScreen(Colour(255, 200, 200));
    g->sleep(3000);
    g->colorScreen(Colour(0, 100, 100));
    g->sleep(3000);
    return 0;
}
