#ifndef MINESWEEPER_INIT_H
#define MINESWEEPER_INIT_H

#include "minesweeper.h"


bool init(Game *const game) {
	if(!rand_init())
		return false;
	if(!game_init(game))
		return false;
	if(!grid_init(game))
		return false;
	if(!rects_init(game))
		return false;
#ifdef POWERUPS
	if(!pups_init(game))
		return false;
#endif
	return true;
}

bool rand_init(void) {
	srand(time(NULL));
	return true;
}

bool game_init(Game *const game) {
	const char *err_name;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		err_name = "SDL2";
		goto init_failed;
	}
	game->window = SDL_CreateWindow("Endless Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if(game->window == NULL) {
		err_name = "window";
		goto init_failed;
	}
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
	if(game->renderer == NULL) {
		err_name = "renderer";
		goto init_failed;
	}
	return true;
init_failed:
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialise %s: %s\n", err_name, SDL_GetError());
	return false;
}

bool grid_init_quarter(Chunk ***const quarter, uint32_t *const restrict y_size, uint32_t *const restrict x_size) {
	*quarter = malloc((STARTING_HEIGHT >> 1) * sizeof **quarter);
	if(*quarter == NULL)
		return false;
	for(uint32_t r = 0; r != STARTING_HEIGHT >> 1; ++r) {
		(*quarter)[r] = calloc(STARTING_WIDTH >> 1, sizeof ***quarter);
		if((*quarter)[r] == NULL)
			return false;
	}
	*y_size = STARTING_HEIGHT >> 1;
	*x_size = STARTING_WIDTH >> 1;
	return true;
}

bool grid_init(Game *const game) {
	Grid *const grid = &game->grid;
	Chunk ***quarter_ptr = &grid->pypx;
	uint32_t *restrict x_ptr = &grid->pypx_x;
	uint32_t *restrict y_ptr = &grid->pypx_y;
	for(uint8_t i = 0; i != 4; ++i) {
		if(!grid_init_quarter(quarter_ptr, y_ptr, x_ptr)) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialise grid\n");
			return false;
		}
		++quarter_ptr;
		++x_ptr;
		++y_ptr;
	}
	return true;
}

bool rects_init(Game *const game) {
	Rects *const rects = &game->rects;
	for(CellCache idx = 0; idx != NUM_COLORS + 1; ++idx) {
		rects->nums[idx] = malloc(RECTS_INIT * sizeof **rects->nums);
		if(rects->nums[idx] == NULL) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialise rects\n");
			return false;
		}
#ifdef DRAW_PALETTE
		const SDL_Rect palette_rect = {
			.x = cursor_start_x + idx * CELL_WIDTH,
			.y = cursor_start_y1,
			.w = CELL_WIDTH,
			.h = CELL_HEIGHT
		};
		rects->nums[idx][0] = palette_rect;
		rects->nums_used[idx] = 1;
#else
		rects->nums_used[idx] = 0;
#endif
		rects->nums_size[idx] = RECTS_INIT;
	}
	rects->mines_used = 0;
	return true;
}

#ifdef POWERUPS
bool pups_init(Game *const game) {
	Powerups *const pups = &game->pups;
	char text_dir[] = "./assets/pup0.png";
	text_dir[12] += PUP_FIRST_NUM;
	for(uint8_t idx = 0; idx != NUM_PUPS; ++idx) {
		pups->texts[idx] = IMG_LoadTexture(game->renderer, text_dir);
		if(pups->texts[idx] == NULL)
			goto init_failed;
		pups->text_rects[idx] = malloc(PUP_INIT * sizeof **pups->text_rects);
		if(pups->text_rects[idx] == NULL)
			goto init_failed;
		pups->pups_size[idx] = PUP_INIT;
		pups->pups_used[idx] = 0;
		++text_dir[12];
	}
	return true;
init_failed:
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialise pups\n");
	return false;
}
#endif


#endif // MINESWEEPER_INIT_H

