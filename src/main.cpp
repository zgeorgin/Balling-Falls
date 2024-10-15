#include <iostream>
#include <scene.h>
#include <factories.h>
#include <random>
#include <omp.h>
#include <chrono>
const int WIDTH = 1200, HEIGHT = 900;

const int scale = 1;


int ball_test()
{
    int wait;
    int NUM_THREADS = 12;
    omp_set_num_threads(NUM_THREADS);
    SDL_Init(SDL_INIT_EVERYTHING);
    //SDL_Window *window = SDL_CreateWindow( "Bouncing balls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI );
    //SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, 0 );
    //std::shared_ptr<Screen> s = std::make_shared<Screen>(Screen(WIDTH, HEIGHT, "TEST"));
    Screen s(WIDTH, HEIGHT, "TEST");
    //SDL_Window* window = screen->window;
    //std::shared_ptr<BallCollideArena> arena = std::make_shared<BallCollideArena>(BallCollideArena({}, 0, HEIGHT, 0, WIDTH));
    BallCollideArena arena({}, 0, HEIGHT, 0, WIDTH);
    std::vector<std::shared_ptr<BallActor>> actors;
    //std::cin >> wait;
    arena.delta = 0.01;
    arena.animationTicks = 8;
    Scene mainScene = Scene(&s, &arena);

    BallActorFactory fac;
    srand(time(NULL));
    //std::shared_ptr<Ball> cursor_ball = std::make_shared<Ball>(Ball(20, 20, {HEIGHT, WIDTH}, {HEIGHT, WIDTH}));
    //BallCollideArena a({cursor_ball}, 0, HEIGHT, 0, WIDTH);
    //SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, 0 );

    SDL_Event windowEvent;
    int mouse_x = HEIGHT + 10;
    int mouse_y = WIDTH + 10;
    double frame_time = 0;
    while ( true )
    {
        //std::cin >> wait;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        windowEvent = mainScene.CheckInput();
        if (windowEvent.type == SDL_MOUSEMOTION) {
            mouse_x = windowEvent.motion.x ;
            mouse_y = windowEvent.motion.y ;
        }

        if ( SDL_QUIT == windowEvent.type )
        {
            break;
        }

        if (mainScene.getSize() < 100)
        {
            int maxRadius = 25, minRadius = 10;
            int radius = rand()%(maxRadius - minRadius) + minRadius;
            uint8_t r = rand()%255, g = rand()%255, b = rand()%255;
            double x = radius;
            double y = radius;
            std::shared_ptr<Ball> ball = std::make_shared<Ball>(Ball(radius, radius, {x, y}, {x - 5, y}));
            int32_t circle_x = int32_t(ball->position.x * scale);
            int32_t circle_y = int32_t(ball->position.y * scale);
            std::shared_ptr<Circle> image = std::make_shared<Circle>(circle_x, circle_y, ball->radius, true, r, g, b,255);
            std::shared_ptr<BallActor> a = std::make_shared<BallActor>(ball, image, scale);
            //std::cout << (int)a->image->r << " Trying to fix this shit\n";
            //arena.AddObject(ball);
            //actors.push_back(a);
            mainScene.AddActor(a);
        }
        /*arena.ApplyForces();
        SDL_SetRenderDrawColor(s.renderer, 0, 0, 0, 255 );
        SDL_RenderClear(s.renderer);
        //std::cout << "Проверка: " << actors[0]->body->position.x << ' ' << actors[0]->image->centreX << ' ' << actors[0]->scale <<  std::endl;
        for (int i = 0; i < actors.size(); i++)
        {
            actors[i]->UpdateLink();
            //actors[i]->image->centreX = actors[i]->body->position.x;
            //actors[i]->image->centreY = actors[i]->body->position.y;
            actors[i]->Draw(s.renderer);
        }
        SDL_RenderPresent(s.renderer);*/
        mainScene.UpdateScene();
        //std::cout << "5\n";
        //a.objects[0]->position.x = mouse_x;
        //a.objects[0]->prev_position.x = mouse_x;
        //a.objects[0]->prev_position.y = mouse_y;
        //a.objects[0]->position.y = mouse_y;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }

    std::cout << "LAST FRAME TIME: " << frame_time << " ms\n";
    std::cout << "LAST FPS: " << 1000.0 / frame_time << '\n';

    SDL_Quit( );

    return EXIT_SUCCESS;
}

int main( int argc, char *argv[] )
{
    return ball_test();
}