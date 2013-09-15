/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 *
 * This file defines all the mathematical functions used. These values can be
 * changed to allow the library to function with an alternative arithmetic
 * system.
 */

#ifndef OS_COORD_MATH_H
#define OS_COORD_MATH_H

#include <math.h>

// Use double precision floats
#define OS_NUM_T double

// Do whatever is needed to represent a literal. Assumes the argument is a
// C double.
#define OS_NUM_LIT(n) (n)

// Convert to an integer (flooring the value)
#define OS_NUM2INT(n) ((int)(n))

// Convert an integer to a number
#define OS_INT2NUM(i) ((double)(i))

// Compare: 0 if equal, -ve if a < b, +ve if a > b
#define OS_CMP(a,b) ((a)-(b))

// Basic arithmetic
#define OS_ADD(a,b) ((a)+(b))
#define OS_SUB(a,b) ((a)-(b))
#define OS_MUL(a,b) ((a)*(b))
#define OS_DIV(a,b) ((a)/(b))

// Advanced arithmetic
#define OS_POW(n,p) (pow((n),(p)))
#define OS_SQRT(n)  (sqrt((n)))
#define OS_ABS(n)   (fabs((n)))
#define OS_MOD(n,m) (fmod((n),(m)))

// Trig functions
#define OS_SIN(n)     (sin((n)))
#define OS_COS(n)     (cos((n)))
#define OS_TAN(n)     (tan((n)))
#define OS_ASIN(n)    (asin((n)))
#define OS_ACOS(n)    (acos((n)))
#define OS_ATAN(n)    (atan((n)))
#define OS_ATAN2(y,x) (atan2((y),(x)))

// Constants
#define PI             (OS_NUM_LIT(3.141592653589793))
#define DEG_2_RAD(deg) (OS_MUL(OS_DIV((deg), OS_NUM_LIT(180.0)), PI))
#define RAD_2_DEG(rad) (OS_MUL(OS_DIV((rad), PI), OS_NUM_LIT(180.0)))

#endif
