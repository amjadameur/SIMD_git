#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Définition des types
 */
typedef struct vector {
	void ** data;
    int size;
    int count;
}vector_t;


/**
 * Définition des prototypes de fonctions
 */

/**
 * Construit un vecteur
 */
vector_t			*	Vector				();

/**
 * Supprime un vecteur
 */
void					VectorDelete			( vector_t * v );

/**
 * Ajoute un élément dans un vecteur
 */
void					VectorAdd			( vector_t * v, void * data );

/**
 * Supprime un élément dans un vecteur à l'index spécifié
 */
void					VectorRemoveFromIdx		( vector_t * v, int idx );

/**
 * Supprime l'ensemble des éléments d'un vecteur
 */
void					VectorClear			( vector_t * v );

/**
 * Retourne l'élément d'un vecteur à l'index spécifié
 */
void				*	VectorGetFromIdx		( vector_t * v, int idx );

/**
 * Retourne le longueur du vecteur
 */
int					VectorGetLength			( vector_t * v );

/**
 * Retourne l'index d'un élément du vecteur
 */
int					VectorGetDataIdx		( vector_t * v, void * dat );

/**
 * Retourne vrai si le vecteur ne contient pas d'élément
 */
bool					VectorIsEmpty			( vector_t * v );

#endif // __VECTOR_H__
