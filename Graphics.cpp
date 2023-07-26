//
// Created by thomas on 26/07/23.
//

#include "Graphics.h"

Graphics::Graphics(int width_, int height_) : width(width_), height(height_)
{
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

}

Graphics::~Graphics()
{
    SDL_DestroyWindow(window);

    SDL_Quit();
}

//method to call the sdl delay function
void Graphics::sleep(int time)
{
    SDL_Delay(time);
}


//method to fill the screen with a color specified by the rgb value
void Graphics::colorScreen(Colour colour)
{
    setColour(colour);
    SDL_RenderClear(renderer);
}

void Graphics::setColour(Colour colour)
{
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

}

void Graphics::renderSquare(float xPosition, float yPosition, float xScale, float yScale, Colour colour)
{
    float fwidth = width;
    float fheight = height;
    //unNormalise the paramters
    xPosition *= fwidth/100.;
    xScale *= fwidth/100.;
    yPosition *= fheight / 100.;
    yScale *= fheight / 100.;

    //set the draw colour
    setColour(colour);
    //create the rectangle
    SDL_Rect rect(ceil(xPosition), ceil(yPosition), ceil(xScale), ceil(yScale));
    //draw the rectangle
    SDL_RenderFillRect(renderer, &rect);
}

void Graphics::display()
{
    SDL_RenderPresent(renderer);
}

void Graphics::renderBoard()
{
    Colour whiteColour = Colour(255, 200, 200);
    Colour blackColour = Colour(100, 0, 0);
    float anEighth = 12.5;
    //chess board is 8x8, so we need to render 64 squares in a grid.
    bool isWhite = false;
    for(float x=0; x < 8; x++)
    {
        for(float y=0; y<8; y++)
        {
            if(isWhite)
                renderSquare(x*anEighth, y*anEighth, anEighth, anEighth, whiteColour);
            else
                renderSquare(x*anEighth, y*anEighth, anEighth, anEighth, blackColour);
            isWhite = !isWhite;
        }
        isWhite = !isWhite;
    }
    std::cout<<anEighth;
};

std::pair<int, int> Graphics::getSquareFromMousePos(int mouseX, int mouseY)
{
    return std::make_pair(8*mouseX/width, 8*mouseY/height);
}

void Graphics::highlightSquare(int x, int y)
{
    renderSquare(x*12.5, y*12.5, 12.5, 12.5, Colour(0,180, 30, 50));
}