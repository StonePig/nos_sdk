/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  filter.h
 *         \unit  filter
 *        \brief  This is a C language version of commonly used filter algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __filter_H
#define __filter_H

#define FILTER_V_MAJOR                      1
#define FILTER_V_MINOR                      0
#define FILTER_V_PATCH                      0

/**
* \brief Performs median filtering on a data array.
*
* This function applies median filtering to a data array using a sliding window of a specified size.
* The filtered value at each data point is calculated as the median of the values within the window.
*
* \param[in,out] data A pointer to the data array to be filtered. The filtered values will be stored in this array.
* \param[in] size The size of the data array.
* \param[in] window The size of the sliding window for median calculation. If it is an even number, it will be adjusted to the next odd number.
*/
void filter_median(double *data, int size, int window);

/**
* \brief Performs Kalman filtering on a series of measurements.
*
* This function applies Kalman filtering to a series of measurements to estimate the underlying state.
* The estimated state at each measurement is stored in the estimates array.
*
* \param[in] measurements A pointer to the array of measurements.
* \param[out] estimates A pointer to the array where the estimated states will be stored.
* \param[in] numMeasurements The number of measurements.
* \param[in] processNoise The process noise.
* \param[in] measurementNoise The measurement noise.
*/
void filter_kalman(double *measurements, double *estimates, int numMeasurements, double processNoise, double measurementNoise);

/**
* \brief Performs average filtering on a data array.
*
* This function applies average filtering to a data array using a sliding window of a specified size.
* The filtered value at each data point is calculated as the average of the values within the window.
*
* \param[in,out] data A pointer to the data array to be filtered. The filtered values will be stored in this array.
* \param[in] size The size of the data array.
* \param[in] window The size of the sliding window for averaging. If it is an even number, it will be adjusted to the next odd number.
*/
void filter_average(double *data, int size, int window);

#endif
