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

// Uncomment to use the avr_f64 library for arithmetic
//#define USE_AVR_F64

#ifndef USE_AVR_F64
	#include <math.h>
	
	// Use double precision floats
	#define OS_NUM_T double
	
	// Do whatever is needed to represent a literal. Assumes the argument is a
	// C double.
	#define OS_NUM_LIT(n) (n)
	
	// Convert to an integer (flooring the value)
	#define OS_NUM2LONG(n) ((long)(n))
	
	// Convert an integer to a number
	#define OS_LONG2NUM(i) ((double)(i))
	
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
#else /* USE_AVR_F64*/
	#include <avr_f64.h>
	
	// Use double precision floats
	#define OS_NUM_T float_64_t
	
	// Do whatever is needed to represent a literal. Assumes the argument is a
	// C double. (To be converted by a pre-processing pass)
	#define OS_NUM_LIT(n) (n)
	
	// Convert to an integer (flooring the value)
	#define OS_NUM2LONG(n) (f_float64_to_long((n)))
	
	// Convert an integer to a number
	#define OS_LONG2NUM(i) (f_long_to_float64((i)))
	
	// Compare: 0 if equal, -ve if a < b, +ve if a > b
	#define OS_CMP(a,b) (f_compare((a), (b)))
	
	// Basic arithmetic
	#define OS_ADD(a,b) (f_add((a),(b)))
	#define OS_SUB(a,b) (f_sub((a),(b)))
	#define OS_MUL(a,b) (f_mult((a),(b)))
	#define OS_DIV(a,b) (f_div((a),(b)))
	
	// Advanced arithmetic
	#define OS_POW(n,p) (f_pow((n),(p)))
	#define OS_SQRT(n)  (f_sqrt((n)))
	#define OS_ABS(n)   (f_abs((n)))
	#define OS_MOD(n,m) (f_mod((n),(m)))
	
	// Trig functions
	#define OS_SIN(n)     (f_sin((n)))
	#define OS_COS(n)     (f_cos((n)))
	#define OS_TAN(n)     (f_tan((n)))
	#define OS_ASIN(n)    (f_arcsin((n)))
	#define OS_ACOS(n)    (f_arccos((n)))
	#define OS_ATAN(n)    (f_arctan((n)))
	#define OS_ATAN2(y,x) (f_arctan2((y),(x)))
#endif

// Constants
#define PI             (OS_NUM_LIT(3.141592653589793))
#define DEG_2_RAD(deg) (OS_MUL(OS_DIV((deg), OS_NUM_LIT(180.0)), PI))
#define RAD_2_DEG(rad) (OS_MUL(OS_DIV((rad), PI), OS_NUM_LIT(180.0)))

#endif
