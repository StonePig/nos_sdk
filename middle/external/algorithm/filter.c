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
#include "filter.h"

static void filter_median_s(double *data, int size, const int const window, int index)
{
    double array[window];
    double temp;
    int i, j, half; // half windows size
    int start, end;

    half = window / 2;

    /* Calculate the window range for the current data point */
    start = index - half;
    end = start + window;

    /* Ensure the window does not exceed the boundaries of the data */
    if (start < 0) 
    {
        start = 0;
        end = window;
    } 
    else if (end > size) 
    {
        end = size;
    }

    /* Copy the data within the window to the temporary array */
    for (i = start; i < end; i++) 
    {
        array[i - start] = data[i];
    }

    /* Sort the temporary array */
    for (i = 0; i < window - 1; i++) 
    {
        for (j = 0; j < window - i - 1; j++) 
        {
            if (array[j] > array[j + 1]) 
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    /* Calculate the median value and assign it to the current data point */
    data[index] = array[half];
}

void filter_median(double *data, int size, int window) 
{
    int i;

    /* Check if the window size is even, and adjust it to the next odd number if necessary */ 
    if (window % 2 == 0) window++;

    /* Perform filtering for each data point */
    for (i = 0; i < size; i++) 
    {
        filter_median_s(data, size, window, i);
    }
}

void filter_kalman(double *measurements, double *estimates, int numMeasurements, double processNoise, double measurementNoise) 
{
    int i;
    double estimate, priorEstimate, priorError, errorCovariance;
    double kalmanGain, temp;

    /* Initialize the state estimate and covariance */
    estimate = 0.0;
    priorEstimate = 0.0;
    priorError = 1.0;
    errorCovariance = priorError * priorError;

    for (i = 0; i < numMeasurements; i++) 
    {
        /* Calculate the Kalman gain */
        kalmanGain = errorCovariance / (errorCovariance + measurementNoise);

        /* Update the state estimate */
        estimate = priorEstimate + kalmanGain * (measurements[i] - priorEstimate);

        /* Update the covariance */
        errorCovariance = (1 - kalmanGain) * errorCovariance;

        /* Update the prior estimate and error */
        priorEstimate = estimate;
        // priorError = errorCovariance;
    }

    /* Store the final estimate in the estimates array */
    estimates[0] = estimate;
}

void filter_average(double *data, int size, int window) 
{
    int i, j, half;
    double sum = 0.0;
    int start, end;

    /* Check if the window size is even, and adjust it to the next odd number if necessary */
    if (window % 2 == 0) window++;

    half = window / 2;

    /* Perform filtering for each data point */
    for (i = 0; i < size; i++) 
    {
        /* Reset the sum of elements within the window */
        sum = 0.0; 

        /* Calculate the window range for the current data point */
        start = i - half;
        end = start + window;

        /* Ensure the window does not exceed the boundaries of the data */
        if (start < 0) start = 0;
        if (end > size) end = size;

        /* Calculate the sum of all data within the window */
        for (j = start; j < end; j++) 
        {
            sum += data[j];
        }

        /* Calculate the average and assign it to the current data point */
        data[i] = sum / window;
    }
}

