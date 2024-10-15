#ifndef SCENE_H
#define SCENE_H

#include <actor.h>
#include <screen.h>
#include <arena.h>

using ArenaPtr = std::shared_ptr<Arena>;
using ScreenPtr = std::shared_ptr<Screen>;
using ActorPtr = std::shared_ptr<Actor>;

// The main abstraction that writes a picture on a screen
class Scene 
{
private:
    std::vector<ActorPtr> actors;
    Screen* screen;
    Arena* arena;
public:
    Scene(Screen* screen, Arena* arena) : screen(screen), arena(arena) {}
    void AddActor(ActorPtr actor);
    size_t getSize() { return actors.size(); }
    SDL_Event CheckInput() { return screen->CheckInput(); }
    void UpdateScene();
};

#endif