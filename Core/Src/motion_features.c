#include "motion_features.h"
#include "motion_meta.h"
#include <float.h>
#include <math.h>

typedef enum

{

    MOTION_FEAT_DURATION = 0,

    MOTION_FEAT_NUM_SAMPLES = 1,

    MOTION_FEAT_ACCEL_MAG_MEAN = 2,

    MOTION_FEAT_ACCEL_MAG_MAX = 3,

    MOTION_FEAT_ACCEL_MAG_MIN = 4,

    MOTION_FEAT_ACCEL_MAG_RANGE = 5,

    MOTION_FEAT_DPS_MAG_MEAN = 6,

    MOTION_FEAT_DPS_MAG_MAX = 7,

    MOTION_FEAT_DPS_MAG_MIN = 8,

    MOTION_FEAT_DPS_MAG_RANGE = 9,

    MOTION_FEAT_A_X_MEAN = 10,

    MOTION_FEAT_A_X_STD = 11,

    MOTION_FEAT_A_X_MIN = 12,

    MOTION_FEAT_A_X_MAX = 13,

    MOTION_FEAT_A_X_RANGE = 14,

    MOTION_FEAT_A_X_ABS_MAX = 15,

    MOTION_FEAT_A_X_DELTA = 16,

    MOTION_FEAT_A_X_MAX_NEIGHBOR_DIFF = 17,

    MOTION_FEAT_A_X_MIN_NEIGHBOR_DIFF = 18,

    MOTION_FEAT_A_X_AVG_NEIGHBOR_DIFF = 19,

    MOTION_FEAT_A_X_MAX_GRADIENT = 20,

    MOTION_FEAT_A_X_MIN_GRADIENT = 21,

    MOTION_FEAT_A_X_NUM_PEAKS = 22,

    MOTION_FEAT_A_X_ZERO_CROSSINGS = 23,

    MOTION_FEAT_A_Y_MEAN = 24,

    MOTION_FEAT_A_Y_STD = 25,

    MOTION_FEAT_A_Y_MIN = 26,

    MOTION_FEAT_A_Y_MAX = 27,

    MOTION_FEAT_A_Y_RANGE = 28,

    MOTION_FEAT_A_Y_ABS_MAX = 29,

    MOTION_FEAT_A_Y_DELTA = 30,

    MOTION_FEAT_A_Y_MAX_NEIGHBOR_DIFF = 31,

    MOTION_FEAT_A_Y_MIN_NEIGHBOR_DIFF = 32,

    MOTION_FEAT_A_Y_AVG_NEIGHBOR_DIFF = 33,

    MOTION_FEAT_A_Y_MAX_GRADIENT = 34,

    MOTION_FEAT_A_Y_MIN_GRADIENT = 35,

    MOTION_FEAT_A_Y_NUM_PEAKS = 36,

    MOTION_FEAT_A_Y_ZERO_CROSSINGS = 37,

    MOTION_FEAT_A_Z_MEAN = 38,

    MOTION_FEAT_A_Z_STD = 39,

    MOTION_FEAT_A_Z_MIN = 40,

    MOTION_FEAT_A_Z_MAX = 41,

    MOTION_FEAT_A_Z_RANGE = 42,

    MOTION_FEAT_A_Z_ABS_MAX = 43,

    MOTION_FEAT_A_Z_DELTA = 44,

    MOTION_FEAT_A_Z_MAX_NEIGHBOR_DIFF = 45,

    MOTION_FEAT_A_Z_MIN_NEIGHBOR_DIFF = 46,

    MOTION_FEAT_A_Z_AVG_NEIGHBOR_DIFF = 47,

    MOTION_FEAT_A_Z_MAX_GRADIENT = 48,

    MOTION_FEAT_A_Z_MIN_GRADIENT = 49,

    MOTION_FEAT_A_Z_NUM_PEAKS = 50,

    MOTION_FEAT_A_Z_ZERO_CROSSINGS = 51,

    MOTION_FEAT_DPS_X_MEAN = 52,

    MOTION_FEAT_DPS_X_STD = 53,

    MOTION_FEAT_DPS_X_MIN = 54,

    MOTION_FEAT_DPS_X_MAX = 55,

    MOTION_FEAT_DPS_X_RANGE = 56,

    MOTION_FEAT_DPS_X_ABS_MAX = 57,

    MOTION_FEAT_DPS_X_DELTA = 58,

    MOTION_FEAT_DPS_X_MAX_NEIGHBOR_DIFF = 59,

    MOTION_FEAT_DPS_X_MIN_NEIGHBOR_DIFF = 60,

    MOTION_FEAT_DPS_X_AVG_NEIGHBOR_DIFF = 61,

    MOTION_FEAT_DPS_X_MAX_GRADIENT = 62,

    MOTION_FEAT_DPS_X_MIN_GRADIENT = 63,

    MOTION_FEAT_DPS_X_NUM_PEAKS = 64,

    MOTION_FEAT_DPS_X_ZERO_CROSSINGS = 65,

    MOTION_FEAT_DPS_Y_MEAN = 66,

    MOTION_FEAT_DPS_Y_STD = 67,

    MOTION_FEAT_DPS_Y_MIN = 68,

    MOTION_FEAT_DPS_Y_MAX = 69,

    MOTION_FEAT_DPS_Y_RANGE = 70,

    MOTION_FEAT_DPS_Y_ABS_MAX = 71,

    MOTION_FEAT_DPS_Y_DELTA = 72,

    MOTION_FEAT_DPS_Y_MAX_NEIGHBOR_DIFF = 73,

    MOTION_FEAT_DPS_Y_MIN_NEIGHBOR_DIFF = 74,

    MOTION_FEAT_DPS_Y_AVG_NEIGHBOR_DIFF = 75,

    MOTION_FEAT_DPS_Y_MAX_GRADIENT = 76,

    MOTION_FEAT_DPS_Y_MIN_GRADIENT = 77,

    MOTION_FEAT_DPS_Y_NUM_PEAKS = 78,

    MOTION_FEAT_DPS_Y_ZERO_CROSSINGS = 79,

    MOTION_FEAT_DPS_Z_MEAN = 80,

    MOTION_FEAT_DPS_Z_STD = 81,

    MOTION_FEAT_DPS_Z_MIN = 82,

    MOTION_FEAT_DPS_Z_MAX = 83,

    MOTION_FEAT_DPS_Z_RANGE = 84,

    MOTION_FEAT_DPS_Z_ABS_MAX = 85,

    MOTION_FEAT_DPS_Z_DELTA = 86,

    MOTION_FEAT_DPS_Z_MAX_NEIGHBOR_DIFF = 87,

    MOTION_FEAT_DPS_Z_MIN_NEIGHBOR_DIFF = 88,

    MOTION_FEAT_DPS_Z_AVG_NEIGHBOR_DIFF = 89,

    MOTION_FEAT_DPS_Z_MAX_GRADIENT = 90,

    MOTION_FEAT_DPS_Z_MIN_GRADIENT = 91,

    MOTION_FEAT_DPS_Z_NUM_PEAKS = 92,

    MOTION_FEAT_DPS_Z_ZERO_CROSSINGS = 93

} MotionFeatureIndex;

