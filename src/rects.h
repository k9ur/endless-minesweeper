#ifndef MINESWEEPER_RECTS_H
#define MINESWEEPER_RECTS_H

#include "minesweeper.h"


void game_over(Game *const game, const Position *const pos) {
	Rects *const rects = &game->rects;
	const SDL_Rect rect = {
		.x = cursor_start_x + (pos->x - game->cursor.x) * CHUNK_WIDTH,
		.y = cursor_start_y1 + (pos->y - game->cursor.y) * CHUNK_HEIGHT,
		.w = CELL_WIDTH,
		.h = CELL_HEIGHT
	};
	assert(rects->mines_used != 4);
	rects->mines[rects->mines_used] = rect;
	++rects->mines_used;
	game->over = true;
	game->redraw = true; // Will need to render one more time to show the clicked mine
}

void rects_add(Game *const game, const Position *const pos, const CellCache idx) {
	assert(idx <= NUM_COLORS);
	Rects *const rects = &game->rects;
	SDL_Rect **const rect_arr = &rects->nums[idx];
	size_t *const size = &rects->nums_size[idx];
	size_t *const used = &rects->nums_used[idx];
	if(*used == *size) {
		*size <<= 1;
		assert(*size > *used);
		*rect_arr = realloc(*rect_arr, *size * sizeof **rect_arr);
		assert(*rect_arr != NULL);
	}
	const SDL_Rect rect = {
		.x = cursor_start_x + (pos->x - game->cursor.x) * CHUNK_WIDTH,
		.y = cursor_start_y1 + (pos->y - game->cursor.y) * CHUNK_HEIGHT,
		.w = CELL_WIDTH,
		.h = CELL_HEIGHT
	};
	(*rect_arr)[*used] = rect;
	++*used;
	game->redraw = true;
}

void rects_rem(Game *const game, const Position *const pos, const CellCache idx) {
	Rects *const rects = &game->rects;
	size_t *const used = &rects->nums_used[idx];
	if(!*used)
		return;
	SDL_Rect *const rect_arr = rects->nums[idx];
	const SDL_Rect search_rect = {
		.x = cursor_start_x + (pos->x - game->cursor.x) * CHUNK_WIDTH,
		.y = cursor_start_y1 + (pos->y - game->cursor.y) * CHUNK_HEIGHT,
		.w = CELL_WIDTH,
		.h = CELL_HEIGHT
	};
	for(size_t i = 0; i != *used; ++i)
		if(rect_cmp(&rect_arr[i], &search_rect)) {
			rect_arr[i] = rect_arr[--*used]; // Swap with last, then decr used
			game->redraw = true;
			break;
		}
}

void rects_move_all(Game *const game, const Direction dir, const bool jump) {
	Rects *const rects = &game->rects;
	int32_t move_dist;
	bool sub;
	switch(dir) {
		case UP:
			move_dist = CELL_HEIGHT;
			sub = false;
			break;
		case DOWN:
			move_dist = CELL_HEIGHT;
			sub = true;
			break;
		case LEFT:
			move_dist = CELL_WIDTH;
			sub = false;
			break;
		case RIGHT:
			move_dist = CELL_WIDTH;
			sub = true;
			break;
		default:
			break;
	}
	if(jump)
		move_dist *= JUMP_DIST;
	for(CellCache idx = 0; idx != NUM_COLORS + 1; ++idx) {
		SDL_Rect *const rect_arr = rects->nums[idx];
		int32_t *coord;
		switch(dir) {
			case UP:
			case DOWN:
				coord = &rect_arr[0].y;
				break;
			case LEFT:
			case RIGHT:
				coord = &rect_arr[0].x;
				break;
			default:
				break;
		}
		for(size_t i = 0; i != rects->nums_used[idx]; ++i) {
			if(sub)
				*coord -= move_dist;
			else
				*coord += move_dist;
			coord += 4;
		}
	}
	game->redraw = true;
}


#endif // MINESWEEPER_RECTS_H

