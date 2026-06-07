#include "motion_features.h"
#include "motion_meta.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

#define ACCEL_ATT_TYPE 0
#define DPS_ATT_TYPE 1
#define ACCEL_DEADBAND 0.05
#define DPS_DEADBAND 2

typedef enum
{
    MOTION_FEAT_ACCEL_MAG_MEAN = 0,
    MOTION_FEAT_ACCEL_MAG_MAX = 1,
    MOTION_FEAT_ACCEL_MAG_MIN = 2,
    MOTION_FEAT_ACCEL_MAG_RANGE = 3,

    MOTION_FEAT_DPS_MAG_MEAN = 4,
    MOTION_FEAT_DPS_MAG_MAX = 5,
    MOTION_FEAT_DPS_MAG_MIN = 6,
    MOTION_FEAT_DPS_MAG_RANGE = 7,

    MOTION_FEAT_A_X_MEAN = 8,
    MOTION_FEAT_A_X_STD = 9,
    MOTION_FEAT_A_X_MIN = 10,
    MOTION_FEAT_A_X_MAX = 11,
    MOTION_FEAT_A_X_RANGE = 12,
    MOTION_FEAT_A_X_ABS_MAX = 13,
    MOTION_FEAT_A_X_DELTA = 14,
    MOTION_FEAT_A_X_MAX_NEIGHBOR_DIFF = 15,
    MOTION_FEAT_A_X_MIN_NEIGHBOR_DIFF = 16,
    MOTION_FEAT_A_X_AVG_NEIGHBOR_DIFF = 17,
    MOTION_FEAT_A_X_MAX_GRADIENT = 18,
    MOTION_FEAT_A_X_MIN_GRADIENT = 19,
    MOTION_FEAT_A_X_ZERO_CROSSINGS = 20,

    MOTION_FEAT_A_Y_MEAN = 21,
    MOTION_FEAT_A_Y_STD = 22,
    MOTION_FEAT_A_Y_MIN = 23,
    MOTION_FEAT_A_Y_MAX = 24,
    MOTION_FEAT_A_Y_RANGE = 25,
    MOTION_FEAT_A_Y_ABS_MAX = 26,
    MOTION_FEAT_A_Y_DELTA = 27,
    MOTION_FEAT_A_Y_MAX_NEIGHBOR_DIFF = 28,
    MOTION_FEAT_A_Y_MIN_NEIGHBOR_DIFF = 29,
    MOTION_FEAT_A_Y_AVG_NEIGHBOR_DIFF = 30,
    MOTION_FEAT_A_Y_MAX_GRADIENT = 31,
    MOTION_FEAT_A_Y_MIN_GRADIENT = 32,
    MOTION_FEAT_A_Y_ZERO_CROSSINGS = 33,

    MOTION_FEAT_A_Z_MEAN = 34,
    MOTION_FEAT_A_Z_STD = 35,
    MOTION_FEAT_A_Z_MIN = 36,
    MOTION_FEAT_A_Z_MAX = 37,
    MOTION_FEAT_A_Z_RANGE = 38,
    MOTION_FEAT_A_Z_ABS_MAX = 39,
    MOTION_FEAT_A_Z_DELTA = 40,
    MOTION_FEAT_A_Z_MAX_NEIGHBOR_DIFF = 41,
    MOTION_FEAT_A_Z_MIN_NEIGHBOR_DIFF = 42,
    MOTION_FEAT_A_Z_AVG_NEIGHBOR_DIFF = 43,
    MOTION_FEAT_A_Z_MAX_GRADIENT = 44,
    MOTION_FEAT_A_Z_MIN_GRADIENT = 45,
    MOTION_FEAT_A_Z_ZERO_CROSSINGS = 46,

    MOTION_FEAT_DPS_X_MEAN = 47,
    MOTION_FEAT_DPS_X_STD = 48,
    MOTION_FEAT_DPS_X_MIN = 49,
    MOTION_FEAT_DPS_X_MAX = 50,
    MOTION_FEAT_DPS_X_RANGE = 51,
    MOTION_FEAT_DPS_X_ABS_MAX = 52,
    MOTION_FEAT_DPS_X_DELTA = 53,
    MOTION_FEAT_DPS_X_MAX_NEIGHBOR_DIFF = 54,
    MOTION_FEAT_DPS_X_MIN_NEIGHBOR_DIFF = 55,
    MOTION_FEAT_DPS_X_AVG_NEIGHBOR_DIFF = 56,
    MOTION_FEAT_DPS_X_MAX_GRADIENT = 57,
    MOTION_FEAT_DPS_X_MIN_GRADIENT = 58,
    MOTION_FEAT_DPS_X_ZERO_CROSSINGS = 59,

    MOTION_FEAT_DPS_Y_MEAN = 60,
    MOTION_FEAT_DPS_Y_STD = 61,
    MOTION_FEAT_DPS_Y_MIN = 62,
    MOTION_FEAT_DPS_Y_MAX = 63,
    MOTION_FEAT_DPS_Y_RANGE = 64,
    MOTION_FEAT_DPS_Y_ABS_MAX = 65,
    MOTION_FEAT_DPS_Y_DELTA = 66,
    MOTION_FEAT_DPS_Y_MAX_NEIGHBOR_DIFF = 67,
    MOTION_FEAT_DPS_Y_MIN_NEIGHBOR_DIFF = 68,
    MOTION_FEAT_DPS_Y_AVG_NEIGHBOR_DIFF = 69,
    MOTION_FEAT_DPS_Y_MAX_GRADIENT = 70,
    MOTION_FEAT_DPS_Y_MIN_GRADIENT = 71,
    MOTION_FEAT_DPS_Y_ZERO_CROSSINGS = 72,

    MOTION_FEAT_DPS_Z_MEAN = 73,
    MOTION_FEAT_DPS_Z_STD = 74,
    MOTION_FEAT_DPS_Z_MIN = 75,
    MOTION_FEAT_DPS_Z_MAX = 76,
    MOTION_FEAT_DPS_Z_RANGE = 77,
    MOTION_FEAT_DPS_Z_ABS_MAX = 78,
    MOTION_FEAT_DPS_Z_DELTA = 79,
    MOTION_FEAT_DPS_Z_MAX_NEIGHBOR_DIFF = 80,
    MOTION_FEAT_DPS_Z_MIN_NEIGHBOR_DIFF = 81,
    MOTION_FEAT_DPS_Z_AVG_NEIGHBOR_DIFF = 82,
    MOTION_FEAT_DPS_Z_MAX_GRADIENT = 83,
    MOTION_FEAT_DPS_Z_MIN_GRADIENT = 84,
    MOTION_FEAT_DPS_Z_ZERO_CROSSINGS = 85,

    MOTION_NUM_FEATURES = 86
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
    double dps_z_zero_crossings;
};