struct accel_global_features
{
    double accel_mag_mean;
    double accel_mag_max;
    double accel_mag_min;
    double accel_mag_range;
};

struct dps_global_features
{
    double dps_mag_mean;
    double dps_mag_max;
    double dps_mag_min;
    double dps_mag_range;
};

struct axis_accel_features
{
    double a_x_mean;
    double a_x_std;
    double a_x_min;
    double a_x_max;
    double a_x_range;
    double a_x_abs_max;
    double a_x_delta;
    double a_x_max_neighbor_diff;
    double a_x_min_neighbor_diff;
    double a_x_avg_neighbor_diff;
    double a_x_max_gradient;
    double a_x_min_gradient;
    double a_x_num_peaks;
    double a_x_zero_crossings;

    double a_y_mean;
    double a_y_std;
    double a_y_min;
    double a_y_max;
    double a_y_range;
    double a_y_abs_max;
    double a_y_delta;
    double a_y_max_neighbor_diff;
    double a_y_min_neighbor_diff;
    double a_y_avg_neighbor_diff;
    double a_y_max_gradient;
    double a_y_min_gradient;
    double a_y_num_peaks;
    double a_y_zero_crossings;

    double a_z_mean;
    double a_z_std;
    double a_z_min;
    double a_z_max;
    double a_z_range;
    double a_z_abs_max;
    double a_z_delta;
    double a_z_max_neighbor_diff;
    double a_z_min_neighbor_diff;
    double a_z_avg_neighbor_diff;
    double a_z_max_gradient;
    double a_z_min_gradient;
    double a_z_num_peaks;
    double a_z_zero_crossings;
};

struct axis_dps_features
{
    double dps_x_mean;
    double dps_x_std;
    double dps_x_min;
    double dps_x_max;
    double dps_x_range;
    double dps_x_abs_max;
    double dps_x_delta;
    double dps_x_max_neighbor_diff;
    double dps_x_min_neighbor_diff;
    double dps_x_avg_neighbor_diff;
    double dps_x_max_gradient;
    double dps_x_min_gradient;
    double dps_x_num_peaks;
    double dps_x_zero_crossings;

