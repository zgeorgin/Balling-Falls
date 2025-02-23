#include <iostream>
#include <scene.h>
#include <factories.h>
#include <loader.h>
#include <random>
#include <omp.h>
#include <chrono>
#include <yaml-cpp/yaml.h>
#include <opencv2/opencv.hpp>


const int scale = 1;
const char* SAVE_FILEPATH = "../data/test.txt";
const char* PIC_FILEPATH = "../img/image.jpg";
const char* CIRCLE_FILEPATH = "../img/circle.png";
bool saveFrame(const std::string& filename, SDL_Renderer* renderer, int width, int height) {
    // Выделяем память под пиксели
    std::vector<Uint8> pixels(width * height * 4); // 4 байта на пиксель (RGBA)

    // Считываем пиксели текущего рендера
    if (SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA32, pixels.data(), width * 4) != 0) {
        std::cerr << "Ошибка чтения пикселей: " << SDL_GetError() << std::endl;
        return false;
    }

    // Открываем файл для сохранения
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
        return false;
    }

    // Записываем данные кадра
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    file.close();
    return true;
}

bool loadFrame(const std::string& filename, SDL_Renderer* renderer, SDL_Texture** texture, int width, int height) {
    // Читаем raw-данные из файла
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Не удалось открыть файл для чтения: " << filename << std::endl;
        return false;
    }

    std::vector<Uint8> pixels(width * height * 4);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size());

    if (!*texture) {
        *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    }

    if (!texture) {
        std::cerr << "Ошибка создания текстуры: " << SDL_GetError() << std::endl;
        return false;
    }

    // Копируем данные в текстуру
    SDL_UpdateTexture(*texture, nullptr, pixels.data(), width * 4);
    return true;
}

