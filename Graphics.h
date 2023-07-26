//
// Created by thomas on 26/07/23.
//

#ifndef CHESS_GRAPHICS_H
#define CHESS_GRAPHICS_H
#include <iostream>
#include <SDL2/SDL.h>

struct Colour
{
    int r;
    int g;
    int b;
    int a=255;
};


class Graphics {
public:
    Graphics(int width_, int height_);
    ~Graphics();
    void sleep(int time);
    void colorScreen(Colour colour);
    void renderSquare(float xPosition, float yPosition, float xScale, float yScale);

protected:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int height;
    int width;

};


#endif //CHESS_GRAPHICS_H