double features[N_FEATURES];
static struct axis_accel_features aaf;
static struct axis_dps_features adf;
static struct accel_global_features agf;
static struct dps_global_features dgf;

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

void range(struct axis_accel_features *aaf, struct axis_dps_features *adf)
{
    aaf->a_x_range = aaf->a_x_max - aaf->a_x_min;
    aaf->a_y_range = aaf->a_y_max - aaf->a_y_min;
    aaf->a_z_range = aaf->a_z_max - aaf->a_z_min;

    adf->dps_x_range = adf->dps_x_max - adf->dps_x_min;
    adf->dps_y_range = adf->dps_y_max - adf->dps_y_min;
    adf->dps_z_range = adf->dps_z_max - adf->dps_z_min;
}

void abs_max(struct axis_accel_features *aaf, struct axis_dps_features *adf)
{
    aaf->a_x_abs_max = fabs(aaf->a_x_min) > aaf->a_x_max ? fabs(aaf->a_x_min) : aaf->a_x_max;
    aaf->a_y_abs_max = fabs(aaf->a_y_min) > aaf->a_y_max ? fabs(aaf->a_y_min) : aaf->a_y_max;
    aaf->a_z_abs_max = fabs(aaf->a_z_min) > aaf->a_z_max ? fabs(aaf->a_z_min) : aaf->a_z_max;
    adf->dps_x_abs_max = fabs(adf->dps_x_min) > adf->dps_x_max ? fabs(adf->dps_x_min) : adf->dps_x_max;
    adf->dps_y_abs_max = fabs(adf->dps_y_min) > adf->dps_y_max ? fabs(adf->dps_y_min) : adf->dps_y_max;
    adf->dps_z_abs_max = fabs(adf->dps_z_min) > adf->dps_z_max ? fabs(adf->dps_z_min) : adf->dps_z_max;
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

    aaf.a_x_mean = a_x_mean / WINDOW_SIZE;
    aaf.a_x_min = a_x_min;
    aaf.a_x_max = a_x_max;

    aaf.a_y_mean = a_y_mean / WINDOW_SIZE;
    aaf.a_y_min = a_y_min;
    aaf.a_y_max = a_y_max;

    aaf.a_z_mean = a_z_mean / WINDOW_SIZE;
    aaf.a_z_min = a_z_min;
    aaf.a_z_max = a_z_max;

    adf.dps_x_mean = dps_x_mean / WINDOW_SIZE;
    adf.dps_x_min = dps_x_min;
    adf.dps_x_max = dps_x_max;

    adf.dps_y_mean = dps_y_mean / WINDOW_SIZE;
    adf.dps_y_min = dps_y_min;
    adf.dps_y_max = dps_y_max;

    adf.dps_z_mean = dps_z_mean / WINDOW_SIZE;
    adf.dps_z_min = dps_z_min;
    adf.dps_z_max = dps_z_max;
}

