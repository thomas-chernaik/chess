//
// Created by thomas on 26/07/23.
//

#ifndef CHESS_GRAPHICS_H
#define CHESS_GRAPHICS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <memory>
#include "Gamestate.h"

struct Colour
{
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
};

class SDLTextureDeleter
{
public:
    void operator()(SDL_Texture *texture) const
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }
};


class Graphics
{
public:
    std::string startState[64] =
            {"blackrook", "blackknight", "blackbishop", "blackqueen", "blackking", "blackbishop", "blackknight",
             "blackrook",
             "blackpawn", "blackpawn", "blackpawn", "blackpawn", "blackpawn", "blackpawn", "blackpawn", "blackpawn",
             "", "", "", "", "", "", "", "",
             "", "", "", "", "", "", "", "",
             "", "", "", "", "", "", "", "",
             "", "", "", "", "", "", "", "",
             "whitepawn", "whitepawn", "whitepawn", "whitepawn", "whitepawn", "whitepawn", "whitepawn", "whitepawn",
             "whiterook", "whiteknight", "whitebishop", "whiteking", "whitequeen", "whitebishop", "whiteknight",
             "whiterook"
            };


    Graphics(int width_, int height_);

    ~Graphics();

    static void sleep(int time);

    void colorScreen(Colour colour);

    void renderSquare(float xPosition, float yPosition, float xScale, float yScale, Colour colour);

    void renderTexture(float xPosition, float yPosition, float xScale, float yScale, SDL_Texture *texture);

    void renderTextureWithPadding(float xPosition, float yPosition, float xScale, float yScale, float xPadding,
                                  float yPadding, SDL_Texture *texture);

    void display();

    void renderGame(Gamestate::boardGrid board);

    void renderBoard();

    void highlightSquare(int x, int y);

    void updateSize(int w, int h);

    std::pair<int, int> getSquareFromMousePos(int mouseX, int mouseY);

protected:
    SDL_Window *window{};
    SDL_Renderer *renderer{};

    void setColour(Colour colour);

    void loadTextures();

    void unloadTextures();

    SDL_Texture *highlight;
    std::map<std::string, std::shared_ptr<SDL_Texture>> pieces;

    int height;
    int width;

    void loadTextureToDict(std::string key, const char *fileLocation);

};


#endif //CHESS_GRAPHICS_H
