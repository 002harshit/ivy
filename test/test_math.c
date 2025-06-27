#include <stdlib.h>
#include "../ivy.c"
#define RAYMATH_IMPLEMENTATION
#include "raymath.h"
#include <time.h>

void print_matrix(float* a) {
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++)  {
			printf("%f, ", a[j * 4 + i]);
		}
		printf("\n");
	}
}


void print_float_array(float* a, int size) {
	if (size == 16) {
		print_matrix(a);
		return;
	}
	for (int i = 0; i < size; i++)  {
		printf("%f, ", a[i]);
	}
	printf("\n");
}


void compare_float(const char* test_name, float a, float b) {
	if (!fequal(a, b)) {
		WARN("TEST FAILED: %s\nExpected: %f\nGot:%f", test_name, a, b);
	} else {
		INFO("TEST PASSED: %s", test_name);	
	}
}

void compare_float_array(const char* test_name, float* a, float* b, int size) {
	for(int i = 0; i < size; i++) {
		if (!fequal(a[i], b[i])) {
			WARN("TEST FAILED: %s", test_name);
			printf("Expected: ");
			print_float_array(a, size);
			printf("Got: ");
			print_float_array(b, size);
			return;
		}
	}
	INFO("TEST PASSED: %s", test_name);
}

void compare_vec2(const char* test_name, Vector2 a, vec2_t b) {
	compare_float_array(test_name, (float*) &a, (float*) &b, 2);
}

void compare_vec3(const char* test_name, Vector3 a, vec3_t b) {
	compare_float_array(test_name, (float*) &a, (float*) &b, 3);
}

void compare_mat4(const char* test_name, Matrix a, mat4_t b, int transpose) {
	if (transpose) {
		a = MatrixTranspose(a);
	}
	compare_float_array(test_name, (float*) &a, (float*) &b, 4 * 4);
}


Vector3 rand_vec3() {
	return (Vector3) {(float) rand() / rand(), (float) rand() / rand(), (float) rand() / rand()};
}

Matrix rand_mat4() {
	Matrix m = {0};
	float * k = (float*)&m;
	for(int i = 0; i < 4 * 4;i++) {
		k[i] = (float) rand() / rand();
	}
	return m;
}

