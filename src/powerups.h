#ifndef MINESWEEPER_POWERUPS_H
#define MINESWEEPER_POWERUPS_H

#include "minesweeper.h"


#ifdef POWERUPS
void pups_handle(Game *const game, const CellCache cache) {
	if(cache < PUP_FIRST_NUM || cache >= PUP_FIRST_NUM + NUM_PUPS)
		return;
	Powerups *const pups = &game->pups;
	const uint8_t idx = cache - PUP_FIRST_NUM;
	if(rand() > RAND_MAX * PUP_CHANCES[idx])
		return;
	SDL_Rect **const rect_arr = &pups->text_rects[idx];
	size_t *const size = &pups->pups_size[idx];
	size_t *const used = &pups->pups_used[idx];
	if(*used == *size) {
		*size <<= 1;
		assert(*size > *used);
		*rect_arr = realloc(*rect_arr, *size * sizeof **rect_arr);
		assert(*rect_arr != NULL);
	}
	const int32_t x_coord = SCREEN_PADDING_X + *used * PUP_SPACING_X;
	const int32_t y_coord = SCREEN_PADDING_Y + *used * PUP_SPACING_Y;
	const SDL_Rect rect = {
		.x = cache != PUP_FIRST_NUM + NUM_PUPS - 1 ? x_coord : SCREEN_WIDTH - PUP_WIDTH - x_coord,
		.y = cache == PUP_FIRST_NUM ? y_coord : SCREEN_HEIGHT - PUP_HEIGHT - y_coord,
		.w = PUP_WIDTH,
		.h = PUP_HEIGHT
	};
	(*rect_arr)[*used] = rect;
	++*used;
	game->redraw = true;
}

bool pups_use_hint(Game *const game, const Position *const pos) {
	size_t *const hints = &game->pups.pups_used[PUP_HINT];
	if(!*hints)
		return false;
	Cell *const cell = grid_get_and_gen(game, pos);
	if(*cell & (CELL_CLICKED | CELL_FLAGGED)) // Already clicked/flagged, can't use hint
		return false;

	if(*cell & CELL_MINE) {
		grid_flag(game, pos);
		*cell |= CELL_CLICKED; // Make it permanent: both CELL_CLICKED and CELL_MINE set
	} else
		grid_click(game, pos);
	--*hints;
	return true;
}

bool pups_use_save(Game *const game, const Position *const pos) {
	size_t *const saves = &game->pups.pups_used[PUP_SAVE];
	if(!*saves)
		return false;
	Cell *const cell = grid_get_and_gen(game, pos);
	if(!(*cell & CELL_MINE) || *cell & (CELL_CLICKED | CELL_FLAGGED))
		return false;

	grid_flag(game, pos);
	*cell |= CELL_CLICKED; // Make it permanent: both CELL_CLICKED and CELL_MINE set
	--*saves;
	return true;
}

bool pups_use_space(Game *const game, const Position *const pos) {
	size_t *const spaces = &game->pups.pups_used[PUP_SPACE];
	if(!*spaces || !game->clicks)
		return false;
	Cell *const restrict cell = grid_get(game, pos);
	if(*cell != CELL_NULL)
		return false;

	const Cell *restrict other_cell;
	Position other_pos;
	bool done = false;
loop_start:
	for(int32_t y_d = -1; y_d <= 1; ++y_d) {
		other_pos.y = pos->y + y_d;
		for(int32_t x_d = -1; x_d <= 1; ++x_d) {
			if(x_d == 0 && y_d == 0)
				continue;
			other_pos.x = pos->x + x_d;
			if(done) // Ensure no mines around
				grid_set(game, &other_pos, CELL_UNCACHED);
			else {
				other_cell = grid_get(game, &other_pos);
				if(*other_cell != CELL_NULL)
					return false;
			}
		}
	}
	if(!done) {
		done = true;
		cell_set_cache(cell, 0);
		goto loop_start;
	}
	--*spaces;
	grid_click(game, pos); // Click the empty cell
	return true;
}
#endif


#endif // MINESWEEPER_POWERUPS_H

