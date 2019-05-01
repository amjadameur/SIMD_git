#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <math.h>
#include <stdlib.h>

/**
 * Définition des types
 */
typedef float ** matrixf_t;

typedef struct vec2f		{ float x; float y;			} vec2f_t;
typedef struct vec3f		{ float x; float y; float z;		} vec3f_t;
typedef struct vec2i		{ int x; int y;				} vec2i_t;
typedef struct vec3i		{ int x; int y; int z;			} vec3i_t;
typedef struct face		{ int v[ 3 ]; int vt[ 3 ]; int vn[ 3 ];	} face_t;

/**
 * Définition des macros
 */

/**
 * Macro pour le calcul du min et du max
 */

#define MIN( X, Y ) ( ( ( X ) < ( Y ) ) ? ( X ) : ( Y ) )
#define MAX( X, Y ) ( ( ( X ) > ( Y ) ) ? ( X ) : ( Y ) )

/**
 * Macro d'affichage d'un matrice de dimension m x n
 */

#define DBG_PRINT_MATRIX( mtx, m, n ) \
    for ( int i = 0; i < m; i++) { \
        for ( int j = 0; j < n ; j++) { \
            printf( "%f ", mtx[ i ][ j ] ); \
        } \
		printf( "\n" ); \
    } \

/**
 * Définition des prototypes de fonctions et implémentation des fonctions inline
 */

/**
 * Construit une matricee flottante de dimension n x m
 */
matrixf_t	Matrixf		( int n, int m );

/**
 * Supprime une matrice flottante de dimension n
 */
void		MatrixfDelete	( matrixf_t m, int n );

/**
 * Construit une matrice identité flottante de dimension n x n
 */
matrixf_t	MatrixfIdentity	( int n );

/**
 * Multiply deux matrices flottante de dimension n x m
 */
matrixf_t	MatrixfMult	( matrixf_t a, matrixf_t b, int n, int m );

/**
 * Construit une matrice viewport flottante
 */
matrixf_t	MatrixfViewport	( int x, int y, int w, int h );

/**
 * Construit une matrice lookat flottante
 */
matrixf_t	MatrixfLookAt	( vec3f_t eye, vec3f_t center, vec3f_t up );

/**
 * Echange deux entiers entre eux
 */
inline void swap( int * a, int * b ) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Construit un vecteur flottant de dimension 3 et positonne ses composantes
 */
inline vec3f_t Vec3f( float x, float y, float z ) {
	vec3f_t v; v.x = x; v.y = y; v.z = z;
	return v;
}

/**
 * Positonne les composante d'un vecteur flottant de dimension 3
 */

inline void Vec3iSet( vec3i_t * v, int x, int y, int z ) {
	v->x = x; v->y = y; v->z = z;
}

inline void Vec3fSet( vec3f_t * v, float x, float y, float z ) {
	v->x = x; v->y = y; v->z = z;
}

/**
 * Soustrait deux vecteurs flottant de dimension 3
 */
inline vec3f_t Vec3fSub( vec3f_t a, vec3f_t b ) {
	vec3f_t r;
	r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z;
	return r;
}

/**
 * Calcul la longueur d'un vecteur flottant de dimension 3
 */
inline float Vec3fLength( vec3f_t v ) {
	return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
}

/**
 * Normalise un vecteur flottant de dimension 3
 */
inline vec3f_t Vec3fNormalize( vec3f_t v ) {
	vec3f_t r;
	float l = Vec3fLength( v );
	r.x = v.x / l; r.y = v.y / l; r.z = v.z / l;
	return r;
}

/**
 * Effectue le produit vectoriel entre deux vecteurs flottants de dimension 3
 */
inline vec3f_t Vec3fCross( vec3f_t v1, vec3f_t v2 ) {
	vec3f_t r;
	r.x = v1.y * v2.z - v1.z * v2.y; r.y = v1.z * v2.x - v1.x * v2.z; r.z = v1.x * v2.y - v1.y * v2.x;
	return r;
}

/**
 * Echange le contenu de deux vecteurs flottants de dimension 3
 */
inline void Vec3fSwap( vec3f_t * v1, vec3f_t * v2 ) {
	vec3f_t v = *v1;
	*v1 = *v2;
	*v2 = v;
}

/**
 * Echange le contenu de deux vecteurs flottants de dimension 2
 */
inline void Vec2fSwap( vec2f_t * v1, vec2f_t * v2 ) {
	vec2f_t v = *v1;
	*v1 = *v2;
	*v2 = v;

}

/**
 * Echange le contenu de deux vecteurs entiers de dimension 2
 */
inline void Vec3iSwap( vec3i_t * v1, vec3i_t * v2 ) {
	vec3i_t v = *v1;
	*v1 = *v2;
	*v2 = v;
}

/**
 * Convertit une matrice flottante 4x1 en vecteur flottant de dimension 3
 */
inline vec3f_t Matrixf2Vec3f( matrixf_t m ) {
	vec3f_t v;
	v.x = m[ 0 ][ 0 ] / m[ 3 ][ 0 ]; v.y = m[ 1 ][ 0 ] / m[ 3 ][ 0 ]; v.z = m[ 2 ][ 0 ] / m[ 3 ][ 0 ];
	return v;
}

/**
 * Convertit un vecteur flottant de dimension 3 en une matrice flottante de dimension 4x1
 */
inline matrixf_t Vec3f2Matrixf( vec3f_t v ) {
	matrixf_t m = Matrixf( 4, 1 );
	m[ 0 ][ 0 ] = v.x; m[ 1 ][ 0 ] = v.y; m[ 2 ][ 0 ] = v.z; m[ 3 ][ 0 ] = 1.0f;
	return m;
}

#endif //__GEOMETRY_H__
