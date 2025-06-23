#include "common.h"
#include <math.h>

#define IVY_MATH_API static inline

#define EPSILON 0.00001
#define SQ(x) ((x) * (x))

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

// ------------------------------------------
// Float
// ------------------------------------------
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

// ------------------------------------------
// Vector2
// ------------------------------------------
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

// ------------------------------------------
// Vector3
// ------------------------------------------
IVY_MATH_API vec3_t vec3_zero()
{
	return (vec3_t){0, 0, 0};
}

IVY_MATH_API vec3_t vec3_new(float x, float y, float z)
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
	return sqrtf(SQ(a.x)) + (SQ(a.y) + SQ(a.z));
}

IVY_MATH_API float vec3_distsq(vec3_t a, vec3_t b)
{
	return SQ(a.x - b.x) + SQ(a.y - b.y) + SQ(a.z - b.z);
}

IVY_MATH_API float vec3_dist(vec3_t a, vec3_t b)
{
	return sqrtf(SQ(a.x - b.x) + SQ(a.y - b.y) + SQ(a.z - b.z));
}

IVY_MATH_API vec3_t vec3_wrap(vec3_t value, vec3_t min, vec3_t max)
{
	return (vec3_t){fwrap(value.x, min.x, max.x), fwrap(value.y, min.y, max.y), fwrap(value.z, min.z, max.z)};
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
	return (vec3_t){fclamp(value.x, min.x, max.x), fclamp(value.y, min.y, max.y), fclamp(value.z, min.z, max.z)};
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

IVY_MATH_API float vec3_angle(vec3_t a, vec3_t b)
{
	return atan2f(vec3_len(vec3_cross(a, b)), vec3_dot(a, b));
}

IVY_MATH_API vec3_t vec3_rotate_axis(vec3_t v, vec3_t axis, float angle_rad)
{
	// Ref:
	// https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
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

// ------------------------------------------
// Matrix4
// ------------------------------------------
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
		u.x * u.z * (1 - c) + u.y * s, u.y * u.z * (1 - c) - u.z * s,         SQ(u.z) * (1 - c) + c, 0,
		                            0,                             0,                             0, 1,
	};
}

// Here again this reference specifies column major
// Therefore using transpose of it
// Ref: https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml

IVY_MATH_API mat4_t mat4_lookat(vec3_t eye, vec3_t center, vec3_t up)
{
	vec3_t f = vec3_normalize(vec3_sub(center, eye));
	vec3_t u = vec3_normalize(up);
	vec3_t s = vec3_cross(f, u);
	return (mat4_t) {
		s.x, u.x, -f.x, 0,
		s.y, u.y, -f.y, 0,
		s.z, u.z, -f.z, 0,
		  0,   0,    0, 1,
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

// Ref: https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix.html
// IVY_MATH_API mat4_t mat4_perspective() {
	// return (mat4_t) {0};
// }

// TODO: mat4_determinant mat4_invert mat4_add mat4_sub mat4_mul mat4_frustum mat4_perspective mat4_trace

// clang-format on
