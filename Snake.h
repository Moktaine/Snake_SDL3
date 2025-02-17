#pragma once
#include <vector>
#include <SDL3/SDL.h>
class snake
{
public:
	snake();
	void Tick();

	int* get_head_direction();
	std::vector<std::vector<int>> get_snake_blocks();
	void handle_key_event(SDL_Event& event);
	void change_head_direction(int* head_direction, const int* direction);
};

