#ifndef MINESWEEPER_HANDLERS_H
#define MINESWEEPER_HANDLERS_H

#include "minesweeper.h"


void handle_event(Game *const game, const SDL_Event *const e) {
	switch(e->type) {
		case SDL_QUIT:
			game->quit = true;
			break;
		case SDL_KEYDOWN:
			handle_keydown_event(game, &e->key);
			break;
		case SDL_WINDOWEVENT:
			handle_window_event(game, &e->window);
			break;
		default:
			break;
	}
}

void handle_keydown_event(Game *const game, const SDL_KeyboardEvent *const key_e) {
	if(game->over) {
		if(key_e->keysym.sym == SDLK_r)
			reset(game);
		return;
	}
	const bool shifting = key_e->keysym.mod & KMOD_SHIFT;
	switch(key_e->keysym.sym) {
		case SDLK_r:
			if(key_e->keysym.mod & KMOD_CTRL)
				reset(game);
			break;
		case SDLK_KP_9:
			grid_move(game, RIGHT, shifting);
		case SDLK_KP_8:
		case SDLK_w:
		case SDLK_UP:
			grid_move(game, UP, shifting);
			break;
		case SDLK_KP_1:
			grid_move(game, LEFT, shifting);
		case SDLK_KP_2:
		case SDLK_s:
		case SDLK_DOWN:
			grid_move(game, DOWN, shifting);
			break;
		case SDLK_KP_7:
			grid_move(game, UP, shifting);
		case SDLK_KP_4:
		case SDLK_a:
		case SDLK_LEFT:
			grid_move(game, LEFT, shifting);
			break;
		case SDLK_KP_3:
			grid_move(game, DOWN, shifting);
		case SDLK_KP_6:
		case SDLK_d:
		case SDLK_RIGHT:
			grid_move(game, RIGHT, shifting);
			break;
		case SDLK_KP_5:
		case SDLK_c:
		case SDLK_PERIOD:
		case SDLK_SPACE:
			++game->clicks;
			grid_click(game, &game->cursor);
			break;
		case SDLK_KP_0:
		case SDLK_f:
		case SDLK_COMMA:
			grid_flag(game, &game->cursor);
			break;
#ifdef POWERUPS
		case SDLK_h:
			pups_use_hint(game, &game->cursor);
			break;
#endif
		default:
			break;
	}
}

void handle_window_event(Game *const game, const SDL_WindowEvent *const win_e) {
	switch(win_e->event) {
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_LEAVE:
		case SDL_WINDOWEVENT_FOCUS_LOST:
			break;
		case SDL_WINDOWEVENT_CLOSE:
			game->quit = true;
			break;
		default:
			game->redraw = true;
			break;
	}
}


#endif // MINESWEEPER_HANDLERS_H

