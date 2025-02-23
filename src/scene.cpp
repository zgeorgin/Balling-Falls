#include <scene.h>
#include <iostream>

void Scene::AddActor(ActorPtr actor)
{
    actors.push_back(actor);
    arena->AddObject(actor->body);
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

void Scene::save(const char* filepath)
{
    std::ofstream out;
    out.open(filepath);
    if (!out.is_open())
    {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }

    for (ActorPtr actor : actors)
        out << actor->getSaveString() << std::endl;
}