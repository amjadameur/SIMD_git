
#include "main.h"







void drawObjZ(window_t* w, tgaInfo tgaData, vec3f_t luminance, chosenPlane plane) {
	// Draw obj file
	Uint8 r, g, b;
	vec3f_t vect3tmp[3];
	face_t  facetmp;
	int vertexIdx = 0;
	int x[3]; int y[3];
	int z[3], zMax;

	windowXYZ winXYZ;

	float faceLuminance;
	
	//////Z buffer ////////////////////////////////////////////////////////////////
	int** zBuffer = (int**) malloc(w->width*sizeof(int*));
	for (int c = 0; c < w->width; ++c)	{
		zBuffer[c] = (int*) malloc(w->height*sizeof(int));
	}

	///////////////////////////////////////////////////////////////////////////////////

	for (int r = 0; r < w->width; ++r) {
		for (int c = 0; c < w->height; ++c) {
			zBuffer[r][c] = MINUS_INF;
		}
	}

	for (int j = 0; j < ModelFaces()->count; ++j) {
		facetmp = ModelGetFace(j);
		for (int i = 0; i < 3; ++i) {
			vertexIdx = facetmp.v[i];				
			vect3tmp[i] = ModelGetVertex(vertexIdx);
			winXYZ = getXYZ(w, plane, vect3tmp[i]);
			x[i] = (int) winXYZ.x;
			y[i] = (int) winXYZ.y;
			z[i] = (int) winXYZ.z;
		}
		
		zMax = maxInt3(z[0], z[1], z[2]);

		faceLuminance = isFaceEnlighted(luminance, vect3tmp);

		if (faceLuminance>0)	{
			faceLuminance = (faceLuminance > 1) ? 1 : faceLuminance;
			findRgb(facetmp.vt, tgaData, &r, &g, &b);
			WindowDrawTriangleZ(w, zMax, zBuffer, x[0], y[0], x[1], y[1], x[2], y[2], faceLuminance*r, faceLuminance*g, faceLuminance*b);
		}
	}

	freeIntMatrix(w->width, zBuffer);
}

int main( int argc, char ** argv ) {

	const int width		= 1024;
	const int height	= 768;
	const int depth     = 255;


	unsigned int fps = 0;
	char *windowTitle = (char*) malloc(15*sizeof(char));

	unsigned long int elapsedTimePerFrame  = 0;

	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4, depth);

	int done = false;


	///// Load obj file ///////////////////////////////////////////////////////////////////
	ModelLoad("bin/data/diablo.obj");

	///// TGA //////////////////////////////////////////////////////////////
	int tgaWidth, tgaHeight; 
	int comp;
	int req_comp = 3; // RGB
	unsigned char* tgaIm = stbi_tga_load("bin/data/diablo_diffuse.tga", &tgaWidth, &tgaHeight, &comp, req_comp);

	tgaInfo tgaData = {tgaWidth, tgaHeight, tgaIm};


	//////////////////////////////////////////////////////////////////////

	chosenPlane plane = MINUS_YZ;

	vec3f_t luminance = setLuminance(plane);

	//////////////////////////////////////////////////////////////////////

	struct timeval timeBefore, timeAfter;

	unsigned int counter = 0;

	// Tant que l'utilisateur ne ferme pas la fenêtre
	while ( !done ) {

		// méthode pour regarder les 6 faces
		if(++counter == 30) {
			counter = 0;
			printf("%d\n", plane);
			plane = (chosenPlane) ((int) plane + 1);
			if (plane == PLUS_YZ+1) plane = (chosenPlane) 0; 
		}

		gettimeofday(&timeBefore, NULL);

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );

		// Effacement de l'écran avec une couleur
		WindowDrawClearColor( mainwindow, 0, 0, 0);

		drawObjZ(mainwindow, tgaData, luminance, plane);

		WindowUpdate( mainwindow );

		gettimeofday(&timeAfter, NULL);

		elapsedTimePerFrame = (timeAfter.tv_sec - timeBefore.tv_sec)*1000000 + (timeAfter.tv_usec - timeBefore.tv_usec);	
		fps = (unsigned int) (1000000/elapsedTimePerFrame);
		sprintf(windowTitle, "FPS : %u", fps);
		WindowSetTitle(mainwindow, windowTitle);
	}


	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );

	free(tgaIm);
	freeVectors();

	return 1;
}
