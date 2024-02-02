#ifndef MINESWEEPER_RESET_H
#define MINESWEEPER_RESET_H

#include "minesweeper.h"


void reset(Game *const game) {
	game_reset(game);
	grid_reset(game);
	rects_reset(game);
#ifdef POWERUPS
	pups_reset(game);
#endif
}

void game_reset(Game *const game) {
	SDL_Log("You saw %llu cells, manually clicked %llu cells, and flagged %llu cells\n", game->seen, game->clicks, game->flags);
	game->over = false;
	game->redraw = true;
	game->clicks = 0;
	game->flags = 0;
	game->seen = 0;
	game->cursor.x = 0;
	game->cursor.y = 0;
}

void grid_reset_quarter(Chunk *const*const quarter, const uint32_t rows, const uint32_t cols) {
	for(uint32_t r = 0; r != rows; ++r)
		memset(quarter[r], 0, cols * sizeof **quarter); // Set whole chunk(s) to 0 (CELL_NULL)
}

void grid_reset(Game *const game) {
	Grid *const grid = &game->grid;
	grid_reset_quarter(grid->pypx, grid->pypx_y, grid->pypx_x);
	grid_reset_quarter(grid->pynx, grid->pynx_y, grid->pynx_x);
	grid_reset_quarter(grid->nypx, grid->nypx_y, grid->nypx_x);
	grid_reset_quarter(grid->nynx, grid->nynx_y, grid->nynx_x);
}

void rects_reset(Game *const game) {
	Rects *const rects = &game->rects;
	memset(rects->nums_used, 0, (NUM_COLORS + 1) * sizeof *rects->nums_used); // Set all useds to 0
	rects->mines_used = 0;
}

#ifdef POWERUPS
void pups_reset(Game *const game) {
	Powerups *const pups = &game->pups;
	memset(pups->pups_used, 0, NUM_PUPS * sizeof *pups->pups_used); // Set all useds to 0
}
#endif


#endif // MINESWEEPER_RESET_H

