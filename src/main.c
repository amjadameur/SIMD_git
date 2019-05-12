
#include "main.h"




void tgaRgb(tgaInfo tgaData, int x, int y, Uint8* r, Uint8* g, Uint8* b) {
	Uint8 * ptr = tgaData.tgaIm + 3*x + 3*tgaData.tgaWidth*y;
	*b = *ptr++;
	*g = *ptr++;
	*r = *ptr++;
}

void findRgb(int *textureIdx, tgaInfo tgaData, Uint8* r, Uint8* g, Uint8* b) {
	int x = 0, y = 0;
	int rTmp = 0, gTmp = 0, bTmp = 0;

	for (int i = 0; i < 3; ++i)
	{
		x = tgaData.tgaWidth *ModelGetTexcoord(textureIdx[i]).x;
		y = tgaData.tgaHeight*ModelGetTexcoord(textureIdx[i]).y;
		tgaRgb(tgaData, x, y, r, g, b);	
		rTmp += *r;
		gTmp += *g;
		bTmp += *b;
	}

	*r = (Uint8) (rTmp/3);
	*g = (Uint8) (gTmp/3);
	*b = (Uint8) (bTmp/3);
}




void subVect3f(vec3f_t* v2, vec3f_t* v1) {
	v2->x = v2->x - v1->x;
	v2->y = v2->y - v1->y;
	v2->z = v2->z - v1->z;
}

vec3f_t crossProduct(vec3f_t a, vec3f_t b, vec3f_t c) {
	vec3f_t vProduct;
	
	subVect3f(&b, &a);
	subVect3f(&c, &a);

	vProduct.x = c.y*b.z - c.z*b.y;
	vProduct.y = c.z*b.x - c.x*b.z;
	vProduct.z = c.x*b.y - c.y*b.x;

	// Normalizing
	float magX = vProduct.x*vProduct.x;
	float magY = vProduct.y*vProduct.y;
	float magZ = vProduct.z*vProduct.z;

	float magnitude = sqrt(magX+magY+magZ);

	vProduct.x = vProduct.x/magnitude;
	vProduct.y = vProduct.y/magnitude;
	vProduct.z = vProduct.z/magnitude; 

	return vProduct;
}

float dotProduct(vec3f_t a, vec3f_t b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

int maxInt2(int a, int b) {
	return (a > b) ? a : b;
}

int maxInt3(int a, int b, int c) {
	return maxInt2(a, maxInt2(b, c));
}

void drawObjZ(window_t* w, tgaInfo tgaData, int **zBuffer) {
	// Draw obj file
	Uint8 r, g, b;
	vec3f_t vect3tmp[3];
	face_t  facetmp;
	int vertexIdx = 0;
	int x[3]; int y[3];
	int z[3], zMax;

	windowXYZ winXYZ;

	chosenPlane plane = MINUS_XZ;

	vec3f_t luminance = setLuminance(plane);
	
	vec3f_t cProduct= {0, 0, 0};
	float dProduct = 0;

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

		cProduct = crossProduct(vect3tmp[0], vect3tmp[1], vect3tmp[2]);	
		dProduct = dotProduct(luminance, cProduct);

		if (dProduct>0)	{
			dProduct =(dProduct > 1) ? 1 : dProduct;
			findRgb(facetmp.vt, tgaData, &r, &g, &b);
			WindowDrawTriangleZ(w, zMax, zBuffer, x[0], y[0], x[1], y[1], x[2], y[2], dProduct*r, dProduct*g, dProduct*b);
		}
	}
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

	//////Z buffer ////////////////////////////////////////////////////////////////
	int** zBuffer = (int**) malloc(width*sizeof(int*));
	for (int c = 0; c < width; ++c)	{
		zBuffer[c] = (int*) malloc(height*sizeof(int));
	}

	//////////////////////////////////////////////////////////////////////

	struct timeval timeBefore, timeAfter;

	// Tant que l'utilisateur ne ferme pas la fenêtre
	while ( !done ) {
		gettimeofday(&timeBefore, NULL);

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );

		// Effacement de l'écran avec une couleur
		//WindowDrawClearColor( mainwindow, 255, 255, 255);
		WindowDrawClearColor( mainwindow, 0, 0, 0);

		drawObjZ(mainwindow, tgaData, zBuffer);

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
