#ifndef IVY_C
#define IVY_C

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
// COMMON DEFINATIONS
// ---------------------------------------------------------------------------
typedef bool bool_t;
typedef unsigned char u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
typedef signed char i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;

#define ANSI_BLACK "\033[0;30m"
#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_PURPLE "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_WHITE "\033[0;37m"

#define ANSI_BOLD_BLACK "\033[1;30m"
#define ANSI_BOLD_RED "\033[1;31m"
#define ANSI_BOLD_GREEN "\033[1;32m"
#define ANSI_BOLD_YELLOW "\033[1;33m"
#define ANSI_BOLD_BLUE "\033[1;34m"
#define ANSI_BOLD_PURPLE "\033[1;35m"
#define ANSI_BOLD_CYAN "\033[1;36m"
#define ANSI_BOLD_WHITE "\033[1;37m"

#define ANSI_UNDERLINE_BLACK "\033[4;30m"
#define ANSI_UNDERLINE_RED "\033[4;31m"
#define ANSI_UNDERLINE_GREEN "\033[4;32m"
#define ANSI_UNDERLINE_YELLOW "\033[4;33m"
#define ANSI_UNDERLINE_BLUE "\033[4;34m"
#define ANSI_UNDERLINE_PURPLE "\033[4;35m"
#define ANSI_UNDERLINE_CYAN "\033[4;36m"
#define ANSI_UNDERLINE_WHITE "\033[4;37m"

#define ANSI_BG_BLACK "\033[40m"
#define ANSI_BG_RED "\033[41m"
#define ANSI_BG_GREEN "\033[42m"
#define ANSI_BG_YELLOW "\033[43m"
#define ANSI_BG_BLUE "\033[44m"
#define ANSI_BG_PURPLE "\033[45m"
#define ANSI_BG_CYAN "\033[46m"
#define ANSI_BG_WHITE "\033[47m"

#define ANSI_RESET "\033[0m"

#define WARN(...)                                                                                  \
	fprintf(stderr, ANSI_RED "WARNING: " ANSI_WHITE __VA_ARGS__);                                  \
	printf(ANSI_RESET "\n")

#define FATAL(...)                                                                                 \
	fprintf(stdout, ANSI_RED "FATAL: " ANSI_WHITE __VA_ARGS__);                                    \
	printf(ANSI_RESET "\n");                                                                       \
	exit(1)

#define INFO(...)                                                                                  \
	fprintf(stdout, ANSI_BLUE "INFO: " ANSI_WHITE __VA_ARGS__);                                    \
	printf(ANSI_RESET "\n")

#define TODO(str) FATAL("%s:%d NOT IMPLEMENTED: %s", __FILE__, __LINE__, str)

#ifdef IVY_DEBUG
#define ASSERT(n)                                                                                  \
	if (!(n)) {                                                                                    \
		printf(__FILE__ "%s:%d ASSERT FAILED ( %s )", __FILE__, __LINE__, #n);                     \
		exit(1);                                                                                   \
	}
#else
#define ASSERT(n)
#endif

// source:
// https://github.com/ramdeoshubham/macros/blob/c37a57f102921ab7abb8c50a093dfc007049dc53/macros.h#L128
#define DEFER2(head, tail, i) for (int i = (head, 0); !i; tail, i++)
#define DEFER(head, tail) DEFER2(head, tail, __deferVar__##__COUNTER__)

#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

// clang-fomat off
// clang-fomat on

// ---------------------------------------------------------------------------
// IVY MATH DEFINATIONS
// ---------------------------------------------------------------------------
#define PI 3.1415926535897932384626433832795

typedef struct {
	float x, y;
} vec2_t;

typedef struct {
	float x, y, z;
} vec3_t;

// Row Major Matrix
typedef struct {
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
} mat4_t;

// ---------------------------------------------------------------------------
// IVY STL COLOR
// ---------------------------------------------------------------------------
typedef struct {
	u8_t r, g, b, a;
} color_t;

// ---------------------------------------------------------------------------
// IVY STL LOADER DEFINATIONS
// ---------------------------------------------------------------------------
typedef struct {
	vec3_t normal;
	vec3_t vertex1;
	vec3_t vertex2;
	vec3_t vertex3;
} ivy_triangle_t;

typedef struct {
	i32_t triangles_count;
	ivy_triangle_t *triangles;
} stl_data_t;

stl_data_t stl_load(const char *stl_filepath);
void stl_free(stl_data_t stl_data);
int stl_to_c(const char *stl_filepath, const char *output_filepath, const char *name);

// ---------------------------------------------------------------------------
// IVY MATH IMPLEMENTATION
// ---------------------------------------------------------------------------
#define IVY_MATH_API static inline
#define EPSILON 0.00001
#define SQ(x) ((x) * (x))

// ---------------------------------------------------------------------------
// FLOAT
// ---------------------------------------------------------------------------
IVY_MATH_API float deg_to_rad(float angle_rad)
{
	return angle_rad * PI / 180.0;
}

IVY_MATH_API float fwrap(float value, float min, float max)
{
	return value - (max - min) * floorf((value - min) / (max - min));
}

IVY_MATH_API bool fequal(float x, float y)
{
	return (fabsf(x - y)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))));
}

