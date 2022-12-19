#ifndef UTILS_FILTERS_H_
#define UTILS_FILTERS_H_

#include <stdint.h>

typedef struct 
{
    // previous values for filters
    float p_knee_prev;
    float w_knee_prev;
    float p_motor_prev;
    float w_motor_prev;
    float w_thigh_prev;
    float F_prev;
    float M_prev;
    float Md_prev; 

    // filter coefficients
    float a_vfast;
    float b_vfast;
    float a_fast;
    float b_fast;
    float a_normal;
    float b_normal;
    float a_slow;
    float b_slow;

    // filter frequencies
    float freq_fast; // fast cut-off frequency, e.g. used for FSM velocity readings [Hz]
    float freq_normal; // medium fast cut-off frequency, e.g. used for comparing motor with joint velocity [Hz]
    float freq_slow; // slow cut-off frequency velocity filter, e.g. used for determining joint direction [Hz]
    float time_movingWindow;

    // Min Max filter 
    int windowSize; // has to be the same of timeWindow
    float timeWindow[100];
}FiltValues;

extern FiltValues filtValues;

/*
 * @brief   FiltMinMaxN: this is a Nth order FIR-filter for an input signal x.
 *                       The output is the min and the max value of the last N inputs
 *                       and the present input x
 *
 * @param x initial value
 * @param N filter order
 * @param min value to be updated
 * @param max value to be updated
 *
 */
void FiltMinMaxN(float _x, float _n, float* y1_min, float* y2_max);

/*
 * @brief   FiltCoeff: calculate a and b coefficients
 *
 * @param c_f cut off frequency value
 * @out *a value
 * @out *b value
 *
 */
void FiltCoeff(float c_f, float t_s, float *a, float *b);

/*
 * @brief   Filt1: this filter is 1st order IIR-filter for an input signal x.
 *          a and b are filter coefficients
 *
 * @param x input value
 * @param x_1 previous input value
 * @param a coefficient value
 * @param b coefficient value
 * @out *y value
 *
 */
void Filt1(float x, float x_1, float a, float b, float *y);


// valve output methods
float valve_limit(float q, float y_0, float d_0, float p);
float fuzzyOut(float y1, float y2, float ff);
float fuzzyInv(float y1, float y2, float ff);
float fuzzyPow(float y1, float y2, float ff, float p);
float fuzzyFactorQuad(float x, float xmin, float xmax);

// motor output methods
float satLow(float x, float xmin);
float satUp(float x, float xmax);
float satLowUp(float x, float xmin, float xmax);

#endif // UTILS_FILTERS_H_
