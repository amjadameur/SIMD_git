#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "string.h"

void tgaRgb(stbi_uc* data, int x, int y, int width, Uint8* r, Uint8* g, Uint8* b) {
	Uint8 * ptr = data + 3*(x + y*width);
	*r = *ptr++;
	*g = *ptr++;
	*b = *ptr++;
}

void findRgb(face_t facetmp, Uint8* rOut, Uint8* gOut, Uint8* bOut) {
	int r = 0, g = 0, b = 0;
	int x = 0, y = 0;
	int textureIdx = 0;
	vec3f_t vect3tmp;

	*rOut = 0; *gOut = 0; *bOut = 0;

	///////////////////////////////////////////////////////////////////
	int width, height; 
	int comp;
	int req_comp = 3; // RGB

	stbi_uc* data = stbi_tga_load("data/head_diffuse.tga", &width, &height, &comp, req_comp);
	///////////////////////////////////////////////////////////////////
/*
	for (int i = 0; i < 3; ++i) {
		textureIdx = facetmp.vt[i];
		vect3tmp = ModelGetTexcoord(textureIdx);
		x = vect3tmp.x*width;
		y = vect3tmp.y*height;
		tgaRgb(data, x, y, width, rOut, gOut, bOut);
		r += *rOut; 
		g += *gOut;
		b += *bOut;
	}

	*rOut = (Uint8) r/3;
	*gOut = (Uint8) g/3;
	*bOut = (Uint8) b/3;
*/

	textureIdx = facetmp.vt[0];
	vect3tmp = ModelGetTexcoord(textureIdx);
	x = vect3tmp.x*width;
	y = vect3tmp.y*height;
	tgaRgb(data, x, y, width, rOut, gOut, bOut);
	
	
	
}

void drawObj(window_t* w, int width, int height) {
	// Draw obj file
	vec3f_t vect3tmp;
	face_t  facetmp;
	int vertexIdx = 0;
	int x[3]; int y[3];
	Uint8 rOut = 255, gOut = 0, bOut = 0;

	for (int j = 0; j < ModelFaces()->count; ++j) {
		facetmp = ModelGetFace(j);
		for (int i = 0; i < 3; ++i) {
			vertexIdx = facetmp.v[i];				
			vect3tmp = ModelGetVertex(vertexIdx);
			x[i] = (int) (width/2) *(vect3tmp.x + 1);
			y[i] = (int) (height/2)*(vect3tmp.y + 1);
		}

		//findRgb(facetmp, &rOut, &gOut, &bOut);
		WindowDrawTriangle(w, x[0], y[0], x[1], y[1], x[2], y[2], 255, 0, 0);
	}
}

int main( int argc, char ** argv ) {

	const int width		= 1024;
	const int height	= 768;

	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4 );

	int done = false;

	ModelLoad("bin/data/head.obj");

	// Tant que l'utilisateur ne ferme pas la fenêtre
	while ( !done ) {

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );

		// Effacement de l'écran avec une couleur
		WindowDrawClearColor( mainwindow, 0, 0, 0);

		drawObj(mainwindow, width, height);

		//exit(0);

		WindowUpdate( mainwindow );

	}


	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );

	return 1;
}