IVY_MATH_API float fclamp(float value, float min, float max)
{
	float res = (value < min) ? min : value;
	return (res > max) ? max : res;
}

IVY_MATH_API float flerp(float a, float b, float t)
{
	return a + t * (b - a);
}

IVY_MATH_API float fnormalize(float value, float min, float max)
{
	return (value - min) / (max - min);
}

IVY_MATH_API float fremap(float value, float smin, float smax, float emin, float emax)
{
	return emin + (value - smin) * (emax - emin) / (smax - smin);
}

// ---------------------------------------------------------------------------
// VECTOR2
// ---------------------------------------------------------------------------
IVY_MATH_API vec2_t vec2(float x, float y)
{
	return (vec2_t){x, y};
}

IVY_MATH_API vec2_t vec2_zero()
{
	return (vec2_t){0, 0};
}

IVY_MATH_API vec2_t vec2_add(vec2_t a, vec2_t b)
{
	return (vec2_t){a.x + b.x, a.y + b.y};
}

IVY_MATH_API vec2_t vec2_sub(vec2_t a, vec2_t b)
{
	return (vec2_t){a.x - b.x, a.y - b.y};
}

IVY_MATH_API vec2_t vec2_mul(vec2_t a, vec2_t b)
{
	return (vec2_t){a.x * b.x, a.y * b.y};
}

IVY_MATH_API vec2_t vec2_addv(vec2_t a, float v)
{
	return (vec2_t){a.x + v, a.y + v};
}

IVY_MATH_API vec2_t vec2_subv(vec2_t a, float v)
{
	return (vec2_t){a.x - v, a.y - v};
}

IVY_MATH_API vec2_t vec2_mulv(vec2_t a, float v)
{
	return (vec2_t){a.x * v, a.y * v};
}

IVY_MATH_API vec2_t vec2_divv(vec2_t a, float v)
{
	return (vec2_t){a.x / v, a.y / v};
}

IVY_MATH_API float vec2_lensq(vec2_t a)
{
	return SQ(a.x) + SQ(a.y);
}

IVY_MATH_API float vec2_len(vec2_t a)
{
	return sqrtf(SQ(a.x)) + (SQ(a.y));
}

IVY_MATH_API float vec2_distsq(vec2_t a, vec2_t b)
{
	return SQ(a.x - b.x) + SQ(a.y - b.y);
}

IVY_MATH_API float vec2_dist(vec2_t a, vec2_t b)
{
	return sqrtf(SQ(a.x - b.x) + SQ(a.y - b.y));
}

IVY_MATH_API vec2_t vec2_wrap(vec2_t value, vec2_t min, vec2_t max)
{
	return (vec2_t){fwrap(value.x, min.x, max.x), fwrap(value.y, min.y, max.y)};
}

IVY_MATH_API vec2_t vec2_wrap2(vec2_t value, float min, float max)
{
	return (vec2_t){fwrap(value.x, min, max), fwrap(value.y, min, max)};
}

IVY_MATH_API bool vec2_is_almost_eq(vec2_t a, vec2_t b)
{
	return fequal(a.x, b.x) && fequal(a.y, b.y);
}

IVY_MATH_API vec2_t vec2_clamp(vec2_t value, vec2_t min, vec2_t max)
{
	return (vec2_t){fclamp(value.x, min.x, max.x), fclamp(value.y, min.y, max.y)};
}

IVY_MATH_API vec2_t vec2_clamp2(vec2_t value, float min, float max)
{
	return (vec2_t){fclamp(value.x, min, max), fwrap(value.y, min, max)};
}