    double dps_y_mean;
    double dps_y_std;
    double dps_y_min;
    double dps_y_max;
    double dps_y_range;
    double dps_y_abs_max;
    double dps_y_delta;
    double dps_y_max_neighbor_diff;
    double dps_y_min_neighbor_diff;
    double dps_y_avg_neighbor_diff;
    double dps_y_max_gradient;
    double dps_y_min_gradient;
    double dps_y_num_peaks;
    double dps_y_zero_crossings;

    double dps_z_mean;
    double dps_z_std;
    double dps_z_min;
    double dps_z_max;
    double dps_z_range;
    double dps_z_abs_max;
    double dps_z_delta;
    double dps_z_max_neighbor_diff;
    double dps_z_min_neighbor_diff;
    double dps_z_avg_neighbor_diff;
    double dps_z_max_gradient;
    double dps_z_min_gradient;
    double dps_z_num_peaks;
    double dps_z_zero_crossings;
};

double features[N_FEATURES];
struct axis_accel_features aaf;
struct axis_dps_features adf;

#include <math.h>
#include <stdint.h>

void std_dev(Motion_Sample *ordered_window)
{
    double diff_a_x_squared_sum = 0.0;
    double diff_a_y_squared_sum = 0.0;
    double diff_a_z_squared_sum = 0.0;

    double diff_dps_x_squared_sum = 0.0;
    double diff_dps_y_squared_sum = 0.0;
    double diff_dps_z_squared_sum = 0.0;

    for (uint32_t i = 0; i < WINDOW_SIZE; ++i)
    {
        double diff_a_x = ordered_window[i].a_x - aaf.a_x_mean;
        double diff_a_y = ordered_window[i].a_y - aaf.a_y_mean;
        double diff_a_z = ordered_window[i].a_z - aaf.a_z_mean;

        double diff_dps_x = ordered_window[i].dps_x - adf.dps_x_mean;
        double diff_dps_y = ordered_window[i].dps_y - adf.dps_y_mean;
        double diff_dps_z = ordered_window[i].dps_z - adf.dps_z_mean;

        diff_a_x_squared_sum += diff_a_x * diff_a_x;
        diff_a_y_squared_sum += diff_a_y * diff_a_y;
        diff_a_z_squared_sum += diff_a_z * diff_a_z;

        diff_dps_x_squared_sum += diff_dps_x * diff_dps_x;
        diff_dps_y_squared_sum += diff_dps_y * diff_dps_y;
        diff_dps_z_squared_sum += diff_dps_z * diff_dps_z;
    }

    double denom = (double)(WINDOW_SIZE - 1); // pandas-compatible ddof=1

    aaf.a_x_std = sqrt(diff_a_x_squared_sum / denom);
    aaf.a_y_std = sqrt(diff_a_y_squared_sum / denom);
    aaf.a_z_std = sqrt(diff_a_z_squared_sum / denom);

    adf.dps_x_std = sqrt(diff_dps_x_squared_sum / denom);
    adf.dps_y_std = sqrt(diff_dps_y_squared_sum / denom);
    adf.dps_z_std = sqrt(diff_dps_z_squared_sum / denom);
}

void range(struct axis_accel_features aaf, struct axis_dps_features adf)
{
    aaf.a_x_range = aaf.a_x_max - aaf.a_x_min;
    aaf.a_y_range = aaf.a_y_max - aaf.a_y_min;
    aaf.a_z_range = aaf.a_z_max - aaf.a_z_min;

    adf.dps_x_range = adf.dps_x_max - adf.dps_x_min;
    adf.dps_y_range = adf.dps_y_max - adf.dps_y_min;
    adf.dps_z_range = adf.dps_z_max - adf.dps_z_min;
}

void abs_max(struct axis_accel_features aaf, struct axis_dps_features adf)
{
    aaf.a_x_abs_max = fabs(aaf.a_x_min) > aaf.a_x_max ? fabs(aaf.a_x_min) : aaf.a_x_max;
    aaf.a_y_abs_max = fabs(aaf.a_y_min) > aaf.a_y_max ? fabs(aaf.a_y_min) : aaf.a_y_max;
    aaf.a_z_abs_max = fabs(aaf.a_z_min) > aaf.a_z_max ? fabs(aaf.a_z_min) : aaf.a_z_max;

    adf.dps_x_abs_max = fabs(adf.dps_x_min) > adf.dps_x_max ? fabs(adf.dps_x_min) : adf.dps_x_max;
    adf.dps_y_abs_max = fabs(adf.dps_y_min) > adf.dps_y_max ? fabs(adf.dps_y_min) : adf.dps_y_max;
    adf.dps_z_abs_max = fabs(adf.dps_z_min) > adf.dps_z_max ? fabs(adf.dps_z_min) : adf.dps_z_max;
}

