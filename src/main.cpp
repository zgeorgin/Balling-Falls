#include <iostream>
#include <scene.h>
#include <factories.h>
#include <random>
#include <omp.h>
#include <chrono>
#include <yaml-cpp/yaml.h>

const int scale = 1;


int ball_test()
{
    YAML::Node config = YAML::LoadFile("../config.yaml");

    int WIDTH = config["Variables"]["WIDTH"].as<int>();
    int HEIGHT = config["Variables"]["HEIGHT"].as<int>();
    int NUM_THREADS = config["Variables"]["omp_num_threads"].as<int>();

    int GRID_X = config["Variables"]["GRID_X"].as<int>();
    int GRID_Y = config["Variables"]["GRID_Y"].as<int>();
    omp_set_num_threads(NUM_THREADS);

    SDL_Init(SDL_INIT_EVERYTHING);
    Screen s(WIDTH, HEIGHT, "TEST");
    BallCollideArena arena({}, 0, HEIGHT, 0, WIDTH, GRID_X, GRID_Y);
    std::vector<std::shared_ptr<BallActor>> actors;
    arena.delta = config["Variables"]["delta"].as<double>();
    arena.animationTicks = config["Variables"]["animation_ticks"].as<int>();
    Scene mainScene = Scene(&s, &arena);

    BallActorFactory fac;

    srand(time(NULL));

    SDL_Event windowEvent;
    int mouse_x = HEIGHT + 10;
    int mouse_y = WIDTH + 10;
    double frame_time = 0;
    float radius = 20;
    uint8_t r = 0, g = 0, b = 0;
    float x = radius;
    float y = radius;
    std::shared_ptr<BallActor> cursor = nullptr;
    bool cursor_interaction = config["Options"]["cursor_interaction"].as<int>();
    if (cursor_interaction)
    {
        std::shared_ptr<Ball> ball = std::make_shared<Ball>(Ball(radius, radius, {mouse_x, mouse_y}, {mouse_x, mouse_y}, mainScene.getSize()));
        int32_t circle_x = int32_t(ball->position.x * scale);
        int32_t circle_y = int32_t(ball->position.y * scale);
        std::shared_ptr<Circle> image = std::make_shared<Circle>(circle_x, circle_y, ball->radius, true, r, g, b,255);
        cursor = std::make_shared<BallActor>(BallActor(ball, image, scale));
        mainScene.AddActor(cursor);
    }


    while ( true )
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        SDL_Event windowEvent;
        SDL_PollEvent( &windowEvent );

        if ( SDL_QUIT == windowEvent.type )
        {
            break;
        }

        if (cursor_interaction)
        {
            if (windowEvent.type == SDL_MOUSEMOTION) {
                mouse_x = windowEvent.motion.x ;
                mouse_y = windowEvent.motion.y ;
            }
        }

        if ((mainScene.getSize() < config["Variables"]["max_ball_count"].as<int>()))
        {
            int maxRadius = config["Variables"]["max_radius"].as<int>();
            int minRadius = config["Variables"]["min_radius"].as<int>();
            std::shared_ptr<BallActor> a = fac.BuildRandom(maxRadius, minRadius, true, 1, mainScene.getSize());
            mainScene.AddActor(a);
        }

        mainScene.UpdateScene();

        if (cursor_interaction)
        {
            cursor->body->position = {mouse_x, mouse_y};
            cursor->body->prev_position = {mouse_x, mouse_y};
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        if (1000.0 / frame_time > config["Variables"]["max_fps"].as<int>())
        {
            SDL_Delay(1000.0 / (1000.0 / frame_time - config["Variables"]["max_fps"].as<int>()));
        }
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