void max_neighbor_diff(Motion_Sample *ordered_window)
{
    double max_neighbor_diff_a_x = DBL_MIN;
    double max_neighbor_diff_a_y = DBL_MIN;
    double max_neighbor_diff_a_z = DBL_MIN;

    double max_neighbor_diff_dps_x = DBL_MIN;
    double max_neighbor_diff_dps_y = DBL_MIN;
    double max_neighbor_diff_dps_z = DBL_MIN;

    for (uint32_t i = 0; i < WINDOW_SIZE - 1; ++i)
    {
        uint32_t j = i + 1;

        max_neighbor_diff_a_x = (ordered_window[j].a_x - ordered_window[i].a_x) > max_neighbor_diff_a_x
                                    ? (ordered_window[j].a_x - ordered_window[i].a_x)
                                    : max_neighbor_diff_a_x;
        max_neighbor_diff_a_y = (ordered_window[j].a_y - ordered_window[i].a_y) > max_neighbor_diff_a_y
                                    ? (ordered_window[j].a_y - ordered_window[i].a_y)
                                    : max_neighbor_diff_a_y;
        max_neighbor_diff_a_z = (ordered_window[j].a_z - ordered_window[i].a_z) > max_neighbor_diff_a_z
                                    ? (ordered_window[j].a_z - ordered_window[i].a_z)
                                    : max_neighbor_diff_a_z;
        max_neighbor_diff_dps_x = (ordered_window[j].dps_x - ordered_window[i].dps_x) > max_neighbor_diff_dps_x
                                      ? (ordered_window[j].dps_x - ordered_window[i].dps_x)
                                      : max_neighbor_diff_dps_x;
        max_neighbor_diff_dps_y = (ordered_window[j].dps_y - ordered_window[i].dps_y) > max_neighbor_diff_dps_y
                                      ? (ordered_window[j].dps_y - ordered_window[i].dps_y)
                                      : max_neighbor_diff_dps_y;
        max_neighbor_diff_dps_z = (ordered_window[j].dps_z - ordered_window[i].dps_z) > max_neighbor_diff_dps_z
                                      ? (ordered_window[j].dps_z - ordered_window[i].dps_z)
                                      : max_neighbor_diff_dps_z;
    }

    aaf.a_x_max_neighbor_diff = max_neighbor_diff_a_x;
    aaf.a_y_max_neighbor_diff = max_neighbor_diff_a_y;
    aaf.a_z_max_neighbor_diff = max_neighbor_diff_a_z;

    adf.dps_x_max_neighbor_diff = max_neighbor_diff_dps_x;
    adf.dps_y_max_neighbor_diff = max_neighbor_diff_dps_y;
    adf.dps_z_max_neighbor_diff = max_neighbor_diff_dps_z;
}

void min_neighbor_diff(Motion_Sample *ordered_window)
{
    double min_neighbor_diff_a_x = DBL_MAX;
    double min_neighbor_diff_a_y = DBL_MAX;
    double min_neighbor_diff_a_z = DBL_MAX;

    double min_neighbor_diff_dps_x = DBL_MAX;
    double min_neighbor_diff_dps_y = DBL_MAX;
    double min_neighbor_diff_dps_z = DBL_MAX;

    for (uint32_t i = 0; i < WINDOW_SIZE - 1; ++i)
    {
        uint32_t j = i + 1;

        min_neighbor_diff_a_x = (ordered_window[j].a_x - ordered_window[i].a_x) < min_neighbor_diff_a_x
                                    ? (ordered_window[j].a_x - ordered_window[i].a_x)
                                    : min_neighbor_diff_a_x;
        min_neighbor_diff_a_y = (ordered_window[j].a_y - ordered_window[i].a_y) < min_neighbor_diff_a_y
                                    ? (ordered_window[j].a_y - ordered_window[i].a_y)
                                    : min_neighbor_diff_a_y;
        min_neighbor_diff_a_z = (ordered_window[j].a_z - ordered_window[i].a_z) < min_neighbor_diff_a_z
                                    ? (ordered_window[j].a_z - ordered_window[i].a_z)
                                    : min_neighbor_diff_a_z;
        min_neighbor_diff_dps_x = (ordered_window[j].dps_x - ordered_window[i].dps_x) < min_neighbor_diff_dps_x
                                      ? (ordered_window[j].dps_x - ordered_window[i].dps_x)
                                      : min_neighbor_diff_dps_x;
        min_neighbor_diff_dps_y = (ordered_window[j].dps_y - ordered_window[i].dps_y) < min_neighbor_diff_dps_y
                                      ? (ordered_window[j].dps_y - ordered_window[i].dps_y)
                                      : min_neighbor_diff_dps_y;
        min_neighbor_diff_dps_z = (ordered_window[j].dps_z - ordered_window[i].dps_z) < min_neighbor_diff_dps_z
                                      ? (ordered_window[j].dps_z - ordered_window[i].dps_z)
                                      : min_neighbor_diff_dps_z;
    }

    aaf.a_x_min_neighbor_diff = min_neighbor_diff_a_x;
    aaf.a_y_min_neighbor_diff = min_neighbor_diff_a_y;
    aaf.a_z_min_neighbor_diff = min_neighbor_diff_a_z;

    adf.dps_x_min_neighbor_diff = min_neighbor_diff_dps_x;
    adf.dps_y_min_neighbor_diff = min_neighbor_diff_dps_y;
    adf.dps_z_min_neighbor_diff = min_neighbor_diff_dps_z;
}

