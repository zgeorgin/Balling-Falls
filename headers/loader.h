#pragma once
#include <actor.h>
#include <fstream>
#include <opencv2/opencv.hpp>
class BallActorLoader
{
private:
    std::ifstream in;
public:
    BallActorLoader(const char* filename);
    bool readyToLoad() {return !in.eof();}
    std::shared_ptr<BallActor> load(SDL_Renderer* renderer, cv::Mat image, SDL_Texture* texture);
};