void playFrames(SDL_Renderer* renderer, const std::string& framePathTemplate, int frameCount, int width, int height) {
    SDL_Texture* texture = nullptr;

    for (int i = 1; i <= frameCount; ++i) {
        std::string filename = framePathTemplate + std::to_string(i);

        // Загружаем текущий кадр
        if (!loadFrame(filename, renderer, &texture, width, height)) {
            std::cerr << "Ошибка загрузки кадра: " << filename << std::endl;
            continue;
        }

        // Очищаем экран и рисуем текстуру
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        // Ждём 1/30 секунды (33 миллисекунды)
        SDL_Delay(16);
    }
    SDL_Delay(5000);
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

int playVideo()
{
    YAML::Node config = YAML::LoadFile("../config.yaml");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    int width = config["Variables"]["WIDTH"].as<int>();
    int height = config["Variables"]["HEIGHT"].as<int>();
    SDL_Window* window = SDL_CreateWindow("Сохранение и воспроизведение кадров", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Ошибка создания окна: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Ошибка создания рендера: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    playFrames(renderer, "../frames/frame_", 2388, width, height);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int ball_test()
{
    YAML::Node config = YAML::LoadFile("../config.yaml");
    int WIDTH = config["Variables"]["WIDTH"].as<int>();
    int HEIGHT = config["Variables"]["HEIGHT"].as<int>();
    int NUM_THREADS = config["Variables"]["omp_num_threads"].as<int>();

    int GRID_X = config["Variables"]["GRID_X"].as<int>();
    int GRID_Y = config["Variables"]["GRID_Y"].as<int>();

    bool status = config["Options"]["status"].as<int>();
    //omp_set_num_threads(NUM_THREADS);

    SDL_Init(SDL_INIT_EVERYTHING);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Ошибка инициализации SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    SDL_Surface* circle_surf = IMG_Load(CIRCLE_FILEPATH);
    Screen s(WIDTH, HEIGHT, "TEST");
    BallCollideArena arena({}, 0, HEIGHT, 0, WIDTH, GRID_X, GRID_Y);
    std::vector<std::shared_ptr<BallActor>> actors;
    arena.delta = config["Variables"]["delta"].as<double>();
    arena.animationTicks = config["Variables"]["animation_ticks"].as<int>();
    Scene mainScene = Scene(&s, &arena);

    BallActorFactory fac;
    BallActorLoader ld(SAVE_FILEPATH);
    srand(time(NULL));

    cv::Mat image = cv::imread(PIC_FILEPATH, cv::IMREAD_COLOR);
    SDL_Event windowEvent;
    int mouse_x = HEIGHT + 10;
    int mouse_y = WIDTH + 10;
    double frame_time = 0;
    float radius = 20;
    int frameNumber = 0;
    uint8_t r = 0, g = 0, b = 0;
    float x = radius;
    float y = radius;
    std::shared_ptr<BallActor> cursor = nullptr;
    bool cursor_interaction = config["Options"]["cursor_interaction"].as<int>();
    /*if (cursor_interaction)
    {
        std::shared_ptr<Ball> ball = std::make_shared<Ball>(Ball(radius, radius, {mouse_x, mouse_y}, {mouse_x, mouse_y}, mainScene.getSize()));
        int32_t circle_x = int32_t(ball->position.x * scale);
        int32_t circle_y = int32_t(ball->position.y * scale);
        std::shared_ptr<Circle> image = std::make_shared<Circle>(circle_x, circle_y, ball->radius, true, r, g, b,255);
        cursor = std::make_shared<BallActor>(BallActor(ball, image, scale));
        mainScene.AddActor(cursor);
    }*/

    int order = 0;
    std::vector<Ball*> all_balls;
    while ( true )
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        SDL_Event windowEvent;
        SDL_PollEvent( &windowEvent );

        if ( SDL_QUIT == windowEvent.type )
        {
            break;
        }

        if (SDL_KEYDOWN == windowEvent.type)
        {
            if (SDLK_q == windowEvent.key.keysym.sym)
            {
                mainScene.save(SAVE_FILEPATH);
            }
        }

        if (cursor_interaction)
        {
            if (windowEvent.type == SDL_MOUSEMOTION) {
                mouse_x = windowEvent.motion.x ;
                mouse_y = windowEvent.motion.y ;
            }
        }
        
        if (!status)
        {
            if ((mainScene.getSize() < config["Variables"]["max_ball_count"].as<int>()) && (order % 2))
            {
                for (int stream = 0; stream < 2; stream++)
                {
                    int maxRadius = config["Variables"]["max_radius"].as<int>();
                    int minRadius = config["Variables"]["min_radius"].as<int>();
                    Position velocity = (stream == 0) ? Position({-2, 0}) : Position({2, 0});
                    int radius = rand()%(maxRadius - minRadius) + minRadius;
                    uint8_t r = rand()%255, g = rand()%255, b = rand()%255;
                    Position beginPos = (stream == 0) ? Position({radius, radius}) : Position({WIDTH - radius, radius});
                    Ball* ball = new Ball(float(radius), radius, {beginPos.x, beginPos.y}, {beginPos.x + velocity.x, beginPos.y + velocity.y}, mainScene.getSize());
                    all_balls.push_back(ball);
                    std::shared_ptr<Circle> image = std::make_shared<Circle>(Circle(radius, {ball->position.x * scale, ball->position.y * scale}, {r, g, b}, mainScene.screen->renderer, circle_surf));
                    std::shared_ptr<BallActor> a = std::make_shared<BallActor>(BallActor(ball, image, 1));
                    mainScene.AddActor(a);
                }

            }
            order = (order + 1) % 6;
            if ((mainScene.getSize() >= config["Variables"]["max_ball_count"].as<int>()))
            {
                //std::cout << "WE ARE GONE!\n";
                break;
                if (order == 5)
                    mainScene.save(SAVE_FILEPATH);
            }
            //saveFrame("../frames/frame_" + std::to_string(frameNumber), mainScene.screen->renderer, WIDTH, HEIGHT);
            frameNumber++;
        }

        else
        {
            if (ld.readyToLoad() && (order % 2))
            {
                for (int i = 0; i < 2; i++)
                {
                    std::shared_ptr<BallActor> a = ld.load(mainScene.screen->renderer, image, circle_surf);
                    all_balls.push_back(a->body);
                    mainScene.AddActor(a);
                }

            }
            order = (order + 1) % 6;
            saveFrame("../frames/frame_" + std::to_string(frameNumber), mainScene.screen->renderer, WIDTH, HEIGHT);
            frameNumber++;
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

    SDL_Quit();
    IMG_Quit();
    SDL_FreeSurface(circle_surf);
    for (int i = 0; i < all_balls.size(); i++)
        delete all_balls[i];
    mainScene.removeTextures();
    return EXIT_SUCCESS;
}

int test_circle()
{
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Инициализация SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Ошибка инициализации SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Создаём окно
    SDL_Window* window = SDL_CreateWindow("Texture Color Mod Example",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Ошибка создания окна: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Создаём рендерер
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Ошибка создания рендерера: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    std::shared_ptr<Circle> c = std::make_shared<Circle>(Circle(15, {100, 200}, {255, 255, 0}, renderer));
    /*// Загружаем текстуру
    SDL_Surface* surface = IMG_Load("../img/circle.png");  // Замените "example.png" на ваш файл
    if (!surface) {
        std::cerr << "Ошибка загрузки изображения: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Ошибка создания текстуры: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Меняем цвет текстуры (например, на красный оттенок)
    Uint8 red = 255;
    Uint8 green = 0;
    Uint8 blue = 0;
    SDL_SetTextureColorMod(texture, red, green, blue);
    */
    // Основной цикл программы
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Очищаем экран
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Чёрный цвет фона
        SDL_RenderClear(renderer);

        c->draw(renderer);
        // Обновляем экран
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
int main( int argc, char *argv[] )
{
    //return playVideo();
    return ball_test();
}