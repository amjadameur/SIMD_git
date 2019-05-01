#include "vector.h"

vector_t * Vector() {
	vector_t * v = (vector_t *)malloc( sizeof( vector_t ) );
	if ( v != NULL ) {
		v->data  = NULL;
		v->size  = 0;
		v->count = 0;
	}
	return v;
}

void VectorClear( vector_t * v ) {
	if ( v != NULL ) {
		if ( v->count != 0 ) {
			if ( v->data != NULL ) {
				for ( int i = 0; i < v->count; i++ ) {
					if ( v->data[ i ] != NULL ) {
						free( v->data[ i ] );
					}
				}
				free( v->data );
			}
			v->count = 0;
			v->size  = 0;
		}
	}else {
		printf( "(EE) Unable to clear vector\n" );
	}
}

void VectorDelete( vector_t * v ) {
	if ( v != NULL ) {
		if ( v->data != NULL ) {
			for ( int i = 0; i < v->count; i++ ) {
				if ( v->data[ i ] != NULL ) {
					free( v->data[ i ] );
				}
			}
			free( v->data );
		}
		free( v );
	}else {
		printf( "(EE) Unable to delete vector\n" );
	}
}

void VectorAdd( vector_t * v, void * data ) {
	if ( v != NULL ) {
		if ( data != NULL ) {
			if ( v->size == 0 ) {
				v->size = 10;
				v->data = (void**)malloc( sizeof( void * ) * v->size );
				if ( v->data != NULL ) {
					memset( v->data, '\0', sizeof( void * ) * v->size );
				}else {
					printf( "(EE) Unable to add data to vector\n" );
					return;
				}
			} 
			if ( v->size == v->count ) {
				v->size *= 2;
				void ** nv = (void **)realloc( v->data, sizeof( void * ) * v->size );
				if ( nv != NULL ) {
					v->data = nv;
				}else {
					printf( "(EE) Unable to realloc vector\n" );
					return;
				}
			}
			v->data[ v->count ] = data;
			v->count++;
		}
	}
}

void VectorRemoveFromIdx( vector_t * v, int idx ) {
	if ( v != NULL ) {
		if ( idx >= v->count ) {
			return;
		}
 		
		v->data[ idx ] = NULL;

		void ** arr = (void **)malloc( sizeof( void * ) * v->count * 2 );

		if ( arr != NULL ) {
			for ( int i = 0, j = 0; i < v->count; i++ ) {
				if ( v->data[ i ] != NULL ) {
					arr[ j ] = v->data[ i ];
					j++;
				}		
			}
		
			free( v->data );

			v->data = arr;
 			v->size = v->count * 2;
			v->count--;
		}
	}
}

void * VectorGetFromIdx( vector_t * v, int idx ) {
	if ( v != NULL ) {
		if ( idx >= v->count ) {
			return NULL;
		}
		return v->data[ idx ];
	}
	return NULL;
}

int VectorGetLength( vector_t * v ) {
	if ( v != NULL ) {
		return v->count;
	}
	return 0;
}

int VectorGetDataIdx( vector_t * v, void * dat ) {
	if ( v != NULL ) {
		for ( int i = 0; i < v->count; i++ ) {
			if ( v->data[ i ] == dat ) {
				return i;
			}		
		}
	}
	return -1;
}

bool VectorIsEmpty( vector_t * v ) {
	if ( v != NULL ) {
		if ( v->count == 0 ) {
			return true;
		}
	}
	return false;
}
