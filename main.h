#pragma once
#include <vector>

void add_food_to_board();


int** create_squares();
void render_squares(SDL_Renderer* renderer, int** squares);
void render_snakeblock(SDL_Renderer* renderer, std::vector<std::vector<int>> snake, int index_of_snake_block);