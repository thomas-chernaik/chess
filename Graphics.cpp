//
// Created by thomas on 26/07/23.
//

#include "Graphics.h"

Graphics::Graphics(int width_, int height_) : width(width_), height(height_)
{
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    loadTextures();

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

//method to set the drawing colour
void Graphics::setColour(Colour colour)
{
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

}

//method to render a square (using 0-100 coords)
void Graphics::renderSquare(float xPosition, float yPosition, float xScale, float yScale, Colour colour)
{
    float fwidth = width;
    float fheight = height;
    //unNormalise the paramters
    xPosition *= fwidth / 100.;
    xScale *= fwidth / 100.;
    yPosition *= fheight / 100.;
    yScale *= fheight / 100.;

    //set the draw colour
    setColour(colour);
    //create the rectangle
    SDL_Rect rect(ceil(xPosition), ceil(yPosition), ceil(xScale), ceil(yScale));
    //draw the rectangle
    SDL_RenderFillRect(renderer, &rect);
}

//a method to update the display
void Graphics::display()
{
    SDL_RenderPresent(renderer);
}

//a method to render an empty chess board
void Graphics::renderBoard()
{
    Colour whiteColour = Colour(230, 175, 175);
    Colour blackColour = Colour(150, 50, 50);
    float anEighth = 12.5;
    //chess board is 8x8, so we need to render 64 squares in a grid.
    bool isWhite = false;
    for (float x = 0; x < 8; x++)
    {
        for (float y = 0; y < 8; y++)
        {
            if (isWhite)
                renderSquare(x * anEighth, y * anEighth, anEighth, anEighth, whiteColour);
            else
                renderSquare(x * anEighth, y * anEighth, anEighth, anEighth, blackColour);
            isWhite = !isWhite;
        }
        isWhite = !isWhite;
    }
    std::cout << anEighth;
};

//a method to get the square on the chess board from the mouse coord
std::pair<int, int> Graphics::getSquareFromMousePos(int mouseX, int mouseY)
{
    return std::make_pair(8 * mouseX / width, 8 * mouseY / height);
}

//a method to put a highlight on a square on the chess board
void Graphics::highlightSquare(int x, int y)
{
    renderTexture(x * 12.5, y * 12.5, 12.5, 12.5, highlight);
}

void Graphics::loadTextureToDict(std::string key, const char *fileLocation)
{
    SDL_Texture *text = IMG_LoadTexture(renderer, fileLocation);
    std::shared_ptr<SDL_Texture> textsmart(text, SDLTextureDeleter());
    pieces[key] = textsmart;
}

//a method to load textures needed
void Graphics::loadTextures()
{
    //load the highlight
    highlight = IMG_LoadTexture(renderer, "../textures/highlight.png");
    //load the pieces
    loadTextureToDict("blackbishop", "../textures/blackbishop.png");
    loadTextureToDict("blackknight", "../textures/blackknight.png");
    loadTextureToDict("blackpawn", "../textures/blackpawn.png");
    loadTextureToDict("blackqueen", "../textures/blackqueen.png");
    loadTextureToDict("blackrook", "../textures/blackrook.png");
    loadTextureToDict("blackking", "../textures/blackking.png");
    loadTextureToDict("whitebishop", "../textures/whitebishop.png");
    loadTextureToDict("whiteking", "../textures/whiteking.png");
    loadTextureToDict("whiteknight", "../textures/whiteknight.png");
    loadTextureToDict("whitepawn", "../textures/whitpawn.png");
    loadTextureToDict("whitequeen", "../textures/whitequeen.png");
    loadTextureToDict("whiterook", "../textures/whiterook.png");


}

void Graphics::unloadTextures()
{
    SDL_DestroyTexture(highlight);
}

void Graphics::renderTexture(float xPosition, float yPosition, float xScale, float yScale, SDL_Texture *texture)
{
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    float fwidth = width;
    float fheight = height;
    //unNormalise the paramters
    xPosition *= fwidth / 100.;
    xScale *= fwidth / 100.;
    yPosition *= fheight / 100.;
    yScale *= fheight / 100.;

    //create the rectangle
    SDL_Rect rect(ceil(xPosition), ceil(yPosition), ceil(xScale), ceil(yScale));
    //render the texture in the rectangle
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Graphics::updateSize(int w, int h)
{
    width = w;
    height = h;
    SDL_SetWindowSize(window, w, h);
    SDL_RenderClear(renderer);
}

void Graphics::renderGame(std::string *game)
{
    renderBoard();
    int count = 0;
    for (int w = 0; w < 8; w++)
    {
        for (int h = 0; h < 8; h++)
        {
            if (game[count] != "")
            {
                renderTexture(w * 12.5, h * 12.5, 12.5, 12.5, pieces[game[count]].get());
            }
            count++;
        }
    }
}