IVY_MATH_API vec2_t vec2_lerp(vec2_t a, vec2_t b, float t)
{
	return (vec2_t){flerp(a.x, b.x, t), flerp(a.y, b.y, t)};
}

IVY_MATH_API vec2_t vec2_normalize(vec2_t a)
{
	float l = vec2_len(a);
	if (l > 0)
		return (vec2_t){a.x / l, a.y / l};
	return vec2_zero();
}

IVY_MATH_API vec2_t vec2_invert(vec2_t a)
{
	return (vec2_t){1.0 / a.x, 1.0 / a.y};
}

IVY_MATH_API float vec2_dot(vec2_t a, vec2_t b)
{
	return a.x * b.x + a.y * b.y;
}

IVY_MATH_API float vec2_cross(vec2_t a, vec2_t b)
{
	return a.x * b.y - a.y * b.x;
}

IVY_MATH_API float vec2_angle(vec2_t a, vec2_t b)
{
	return atan2f(vec2_cross(a, b), vec2_dot(a, b));
}

IVY_MATH_API vec2_t vec2_rotate(vec2_t a, float angle_rad)
{
	float c = cosf(angle_rad);
	float s = sinf(angle_rad);
	return (vec2_t){a.x * c - a.y * s, a.x * s + a.y * c};
}

// TODO: Reflect, refract, transform

// ---------------------------------------------------------------------------
// VECTOR3
// ---------------------------------------------------------------------------

IVY_MATH_API vec3_t vec3_zero()
{
	return (vec3_t){0, 0, 0};
}

IVY_MATH_API vec3_t vec3(float x, float y, float z)
{
	return (vec3_t){x, y, z};
}

IVY_MATH_API vec3_t vec3_new2(float x)
{
	return (vec3_t){x, x, x};
}

