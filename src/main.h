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
	// fonction servant à récuperer la texture d'un point dans le fichier tga
	Uint8 * ptr = tgaData.tgaIm + 4*x + (4*tgaData.tgaWidth)*y;
	*b = *ptr++;
	*g = *ptr++;
	*r = *ptr++;
}

void findRgb(int *textureIdx, tgaInfo tgaData, Uint8* r, Uint8* g, Uint8* b) {
	// récuperer et calculer la moyenne des textures des 3 points du triangle
	int x = 0, y = 0;

	for (int i = 0; i < 3; ++i)	{
		x += tgaData.tgaWidth *ModelGetTexcoord(textureIdx[i]).x;
		y += tgaData.tgaHeight*ModelGetTexcoord(textureIdx[i]).y;
	}

	x /= 3;
	y /= 3;

	y = tgaData.tgaHeight-1 - y;

	tgaRgb(tgaData, x, y, r, g, b);	
}


windowXYZ getXYZ(window_t* w, chosenPlane plane, vec3f_t vertexVect) {
	// Récuperer les coordonnées du sommet par rapport au plan demandé
	windowXYZ winXYZ;

	// Les coordonnées du sommet dans le fichier .obj
	float xIm(vertexVect.x);
	float yIm(vertexVect.y);
	float zIm(vertexVect.z);

	// Multiplexage des coordonnées suivant le plan demané
	if(plane == MINUS_XY || plane == PLUS_XY) winXYZ = {xIm, yIm, zIm}; // profondeur en Z
	if(plane == MINUS_YZ || plane == PLUS_YZ) winXYZ = {zIm, yIm, xIm}; // profondeur en X
	if(plane == MINUS_XZ || plane == PLUS_XZ) winXYZ = {xIm, zIm, yIm}; // profondeur en Y

	// Adaptation des coordonnées aux dimensions de la fenêtre d'affichage
	winXYZ.x = ((w->width -1)/2) *(winXYZ.x + 1);
	winXYZ.y = ((w->height-1)/2) *(winXYZ.y + 1);
	winXYZ.z = ((w->depth -1)/2) *(winXYZ.z + 1);
	
	// Nous inversons la pronfondeur dans le cas écheant 
	if(plane >= PLUS_XY && plane <= PLUS_YZ) {
		winXYZ.z *= -1; 
	}

	return winXYZ;
}



float dotProduct(vec3f_t a, vec3f_t b) {
	// Produit scalaire
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float isFaceEnlighted(vec3f_t luminance, vec3f_t *vect3tmp) {
	// cette fonction calcule la normale du triangle, puis fait le prduit scalaire de cette normale avec la source de la lumière

	// on prend vect3tmp[0] comme référence
	vec3f_t v1 = Vec3fSub(vect3tmp[1], vect3tmp[0]); 
	vec3f_t v2 = Vec3fSub(vect3tmp[2], vect3tmp[0]);
	
	// produit vectoriel
	vec3f_t cProduct = Vec3fCross(v2, v1);

	// normalisation
	cProduct = Vec3fNormalize(cProduct);
	
	// produit scalaire
	return dotProduct(luminance, cProduct);
}

void freeIntMatrix(int width, int **matrix) {
	for (int w = 0; w < width; ++w)
	{
		free(matrix[w]);
	}
	free(matrix);
}
