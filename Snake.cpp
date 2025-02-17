
#include "snake.h"
#include <SDL3/SDL.h>
#include "main.h"


const int direction_up[2] = { 0,-1 };
const int direction_down[2] = { 0,1 };
const int direction_left[2] = { -1,0 };
const int direction_right[2] = { 1,0 };


int* head_direction;
std::vector<std::vector<int>> snake_blocks;


snake::snake()
{
	head_direction = new int[2] {-1,0};
	snake_blocks = std::vector<std::vector<int>>(3);
	snake_blocks[0] = { 12 , 0 };
	snake_blocks[1] = { 11 , 0 };
	snake_blocks[2] = { 10 , 0 };

}



void snake::Tick()
{
	//Snake Logic
	for (int i = 0; i < snake_blocks.size(); i++) {

		if (i == snake_blocks.size() - 1) {
			snake_blocks[i][0] += head_direction[0];
			snake_blocks[i][1] += head_direction[1];
		}
		else {
			int block_direction[2] = { snake_blocks[i + 1][0] - snake_blocks[i][0], snake_blocks[i + 1][1] - snake_blocks[i][1] };
			snake_blocks[i][0] += block_direction[0];
			snake_blocks[i][1] += block_direction[1];
		}
	}
	
	
}



std::vector<std::vector<int>> snake::get_snake_blocks() {
	return snake_blocks;
}

int* snake::get_head_direction() {
	return head_direction;
}


void snake::change_head_direction(int* head_direction, const int* direction) {
	if (head_direction[0] == -direction[0] && head_direction[1] == -direction[1]) {
		return;
	}
	head_direction[0] = direction[0];
	head_direction[1] = direction[1];
}

void snake::add_snake_block() {
	int direction[2] = { snake_blocks[0][0] - snake_blocks[1][0], snake_blocks[0][1] - snake_blocks[1][1] };
	std::vector<int> new_block = { snake_blocks[0][0] - direction[0], snake_blocks[0][1] - direction[1] };
	snake_blocks.insert(snake_blocks.begin(), new_block);
}


void snake::handle_key_event(SDL_Event& event) {
	SDL_assert(event.type == SDL_EVENT_KEY_DOWN);

	switch (event.key.scancode) {
	case SDL_SCANCODE_UP:
		change_head_direction(head_direction, direction_up);
		break;
	case SDL_SCANCODE_DOWN:
		change_head_direction(head_direction, direction_down);
		break;
	case SDL_SCANCODE_LEFT:
		change_head_direction(head_direction, direction_left);
		break;
	case SDL_SCANCODE_RIGHT:
		change_head_direction(head_direction, direction_right);
		break;
	case SDL_SCANCODE_SPACE:
		add_snake_block();
		break;

	}
}