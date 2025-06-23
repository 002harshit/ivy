#ifndef IVY_COMMON_H
#define IVY_COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef bool bool_t;
typedef unsigned char u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
typedef signed char i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;

#define ANSI_WHITE "\e[0;37m"
#define ANSI_RED "\e[1;31m"
#define ANSI_BLUE "\e[1;36m"

// clang-fomat off
#define warn(...)                                                                                                      \
	fprintf(stderr, ANSI_RED "WARNING: " ANSI_WHITE __VA_ARGS__);                                                      \
	printf("\n")
#define fatal(...)                                                                                                     \
	fprintf(stdout, ANSI_BLUE "FATAL: " ANSI_WHITE __VA_ARGS__);                                                       \
	printf("\n");                                                                                                      \
	exit(1)
#define info(...)                                                                                                      \
	fprintf(stdout, ANSI_BLUE "INFO: " ANSI_WHITE __VA_ARGS__);                                                        \
	printf("\n")
#define todo(str) fatal("NOT IMPLEMENTED: %s", str)
// clang-fomat on

#endif // IVY_COMMON_H
