#include <iostream>
#include "primitives.h"
#include "physics.h"
#include <random>
#include <omp.h>
const int WIDTH = 1200, HEIGHT = 900;

const int scale = 1;


int ball_test()
{
    int NUM_THREADS = 12;
    omp_set_num_threads(NUM_THREADS);

    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_Window *window = SDL_CreateWindow( "Bouncing balls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI );

    if ( NULL == window )
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    srand(time(NULL));
    std::shared_ptr<Ball> cursor_ball = std::make_shared<Ball>(Ball(20, 20, {HEIGHT, WIDTH}, {HEIGHT, WIDTH}));
    BallCollideArena a({cursor_ball}, 0, HEIGHT, 0, WIDTH);
    a.delta = 0.01;
    a.animationTicks = 8;
    std::vector<std::shared_ptr<Circle>> circles;
    for (int i = 1; i < a.objects.size(); i++)
    {
        int32_t x = int32_t(a.objects[i]->position.x * scale);
        int32_t y = int32_t(a.objects[i]->position.y * scale);
        circles.push_back(std::make_shared<Circle>(x, y, a.objects[i]->radius, true,255,255,255,255));
    }

    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, 0 );

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
    SDL_Event windowEvent;
    int mouse_x = HEIGHT + 10;
    int mouse_y = WIDTH + 10;
    while ( true )
    {
        if ( SDL_PollEvent( &windowEvent ))
        {
            if (windowEvent.type == SDL_MOUSEMOTION) {
                mouse_x = windowEvent.motion.x ;
                mouse_y = windowEvent.motion.y ;
            }

            if ( SDL_QUIT == windowEvent.type )
            {
                break;
            }
        }
        SDL_RenderClear( renderer );
        if (a.objects.size() < 1000)
        {
            double radius = rand()%20 + 5;
            uint8_t r = rand()%255, g = rand()%255, b = rand()%255;
            double x = radius;
            double y = radius;
            std::shared_ptr<Ball> ball = std::make_shared<Ball>(Ball(radius, radius, {x, y}, {x - 5, y}));
            a.objects.push_back(ball);
            int32_t circle_x = int32_t(ball->position.x * scale);
            int32_t circle_y = int32_t(ball->position.y * scale);
            circles.push_back(std::make_shared<Circle>(circle_x, circle_y, ball->radius, true, r, g, b,255));
        }
        
        for (int i = 1; i < a.objects.size(); i++)
        {
            int32_t x = int32_t(a.objects[i]->position.x * scale);
            int32_t y = int32_t(a.objects[i]->position.y * scale);
            circles[i - 1]->centreX = x;
            circles[i - 1]->centreY = y;
            circles[i - 1]->Draw(renderer);
        }
        SDL_RenderPresent( renderer );
        a.ApplyForces();
        a.objects[0]->position.x = mouse_x;
        a.objects[0]->prev_position.x = mouse_x;
        a.objects[0]->prev_position.y = mouse_y;
        a.objects[0]->position.y = mouse_y;
    }

    SDL_DestroyWindow ( window );
    SDL_Quit( );

    return EXIT_SUCCESS;
}

int main( int argc, char *argv[] )
{
    
    return ball_test();
}