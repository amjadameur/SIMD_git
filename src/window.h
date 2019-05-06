#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "SDL2/SDL.h"


#define DEPTH 255

/**
 * Définition des types
 */
typedef struct window {
	SDL_Window	*	sdlwindow;
	SDL_Renderer	*	renderer;
	SDL_Texture	*	texture;
	unsigned char	*	framebuffer;
	int			width;
	int			height;
	int			bpp;
	int			pitch;
}window_t;

/**
 * Définition des prototypes de fonctions
 */

/**
 * Initialise et ouvre une nouvelle fenêtre
 */
window_t	*	WindowInit		( int width, int height, int bpp );

/**
 * Ferme et detruit une fênetre
 */
void			WindowDestroy		( window_t * w );

/**
 * Efface une fenêtre avec la couleur souhaîtée
 */
void			WindowDrawClearColor	( window_t * w, unsigned char r, unsigned char g, unsigned char b );

/**
 * Met à jour le contenu de la fenêtre
 */
void			WindowUpdate		( window_t * w );

/**
 * Met à jour le titre de la fenêtre
 */
void 			WindowSetTitle		( window_t * w, const char * title );

/**
 * Dessine un point coloré dans la fenêtre
 */
void			WindowDrawPoint		( window_t * w, int x, int y, Uint8 r, Uint8 g, Uint8 b );
void			WindowDrawLine		( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b );
void 			WindowDrawTriangle  ( window_t * w, int x0, int y0, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b );


void			WindowDrawPointZ	( window_t * w, int zMax, int **zBuffer, int x, int y, Uint8 r, Uint8 g, Uint8 b );
void			WindowDrawLineZ		( window_t * w, int zMax, int **zBuffer, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b );
void 			WindowDrawTriangleZ ( window_t * w, int zMax, int **zBuffer, int x0, int y0, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b );

#endif //__WINDOW_H__
