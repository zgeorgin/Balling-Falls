#include <screen.h>

Screen::Screen(int width, int height, const char windowName[])
{
    window = SDL_CreateWindow( windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI );
    if ( NULL == window )
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer( window, -1, 0 );
    objects = {};
}

Screen::~Screen()
{
    SDL_DestroyWindow ( window );
}

void Screen::Draw()
{
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);
    /*for (int i = 0; i < objects.size(); i++)
    {
        std::cout << "10\n";
        //objects[i]->Draw(renderer);
        std::cout << "11\n";
    }*/
    SDL_RenderPresent(renderer);
}

SDL_Event Screen::CheckInput()
{
    SDL_Event windowEvent;
    SDL_PollEvent( &windowEvent );
    return windowEvent;
}