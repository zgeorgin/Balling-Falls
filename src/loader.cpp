#include <loader.h>
BallActorLoader::BallActorLoader(const char* filepath)
{
    in = std::ifstream(filepath);
    if (!in.is_open()) {
        std::cerr << "Файл не существует или не может быть открыт.\n";
    }
}

std::shared_ptr<BallActor> BallActorLoader::load(SDL_Renderer* renderer, cv::Mat img, SDL_Surface* surf)
{
    int id, radius;
    Position beginPos = {0, 0}, beginPrevPos = {0, 0}, endPos = {0, 0};
    RGB color = {0, 0, 0};
    int r, g, b;
    in >> id >> endPos.x >> endPos.y >> radius >> r >> g >> b >> beginPos.x >> beginPos.y >> beginPrevPos.x >> beginPrevPos.y;
    cv::Vec3b pixel = img.at<cv::Vec3b>(endPos.y, endPos.x); // Vec3b — вектор из 3 байтов

    // Вывод значений каналов
    uchar blue = pixel[0];
    uchar green = pixel[1];
    uchar red = pixel[2];

    Ball* ball = new Ball(float(radius), radius, {beginPos.x, beginPos.y}, {beginPrevPos.x, beginPrevPos.y}, id);
    std::shared_ptr<Circle> image = std::make_shared<Circle>(Circle(radius, {beginPos.x, beginPos.y}, {red, green, blue}, renderer, surf));

    return std::make_shared<BallActor>(BallActor(ball, image, 1));
}