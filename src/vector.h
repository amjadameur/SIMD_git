#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * D�finition des types
 */
typedef struct vector {
	void ** data;
    int size;
    int count;
}vector_t;


/**
 * D�finition des prototypes de fonctions
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
 * Ajoute un �l�ment dans un vecteur
 */
void					VectorAdd			( vector_t * v, void * data );

/**
 * Supprime un �l�ment dans un vecteur � l'index sp�cifi�
 */
void					VectorRemoveFromIdx		( vector_t * v, int idx );

/**
 * Supprime l'ensemble des �l�ments d'un vecteur
 */
void					VectorClear			( vector_t * v );

/**
 * Retourne l'�l�ment d'un vecteur � l'index sp�cifi�
 */
void				*	VectorGetFromIdx		( vector_t * v, int idx );

/**
 * Retourne le longueur du vecteur
 */
int					VectorGetLength			( vector_t * v );

/**
 * Retourne l'index d'un �l�ment du vecteur
 */
int					VectorGetDataIdx		( vector_t * v, void * dat );

/**
 * Retourne vrai si le vecteur ne contient pas d'�l�ment
 */
bool					VectorIsEmpty			( vector_t * v );

#endif // __VECTOR_H__
