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
    omp_set_num_threads(NUM_THREADS);

    SDL_Init(SDL_INIT_EVERYTHING);
    Screen s(WIDTH, HEIGHT, "TEST");
    BallCollideArena arena({}, 0, HEIGHT, 0, WIDTH);
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
    while ( true )
    {
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
        if (mainScene.getSize() < config["Variables"]["max_ball_count"].as<int>())
        {
            int maxRadius = config["Variables"]["max_radius"].as<int>();
            int minRadius = config["Variables"]["min_radius"].as<int>();
            std::shared_ptr<BallActor> a = fac.BuildRandom(maxRadius, minRadius);
            mainScene.AddActor(a);
        }
        mainScene.UpdateScene();
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