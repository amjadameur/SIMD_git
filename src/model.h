#ifndef __MODEL_H__
#define __MODEL_H__

#include "vector.h"
#include "geometry.h"
#include "window.h"



   

/**
 * Définition des prototypes de fonctions
 */

/**
   Libérer la mémoire allouée aux vecteurs :
*/
void 			freeVectors         ();


/**
 * Retourne la liste des sommets du modèle
 */
vector_t	*	ModelVertices		();

/**
 * Retourne la liste des normales du modèle
 */
vector_t	*	ModelNormals		();

/**
 * Retourne la liste des coordonnées de texture du modèle
 */
vector_t	*	ModelTexcoords		();

/**
 * Retourne la liste des faces du modèle
 */
vector_t	*	ModelFaces		();

/**
 * Retourne le sommet du modèle à l'index spécifié
 */
vec3f_t			ModelGetVertex		( int idx );

/**
 * Retourne la normale du modèle à l'index spécifié
 */
vec3f_t			ModelGetNormal		( int idx );

/**
 * Retourne les coordonnés de texture du modèle à l'index spécifié
 */
vec3f_t			ModelGetTexcoord	( int idx );

/**
 * Retourne la face du modèle à l'index spécifié
 */
face_t			ModelGetFace		( int idx );

/**
 * Charge un modèle 3D à partir du fichier spécifié
 */
bool			ModelLoad		(char * objfilename );

#endif // __MODEL_H__
