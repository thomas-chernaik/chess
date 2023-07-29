#include <iostream>
#include "Graphics.h"
#define STARTSTATE =

int main()
{
    Graphics *g = new Graphics(800, 800);
    g->renderGame(g->startState);
    g->display();
    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    int x, y;
                    x = g->getSquareFromMousePos(mouseX, mouseY).first;
                    y = g->getSquareFromMousePos(mouseX, mouseY).second;
                    g->highlightSquare(x, y);
                    g->display();
                }
            } else if(event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    int newWidth = event.window.data1;
                    int newHeight = event.window.data2;
                    int newSize = fmin(newHeight, newWidth);
                    g->updateSize(newSize, newSize);
                    g->renderGame(g->startState);
                    g->display();
                }


            }
        }
    }
    return 0;
}
