#define IVY_STL_LOADER_IMPL
#include "../stl_loader.c"
int main() {
	stl_to_c("cube.stl","cube.stl.c","cube");
}
