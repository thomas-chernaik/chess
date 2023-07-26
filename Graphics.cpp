//
// Created by thomas on 26/07/23.
//

#include "Graphics.h"

Graphics::Graphics(int width_, int height_) : width(width_), height(height_){
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

    SDL_Init(SDL_INIT_VIDEO);

}
Graphics::~Graphics() {
    SDL_DestroyWindow(window);

    SDL_Quit();
}

//method to call the sdl delay function
void Graphics::sleep(int time) {
    SDL_Delay(time);
}


//method to fill the screen with a color specified by the rgb value
void Graphics::colorScreen(Colour colour) {
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}