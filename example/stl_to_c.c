#include "../ivy.h"

void stl_to_c(const char *input_path, const char *output_path, const char *mesh_name)
{
	stl_data_t stl_data = stl_load(input_path);
	FILE *out = fopen(output_path, "w");
	if (!out) {
		stl_free(stl_data);
		FATAL("Unable to write output file at [%s]", output_path);
	}

	fprintf(out, "static const size_t %s_triangles_count = %zu;", mesh_name,
			stl_data.triangles_count);
	fprintf(out, "\nstatic const float %s_data[%zu][4][3] = {", mesh_name,
			stl_data.triangles_count);
	for (size_t i = 0; i < stl_data.triangles_count; i++) {
		stl_face_t t = stl_data.triangles[i];
		fprintf(out, "\n\t{");
		fprintf(out, "\n\t\t{ %f, %f, %f },", t.normal.x, t.normal.y, t.normal.z);
		fprintf(out, "\n\t\t{ %f, %f, %f },", t.vertex1.x, t.vertex1.y, t.vertex1.z);
		fprintf(out, "\n\t\t{ %f, %f, %f },", t.vertex2.x, t.vertex2.y, t.vertex2.z);
		fprintf(out, "\n\t\t{ %f, %f, %f },", t.vertex3.x, t.vertex3.y, t.vertex3.z);
		fprintf(out, "\n\t},");
	}
	fprintf(out, "\n};");
	fclose(out);
	stl_free(stl_data);
}

int main(int argc, const char *argv[])
{
	if (argc < 2) {
		FATAL("Provide a input stl path");
	}
	const char* input_path = argv[1];
	
	if (argc < 3) {
		FATAL("Provide a output path");
	}
	const char* output_path = argv[2];

	if (argc < 4) {
		FATAL("Provide a mesh name");
	}
	const char* mesh_name = argv[3];
	
	stl_to_c(input_path, output_path, mesh_name);
	return 0;
}