void delta(Motion_Sample *ordered_window)
{
    uint32_t last = WINDOW_SIZE - 1;
    aaf.a_x_delta = ordered_window[last].a_x - ordered_window[0].a_x;
    aaf.a_y_delta = ordered_window[last].a_y - ordered_window[0].a_y;
    aaf.a_z_delta = ordered_window[last].a_z - ordered_window[0].a_z;

    adf.dps_x_delta = ordered_window[last].dps_x - ordered_window[0].dps_x;
    adf.dps_y_delta = ordered_window[last].dps_y - ordered_window[0].dps_y;
    adf.dps_z_delta = ordered_window[last].dps_z - ordered_window[0].dps_z;
}

double *MotionFeatures_Extract(Motion_Sample *ordered_window)
{
    mean_min_max(ordered_window);
    std_dev(ordered_window);
    range(aaf, adf);
    return features;
}

void mean_min_max(Motion_Sample *ordered_window)
{
    double a_x_mean = 0;
    double a_x_min = DBL_MAX;
    double a_x_max = DBL_MIN;

    double a_y_mean = 0;
    double a_y_min = DBL_MAX;
    double a_y_max = DBL_MIN;

    double a_z_mean = 0;
    double a_z_min = DBL_MAX;
    double a_z_max = DBL_MIN;

    double dps_x_mean = 0;
    double dps_x_min = DBL_MAX;
    double dps_x_max = DBL_MIN;

    double dps_y_mean = 0;
    double dps_y_min = DBL_MAX;
    double dps_y_max = DBL_MIN;

    double dps_z_mean = 0;
    double dps_z_min = DBL_MAX;
    double dps_z_max = DBL_MIN;

    for (int i = 0; i < WINDOW_SIZE; ++i)
    {
        a_x_mean += (double)ordered_window[i].a_x;
        a_x_min = (double)ordered_window[i].a_x < a_x_min ? (double)ordered_window[i].a_x : a_x_min;
        a_x_max = (double)ordered_window[i].a_x > a_x_max ? (double)ordered_window[i].a_x : a_x_max;

        a_y_mean += (double)ordered_window[i].a_y;
        a_y_min = (double)ordered_window[i].a_y < a_y_min ? (double)ordered_window[i].a_y : a_y_min;
        a_y_max = (double)ordered_window[i].a_y > a_y_max ? (double)ordered_window[i].a_y : a_y_max;

        a_z_mean += (double)ordered_window[i].a_z;
        a_z_min = (double)ordered_window[i].a_z < a_z_min ? (double)ordered_window[i].a_z : a_z_min;
        a_z_max = (double)ordered_window[i].a_z > a_z_max ? (double)ordered_window[i].a_z : a_z_max;

        dps_x_mean += (double)ordered_window[i].dps_x;
        dps_x_min = (double)ordered_window[i].dps_x < dps_x_min ? (double)ordered_window[i].dps_x : dps_x_min;
        dps_x_max = (double)ordered_window[i].dps_x > dps_x_max ? (double)ordered_window[i].dps_x : dps_x_max;

        dps_y_mean += (double)ordered_window[i].dps_y;
        dps_y_min = (double)ordered_window[i].dps_y < dps_y_min ? (double)ordered_window[i].dps_y : dps_y_min;
        dps_y_max = (double)ordered_window[i].dps_y > dps_y_max ? (double)ordered_window[i].dps_y : dps_y_max;

        dps_z_mean += (double)ordered_window[i].dps_z;
        dps_z_min = (double)ordered_window[i].dps_z < dps_z_min ? (double)ordered_window[i].dps_z : dps_z_min;
        dps_z_max = (double)ordered_window[i].dps_z > dps_z_max ? (double)ordered_window[i].dps_z : dps_z_max;
    }

    aaf.a_x_mean = a_x_mean;
    aaf.a_x_min = a_x_min;
    aaf.a_x_max = a_x_max;

    aaf.a_y_mean = a_y_mean;
    aaf.a_y_min = a_y_min;
    aaf.a_y_max = a_y_max;

    aaf.a_z_mean = a_y_mean;
    aaf.a_z_min = a_z_min;
    aaf.a_z_max = a_z_max;

    adf.dps_x_mean = dps_x_mean;
    adf.dps_x_min = dps_x_min;
    adf.dps_x_max = dps_x_max;

    adf.dps_y_mean = dps_y_mean;
    adf.dps_y_min = dps_y_min;
    adf.dps_y_max = dps_y_max;

    adf.dps_z_mean = dps_z_mean;
    adf.dps_z_min = dps_z_min;
    adf.dps_z_max = dps_z_max;
}
