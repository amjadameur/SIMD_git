#include "model.h"

vector_t * g_vertex;
vector_t * g_norm;
vector_t * g_texcoord;
vector_t * g_face;

void freeVectors () {
	VectorDelete(g_vertex);
	VectorDelete(g_norm);
	VectorDelete(g_texcoord);
	VectorDelete(g_face);
}

vector_t * ModelVertices() {
	return g_vertex;
}

vector_t * ModelNormals() {
	return g_norm;
}

vector_t * ModelTexcoords() {
	return g_texcoord;
}

vector_t * ModelFaces() {
	return g_face;
}

vec3f_t ModelGetVertex( int index ) {
	vec3f_t v = *(vec3f_t*)VectorGetFromIdx( ModelVertices(), index );
	return v;
}

vec3f_t ModelGetNormal( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelNormals(), index );
	return t;
}

vec3f_t ModelGetTexcoord( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelTexcoords(), index );
	return t;
}

face_t ModelGetFace( int index ) {
	face_t f = *(face_t*)VectorGetFromIdx( ModelFaces(), index );
	return f;
}

void jumpLine(void) {
	printf("\n");
}

void showVectors(void) {
	int f1[3]; int f2[3]; int f3[3];

	printf("\n");

	for (int i = 0; i < g_vertex->count; ++i)
	{
		printf("v %f %f %f\n", ModelGetVertex(i).x, ModelGetVertex(i).y, ModelGetVertex(i).z);
	}

	printf("\n");

	for (int i = 0; i < g_texcoord->count; ++i)
	{
		printf("vt %f %f %f\n", ModelGetTexcoord(i).x, ModelGetTexcoord(i).y, ModelGetTexcoord(i).z);
	}

	printf("\n");

	for (int i = 0; i < g_norm->count; ++i)
	{
		printf("vn %f %f %f\n", ModelGetNormal(i).x, ModelGetNormal(i).y, ModelGetNormal(i).z);
	}

	printf("\n");

	for (int i = 0; i < g_face->count; ++i)
	{
		for (int j = 0; j < 3; j++) {
			f1[j] = ModelGetFace(i).v [j] ;
			f2[j] = ModelGetFace(i).vt[j];
			f3[j] = ModelGetFace(i).vn[j];
		}
		printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n", f1[0], f2[0], f3[0], f1[1], f2[1], f3[1], f1[2], f2[2], f3[2]);
	}

	printf("\n");
}

bool ModelLoad(char * objfilename) {
	// Fonction à implementer
	size_t buffer_size = 100;
	char *buffer = (char*)malloc(buffer_size * sizeof(char));

	FILE *file = fopen(objfilename, "r");

	if(file == NULL)
	{
		printf("couldn't read file\n");
		return false;
	}

	g_vertex   = Vector();
	g_texcoord = Vector();
	g_norm     = Vector();
	g_face     = Vector();

	int readElements;
	float v1, v2, v3;
	char s[5];
	int f1[3]; int f2[3]; int f3[3];
	
	vec3f_t* vect3tmp = (vec3f_t*) malloc(sizeof(vec3f_t));
	face_t*  facetmp  = (face_t*)  malloc(sizeof(face_t));

	while(-1 != getline(&buffer, &buffer_size, file))
	{
		readElements = sscanf(buffer, "%s %d/%d/%d %d/%d/%d %d/%d/%d", s, &f1[0], &f2[0], &f3[0], &f1[1], &f2[1], &f3[1], &f1[2], &f2[2], &f3[2]);
		if (readElements == 10 && s[0] == 'f') {
			VectorAdd(g_face, (void*) malloc(sizeof(face_t)));
			facetmp = (face_t*) g_face->data[g_face->count-1];
			for (int i = 0; i < 3; i++) {
				facetmp->v[i]  = f1[i]-1;
				facetmp->vt[i] = f2[i]-1;
				facetmp->vn[i] = f3[i]-1;
			}
		} else {
			readElements = sscanf(buffer, "%s %f %f %f", s, &v1, &v2, &v3);
			if (readElements == 3 || readElements == 4) {
				if (strcmp(s, "v") == 0) {
					VectorAdd(g_vertex, (void *) malloc(sizeof(vec3f_t)));
					vect3tmp = (vec3f_t*) g_vertex->data[g_vertex->count-1];
					Vec3fSet(vect3tmp, v1, v2, v3);
				}
				else if (strcmp(s, "vt") == 0) {
					VectorAdd(g_texcoord, (void *) malloc(sizeof(vec3f_t)));
					vect3tmp = (vec3f_t*) g_texcoord->data[g_texcoord->count-1];
					Vec3fSet(vect3tmp, v1, v2, v3);
				}
				else if (strcmp(s, "vn") == 0) {
					VectorAdd(g_norm, (void *) malloc(sizeof(vec3f_t)));
					vect3tmp = (vec3f_t*) g_norm->data[g_norm->count-1];
					Vec3fSet(vect3tmp, v1, v2, v3);
				}
			}
		}
		buffer[0] = '\0';
	}

	showVectors();

	vect3tmp = NULL;
	facetmp = NULL;

	fclose(file);
	free(buffer);
	free(vect3tmp);
	free(facetmp);

	return true;
}

