#include <opencv2/opencv.hpp>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
/*int main() {
    std::string imagePath = "../../processingPhoto/image.jpg";

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    cv::Mat grayImage;
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    cv::Mat downsampledImage;
    resize(image, downsampledImage, cv::Size(), 0.1, 0.1, cv::INTER_AREA);
    cv::imwrite("../gray_image.jpg", grayImage);
    cv::imwrite("../down_image.jpg", downsampledImage);
    if (image.empty()) {
        std::cerr << "Не удалось загрузить изображение по пути: " << imagePath << std::endl;
        return -1;
    }

    // Размеры изображения
    int width = downsampledImage.cols;  // Ширина
    int height = downsampledImage.rows; // Высота
    int channels = downsampledImage.channels(); // Количество каналов (3 для BGR)

    std::cout << "Размер изображения: " << width << "x" << height << ", Каналов: " << channels << std::endl;

    // Доступ к пикселям
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Получаем пиксель (в формате BGR)
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x); // Vec3b — вектор из 3 байтов

            // Вывод значений каналов
            uchar blue = pixel[0];
            uchar green = pixel[1];
            uchar red = pixel[2];

            // Пример: вывод первого пикселя (можно убрать для больших изображений)
            if (x == 0 && y == 0) {
                std::cout << "Пиксель (0,0): B=" << (int)blue 
                          << " G=" << (int)green 
                          << " R=" << (int)red << std::endl;
            }
        }
    }

    // Преобразование изображения в массив (если нужно работать напрямую)
    uchar* pixelArray = image.data; // Указатель на данные изображения
    size_t dataSize = width * height * channels; // Размер массива в байтах

    std::cout << "Массив пикселей размером " << dataSize << " байт успешно загружен." << std::endl;

    // Если необходимо, можно сохранить изображение
    // cv::imwrite("output.jpg", image);

    return 0;
}*/

int main(int argc, char* argv[]) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Инициализация SDL_image для работы с PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Ошибка инициализации SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow("PNG Scaler", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Ошибка создания окна: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Создание рендера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Ошибка создания рендера: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Загрузка изображения в текстуру
    const char* pngFile = "../../img/circle.png"; // Укажите путь к вашему PNG
    SDL_Surface* surface = IMG_Load(pngFile); // Загружаем PNG в поверхность
    if (!surface) {
        std::cerr << "Ошибка загрузки изображения: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Поверхность больше не нужна
    if (!texture) {
        std::cerr << "Ошибка создания текстуры: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Размеры для масштабирования
    int originalWidth, originalHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &originalWidth, &originalHeight);

    // Новые размеры для текстуры
    int targetWidth = 4;  // Ширина после сжатия
    int targetHeight = 4; // Высота после сжатия

    // Основной цикл программы
    bool running = true;
    SDL_Event event;

    while (running) {
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Очистка окна
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Прямоугольники для исходного изображения (целиком) и целевого масштаба
        SDL_Rect srcRect = {0, 0, originalWidth, originalHeight}; // Исходное изображение полностью
        SDL_Rect destRect = {200, 150, targetWidth, targetHeight}; // Отображение по центру окна

        // Рендеринг текстуры с масштабированием
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);

        // Обновление экрана
        SDL_RenderPresent(renderer);
    }

    // Очистка ресурсов
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}