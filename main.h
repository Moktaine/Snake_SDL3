#pragma once
#include <vector>

SDL_FRect** create_squares();
void render_squares(SDL_Renderer* renderer, SDL_FRect** squares);
void render_snakeblock(SDL_Renderer* renderer, SDL_FRect** squares, std::vector<std::vector<int>> snake, int i);