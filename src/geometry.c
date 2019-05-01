#include "geometry.h"

matrixf_t Matrixf( int n, int m ) {
	float ** a = (float **)malloc( sizeof( float * ) * n );
	if ( a ) {
		for ( int i = 0; i < n; i++ ) {
            		a[ i ] = (float*)malloc( sizeof( float ) * m );
            		if ( a[ i ] == NULL ) {
                		return NULL;
            		}
        	}
    	}
    	for ( int i = 0; i < n; i++ ) {
        	for ( int j = 0; j < m; j++ ) {
            		a[ i ][ j ] = 0.0f;
        	}
    	}
	return a;
}

void MatrixfDelete( matrixf_t m, int n ) {
	for ( int i = 0; i < n; i++) {
		free( m[ i ] );
	}
    	free( m );
}

matrixf_t MatrixfIdentity( int n ) {
    	matrixf_t m = Matrixf( n, n );
    	for ( int i = 0; i < n; i++) {
        	for ( int j = 0; j < n ; j++) {
            	m[ i ][ j ] = ( i == j ? 1.0f : 0.0f );
        	}
    	}
    	return m;
}

matrixf_t MatrixfMult( matrixf_t a, matrixf_t b, int n, int m ) {
    	matrixf_t mtx = Matrixf( n, m );
    	for ( int i = 0; i < n; i++ ) {
        	for ( int j = 0; j < m; j++ ) {
            	mtx[ i ][ j ] = 0.0f;
            	for ( int k = 0; k < n; k++ ) {
                	mtx[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];
            	}
        	}
    	}
    	return mtx;
}

matrixf_t MatrixfViewport( int x, int y, int w, int h ) {
	// Fonction à implementer
    	matrixf_t m = MatrixfIdentity( 4 );
    	return m;
}

matrixf_t MatrixfLookAt( vec3f_t eye, vec3f_t center, vec3f_t up ) {
	// Fonction à implementer
    	matrixf_t r = MatrixfIdentity( 4 );
    	return r;
}
