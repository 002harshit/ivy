#define IVY_STL_LOADER_IMPL
#include "../ivy.c"

int main(int argc, const char** argv) {
	if (argc < 2) {
		FATAL("Provide a input stl filepath");
	}
	const char* input_stl = argv[1];
	const char* output_c = "output.stl.c";
	const char* name = "output";
	if (argc >= 3) {
		output_c = argv[2];
	}
	if (argc >= 4) {
		name = argv[3];
	}
	stl_to_c(input_stl, output_c, name);
	return 0;
}
