#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3_Image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include "main.h"
#include "snake.h"

const int direction_up[2] = { 0,-1 };
const int direction_down[2] = { 0,1 };
const int direction_left[2] = { -1,0 };
const int direction_right[2] = { 1,0 };

int window_width = 600, window_length = 600;
const int square_count = 15;
float square_width = 35.0f;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;


int** squares;
snake snake_obj;
std::vector<std::vector<int>> snake_arr;
std::vector<int> head;


long long timer_start_time = 1000;
float game_speed = 1;


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!SDL_CreateWindowAndRenderer("Snake", window_width, window_length, 0, &window, &renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window and renderer: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetPointerProperty(SDL_GetRendererProperties(renderer), SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
	SDL_SetNumberProperty(SDL_GetRendererProperties(renderer), SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER, SDL_COLORSPACE_SRGB_LINEAR);


	snake_obj = snake();
	squares = create_squares();
	add_food_to_board();
	add_food_to_board();
	add_food_to_board();

	return SDL_APP_CONTINUE;

}


SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS; 
	}
	else if (event->type == SDL_EVENT_KEY_DOWN) {
		snake_obj.handle_key_event(*event);
	}
	
	return SDL_APP_CONTINUE;  
}


SDL_AppResult SDL_AppIterate(void* appstate)
{

	SDL_SetRenderDrawColor(renderer, 59, 161, 86, 0);
	SDL_RenderClear(renderer);

	render_squares(renderer, squares);
	for (int i = 0; i < snake_arr.size(); i++) {
		squares[snake_arr[i][0]][snake_arr[i][1]] = -1;
	}
	
	if (SDL_GetTicks() - timer_start_time > 250 / game_speed) {
		snake_obj.Tick();


		snake_arr = snake_obj.get_snake_blocks();
		head = snake_arr[snake_arr.size() - 1];

		//Check if snake is out of bounds
		if (head[0] < 0 || head[0] >= square_count || head[1] < 0 || head[1] >= square_count) {
			return SDL_APP_SUCCESS;
		}

		for (int i = 0; i < snake_arr.size(); i++) {

			//Check if snake has collided with itself
			if (i != snake_arr.size() - 1 && head[0] == snake_arr[i][0] && head[1] == snake_arr[i][1]) {
				return SDL_APP_SUCCESS;
			}
		}


		if (squares[head[0]][head[1]] == 1) {
			snake_obj.add_snake_block();
			squares[head[0]][head[1]] = 0;
			add_food_to_board();

			if (250 / game_speed > 50) {
				game_speed += game_speed / 50;
			}
		}

		timer_start_time = SDL_GetTicks();

	}


	for (int i = 0; i < snake_arr.size(); i++) {
		render_snakeblock(renderer, snake_arr, i);
	}
	
	
	SDL_RenderPresent(renderer);

	

	return SDL_APP_CONTINUE;  
}



void SDL_AppQuit(void* appstate, SDL_AppResult result) {

}



int** create_squares() {
	int** squares = new int*[square_count];
	
	for (int i = 0; i < square_count; i++) {
		squares[i] = new int[square_count];

		for (int j = 0; j < square_count; j++) {
			squares[i][j] = {0};
		}
	}
	
	return squares;
}

void render_squares(SDL_Renderer* renderer, int** squares) {
	SDL_Color color1 = { 79, 189, 108, 255 };
	SDL_Color color2 = { 97, 194, 122, 255 };
	bool choose_color1 = true;

	float initial_starting_position = (window_width - square_width * square_count) / 2;
	for (int i = 0; i < square_count; i++) {
		for (int j = 0; j < square_count; j++) {
			
			if (choose_color1) {
				SDL_SetRenderDrawColor(renderer, color1.r, color1.g, color1.b, color1.a);
			}
			else {
				SDL_SetRenderDrawColor(renderer, color2.r, color2.g, color2.b, color2.a);
			}
			
			SDL_FRect square = {
				initial_starting_position + square_width * i,
				initial_starting_position + square_width * j,
				square_width,
				square_width
			};
			
			SDL_RenderFillRect(renderer, &square);
			choose_color1 = !choose_color1;

			if (squares[i][j] == 1) {
				SDL_Texture* texture = IMG_LoadTexture(renderer, "Sprites/apple.png");
				SDL_RenderTexture(renderer, texture, NULL, &square);
			}

			if (squares[i][j] == -1) {
				squares[i][j] = 0;
			}

		}
	}
}

void render_snakeblock(SDL_Renderer* renderer, std::vector<std::vector<int>> snake, int index_of_snake_block) {
	float initial_starting_position = (window_width - square_width * square_count) / 2;
	float width = square_width;
	float height = square_width;
	float x = initial_starting_position + square_width * snake[index_of_snake_block][0];
	float y = initial_starting_position + square_width * snake[index_of_snake_block][1];

	SDL_FRect rect = { x, y, width, height };

	int direction[2] = {0,0};
	if (index_of_snake_block != 0) {
		direction[0] = snake[index_of_snake_block][0] - snake[index_of_snake_block - 1][0];
		direction[1] = snake[index_of_snake_block][1] - snake[index_of_snake_block - 1][1];
	}
	else {
		direction[0] = snake[0][0] - snake[1][0];
		direction[1] = snake[0][1] - snake[1][1];
	}

	int angle = direction[0] * 90;
	
	if (direction[1] == 1) {
		angle = 180;
	}

	if (index_of_snake_block == snake.size() - 1) {
		SDL_Texture* texture = IMG_LoadTexture(renderer, "Sprites/head_up.png");
		SDL_RenderTextureRotated(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	}
	else if(index_of_snake_block == 0){
		SDL_Texture* texture = IMG_LoadTexture(renderer, "Sprites/tail_up.png");
		SDL_RenderTextureRotated(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	}
	else {
		int diff_x = snake[index_of_snake_block + 1][0] - snake[index_of_snake_block - 1][0];
		int diff_y = snake[index_of_snake_block + 1][1] - snake[index_of_snake_block - 1][1];
		
		if (abs(diff_x) == 1 && abs(diff_y) == 1) {
			
			if (diff_x == diff_y) {
				angle += abs(angle) == 90 ? 90 : 180;
			}
			else {
				angle += abs(angle) == 90 ? 180 : 90;
			}


			SDL_Texture* texture = IMG_LoadTexture(renderer, "Sprites/body_topright.png");
			SDL_RenderTextureRotated(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
		}
		else {
			SDL_Texture* texture = IMG_LoadTexture(renderer, "Sprites/body_vertical.png");
			SDL_RenderTextureRotated(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
		}

		
	}

}


void add_food_to_board() {
	int x = rand() % square_count;
	int y = rand() % square_count;

	
	while (squares[x][y] == 1 || squares[x][y] == -1) {
		x = rand() % square_count;
		y = rand() % square_count;
	}
	squares[x][y] = 1;
}