void avg_neighbor_diff(Motion_Sample *ordered_window)
{
    double avg_neighbor_diff_a_x = 0;
    double avg_neighbor_diff_a_y = 0;
    double avg_neighbor_diff_a_z = 0;

    double avg_neighbor_diff_dps_x = 0;
    double avg_neighbor_diff_dps_y = 0;
    double avg_neighbor_diff_dps_z = 0;

    for (uint32_t i = 0; i < WINDOW_SIZE - 1; ++i)
    {
        avg_neighbor_diff_a_x += ordered_window[i + 1].a_x - ordered_window[i].a_x;
        avg_neighbor_diff_a_y += ordered_window[i + 1].a_y - ordered_window[i].a_y;
        avg_neighbor_diff_a_z += ordered_window[i + 1].a_z - ordered_window[i].a_z;

        avg_neighbor_diff_dps_x += ordered_window[i + 1].dps_x - ordered_window[i].dps_x;
        avg_neighbor_diff_dps_y += ordered_window[i + 1].dps_y - ordered_window[i].dps_y;
        avg_neighbor_diff_dps_z += ordered_window[i + 1].dps_z - ordered_window[i].dps_z;
    }

    aaf.a_x_avg_neighbor_diff = avg_neighbor_diff_a_x / (WINDOW_SIZE - 1);
    aaf.a_y_avg_neighbor_diff = avg_neighbor_diff_a_y / (WINDOW_SIZE - 1);
    aaf.a_z_avg_neighbor_diff = avg_neighbor_diff_a_z / (WINDOW_SIZE - 1);

    adf.dps_x_avg_neighbor_diff = avg_neighbor_diff_dps_x / (WINDOW_SIZE - 1);
    adf.dps_y_avg_neighbor_diff = avg_neighbor_diff_dps_y / (WINDOW_SIZE - 1);
    adf.dps_z_avg_neighbor_diff = avg_neighbor_diff_dps_z / (WINDOW_SIZE - 1);
}

double min_gradient(Motion_Sample *ordered_window, Motion_Attributes att)
{
    if (WINDOW_SIZE < 2)
    {
        return 0.0;
    }

    double x0 = MotionFeatures_GetSample(att, ordered_window, 0);
    double x1 = MotionFeatures_GetSample(att, ordered_window, 1);

    double grad = x1 - x0;
    double min_grad = grad;

    for (uint32_t i = 1; i < WINDOW_SIZE - 1; ++i)
    {
        double prev = MotionFeatures_GetSample(att, ordered_window, i - 1);
        double next = MotionFeatures_GetSample(att, ordered_window, i + 1);

        grad = (next - prev) / 2.0;

        if (grad < min_grad)
        {
            min_grad = grad;
        }
    }

    double xn_1 = MotionFeatures_GetSample(att, ordered_window, WINDOW_SIZE - 1);
    double xn_2 = MotionFeatures_GetSample(att, ordered_window, WINDOW_SIZE - 2);

    grad = xn_1 - xn_2;

    if (grad < min_grad)
    {
        min_grad = grad;
    }

    return min_grad;
}

double max_gradient(Motion_Sample *ordered_window, Motion_Attributes att)
{
    if (WINDOW_SIZE < 2)
    {
        return 0.0;
    }

    double x0 = MotionFeatures_GetSample(att, ordered_window, 0);
    double x1 = MotionFeatures_GetSample(att, ordered_window, 1);

    double grad = x1 - x0;
    double max_grad = grad;

    for (uint32_t i = 1; i < WINDOW_SIZE - 1; ++i)
    {
        double prev = MotionFeatures_GetSample(att, ordered_window, i - 1);
        double next = MotionFeatures_GetSample(att, ordered_window, i + 1);

        grad = (next - prev) / 2.0;

        if (grad > max_grad)
        {
            max_grad = grad;
        }
    }

    double xn_1 = MotionFeatures_GetSample(att, ordered_window, WINDOW_SIZE - 1);
    double xn_2 = MotionFeatures_GetSample(att, ordered_window, WINDOW_SIZE - 2);

    grad = xn_1 - xn_2;

    if (grad > max_grad)
    {
        max_grad = grad;
    }

    return max_grad;
}

