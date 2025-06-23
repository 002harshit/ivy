#ifndef IVY_STL_LOADER_C
#define IVY_STL_LOADER_C

#include "common.h"
#include "math.c"

typedef struct {
	vec3_t normal;
	vec3_t v1;
	vec3_t v2;
	vec3_t v3;
} triangle_t;

typedef struct {
	i32_t triangles_count;
	triangle_t *triangles;
} stl_data_t;

stl_data_t stl_load(const char *stl_filepath);
void stl_free(stl_data_t stl_data);
int stl_to_c(const char *stl_filepath, const char *output_filepath, const char *name);

// ------------------------ IMPLEMENTATION ------------------------ //
#ifdef IVY_STL_LOADER_IMPL

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

stl_data_t stl_load(const char *stl_filepath)
{
	stl_data_t stl_data;
	FILE *stl_file = fopen(stl_filepath, "r");
	fseek(stl_file, 80, SEEK_SET);
	fread(&stl_data.triangles_count, 4, 1, stl_file);
	// TODO: Let uses use their own malloc
	stl_data.triangles = malloc(sizeof(triangle_t) * stl_data.triangles_count);
	for (int i = 0; i < stl_data.triangles_count; i++) {
		triangle_t *t = &stl_data.triangles[i];

		fread(&t->normal.x, 4, 1, stl_file);
		fread(&t->normal.y, 4, 1, stl_file);
		fread(&t->normal.z, 4, 1, stl_file);

		fread(&t->v1.x, 4, 1, stl_file);
		fread(&t->v1.y, 4, 1, stl_file);
		fread(&t->v1.z, 4, 1, stl_file);

		fread(&t->v2.x, 4, 1, stl_file);
		fread(&t->v2.y, 4, 1, stl_file);
		fread(&t->v2.z, 4, 1, stl_file);

		fread(&t->v3.x, 4, 1, stl_file);
		fread(&t->v3.y, 4, 1, stl_file);
		fread(&t->v3.z, 4, 1, stl_file);

		// attribue byte count
		fseek(stl_file, 2, SEEK_CUR);
	}
	fclose(stl_file);
	return stl_data;
}

void stl_free(stl_data_t stl_data) {
	free(stl_data.triangles);
}

// TODO: return error code on failure
int stl_to_c(const char *stl_filepath, const char *output_filepath, const char *mesh_name)
{
	stl_data_t stl_data = stl_load(stl_filepath);
	FILE *out_file = fopen(output_filepath, "w");
	if (!out_file)
		return -1;

	size_t mesh_name_len = strlen(mesh_name);
	char *mesh_name_upper = malloc(mesh_name_len);
	memcpy(mesh_name_upper, mesh_name, mesh_name_len);
	for (size_t i = 0; i < mesh_name_len; i++) {
		mesh_name_upper[i] = toupper(mesh_name_upper[i]);
	}
	mesh_name_upper[mesh_name_len] = '\0';

	fprintf(out_file, "#ifndef %s_STL_H", mesh_name_upper);
	fprintf(out_file, "\n#define %s_STL_H", mesh_name_upper);
	fprintf(out_file, "\n\ntypedef struct {\n\tfloat x, y, z;\n} vec3_t;\n\ntypedef struct {\n\tvec3_t normal;\n\tvec3_t v1, v2, v3;\n} triangle_t;");	
	fprintf(out_file, "\n\nstatic const int %s_triangles_count = %d;", mesh_name, stl_data.triangles_count);
	fprintf(out_file, "\nstatic const triangle_t %s_data[%d] = {", mesh_name, stl_data.triangles_count);
	for (int i = 0; i < stl_data.triangles_count; i++) {
		triangle_t t = stl_data.triangles[i];
		fprintf(out_file, "\n\t {");
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.normal.x, t.normal.y, t.normal.z);
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.v1.x, t.v1.y, t.v1.z);
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.v2.x, t.v2.y, t.v2.z);
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.v3.x, t.v3.y, t.v3.z);
		fprintf(out_file, "\n\t},");
	}
	fprintf(out_file, "\n};");
	fprintf(out_file, "\n\n#endif // %s_STL_H", mesh_name_upper);
	fclose(out_file);
	free(mesh_name_upper);
	stl_free(stl_data);
	return 0;
}
#endif // IVY_STL_LOADER_IMPL
#endif // IVY_STL_LOADER_C
