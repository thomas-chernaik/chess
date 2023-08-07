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
    auto whiteColour = Colour(230, 175, 175);
    auto blackColour = Colour(150, 50, 50);
    float anEighth = 12.5;
    //chess board is 8x8, so we need to render 64 squares in a grid.
    bool isWhite = true;
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
}

//a method to get the square on the chess board from the mouse coord
std::pair<int, int> Graphics::getSquareFromMousePos(int mouseX, int mouseY) const
{
    return std::make_pair(8 * mouseX / width, 8 * mouseY / height);
}

//a method to put a highlight on a square on the chess board
void Graphics::highlightSquare(int2 square)
{
    renderTexture(square.a * 12.5, square.b * 12.5, 12.5, 12.5, highlight);
}


//a method to add a texture to the dictionary as a smart pointer
void Graphics::loadTextureToDict(const std::string& key, const char *fileLocation)
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
    loadTextureToDict("whitepawn", "../textures/whitepawn.png");
    loadTextureToDict("whitequeen", "../textures/whitequeen.png");
    loadTextureToDict("whiterook", "../textures/whiterook.png");


}

//a method to destroy all the textures that aren't smart
void Graphics::unloadTextures()
{
    SDL_DestroyTexture(highlight);

}

//a method to put render a texture in a rect (takes location as %, not px)
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

//a method to update the size of the render window, for when the user changes the window size
void Graphics::updateSize(int w, int h)
{
    width = w;
    height = h;
    SDL_SetWindowSize(window, w, h);
    SDL_RenderClear(renderer);
}

//a method to render a game state represented in an array of strings, where each element corresponds to one square, from the top left to right top to bottom.
void Graphics::renderGame(const Gamestate::boardGrid& board)
{
    renderBoard();
    for (int h = 0; h < 8; h++)
    {
        for (int w = 0; w < 8; w++)
        {
            if (!board[h][w].pieceType.empty())
            {
                if(board[h][w].isWhite)
                {
                    renderTextureWithPadding(w * 12.5, h * 12.5, 12.5, 12.5, 2, 2, pieces["white"+board[h][w].pieceType].get());
                }
                else
                {
                    renderTextureWithPadding(w * 12.5, h * 12.5, 12.5, 12.5, 2, 2, pieces["black"+board[h][w].pieceType].get());
                }
            }
        }
    }
}

//another method to render a texture, now with padding! (in % not px)
void Graphics::renderTextureWithPadding(float xPosition, float yPosition, float xScale, float yScale, float xPadding,
                                        float yPadding, SDL_Texture *texture)
{
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    float fwidth = width;
    float fheight = height;
    //unNormalise the paramters
    xPosition *= fwidth / 100.;
    xScale *= fwidth / 100.;
    yPosition *= fheight / 100.;
    yScale *= fheight / 100.;
    xPadding *= fwidth / 100.;
    yPadding *= fheight / 100.;

    //create the rectangle
    SDL_Rect rect(ceil(xPosition + xPadding), ceil(yPosition + yPadding), ceil(xScale - 2 * xPadding),
                  ceil(yScale - 2 * yPadding));
    //render the texture in the rectangle
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Graphics::highlightSquares(const std::shared_ptr<int2[]>& highlighted, int size)
{
    if(highlighted == nullptr)
        return;
    for(int i=0; i<size; i++)
    {
        highlightSquare(highlighted[i]);
    }
}