void set_gradients(Motion_Sample *ordered_window)
{
    aaf.a_x_max_gradient = max_gradient(ordered_window, A_X);
    aaf.a_y_max_gradient = max_gradient(ordered_window, A_Y);
    aaf.a_z_max_gradient = max_gradient(ordered_window, A_Z);

    adf.dps_x_max_gradient = max_gradient(ordered_window, DPS_X);
    adf.dps_y_max_gradient = max_gradient(ordered_window, DPS_Y);
    adf.dps_z_max_gradient = max_gradient(ordered_window, DPS_Z);

    aaf.a_x_min_gradient = min_gradient(ordered_window, A_X);
    aaf.a_y_min_gradient = min_gradient(ordered_window, A_Y);
    aaf.a_z_min_gradient = min_gradient(ordered_window, A_Z);

    adf.dps_x_min_gradient = min_gradient(ordered_window, DPS_X);
    adf.dps_y_min_gradient = min_gradient(ordered_window, DPS_Y);
    adf.dps_z_min_gradient = min_gradient(ordered_window, DPS_Z);
}

void zero_crossings(Motion_Sample *ordered_window)
{
    double a_x_zero_cross = 0;
    double a_y_zero_cross = 0;
    double a_z_zero_cross = 0;

    double dps_x_zero_cross = 0;
    double dps_y_zero_cross = 0;
    double dps_z_zero_cross = 0;

    uint8_t prev_neg_a_x = ordered_window[0].a_x < (-1 * ACCEL_DEADBAND) ? 1 : 0;
    uint8_t prev_pos_a_x = ordered_window[0].a_x > ACCEL_DEADBAND ? 1 : 0;

    uint8_t prev_neg_a_y = ordered_window[0].a_y < (-1 * ACCEL_DEADBAND) ? 1 : 0;
    uint8_t prev_pos_a_y = ordered_window[0].a_y > ACCEL_DEADBAND ? 1 : 0;

    uint8_t prev_neg_a_z = ordered_window[0].a_z < (-1 * ACCEL_DEADBAND) ? 1 : 0;
    uint8_t prev_pos_a_z = ordered_window[0].a_z > ACCEL_DEADBAND ? 1 : 0;

    uint8_t prev_neg_dps_x = ordered_window[0].dps_x < (-1 * DPS_DEADBAND) ? 1 : 0;
    uint8_t prev_pos_dps_x = ordered_window[0].dps_x > DPS_DEADBAND ? 1 : 0;

    uint8_t prev_neg_dps_y = ordered_window[0].dps_y < (-1 * DPS_DEADBAND) ? 1 : 0;
    uint8_t prev_pos_dps_y = ordered_window[0].dps_y > DPS_DEADBAND ? 1 : 0;

    uint8_t prev_neg_dps_z = ordered_window[0].dps_z < (-1 * DPS_DEADBAND) ? 1 : 0;
    uint8_t prev_pos_dps_z = ordered_window[0].dps_z > DPS_DEADBAND ? 1 : 0;

    for (uint32_t i = 1; i < WINDOW_SIZE; ++i)
    {

        double a_x_cur = ordered_window[i].a_x;

        sign_check(&a_x_zero_cross, a_x_cur, &prev_pos_a_x, &prev_neg_a_x, ACCEL_ATT_TYPE);

        double a_y_cur = ordered_window[i].a_y;

        sign_check(&a_y_zero_cross, a_y_cur, &prev_pos_a_y, &prev_neg_a_y, ACCEL_ATT_TYPE);

        double a_z_cur = ordered_window[i].a_z;

        sign_check(&a_z_zero_cross, a_z_cur, &prev_pos_a_z, &prev_neg_a_z, ACCEL_ATT_TYPE);

        double dps_x_cur = ordered_window[i].dps_x;

        sign_check(&dps_x_zero_cross, dps_x_cur, &prev_pos_dps_x, &prev_neg_dps_x, DPS_ATT_TYPE);

        double dps_y_cur = ordered_window[i].dps_y;

        sign_check(&dps_y_zero_cross, dps_y_cur, &prev_pos_dps_y, &prev_neg_dps_y, DPS_ATT_TYPE);

        double dps_z_cur = ordered_window[i].dps_z;

        sign_check(&dps_z_zero_cross, dps_z_cur, &prev_pos_dps_z, &prev_neg_dps_z, DPS_ATT_TYPE);
    }

    aaf.a_x_zero_crossings = a_x_zero_cross;
    aaf.a_y_zero_crossings = a_y_zero_cross;
    aaf.a_z_zero_crossings = a_z_zero_cross;

    adf.dps_x_zero_crossings = dps_x_zero_cross;
    adf.dps_y_zero_crossings = dps_y_zero_cross;
    adf.dps_z_zero_crossings = dps_z_zero_cross;
}

