#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "string.h"
#include "math.h"


#define MINUS_INF -99999

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

void showVect(vec3f_t v) {
	printf("x : %f, y : %f, z : %f\n", v.x, v.y, v.z);
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

	//printf("\n\n\n");
	//showVect(a);
	//showVect(b);

	vProduct.x = c.y*b.z - c.z*b.y;
	vProduct.y = c.z*b.x - c.x*b.z;
	vProduct.z = c.x*b.y - c.y*b.x;

	//showVect(vProduct);
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
	float cProduct = a.x*b.x + a.y*b.y + a.z*b.z;

	return cProduct;
}



void drawObj(window_t* w, int width, int height) {
	// Draw obj file
	vec3f_t vect3tmp[3];
	face_t  facetmp;
	int vertexIdx = 0;
	int x[3]; int y[3];
	Uint8 rOut = 255, gOut = 0, bOut = 0;

	vec3f_t luminance = {1, 0, 0};
	
	vec3f_t cProduct= {0, 0, 0};
	float dProduct = 0;

	for (int j = 0; j < ModelFaces()->count; ++j) {
		facetmp = ModelGetFace(j);
		for (int i = 0; i < 3; ++i) {
			vertexIdx = facetmp.v[i];				
			vect3tmp[i] = ModelGetVertex(vertexIdx);
			x[i] = ((width-1)/2) *(vect3tmp[i].x + 1);
			y[i] = ((height-1)/2)*(vect3tmp[i].y + 1);
		}
		
		cProduct = crossProduct(vect3tmp[0], vect3tmp[1], vect3tmp[2]);	
		dProduct = dotProduct(luminance, cProduct);

		dProduct =(dProduct < 0) ? 0 : dProduct;
		dProduct =(dProduct > 1) ? 1 : dProduct;
		
		//findRgb(facetmp, &rOut, &gOut, &bOut);
		WindowDrawTriangle(w, x[0], y[0], x[1], y[1], x[2], y[2], dProduct*255, dProduct*255, dProduct*255);
	}
}


int minInt2(int a, int b) {
	return (a < b) ? a : b;
}

int minInt3(int a, int b, int c) {
	return minInt2(a, minInt2(b, c));
}

int maxInt2(int a, int b) {
	return (a > b) ? a : b;
}

int maxInt3(int a, int b, int c) {
	return maxInt2(a, maxInt2(b, c));
}

void drawObjZ(window_t* w, int **zBuffer, int width, int height) {
	// Draw obj file
	vec3f_t vect3tmp[3];
	face_t  facetmp;
	int vertexIdx = 0;
	int x[3]; int y[3];
	int z[3], zMax;


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
			x[i] = ((width-1)/2) *(vect3tmp[i].x + 1);
			y[i] = ((height-1)/2)*(vect3tmp[i].y + 1);
			z[i] = ((DEPTH-1)/2)*(vect3tmp[i].z + 1);
		}
		
		zMax = maxInt3(z[0], z[1], z[2]);
		if (zMax > DEPTH) printf("zMax_main : %d\n", zMax);
		WindowDrawTriangleZ(w, zMax, zBuffer, x[0], y[0], x[1], y[1], x[2], y[2], 255, 255, 255);
	}
}

int main( int argc, char ** argv ) {

	const int width		= 1024;
	const int height	= 768;

	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4 );

	int done = false;

	ModelLoad("bin/data/head.obj");

	int** zBuffer = (int**) malloc(width*sizeof(int*));
	for (int c = 0; c < width; ++c)	{
		zBuffer[c] = (int*) malloc(height*sizeof(int));
	}

	// Tant que l'utilisateur ne ferme pas la fenêtre
	while ( !done ) {

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );

		// Effacement de l'écran avec une couleur
		WindowDrawClearColor( mainwindow, 0, 0, 0);
		
		drawObjZ(mainwindow, zBuffer, width, height);

		WindowUpdate( mainwindow );

	}


	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );

	return 1;
}
