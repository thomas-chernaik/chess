#include <iostream>
#include "Graphics.h"

int main()
{
    Graphics *g = new Graphics(800, 800);
    g->renderBoard();
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
                    std::cout << mouseX << " " << mouseY << "\n";
                    int x,y;
                    x = g->getSquareFromMousePos(mouseX, mouseY).first;
                    y = g->getSquareFromMousePos(mouseX, mouseY).second;
                    g->highlightSquare(x,y);
                    g->display();
                }
            }
        }
    }
    return 0;
}
