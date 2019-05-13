
#include "main.h"

//

int **zBuffer;

void drawObjZ(window_t* w, tgaInfo tgaData, vec3f_t luminance, chosenPlane plane) {
	// Draw obj file
	Uint8 r, g, b;            // texture d'un triangle
	vec3f_t vect3tmp[3];      // les 3 sommets d'un triangle
	face_t  facetmp;          // 
	int vertexIdx = 0;		  // indice de la texture à extraire du vecteur g_texcoord	
	int x[3], y[3], z[3];     // coordonnées des sommets d'un triangle donné 
	int zMean;			      // la coordonnée z du point le plus proche "à l'écran"		

	windowXYZ winXYZ; // structure de donnée où l'on stocke les coordonnées (x, y) du point a dessiner et la profondeur z de ce dernier.

	float faceLuminance; // elle représente la position d'un vecteur donnée par rapport à la source de la lumière.

	/////// Initialisation du zBuffer /////////////////////////////////////////////////////////////////////
	for (int r = 0; r < w->width; ++r) {
		for (int c = 0; c < w->height; ++c) {
			zBuffer[r][c] = MINUS_INF;
		}
	}
	////////////////////////////////////////////////////////////////////////////

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
		
		zMean = (z[0] + z[1] + z[2])/3; // barycentre de la pronfondeur du triangle

		faceLuminance = isFaceEnlighted(luminance, vect3tmp); // produit scalaire entre la lumière et la normale du triangle

		// Si le produit scalaire est négative ou nul, nous ne dessinons pas le triangle
		if (faceLuminance>0)	{
			faceLuminance = (faceLuminance > 1) ? 1 : faceLuminance;

			findRgb(facetmp.vt, tgaData, &r, &g, &b);  // On récupère la texture du triangle
			WindowDrawTriangleZ(w, zMean, zBuffer, x[0], y[0], x[1], y[1], x[2], y[2], faceLuminance*r, faceLuminance*g, faceLuminance*b);
		}
	}
}

unsigned int getFPS(struct timeval timeBefore, struct timeval timeAfter) {
	unsigned long int elapsedTimePerFrame = (timeAfter.tv_sec - timeBefore.tv_sec)*1000000 + (timeAfter.tv_usec - timeBefore.tv_usec);	
	return (unsigned int) (1000000/elapsedTimePerFrame) ;
}


int main( int argc, char ** argv ) {

	const int width		= 1024;
	const int height	= 768;
	const int depth     = 255;


	unsigned int fps = 0;
	char *windowTitle = (char*) malloc(15*sizeof(char));

	

	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4, depth);

	int done = false;


	///// Load obj file ///////////////////////////////////////////////////////////////////
	ModelLoad("bin/data/head.obj");

	///// TGA //////////////////////////////////////////////////////////////
	int tgaWidth, tgaHeight; 
	int comp;
	int req_comp = 4; // RGB
	unsigned char* tgaIm = stbi_tga_load("bin/data/head_diffuse.tga", &tgaWidth, &tgaHeight, &comp, req_comp);
	//printf("comp : %d\n", comp);
	tgaInfo tgaData = {tgaWidth, tgaHeight, tgaIm};

	//////Z buffer ////////////////////////////////////////////////////////////////////
	zBuffer = (int**) malloc(mainwindow->width*sizeof(int*));
	for (int c = 0; c < mainwindow->width; ++c)	{
		zBuffer[c] = (int*) malloc(mainwindow->height*sizeof(int));
	}

	///////////////////////////////////////////////////////////////////////////////////
	chosenPlane plane = MINUS_XY;

	vec3f_t luminance = setLuminance(plane);

	///////////////////////////////////////////////////////////////////////////////////

	struct timeval timeBefore, timeAfter;

	unsigned int counter = 0;

	// Tant que l'utilisateur ne ferme pas la fenêtre
	while ( !done ) {

		// méthode pour regarder les 6 faces
		if(++counter == 40) {
			counter = 0;
			printf("%d\n", plane);
			plane = (chosenPlane) (plane + 1);
			if (plane == PLUS_YZ+1) plane = (chosenPlane) 0; 
			luminance = setLuminance(plane);
		}
		
		gettimeofday(&timeBefore, NULL);

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );

		// Effacement de l'écran avec une couleur
		WindowDrawClearColor( mainwindow, 0, 0, 0);
		
		drawObjZ(mainwindow, tgaData, luminance, plane);

		WindowUpdate( mainwindow );

		gettimeofday(&timeAfter, NULL);

		fps = getFPS(timeBefore, timeAfter);
		
		sprintf(windowTitle, "FPS : %u", fps);
		WindowSetTitle(mainwindow, windowTitle);
	}


	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );
	freeIntMatrix(mainwindow->width, zBuffer); // Nous libérons la mémoire occupée par zBuffer
	free(tgaIm);
	freeVectors();

	return 1;
}
