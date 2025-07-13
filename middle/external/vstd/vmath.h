/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vmath.h
 *         \unit  vmath
 *        \brief  Similar to the C standard library math
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vmath_H
#define __vmath_H

/* A constant in mathematics
 */

#ifndef PI
#define PI 3.141592653589793238462643383279502884197169399375105820974944592308
#endif
#ifndef E
#define E 2.718281828459045235360287471352662497757247093699959574966967627724
#endif

#define INFINITY ((float)(1e+300 * 1e+300))

double v_fmod(double x, double y);
double v_sin(double x);
double v_cos(double x);
double v_sec(double x);
double v_csc(double x);
double v_tan(double x);
double v_cot(double x);
double v_ln(double x);
double v_lg(double x);
double v_log2(double x);
double v_loga(double x,double y);
double v_exp(double x);
double v_pow(double x,double y);
double v_sigmoid(double x);
double v_dsigmoid(double x);
double v_sinh(double x);
double v_cosh(double x);
double v_tanh(double x);
double v_coth(double x);
double v_sech(double x);
double v_csch(double x);
double v_sqrt(double x);
double v_gaussian(double x, double mean, double variance);

#endif