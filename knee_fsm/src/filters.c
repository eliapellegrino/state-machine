#include <math.h>

#include "utils/filters.h"
#include "utils/utilities.h"

FiltValues filtValues = {
    .p_knee_prev = 0.0f,
    .w_knee_prev = 0.0f,
    .p_motor_prev = 0.0f,
    .w_motor_prev = 0.0f,
    .w_thigh_prev = 0.0f,
    .F_prev = 0.0f,
    .M_prev = 0.0f,
    .Md_prev = 0.0f,
    .a_fast = 0.0f,
    .b_fast = 0.0f,
    .a_normal = 0.0f,
    .b_normal = 0.0f,
    .a_slow = 0.0f,
    .b_slow = 0.0f,

    .freq_fast = 12.0f, // fast filter frequency [Hz]
    .freq_normal = 4.0f,  // normal filter frequency [Hz]
    .freq_slow = 1.5f, // slow filter frequency [Hz]

    .time_movingWindow = 0.0f,
    .windowSize = 100, // has to be the same of timeWindow
    .timeWindow = {0.0f}};

static float x_n[30]; // todo calloc wasn't working

void FiltMinMaxN(float _x, float _n, float* y1_min, float* y2_max)
{
    static uint8_t init = 0;
    int i = 0;
    static int c = 0;
    uint16_t sizeArray = _n;
    float min, max;

    // Initialization
    if (init == 0)
    {
        *y1_min = _x;
        *y2_max = _x;

        // assign values
        for (i = 0; i < sizeArray; i++){
            x_n[i] = _x;
        }

        init = 1;
        i = 0;
    }
    else
    {
        x_n[c] = _x;
        if(c >= sizeArray)
        {
            c = 0;
        }
        else
        {
            c = c + 1;
        }
        

        // Find min and max
        min = x_n[0];
        max = x_n[0];
        for (i = 1; i < sizeArray; i++)
        {
            if(x_n[i] < min){
                min = x_n[i];
            }
            if(x_n[i] > max){
                max = x_n[i];
            }
        }

        *y1_min = min;
        *y2_max = max;
    }
}


void FiltCoeff(float c_f, float t_s, float *a, float *b)
{
    float C = tanf(M_PI * c_f * t_s);
    *b = C / (C + 1.0f);
    *a = (C - 1.0f) / (C + 1.0f);

    return;
}


void Filt1(float x, float x_1, float a, float b, float *y)
{
    *y = -a * (*y) + (b *  (x + x_1));
}

/*
 * @brief valve_limit() computes the limited position
 *
 */
float valve_limit(float q, float y_0, float d_0, float p)
{
    float y;

    if (q > 0.0f)
    {
        y = y_0 * powf((p / ((d_0 / y_0) * q + p)), p);
    }
    else if (q <= (y_0 - 1.0f) / d_0)
    {
        y = 1.0f;
    }
    else
    {
        y = y_0 - (q * d_0);
    }

    return y;
}


/*
 * @brief satLow: saturation lower limit
 * 
 */

float satLow(float x, float xmin)
{
    float xOut = x > xmin ? x : xmin; // min saturation

    return xOut;
}

/*
 * @brief satUp: saturation upper limit
 * 
 */
float satUp(float x, float xmax)
{
    float xOut = x < xmax ? x : xmax; // max saturation

    return xOut;
}


/*
 * @brief satLowUp: saturation lower and upper limit
 * 
 */

float satLowUp(float x, float xmin, float xmax)
{
    float xOut = x > xmin ? x : xmin; // min saturation

    xOut = xOut < xmax ? xOut : xmax; // max saturation

    return xOut;
}


//*****************************************************************************
// Fuzzy section
//*****************************************************************************

/*
 * @brief fuzzyOut() // linear fuzzy transition (ff=0: y1, ff=1: y2)
 *
 */
float fuzzyOut(float y1, float y2, float ff)
{
    //return 1.0f / ((1.0f - ff) / y1 + ff / y2); // this is the inverse law
    return (y1 * (1.0f - ff) + (y2 * ff)); //
}

/*
 * @brief fuzzyInv() // inverse linear fuzzy transition (ff=0: y1, ff=1: y2)
 *
 */
float fuzzyInv(float y1, float y2, float ff)
{
    return 1.0f / ((1.0f - ff) / y1 + ff / y2);
}

/*
 * @brief fuzzyPow() // custom power fuzzy transition (ff=0: y1, ff=1: y2)
 *
 */
float fuzzyInvPow(float y1, float y2, float ff, float p)
{
    return powf((1.0f - ff) * powf(y1, p) + ff * powf(y2, p), 1.0f / p);
}

/*
 * @brief fuzzyFactorQuad()
 *
 */
float fuzzyFactorQuad(float x, float xmin, float xmax)
{
    float ff;
    float x0 = (xmin + xmax) / 2.0f;

    if (x >= xmax)
    {
        ff = 1.0f;
    }
    else if (x <= xmin)
    {
        ff = -1.0f;
    }
    else if (x <= x0)
    {
        ff = powf(((x - xmin) / (x0 - xmin)), 2.0f) - 1.0f;
    }
    else
    {
        ff = 1.0f - powf(((x - xmax) / (x0 - xmin)), 2.0f);
    }

    ff = (1 + ff) / 2.0f;

    return ff;
}

