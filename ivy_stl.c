#include "ivy.h"

stl_data_t stl_load(const char *input_filepath)
{
	stl_data_t stl_data;
	FILE *stl_file = fopen(input_filepath, "r");
	if (!stl_file) {
		WARN("IVY STL: Unable to read file at [%s]", input_filepath);
		stl_data.triangles_count = 0;
		return stl_data;
	}
	fseek(stl_file, 80, SEEK_SET);
	fread(&stl_data.triangles_count, 4, 1, stl_file);
	stl_data.triangles = IVY_MALLOC(sizeof(stl_face_t) * stl_data.triangles_count);
	for (size_t i = 0; i < stl_data.triangles_count; i++) {
		stl_face_t *t = &stl_data.triangles[i];

		fread(&t->normal.x, 4, 1, stl_file);
		fread(&t->normal.y, 4, 1, stl_file);
		fread(&t->normal.z, 4, 1, stl_file);

		fread(&t->vertex1.x, 4, 1, stl_file);
		fread(&t->vertex1.y, 4, 1, stl_file);
		fread(&t->vertex1.z, 4, 1, stl_file);

		fread(&t->vertex2.x, 4, 1, stl_file);
		fread(&t->vertex2.y, 4, 1, stl_file);
		fread(&t->vertex2.z, 4, 1, stl_file);

		fread(&t->vertex3.x, 4, 1, stl_file);
		fread(&t->vertex3.y, 4, 1, stl_file);
		fread(&t->vertex3.z, 4, 1, stl_file);

		// attribue byte count
		fseek(stl_file, 2, SEEK_CUR);
	}
	fclose(stl_file);
	return stl_data;
}

void stl_free(stl_data_t stl_data)
{
	IVY_FREE(stl_data.triangles);
}
