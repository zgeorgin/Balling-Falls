#include "primitives.h"

void Circle::DrawCircle(SDL_Renderer* renderer)
{
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);
    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void Circle::FillCircle(SDL_Renderer* renderer)
{
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);
    while (x >= y)
    {
        SDL_RenderDrawLine(renderer, centreX - y, centreY + x,
                           centreX + y, centreY + x);
        SDL_RenderDrawLine(renderer, centreX - x, centreY + y,
                           centreX + x, centreY + y);
        SDL_RenderDrawLine(renderer, centreX - x, centreY - y,
                           centreX + x, centreY - y);
        SDL_RenderDrawLine(renderer, centreX - y, centreY - x,
                           centreX + y, centreY - x);
        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void Circle::Draw(SDL_Renderer* renderer)
{
    Uint8 tr, tg, tb, ta;
    SDL_GetRenderDrawColor(renderer, &tr, &tg, &tb, &ta);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    if (painted)
        FillCircle(renderer);

    DrawCircle(renderer);

    SDL_SetRenderDrawColor(renderer, tr, tg, tb, ta);
}

void Rect::DrawRect(SDL_Renderer* renderer)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y1);
    SDL_RenderDrawLine(renderer, x1, y1, x1, y2);
    SDL_RenderDrawLine(renderer, x2, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x1, y2, x2, y2);
}

void Rect::FillRect(SDL_Renderer* renderer)
{
    for (int c = x1; c <= x2; c++)
    {
        SDL_RenderDrawLine(renderer, c, y1, c, y2);
    }
}

void Rect::Draw(SDL_Renderer* renderer)
{
    Uint8 tr, tg, tb, ta;
    SDL_GetRenderDrawColor(renderer, &tr, &tg, &tb, &ta);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    if (painted)
        FillRect(renderer);
    else
        DrawRect(renderer);

    SDL_SetRenderDrawColor(renderer, tr, tg, tb, ta);
}

void Triangle::DrawTriangle(SDL_Renderer* renderer)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x1, y1, x3, y3);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
}

void Triangle::Draw(SDL_Renderer* renderer)
{
    Uint8 tr, tg, tb, ta;
    SDL_GetRenderDrawColor(renderer, &tr, &tg, &tb, &ta);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    DrawTriangle(renderer);

    SDL_SetRenderDrawColor(renderer, tr, tg, tb, ta);
}

void Grid::Draw(SDL_Renderer* renderer, int height, int width)
{
    Uint8 tr, tg, tb, ta;
    SDL_GetRenderDrawColor(renderer, &tr, &tg, &tb, &ta);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    for (int x = 0; x < width; x += offset)
    {
        SDL_RenderDrawLine(renderer, x, 0, x, height/offset * offset);
    }
    for (int y = 0; y < height; y += offset)
    {
        SDL_RenderDrawLine(renderer, 0, y, width/offset * offset, y);
    }

    SDL_SetRenderDrawColor(renderer, tr, tg, tb, ta);
}