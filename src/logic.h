#ifndef MINESWEEPER_LOGIC_H
#define MINESWEEPER_LOGIC_H

#include "minesweeper.h"


bool rect_cmp(const SDL_Rect *const a, const SDL_Rect *const b) {
	return a->x == b->x && a->y == b->y && a->w == b->w && a->h == b->h;
}


#endif // MINESWEEPER_LOGIC_H

