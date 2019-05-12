#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "string.h"
#include "math.h"
#include "sys/time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>


#define MINUS_INF -99999

typedef enum {MINUS_XY = 0, MINUS_XZ, MINUS_YZ, PLUS_XY, PLUS_XZ, PLUS_YZ} chosenPlane;


typedef struct tga_info {
	int tgaWidth;
	int tgaHeight;
	unsigned char *tgaIm;
} tgaInfo;

typedef struct window_XYZ {float x; float y; float z;} windowXYZ; 

int maxInt2(int a, int b) {
	return (a > b) ? a : b;
}

int maxInt3(int a, int b, int c) {
	return maxInt2(a, maxInt2(b, c));
}

vec3f_t setLuminance(chosenPlane plane) {
	vec3f_t luminance = {0, 0, 0};

	switch(plane) {
		case MINUS_XY : luminance = {0, 0, -1}; break;
		case PLUS_XY  : luminance = {0, 0,  1}; break;

		case MINUS_YZ : luminance = {-1, 0,  0}; break;
		case PLUS_YZ  : luminance = {1 , 0,  0}; break;

		case MINUS_XZ : luminance = {0, -1, 0}; break;
		case PLUS_XZ  : luminance = {0,  1, 0}; break;
	}

	return luminance;
}

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


windowXYZ getXYZ(window_t* w, chosenPlane plane, vec3f_t vertexVect) {
	windowXYZ winXYZ;

	float xIm(vertexVect.x);
	float yIm(vertexVect.y);
	float zIm(vertexVect.z);

	if(plane == MINUS_XY || plane == PLUS_XY) winXYZ = {xIm, yIm, zIm}; // profondeur en Z
	if(plane == MINUS_YZ || plane == PLUS_YZ) winXYZ = {zIm, yIm, xIm}; // profondeur en X
	if(plane == MINUS_XZ || plane == PLUS_XZ) winXYZ = {xIm, zIm, yIm}; // profondeur en Y

	winXYZ.x = ((w->width -1)/2) *(winXYZ.x + 1);
	winXYZ.y = ((w->height-1)/2) *(winXYZ.y + 1);
	winXYZ.z = ((w->depth -1)/2) *(winXYZ.z + 1);
	
	if(plane >= PLUS_XY && plane <= PLUS_YZ) {
		winXYZ.z *= -1; // ceci permet d'inverser la profondeur.
	}

	return winXYZ;
}


void freeIntMatrix(int width, int **matrix) {
	for (int w = 0; w < width; ++w)
	{
		free(matrix[w]);
	}
	free(matrix);
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

float isFaceEnlighted(vec3f_t luminance, vec3f_t *vect3tmp) {

	vec3f_t cProduct= {0, 0, 0};
	float dProduct = 0;

	cProduct = crossProduct(vect3tmp[0], vect3tmp[1], vect3tmp[2]);	
	dProduct = dotProduct(luminance, cProduct);

	return dProduct;
}

