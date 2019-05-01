#include "events.h"

/**
 * Récupère et traite les évenements d'une fenêtre
 */
int EventsUpdate( window_t * w ) {
	SDL_Event event;
	bool done = false;
	while ( SDL_PollEvent( &event ) ) {      
		unsigned int e = event.type;
		if ( e == SDL_KEYDOWN ) {
			if( event.key.keysym.sym == SDLK_f ) {
				// appui sur la touche f du clavier
			}
		}else if ( e == SDL_QUIT ) {
			done = true;
		}
	}
	return done;
}