double MotionFeatures_GetSample(Motion_Attributes att, Motion_Sample *ordered_window, uint32_t index)
{
    switch (att)
    {
    case A_X:
        return ordered_window[index].a_x;
    case A_Y:
        return ordered_window[index].a_y;
    case A_Z:
        return ordered_window[index].a_z;
    case DPS_X:
        return ordered_window[index].dps_x;
    case DPS_Y:
        return ordered_window[index].dps_y;
    case DPS_Z:
        return ordered_window[index].dps_z;
    default:
        return 0;
    }

    return 0;
}

void sign_check(double *att_zero_cross, double cur, uint8_t *prev_pos, uint8_t *prev_neg, uint8_t att_type)
{

    if (att_type == ACCEL_ATT_TYPE)
    {
        if (cur < (-1 * ACCEL_DEADBAND))
        {
            if (*prev_pos)
            {
                ++*att_zero_cross;
                *prev_pos = 0;
            }

            *prev_neg = 1;
        }

        if (cur > ACCEL_DEADBAND)
        {
            if (*prev_neg)
            {
                ++*att_zero_cross;
                *prev_neg = 0;
            }

            *prev_pos = 1;
        }
    }

    if (att_type == DPS_ATT_TYPE)
    {
        if (cur < (-1 * DPS_DEADBAND))
        {
            if (*prev_pos)
            {
                ++*att_zero_cross;
                *prev_pos = 0;
            }

            *prev_neg = 1;
        }

        if (cur > DPS_DEADBAND)
        {
            if (*prev_neg)
            {
                ++*att_zero_cross;
                *prev_neg = 0;
            }

            *prev_pos = 1;
        }
    }
}

void mag_mean_min_max(Motion_Sample *ordered_window)
{
    double accel_mag_mean = 0;
    double accel_mag_min = DBL_MAX;
    double accel_mag_max = DBL_MIN;

    double dps_mag_mean = 0;
    double dps_mag_min = DBL_MAX;
    double dps_mag_max = DBL_MIN;

    for (int i = 0; i < WINDOW_SIZE; ++i)
    {
        double a_x = ordered_window[i].a_x;
        double a_y = ordered_window[i].a_y;
        double a_z = ordered_window[i].a_z;

        double dps_x = ordered_window[i].dps_x;
        double dps_y = ordered_window[i].dps_y;
        double dps_z = ordered_window[i].dps_z;

        double accel_mag = sqrt((a_x * a_x) + (a_y * a_y) + (a_z * a_z));
        double dps_mag = sqrt((dps_x * dps_x) + (dps_y * dps_y) + (dps_z * dps_z));

        accel_mag_mean += accel_mag;
        dps_mag_mean += dps_mag;

        accel_mag_min = accel_mag < accel_mag_min ? accel_mag : accel_mag_min;
        accel_mag_max = accel_mag > accel_mag_max ? accel_mag : accel_mag_max;

        dps_mag_min = dps_mag < dps_mag_min ? dps_mag : dps_mag_min;
        dps_mag_max = dps_mag > dps_mag_max ? dps_mag : dps_mag_max;
    }

    agf.accel_mag_mean = accel_mag_mean / WINDOW_SIZE;
    agf.accel_mag_min = accel_mag_min;
    agf.accel_mag_max = accel_mag_max;

    dgf.dps_mag_mean = dps_mag_mean / WINDOW_SIZE;
    dgf.dps_mag_min = dps_mag_min;
    dgf.dps_mag_max = dps_mag_max;
}

void mag_range(struct accel_global_features *agf, struct dps_global_features *dgf)
{
    agf->accel_mag_range = agf->accel_mag_max - agf->accel_mag_min;
    dgf->dps_mag_range = dgf->dps_mag_max - dgf->dps_mag_min;
}

