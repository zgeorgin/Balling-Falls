#include <scene.h>
#include <iostream>
void Scene::AddActor(ActorPtr actor)
{
    actors.push_back(actor);
    arena->AddObject(actor->body);
    screen->AddObject(actor->image);
}

void Scene::UpdateScene()
{
    arena->ApplyForces();
    //std::cout << "FORCES APPLIED!\n";
    SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255 );
    SDL_RenderClear(screen->renderer);
    for (int i = 0; i < actors.size(); i++)
    {
        actors[i]->UpdateLink();
        actors[i]->Draw(screen->renderer);
    }
    SDL_RenderPresent(screen->renderer);
    //screen->Draw();
}