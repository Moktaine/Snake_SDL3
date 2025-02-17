#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "main.h"
#include "snake.h"

const int direction_up[2] = { 0,-1 };
const int direction_down[2] = { 0,1 };
const int direction_left[2] = { -1,0 };
const int direction_right[2] = { 1,0 };

const int square_count = 15;
int window_width = 600, window_length = 600;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;


SDL_FRect** squares;
snake snake_obj;


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Snake", window_width, window_length, SDL_WINDOW_OPENGL, &window, &renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window and renderer: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	snake_obj = snake();
	squares = create_squares();
	return SDL_APP_CONTINUE;

}


SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}
	else if (event->type == SDL_EVENT_KEY_DOWN) {
		snake_obj.handle_key_event(*event);
	}
	
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}


SDL_AppResult SDL_AppIterate(void* appstate)
{

	SDL_SetRenderDrawColor(renderer, 59, 161, 86, 0);
	SDL_RenderClear(renderer);


	render_squares(renderer, squares);

	snake_obj.Tick();
	std::vector<std::vector<int>> snake_blocks = snake_obj.get_snake_blocks();
	int* head_direction = snake_obj.get_head_direction();


	std::vector<int> head = snake_blocks[snake_blocks.size() - 1];
	if (head[0] < 0 || head[0] >= square_count || head[1] < 0 || head[1] >= square_count) {
		return SDL_APP_SUCCESS;
	}

	for (int i = 0; i < snake_blocks.size(); i++) {
		render_snakeblock(renderer, squares, snake_blocks, i);
	}
	

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}



void SDL_AppQuit(void* appstate, SDL_AppResult result) {

}



static SDL_FRect** create_squares() {
	SDL_FRect** squares = new SDL_FRect*[square_count];
	float square_width = 35.0f;
	float start = (window_width - square_width * square_count) / 2;
	for (int i = 0; i < square_count; i++) {
		squares[i] = new SDL_FRect[square_count];

		for (int j = 0; j < square_count; j++) {
			squares[i][j] = {
				start + square_width *i,
				start + square_width *j,
				square_width,
				square_width
			};
		}
	}
	
	return squares;
}

void render_squares(SDL_Renderer* renderer, SDL_FRect** squares) {
	SDL_Color color1 = { 79, 189, 108, 255 };
	SDL_Color color2 = { 97, 194, 122, 255 };
	bool choose_color1 = true;
	for (int i = 0; i < square_count; i++) {
		for (int j = 0; j < square_count; j++) {
			if (choose_color1) {
				SDL_SetRenderDrawColor(renderer, color1.r, color1.g, color1.b, color1.a);
			}
			else {
				SDL_SetRenderDrawColor(renderer, color2.r, color2.g, color2.b, color2.a);
			}
			
			SDL_RenderFillRect(renderer, &squares[i][j]);
			choose_color1 = !choose_color1;
		}
	}
}

void render_snakeblock(SDL_Renderer* renderer, SDL_FRect** squares, std::vector<std::vector<int>> snake, int i) {
	float width = squares[0][0].w;
	float height = squares[0][0].h;
	float x = squares[snake[i][0]][snake[i][1]].x;
	float y = squares[snake[i][0]][snake[i][1]].y;

	SDL_FRect rect = { x, y, width, height };


	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderFillRect(renderer, &rect);
}