static void load_features_array(void)
{
    features[MOTION_FEAT_ACCEL_MAG_MEAN] = agf.accel_mag_mean;
    features[MOTION_FEAT_ACCEL_MAG_MAX] = agf.accel_mag_max;
    features[MOTION_FEAT_ACCEL_MAG_MIN] = agf.accel_mag_min;
    features[MOTION_FEAT_ACCEL_MAG_RANGE] = agf.accel_mag_range;

    features[MOTION_FEAT_DPS_MAG_MEAN] = dgf.dps_mag_mean;
    features[MOTION_FEAT_DPS_MAG_MAX] = dgf.dps_mag_max;
    features[MOTION_FEAT_DPS_MAG_MIN] = dgf.dps_mag_min;
    features[MOTION_FEAT_DPS_MAG_RANGE] = dgf.dps_mag_range;

    features[MOTION_FEAT_A_X_MEAN] = aaf.a_x_mean;
    features[MOTION_FEAT_A_X_STD] = aaf.a_x_std;
    features[MOTION_FEAT_A_X_MIN] = aaf.a_x_min;
    features[MOTION_FEAT_A_X_MAX] = aaf.a_x_max;
    features[MOTION_FEAT_A_X_RANGE] = aaf.a_x_range;
    features[MOTION_FEAT_A_X_ABS_MAX] = aaf.a_x_abs_max;
    features[MOTION_FEAT_A_X_DELTA] = aaf.a_x_delta;
    features[MOTION_FEAT_A_X_MAX_NEIGHBOR_DIFF] = aaf.a_x_max_neighbor_diff;
    features[MOTION_FEAT_A_X_MIN_NEIGHBOR_DIFF] = aaf.a_x_min_neighbor_diff;
    features[MOTION_FEAT_A_X_AVG_NEIGHBOR_DIFF] = aaf.a_x_avg_neighbor_diff;
    features[MOTION_FEAT_A_X_MAX_GRADIENT] = aaf.a_x_max_gradient;
    features[MOTION_FEAT_A_X_MIN_GRADIENT] = aaf.a_x_min_gradient;
    features[MOTION_FEAT_A_X_ZERO_CROSSINGS] = aaf.a_x_zero_crossings;

    features[MOTION_FEAT_A_Y_MEAN] = aaf.a_y_mean;
    features[MOTION_FEAT_A_Y_STD] = aaf.a_y_std;
    features[MOTION_FEAT_A_Y_MIN] = aaf.a_y_min;
    features[MOTION_FEAT_A_Y_MAX] = aaf.a_y_max;
    features[MOTION_FEAT_A_Y_RANGE] = aaf.a_y_range;
    features[MOTION_FEAT_A_Y_ABS_MAX] = aaf.a_y_abs_max;
    features[MOTION_FEAT_A_Y_DELTA] = aaf.a_y_delta;
    features[MOTION_FEAT_A_Y_MAX_NEIGHBOR_DIFF] = aaf.a_y_max_neighbor_diff;
    features[MOTION_FEAT_A_Y_MIN_NEIGHBOR_DIFF] = aaf.a_y_min_neighbor_diff;
    features[MOTION_FEAT_A_Y_AVG_NEIGHBOR_DIFF] = aaf.a_y_avg_neighbor_diff;
    features[MOTION_FEAT_A_Y_MAX_GRADIENT] = aaf.a_y_max_gradient;
    features[MOTION_FEAT_A_Y_MIN_GRADIENT] = aaf.a_y_min_gradient;
    features[MOTION_FEAT_A_Y_ZERO_CROSSINGS] = aaf.a_y_zero_crossings;

    features[MOTION_FEAT_A_Z_MEAN] = aaf.a_z_mean;
    features[MOTION_FEAT_A_Z_STD] = aaf.a_z_std;
    features[MOTION_FEAT_A_Z_MIN] = aaf.a_z_min;
    features[MOTION_FEAT_A_Z_MAX] = aaf.a_z_max;
    features[MOTION_FEAT_A_Z_RANGE] = aaf.a_z_range;
    features[MOTION_FEAT_A_Z_ABS_MAX] = aaf.a_z_abs_max;
    features[MOTION_FEAT_A_Z_DELTA] = aaf.a_z_delta;
    features[MOTION_FEAT_A_Z_MAX_NEIGHBOR_DIFF] = aaf.a_z_max_neighbor_diff;
    features[MOTION_FEAT_A_Z_MIN_NEIGHBOR_DIFF] = aaf.a_z_min_neighbor_diff;
    features[MOTION_FEAT_A_Z_AVG_NEIGHBOR_DIFF] = aaf.a_z_avg_neighbor_diff;
    features[MOTION_FEAT_A_Z_MAX_GRADIENT] = aaf.a_z_max_gradient;
    features[MOTION_FEAT_A_Z_MIN_GRADIENT] = aaf.a_z_min_gradient;
    features[MOTION_FEAT_A_Z_ZERO_CROSSINGS] = aaf.a_z_zero_crossings;

    features[MOTION_FEAT_DPS_X_MEAN] = adf.dps_x_mean;
    features[MOTION_FEAT_DPS_X_STD] = adf.dps_x_std;
    features[MOTION_FEAT_DPS_X_MIN] = adf.dps_x_min;
    features[MOTION_FEAT_DPS_X_MAX] = adf.dps_x_max;
    features[MOTION_FEAT_DPS_X_RANGE] = adf.dps_x_range;
    features[MOTION_FEAT_DPS_X_ABS_MAX] = adf.dps_x_abs_max;
    features[MOTION_FEAT_DPS_X_DELTA] = adf.dps_x_delta;
    features[MOTION_FEAT_DPS_X_MAX_NEIGHBOR_DIFF] = adf.dps_x_max_neighbor_diff;
    features[MOTION_FEAT_DPS_X_MIN_NEIGHBOR_DIFF] = adf.dps_x_min_neighbor_diff;
    features[MOTION_FEAT_DPS_X_AVG_NEIGHBOR_DIFF] = adf.dps_x_avg_neighbor_diff;
    features[MOTION_FEAT_DPS_X_MAX_GRADIENT] = adf.dps_x_max_gradient;
    features[MOTION_FEAT_DPS_X_MIN_GRADIENT] = adf.dps_x_min_gradient;
    features[MOTION_FEAT_DPS_X_ZERO_CROSSINGS] = adf.dps_x_zero_crossings;

    features[MOTION_FEAT_DPS_Y_MEAN] = adf.dps_y_mean;
    features[MOTION_FEAT_DPS_Y_STD] = adf.dps_y_std;
    features[MOTION_FEAT_DPS_Y_MIN] = adf.dps_y_min;
    features[MOTION_FEAT_DPS_Y_MAX] = adf.dps_y_max;
    features[MOTION_FEAT_DPS_Y_RANGE] = adf.dps_y_range;
    features[MOTION_FEAT_DPS_Y_ABS_MAX] = adf.dps_y_abs_max;
    features[MOTION_FEAT_DPS_Y_DELTA] = adf.dps_y_delta;
    features[MOTION_FEAT_DPS_Y_MAX_NEIGHBOR_DIFF] = adf.dps_y_max_neighbor_diff;
    features[MOTION_FEAT_DPS_Y_MIN_NEIGHBOR_DIFF] = adf.dps_y_min_neighbor_diff;
    features[MOTION_FEAT_DPS_Y_AVG_NEIGHBOR_DIFF] = adf.dps_y_avg_neighbor_diff;
    features[MOTION_FEAT_DPS_Y_MAX_GRADIENT] = adf.dps_y_max_gradient;
    features[MOTION_FEAT_DPS_Y_MIN_GRADIENT] = adf.dps_y_min_gradient;
    features[MOTION_FEAT_DPS_Y_ZERO_CROSSINGS] = adf.dps_y_zero_crossings;

    features[MOTION_FEAT_DPS_Z_MEAN] = adf.dps_z_mean;
    features[MOTION_FEAT_DPS_Z_STD] = adf.dps_z_std;
    features[MOTION_FEAT_DPS_Z_MIN] = adf.dps_z_min;
    features[MOTION_FEAT_DPS_Z_MAX] = adf.dps_z_max;
    features[MOTION_FEAT_DPS_Z_RANGE] = adf.dps_z_range;
    features[MOTION_FEAT_DPS_Z_ABS_MAX] = adf.dps_z_abs_max;
    features[MOTION_FEAT_DPS_Z_DELTA] = adf.dps_z_delta;
    features[MOTION_FEAT_DPS_Z_MAX_NEIGHBOR_DIFF] = adf.dps_z_max_neighbor_diff;
    features[MOTION_FEAT_DPS_Z_MIN_NEIGHBOR_DIFF] = adf.dps_z_min_neighbor_diff;
    features[MOTION_FEAT_DPS_Z_AVG_NEIGHBOR_DIFF] = adf.dps_z_avg_neighbor_diff;
    features[MOTION_FEAT_DPS_Z_MAX_GRADIENT] = adf.dps_z_max_gradient;
    features[MOTION_FEAT_DPS_Z_MIN_GRADIENT] = adf.dps_z_min_gradient;
    features[MOTION_FEAT_DPS_Z_ZERO_CROSSINGS] = adf.dps_z_zero_crossings;
}

double *MotionFeatures_Extract(Motion_Sample *ordered_window)
{
    mean_min_max(ordered_window); // check mean
    std_dev(ordered_window);      // check std
    range(&aaf, &adf);
    abs_max(&aaf, &adf);
    delta(ordered_window);
    max_neighbor_diff(ordered_window);
    min_neighbor_diff(ordered_window);
    avg_neighbor_diff(ordered_window);
    set_gradients(ordered_window);
    zero_crossings(ordered_window);
    mag_mean_min_max(ordered_window);
    mag_range(&agf, &dgf);
    load_features_array();
    return features;
}