IVY_MATH_API vec3_t vec3_add(vec3_t a, vec3_t b)
{
	return (vec3_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

IVY_MATH_API vec3_t vec3_sub(vec3_t a, vec3_t b)
{
	return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

IVY_MATH_API vec3_t vec3_mul(vec3_t a, vec3_t b)
{
	return (vec3_t){a.x * b.x, a.y * b.y, a.z * b.z};
}

IVY_MATH_API vec3_t vec3_addv(vec3_t a, float v)
{
	return (vec3_t){a.x + v, a.y + v, a.z + v};
}

IVY_MATH_API vec3_t vec3_subv(vec3_t a, float v)
{
	return (vec3_t){a.x - v, a.y - v, a.z - v};
}

IVY_MATH_API vec3_t vec3_mulv(vec3_t a, float v)
{
	return (vec3_t){a.x * v, a.y * v, a.z * v};
}

IVY_MATH_API vec3_t vec3_divv(vec3_t a, float v)
{
	return (vec3_t){a.x / v, a.y / v, a.z / v};
}

IVY_MATH_API float vec3_lensq(vec3_t a)
{

	return SQ(a.x) + SQ(a.y) + +SQ(a.z);
}

IVY_MATH_API float vec3_len(vec3_t a)
{
	return sqrtf(vec3_lensq(a));
}

IVY_MATH_API float vec3_distsq(vec3_t a, vec3_t b)
{
	return SQ(a.x - b.x) + SQ(a.y - b.y) + SQ(a.z - b.z);
}

IVY_MATH_API float vec3_dist(vec3_t a, vec3_t b)
{
	return sqrtf(vec3_distsq(a, b));
}

IVY_MATH_API vec3_t vec3_wrap(vec3_t value, vec3_t min, vec3_t max)
{
	return (vec3_t){fwrap(value.x, min.x, max.x), fwrap(value.y, min.y, max.y),
					fwrap(value.z, min.z, max.z)};
}

IVY_MATH_API vec3_t vec3_wrap2(vec3_t value, float min, float max)
{
	return (vec3_t){fwrap(value.x, min, max), fwrap(value.y, min, max), fwrap(value.z, min, max)};
}

IVY_MATH_API bool vec3_is_almost_eq(vec3_t a, vec3_t b)
{
	return fequal(a.x, b.x) && fequal(a.y, b.y) && fequal(a.z, b.z);
}

IVY_MATH_API vec3_t vec3_clamp(vec3_t value, vec3_t min, vec3_t max)
{
	return (vec3_t){fclamp(value.x, min.x, max.x), fclamp(value.y, min.y, max.y),
					fclamp(value.z, min.z, max.z)};
}

IVY_MATH_API vec3_t vec3_clamp2(vec3_t value, float min, float max)
{
	return (vec3_t){fclamp(value.x, min, max), fwrap(value.y, min, max), fwrap(value.z, min, max)};
}

IVY_MATH_API vec3_t vec3_lerp(vec3_t a, vec3_t b, float t)
{
	return (vec3_t){flerp(a.x, b.x, t), flerp(a.y, b.y, t), flerp(a.z, b.z, t)};
}

IVY_MATH_API vec3_t vec3_normalize(vec3_t a)
{
	float l = vec3_len(a);
	if (l > 0)
		return (vec3_t){a.x / l, a.y / l, a.z / l};
	return vec3_zero();
}

IVY_MATH_API vec3_t vec3_invert(vec3_t a)
{
	return (vec3_t){1.0 / a.x, 1.0 / a.y, 1.0 / a.z};
}

IVY_MATH_API float vec3_dot(vec3_t a, vec3_t b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

IVY_MATH_API vec3_t vec3_cross(vec3_t a, vec3_t b)
{
	return (vec3_t){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
	};
}

IVY_MATH_API vec3_t vec3_transform(vec3_t v, mat4_t m)
{
	return (vec3_t){
		v.x * m.m00 + v.y * m.m10 + v.z * m.m20 + 1.0 * m.m30,
		v.x * m.m01 + v.y * m.m11 + v.z * m.m21 + 1.0 * m.m31,
		v.x * m.m02 + v.y * m.m12 + v.z * m.m22 + 1.0 * m.m32,
	};
}

IVY_MATH_API float vec3_angle(vec3_t a, vec3_t b)
{

	return atan2f(vec3_len(vec3_cross(a, b)), vec3_dot(a, b));
}

IVY_MATH_API vec3_t vec3_rotate_axis(vec3_t v, vec3_t axis, float angle_rad)
{
	// Ref:
	// https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
	axis = vec3_normalize(axis);
	float c = cosf(angle_rad);
	float s = sinf(angle_rad);
	vec3_t p0 = vec3_mulv(v, c);
	vec3_t p1 = vec3_mulv(vec3_cross(axis, v), s);
	vec3_t p2 = vec3_mulv(axis, vec3_dot(axis, v) * (1 - c));
	return vec3_add(vec3_add(p0, p1), p2);
}

IVY_MATH_API vec3_t vec3_barycentric(vec3_t p, vec3_t a, vec3_t b, vec3_t c)
{
	// Ref:
	// https://realtimecollisiondetection.net/books/rtcd/
	vec3_t v0 = vec3_sub(b, a);
	vec3_t v1 = vec3_sub(c, a);
	vec3_t v2 = vec3_sub(p, a);
	float d00 = vec3_dot(v0, v0);
	float d01 = vec3_dot(v0, v1);
	float d11 = vec3_dot(v1, v1);
	float d20 = vec3_dot(v2, v0);
	float d21 = vec3_dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	vec3_t result = vec3_zero();
	result.y = (d11 * d20 - d01 * d21) / denom; // v
	result.z = (d00 * d21 - d01 * d20) / denom; // w
	result.x = 1.0 - result.y - result.z;		// u
	return result;
}

// ---------------------------------------------------------------------------
// MATRIX4
// ---------------------------------------------------------------------------

// clang-format off
IVY_MATH_API mat4_t mat4_zero()
{
	return (mat4_t) {0};
}

IVY_MATH_API mat4_t mat4_identity()
{
	return (mat4_t) {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
}


IVY_MATH_API mat4_t mat4_mul(mat4_t a, mat4_t b) {
	return (mat4_t) {
		(a.m00 * b.m00) + (a.m01 * b.m10) + (a.m02 * b.m20) + (a.m03 * b.m30),
		(a.m00 * b.m01) + (a.m01 * b.m11) + (a.m02 * b.m21) + (a.m03 * b.m31),
		(a.m00 * b.m02) + (a.m01 * b.m12) + (a.m02 * b.m22) + (a.m03 * b.m32),
		(a.m00 * b.m03) + (a.m01 * b.m13) + (a.m02 * b.m23) + (a.m03 * b.m33),
		
		(a.m10 * b.m00) + (a.m11 * b.m10) + (a.m12 * b.m20) + (a.m13 * b.m30),
		(a.m10 * b.m01) + (a.m11 * b.m11) + (a.m12 * b.m21) + (a.m13 * b.m31),
		(a.m10 * b.m02) + (a.m11 * b.m12) + (a.m12 * b.m22) + (a.m13 * b.m32),
		(a.m10 * b.m03) + (a.m11 * b.m13) + (a.m12 * b.m23) + (a.m13 * b.m33),
		
		(a.m20 * b.m00) + (a.m21 * b.m10) + (a.m22 * b.m20) + (a.m23 * b.m30),
		(a.m20 * b.m01) + (a.m21 * b.m11) + (a.m22 * b.m21) + (a.m23 * b.m31),
		(a.m20 * b.m02) + (a.m21 * b.m12) + (a.m22 * b.m22) + (a.m23 * b.m32),
		(a.m20 * b.m03) + (a.m21 * b.m13) + (a.m22 * b.m23) + (a.m23 * b.m33),
		
		(a.m30 * b.m00) + (a.m31 * b.m10) + (a.m32 * b.m20) + (a.m33 * b.m30),
		(a.m30 * b.m01) + (a.m31 * b.m11) + (a.m32 * b.m21) + (a.m33 * b.m31),
		(a.m30 * b.m02) + (a.m31 * b.m12) + (a.m32 * b.m22) + (a.m33 * b.m32),
		(a.m30 * b.m03) + (a.m31 * b.m13) + (a.m32 * b.m23) + (a.m33 * b.m33),
	};
}

IVY_MATH_API mat4_t mat4_transpose(mat4_t m)
{
	return (mat4_t) {
		m.m00, m.m10, m.m20, m.m30,
		m.m01, m.m11, m.m21, m.m31,
		m.m02, m.m12, m.m22, m.m32,
		m.m03, m.m13, m.m23, m.m33,
	};
}

// Ref: https://journalppw.com/index.php/jpsp/article/download/1313/668/1516

IVY_MATH_API mat4_t mat4_translate(vec3_t t)
{
	return (mat4_t) {
		  1,   0,   0, 0,
		  0,   1,   0, 0,
		  0,   0,   1, 0,
		t.x, t.y, t.z, 1,
	};
}

IVY_MATH_API mat4_t mat4_rotate_x(float angle_rad)
{
	float c = cosf(angle_rad);
	float s = sinf(angle_rad);
	return (mat4_t) {
		 1,  0,  0,  0,
		 0,  c, -s,  0,
		 0,  s,  c,  0,
		 0,  0,  0,  1
	};
}

IVY_MATH_API mat4_t mat4_rotate_y(float angle_rad)
{
	float c = cosf(angle_rad);
	float s = sinf(angle_rad);
	return (mat4_t) {
		 c,  0,  s,  0,
		 0,  1,  0,  0,
		-s,  0,  c,  0,
		 0,  0,  0,  1,
	};
}

IVY_MATH_API mat4_t mat4_rotate_z(float angle_rad)
{
	float c = cosf(angle_rad);
	float s = sinf(angle_rad);
	return (mat4_t) {
		 c, -s,  0,  0,
		 s,  c,  0,  0,
		 0,  0,  1,  0,
		 0,  0,  0,  1,
	};
}

IVY_MATH_API mat4_t mat4_scale(vec3_t s)
{
	return (mat4_t) {
		 s.x,   0,   0,   0,
		   0, s.y,   0,   0,
		   0,   0, s.z,   0,
		   0,   0,   0,   1,
	};
}

// Wikipedia specifies axis aligned rotation for column major rotation
// As we are using row major we will use transpose of it
// Ref: https://en.wikipedia.org/wiki/Rotation_matrix#General_3D_rotations

IVY_MATH_API mat4_t mat4_rotate_axis(vec3_t axis, float angle_rad)
{
	float c = cosf(angle_rad);
	float s = sinf(angle_rad);
	vec3_t u = vec3_normalize(axis);
	return (mat4_t) {
		        SQ(u.x) * (1 - c) + c, u.x * u.y * (1 - c) + u.z * s, u.x * u.z * (1 - c) - u.y * s, 0,
		u.x * u.y * (1 - c) - u.z * s,         SQ(u.y) * (1 - c) + c, u.y * u.z * (1 - c) + u.x * s, 0,
		u.x * u.z * (1 - c) + u.y * s, u.y * u.z * (1 - c) - u.x * s,         SQ(u.z) * (1 - c) + c, 0,
		                            0,                             0,                             0, 1,
	};
}

// Ref: https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml
// Ref: https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
// Ref: https://stackoverflow.com/questions/349050/calculating-a-lookat-matrix

IVY_MATH_API mat4_t mat4_lookat(vec3_t eye, vec3_t center, vec3_t up)
{
	vec3_t f = vec3_normalize(vec3_sub(eye, center)); // vz
	vec3_t s = vec3_normalize(vec3_cross(up, f)); // vx
	vec3_t u = vec3_cross(f, s); // vy
	
	return (mat4_t) {
		              s.x,               u.x,               f.x, 0,
		              s.y,               u.y,               f.y, 0,
		              s.z,               u.z,               f.z, 0,
		-vec3_dot(s, eye), -vec3_dot(u, eye), -vec3_dot(f, eye), 1,
	};
}

// Reference uses column major
// Ref: https://docs.gl/gl3/glFrustum

// IVY_MATH_API mat4_t mat4_frustum(float left, float right, float bottom, float top, float near, float far)
// {
// 
// }

// Reference uses column major
// Ref: https://docs.gl/gl3/glOrtho

IVY_MATH_API mat4_t mat4_ortho(float left, float right, float bottom, float top, float near, float far)
{
	vec3_t t = (vec3_t) {
		-(right + left) / (right - left),
		-(top + bottom) / (top - bottom),
		-(far + near)   / (far - near),
	};
	return (mat4_t) {
		2 / (right - left),                  0,                 0, 0,
		                 0, 2 / (top - bottom),                 0, 0,
		                 0,                  0, -2 / (far - near), 0,
		               t.x,                t.y,               t.z, 1,
	};	
}

IVY_MATH_API mat4_t mat4_perspective(float fov_rad, float aspect, float n, float f) {
// Ref: https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix.html
	float scale = tanf(fov_rad * 0.5) * n; 
    float t = scale;
    float b = -t;
    float r = t * aspect;
    float l  = -r;
	return (mat4_t) {
		  2 * n / (r - l),                 0,                     0,  0,
		                0,   2 * n / (t - b),                     0,  0,
		(r + l) / (r - l), (t + b) / (t - b),     -(f + n)/ (f - n), -1,
		                0,                 0, - 2 * f * n / (f - n),  0
	};
}

// clang-format on

// TODO: mat4_determinant mat4_invert mat4_add mat4_sub mat4_mul mat4_frustum
// mat4_trace

#undef IVY_MATH_API
#undef EPSILON
#undef SQ

// ---------------------------------------------------------------------------
// IVY STL LOADER IMPLEMENTATION
// ---------------------------------------------------------------------------

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
	stl_data.triangles = malloc(sizeof(ivy_triangle_t) * stl_data.triangles_count);
	for (int i = 0; i < stl_data.triangles_count; i++) {
		ivy_triangle_t *t = &stl_data.triangles[i];

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

	fprintf(out_file, "#ifndef %s_STL_C", mesh_name_upper);
	fprintf(out_file, "\n#define %s_STL_C", mesh_name_upper);
	fprintf(out_file, "\n\nstatic const int %s_triangles_count = %d;", mesh_name,
			stl_data.triangles_count);
	fprintf(out_file, "\nstatic const float %s_data[%d][4][3] = {", mesh_name,
			stl_data.triangles_count);
	for (int i = 0; i < stl_data.triangles_count; i++) {
		ivy_triangle_t t = stl_data.triangles[i];
		fprintf(out_file, "\n\t{");
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.normal.x, t.normal.y, t.normal.z);
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.vertex1.x, t.vertex1.y, t.vertex1.z);
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.vertex2.x, t.vertex2.y, t.vertex2.z);
		fprintf(out_file, "\n\t\t{ %f, %f, %f },", t.vertex3.x, t.vertex3.y, t.vertex3.z);
		fprintf(out_file, "\n\t},");
	}
	fprintf(out_file, "\n};");
	fprintf(out_file, "\n\n#endif // %s_STL_C", mesh_name_upper);
	fclose(out_file);
	free(mesh_name_upper);
	stl_free(stl_data);
	return 0;
}
#endif // IVY_STL_LOADER_IMPL
#endif // IVY_C

