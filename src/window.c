#include "window.h"

static void WindowUpdateTexture( window_t * w ) {
	Uint32 * dst;
	int row, col;
	void * pixels;
	int pitch;
	if ( SDL_LockTexture( w->texture, NULL, &pixels, &pitch ) < 0 ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError() );
		SDL_Quit();
	}
	Uint8 * ptr = w->framebuffer;
	for ( row = 0; row < w->height; ++row ) {
		dst = (Uint32*)( (Uint8*)pixels + row * pitch );
		for ( col = 0; col < w->width; ++col ) {
			Uint8 b = *ptr++;
			Uint8 g = *ptr++;
			Uint8 r = *ptr++;
			Uint8 a = *ptr++;
			*dst++ = ( ( b << 24 ) | ( g << 16 ) | ( r << 8 ) | a );
		}
	}
	SDL_UnlockTexture( w->texture );
}

static Uint8 * WindowInitFramebuffer( window_t * w ) {
	size_t sz = w->width * w->height * w->bpp * sizeof( Uint8 );
	Uint8 * buffer = (Uint8*)malloc( sz );
	if ( buffer == NULL) return NULL;
	memset( buffer, 0, sz );
	return buffer;
}

window_t * WindowInit( int width, int height, int bpp ) {

	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
		printf( "SDL_Init Error: %s", SDL_GetError() );
		return NULL;
	}

	SDL_Window * sdlwindow = SDL_CreateWindow(	"Software OpenGL renderer", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		width, height,
		SDL_WINDOW_ALLOW_HIGHDPI );

	if ( sdlwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL window: %s", SDL_GetError() );
		SDL_DestroyWindow( sdlwindow );
		SDL_Quit();
		return NULL;
	}

	SDL_Renderer * renderer = SDL_CreateRenderer( sdlwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	if ( renderer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError() );
		SDL_DestroyRenderer( renderer );
		SDL_Quit();
		return NULL;
	}

	SDL_Texture * texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, width, height );
	
	if ( texture == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError() );
		SDL_DestroyTexture( texture );
		SDL_Quit();
		return NULL;
	}

	window_t * mainwindow = (window_t*)malloc( sizeof( window_t ) );

	if ( mainwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate window\n" );
		SDL_Quit();
		return NULL;
	}

	mainwindow->width	= width;
	mainwindow->height	= height;
	mainwindow->bpp		= bpp;
	mainwindow->pitch	= width * bpp;

	Uint8 * framebuffer = WindowInitFramebuffer( mainwindow );

	if ( framebuffer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate framebuffer: %s\n", SDL_GetError() );
		SDL_Quit();
		return NULL;
	}

	mainwindow->framebuffer = framebuffer;
	mainwindow->sdlwindow	= sdlwindow;
	mainwindow->renderer	= renderer;
	mainwindow->texture		= texture;

	return mainwindow;
}

void WindowDestroy( window_t * w ) {
	SDL_DestroyRenderer( w->renderer );
	SDL_DestroyTexture( w->texture );
	SDL_DestroyWindow( w->sdlwindow );
	free( w->framebuffer );
	SDL_Quit();
}

void WindowUpdate( window_t * w ) {
	SDL_RenderClear( w->renderer );
	SDL_RenderCopy( w->renderer, w->texture, NULL, NULL );
	SDL_RenderPresent( w->renderer );
	WindowUpdateTexture( w );
}

void WindowSetTitle( window_t * w, const char * title ) {
	SDL_SetWindowTitle( w->sdlwindow, title );
}

void WindowDrawPoint( window_t * w, int x, int y, Uint8 r, Uint8 g, Uint8 b ) {
	// Fonction à implementer
	Uint8 * ptr = w->framebuffer + 4*x + (w->height-1)*w->pitch - y*w->pitch;
	*ptr++ = b;
	*ptr++ = g;
	*ptr++ = r;
	*ptr = 255;

}

void WindowDrawClearColor( window_t * w, Uint8 r, Uint8 g, Uint8 b ) {
	// Fonction à implementer
	for (int x = 0; x < w->width; ++x) {
		for (int y = 0; y < w->height; ++y)
		{
			WindowDrawPoint(w, x, y, r, g, b);
		}
	}
}


void WindowDrawLine(window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b )
{
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		WindowDrawPoint(w, x0, y0, r, g, b);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}


void WindowDrawTriangle( window_t * w, int x0, int y0, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b ) { 
	// Fonction à implementer
/*	WindowDrawLine(w, x0, y0, x1, y1, r, g, b);
	WindowDrawLine(w, x1, y1, x2, y2, r, g, b);
	WindowDrawLine(w, x2, y2, x0, y0, r, g, b);
*/
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		WindowDrawLine(w, x0, y0, x2, y2, r, g, b);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
	
}
