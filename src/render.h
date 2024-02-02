#ifndef MINESWEEPER_RENDER_H
#define MINESWEEPER_RENDER_H

#include "minesweeper.h"


void render_update(Game *const game) {
	SDL_RenderPresent(game->renderer);
}

void render_clear(Game *const game) {
	SDL_Renderer *const renderer = game->renderer;
	SDL_SetRenderDrawColor(renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, COLOR_BG.a);
	SDL_RenderClear(renderer);
}

void render_cursor(Game *const game) {
	const SDL_Rect cursor_rects[8] = {
		{ .x = cursor_start_x, .y = cursor_start_y1, .w = CURSOR_LEN,   .h = CURSOR_WIDTH },
		{ .x = cursor_start_x, .y = cursor_start_y2, .w = CURSOR_WIDTH, .h = CURSOR_LEN - CURSOR_WIDTH },
		{ .x = cursor_end_x1,  .y = cursor_start_y1, .w = CURSOR_LEN,   .h = CURSOR_WIDTH },
		{ .x = cursor_end_x2,  .y = cursor_start_y2, .w = CURSOR_WIDTH, .h = CURSOR_LEN - CURSOR_WIDTH },
		{ .x = cursor_start_x, .y = cursor_end_y1,   .w = CURSOR_LEN,   .h = CURSOR_WIDTH },
		{ .x = cursor_start_x, .y = cursor_end_y2,   .w = CURSOR_WIDTH, .h = CURSOR_LEN - CURSOR_WIDTH },
		{ .x = cursor_end_x1,  .y = cursor_end_y1,   .w = CURSOR_LEN,   .h = CURSOR_WIDTH },
		{ .x = cursor_end_x2,  .y = cursor_end_y2,   .w = CURSOR_WIDTH, .h = CURSOR_LEN - CURSOR_WIDTH }
	};
	SDL_Renderer *const renderer = game->renderer;
	SDL_SetRenderDrawColor(renderer, COLOR_CURSOR.r, COLOR_CURSOR.g, COLOR_CURSOR.b, COLOR_CURSOR.a);
	SDL_RenderFillRects(renderer, cursor_rects, 8);
}

void render_rects(Game *const game) {
	SDL_Renderer *const renderer = game->renderer;
	const Rects *const rects = &game->rects;
	for(CellCache idx = 0; idx != NUM_COLORS + 1; ++idx)
		if(rects->nums_used[idx]) {
			SDL_SetRenderDrawColor(renderer, COLOR_NUM[idx].r, COLOR_NUM[idx].g, COLOR_NUM[idx].b, COLOR_NUM[idx].a);
			SDL_RenderFillRects(renderer, rects->nums[idx], rects->nums_used[idx]);
		}
	if(rects->mines_used) {
		SDL_SetRenderDrawColor(renderer, COLOR_MINE.r, COLOR_MINE.g, COLOR_MINE.b, COLOR_MINE.a);
		SDL_RenderFillRects(renderer, rects->mines, rects->mines_used);
	}
}

#ifdef POWERUPS
void render_pups(Game *const game) {
	SDL_Renderer *const renderer = game->renderer;
	const Powerups *const pups = &game->pups;
	for(uint8_t idx = 0; idx != NUM_PUPS; ++idx)
		if(pups->pups_used[idx]) {
			SDL_SetRenderDrawColor(renderer, COLOR_PUP[idx].r, COLOR_PUP[idx].g, COLOR_PUP[idx].b, COLOR_PUP[idx].a);
			for(size_t i = 0; i != pups->pups_used[idx]; ++i)
				SDL_RenderCopy(renderer, pups->texts[idx], NULL, &pups->text_rects[idx][i]);
		}
}
#endif


#endif // MINESWEEPER_RENDER_H

