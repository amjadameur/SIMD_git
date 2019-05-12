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

typedef enum {MINUS_XY, PLUS_XZ, MINUS_YZ, PLUS_XY, PLUS_XZ, MINUS_YZ} chosenPlane;


typedef struct tga_info {
	int tgaWidth;
	int tgaHeight;
	unsigned char *tgaIm;
} tgaInfo;

typedef struct window_XYZ {float x; float y; float z;} windowXYZ; 

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
	
	if(plane >= MINUS_XY && plane <= MINUS_YZ) {
		winXYZ.z *= -1; // ceci permet d'inverser la profondeur.
	}

	return winXYZ;
}