#ifndef MINESWEEPER_DESTROY_H
#define MINESWEEPER_DESTROY_H

#include "minesweeper.h"


void destroy(Game *const game) {
#ifdef POWERUPS
	pups_destroy(game);
#endif
	rects_destroy(game);
	grid_destroy(game);
	game_destroy(game);
}

void game_destroy(Game *const game) {
	SDL_DestroyRenderer(game->renderer);
	game->renderer = NULL;
	SDL_DestroyWindow(game->window);
	game->window = NULL;
	SDL_Quit();
}

void grid_destroy_quarter(Chunk ***const quarter, const uint32_t rows) {
	if(*quarter == NULL)
		return;
	for(uint32_t r = 0; r != rows; ++r)
		if((*quarter)[r] != NULL)
			free((*quarter)[r]);
	free(*quarter);
	*quarter = NULL;
}

void grid_destroy(Game *const game) {
	Grid *const grid = &game->grid;
	grid_destroy_quarter(&grid->pypx, grid->pypx_y);
	grid_destroy_quarter(&grid->pynx, grid->pynx_y);
	grid_destroy_quarter(&grid->nypx, grid->nypx_y);
	grid_destroy_quarter(&grid->nynx, grid->nynx_y);
	memset(&grid->pypx_x, 0, 8 * sizeof grid->pypx_x); // Set all sizes to 0
}

void rects_destroy(Game *const game) {
	Rects *const rects = &game->rects;
	for(CellCache idx = 0; idx != NUM_COLORS + 1; ++idx)
		if(rects->nums[idx] != NULL) {
			free(rects->nums[idx]);
			rects->nums[idx] = NULL;
		}
	memset(rects->nums_size, 0, 2 * (NUM_COLORS + 1) * sizeof *rects->nums_size); // Set all sizes/useds to 0
	rects->mines_used = 0;
}

#ifdef POWERUPS
void pups_destroy(Game *const game) {
	Powerups *const pups = &game->pups;
	for(uint8_t idx = 0; idx != NUM_PUPS; ++idx) {
		if(pups->texts[idx] != NULL) {
			SDL_DestroyTexture(pups->texts[idx]);
			pups->texts[idx] = NULL;
		}
		if(pups->text_rects[idx] != NULL) {
			free(pups->text_rects[idx]);
			pups->text_rects[idx] = NULL;
		}
	}
	memset(pups->pups_size, 0, 2 * NUM_PUPS * sizeof *pups->pups_size); // Set all sizes/useds to 0
}
#endif


#endif // MINESWEEPER_DESTROY_H

