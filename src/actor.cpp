#include <actor.h>

void BallActor::UpdateLink()
{
    int32_t x = static_cast<int32_t>(body->position.x * scale);
    int32_t y = static_cast<int32_t>(body->position.y * scale);
    
    image->changePosition({x, y});
    //std::cout << x << ' ' << image->centreX << ' ' << scale << ' ' << body->position.x << std::endl;
}

void BallActor::Draw(SDL_Renderer* renderer)
{
    image->draw(renderer);
}

std::string BallActor::getSaveString()
{
    std::stringstream ss;
    ss << body->id << " " << image->getPosition().x << " " << image->getPosition().y <<  " " << image->getRadius() << " " << int(image->getColor().r) << " " << int(image->getColor().g) << " " << int(image->getColor().b) << " " << body->getBeginPosition().x << " " << body->getBeginPosition().y << " " << body->getBeginPrevPosition().x << " " << body->getBeginPrevPosition().y; 
    return ss.str();
}

BallActor::BallActor(std::string saveString, int scale)
{
    std::stringstream ss(saveString);
    //ss << body->id << " " << image->getPosition().x << " " << image->getPosition().y <<  " " << image->getRadius() << " " << image->getColor().r << " " << image->getColor().g << " " << image->getColor().b << " " << body->getBeginPosition().x << " " << body->getBeginPosition().y << " " << body->getBeginPrevPosition().x << " " << body->getBeginPrevPosition().y; 
}