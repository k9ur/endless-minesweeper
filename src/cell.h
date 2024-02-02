#ifndef MINESWEEPER_CELL_H
#define MINESWEEPER_CELL_H

#include "minesweeper.h"


CellCache cell_calc_cache(Game *const game, const Position *const pos) {
	const Cell *restrict other_cell;
	Position other_pos;
	CellCache cache = 0;
	for(int32_t y_d = -1; y_d <= 1; ++y_d) {
		other_pos.y = pos->y + y_d;
		for(int32_t x_d = -1; x_d <= 1; ++x_d) {
			if(x_d == 0 && y_d == 0)
				continue;
			other_pos.x = pos->x + x_d;
			other_cell = grid_get_and_gen(game, &other_pos);
			if(*other_cell & CELL_MINE)
				++cache;
		}
	}
	return cache;
}

CellCache cell_get_cache(const Cell *const cell) {
	// Will return 0 if no cache is set yet, so that should be checked beforehand
	const CellCache cache = *cell >> 4;
	assert(cache && (cache <= 8 || cache == CELL_SURR_NONE));
	return cache == CELL_SURR_NONE ? 0 : cache;
}

void cell_set_cache(Cell *const cell, const CellCache cache) {
	if(*cell & CELL_UNCACHED)
		*cell ^= CELL_UNCACHED; // Mark as cached
	*cell &= CELL_BITS; // Remove current cache
	*cell |= (cache ? cache : CELL_SURR_NONE) << 4; // Set new cache
	assert(*cell != CELL_NULL);
}


#endif // MINESWEEPER_CELL_H