void test_vec3() {
	Vector3 a, b, c, d, expect_f3;
	Matrix ma, mb;
	vec3_t got_f3;
	float expect, got, theta;

	a = rand_vec3();
	b = rand_vec3();
	c = rand_vec3();
	d = rand_vec3();
	ma = rand_mat4();
	theta = (float) rand() / rand();

	expect_f3 = Vector3Add(a, b);
	got_f3 = vec3_add(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_vec3("Add", expect_f3, got_f3);

	expect_f3 = Vector3Subtract(a, b);
	got_f3 = vec3_sub(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_vec3("Subtract", expect_f3, got_f3);

	expect_f3 = Vector3Multiply(a, b);
	got_f3 = vec3_mul(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_vec3("Multiply", expect_f3, got_f3);

	expect_f3 = Vector3Scale(a, theta);
	got_f3 = vec3_mulv(*(vec3_t*)&a, theta);
	compare_vec3("Multiply Value", expect_f3, got_f3);

	expect_f3 = Vector3Multiply(a, b);
	got_f3 = vec3_mul(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_vec3("Divide Value", expect_f3, got_f3);

	expect = Vector3DotProduct(a, b);
	got = vec3_dot(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_float("Dot Product", expect, got);
	
	expect_f3 = Vector3CrossProduct(a, b);
	got_f3 = vec3_cross(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_vec3("Cross Product", expect_f3, got_f3);

	expect_f3 = Vector3Transform(a, ma);
	mb = MatrixTranspose(ma);
	got_f3 = vec3_transform(*(vec3_t*)&a, *(mat4_t*)&mb);
	compare_vec3("Transform", expect_f3, got_f3);

	expect = Vector3Length(a);
	got = vec3_len(*(vec3_t*)&a);
	compare_float("Length", expect, got);

	expect = Vector3Angle(a, b);
	got = vec3_angle(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_float("Angle", expect, got);

	expect_f3 = Vector3Normalize(a);
	got_f3 = vec3_normalize(*(vec3_t*)&a);
	compare_vec3("Normalize", expect_f3, got_f3);

	expect_f3 = Vector3RotateByAxisAngle(a, b, theta);
	got_f3 = vec3_rotate_axis(*(vec3_t*)&a, *(vec3_t*)&b, theta);
	compare_vec3("Rotate Axis", expect_f3, got_f3);

	expect_f3 = Vector3Invert(a);
	got_f3 = vec3_invert(*(vec3_t*)&a);
	compare_vec3("Invert", expect_f3, got_f3);

	expect = Vector3Distance(a, b);
	got = vec3_dist(*(vec3_t*)&a, *(vec3_t*)&b);
	compare_float("Distance", expect, got);

	expect_f3 = Vector3Barycenter(a, a, c, d);
	got_f3 = vec3_barycentric(*(vec3_t*)&a, *(vec3_t*)&a, *(vec3_t*)&c, *(vec3_t*)&d);
	compare_vec3("Barycentric", expect_f3, got_f3);
}

void test_mat4() {
	Vector3 a, b, c, d, expect_f3;
	Matrix ma, mb, tma, tmb, expect_m4;
	vec3_t got_f3;
	mat4_t got_m4;
	float expect, got, theta;
	a = rand_vec3();
	b = rand_vec3();
	c = rand_vec3();
	d = rand_vec3();
	ma = rand_mat4();
	mb = rand_mat4();
	tma = MatrixTranspose(ma);
	tmb = MatrixTranspose(mb);
	theta = (float) rand() / rand();

	expect_m4 = MatrixIdentity();
	got_m4 = mat4_identity();
	compare_mat4("Identity", expect_m4, got_m4, 0);

	expect_m4 = MatrixMultiply(ma, mb);
	got_m4 = mat4_mul(*(mat4_t*)&mb, *(mat4_t*)&ma);
	compare_mat4("Multiply", expect_m4, got_m4, 0);

	expect_m4 = MatrixTranspose(ma);
	got_m4 = mat4_transpose(*(mat4_t*)&ma);
	compare_mat4("Transpose", expect_m4, got_m4, 0);

	expect_m4 = MatrixTranslate(a.x, a.y, a.z);
	got_m4 = mat4_translate(*(vec3_t*)&a);
	compare_mat4("Translate", expect_m4, got_m4, 1);

	expect_m4 = MatrixRotate(a, theta);
	got_m4 = mat4_rotate_axis(*(vec3_t*)&a, theta);
	compare_mat4("Rotate", expect_m4, got_m4, 1);

	expect_m4 = MatrixLookAt(a, b, c);
	got_m4 = mat4_lookat(*(vec3_t*)&a, *(vec3_t*)&b, *(vec3_t*)&c);
	compare_mat4("LookAt", expect_m4, got_m4, 1);

	expect_m4 = MatrixOrtho(a.x, a.y, a.z, b.x, b.y, b.z);
	got_m4 = mat4_ortho(a.x, a.y, a.z, b.x, b.y, b.z);
	compare_mat4("Ortho", expect_m4, got_m4, 1);

	expect_m4 = MatrixPerspective(a.x, a.y, a.z, b.x);
	got_m4 = mat4_perspective(a.x, a.y, a.z, b.x);
	compare_mat4("Perspective", expect_m4, got_m4, 1);
	

}

int main() {
	srand(time(NULL));
	INFO("----------------- TESTING VEC3 -----------------");
	test_vec3();
	INFO("----------------- TESTING MAT4 -----------------");
	test_mat4();
	return 0;
}
