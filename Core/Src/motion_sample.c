#include "motion_sample.h"
#include "sensors.h"

#define WINDOW_SIZE 20
#define STRIDE_SIZE 10

static Motion_Sample buffer[WINDOW_SIZE];

static uint32_t write_index = 0;
static uint32_t sample_count = 0;
static uint32_t new_samples_since_prediction = 0;
static bool first_prediction_done = false;

Motion_Sample ordered_window[WINDOW_SIZE];

void MotionBuffer_AddSample(Motion_Sample sample)
{
    buffer[write_index] = sample;

    write_index = (write_index + 1) % WINDOW_SIZE;

    if (sample_count < WINDOW_SIZE)
    {
        sample_count++;
    }

    new_samples_since_prediction++;
}

bool MotionBuffer_ShouldClassify(void)
{
    if (sample_count < WINDOW_SIZE)
    {
        return false;
    }

    if (!first_prediction_done)
    {
        return true;
    }

    return new_samples_since_prediction >= STRIDE_SIZE;
}

Motion_Sample *MotionBuffer_OrderedWindow(void)
{

    uint32_t buff_idx = write_index;
    uint32_t window_idx = 0;

    while (buff_idx < WINDOW_SIZE)
    {
        ordered_window[window_idx] = buffer[buff_idx];
        ++window_idx;
        ++buff_idx;
    }

    buff_idx = 0;
    while (buff_idx < write_index)
    {
        ordered_window[window_idx] = buffer[buff_idx];
        ++buff_idx;
        ++window_idx;
    }

    return ordered_window;
}

void MotionBuffer_MarkClassified(void)
{
    first_prediction_done = true;
    new_samples_since_prediction = 0;
}

void MotionBuffer_LoadBumpsTestWindow(void)
{
    static const Motion_Sample clockwise_circle_test_samples[WINDOW_SIZE] = {
        {0.0153, 0.0207, 0.9947, 0.6037, 0.0350, 0.3850},  {0.0146, 0.0182, 0.9944, 0.2188, -0.2275, 0.2713},
        {0.0181, 0.0227, 0.9902, 0.1838, -0.1312, 0.2013}, {0.0180, 0.0232, 0.9960, 0.4638, 0.0175, 0.2275},
        {0.0176, 0.0223, 0.9914, 0.2275, -0.0175, 0.1575}, {0.0163, 0.0207, 0.9913, 0.2362, 0.0787, 0.1838},
        {0.0183, 0.0203, 0.9960, 0.1838, 0.1488, 0.1312},  {0.0168, 0.0178, 0.9906, 0.1663, 0.1050, 0.2275},
        {0.0173, 0.0214, 0.9923, 0.3063, 0.1400, 0.1663},  {0.0164, 0.0198, 0.9938, 0.2800, 0.0700, 0.2100},
        {0.0181, 0.0214, 0.9960, 0.2013, 0.0875, 0.2013},  {0.0176, 0.0180, 0.9924, 0.1925, 0.1400, 0.1488},
        {0.0164, 0.0302, 1.0033, 0.5863, -0.0525, 0.1138}, {0.0251, 0.0285, 0.9938, 0.0000, -0.3237, 0.1312},
        {0.0180, 0.0212, 1.0016, 0.2800, 0.2100, 0.2362},  {0.0187, 0.0217, 0.9862, 0.3413, 0.0262, 0.1663},
        {0.0157, 0.0229, 0.9909, 0.3150, -0.0088, 0.1663}, {0.0180, 0.0249, 0.9943, 0.2713, 0.1225, 0.1400},
        {0.0176, 0.0209, 0.9955, 0.4112, -0.0613, 0.1750}, {0.0112, 0.0315, 0.9884, 0.2100, 0.1575, 0.1400},
    };

    write_index = 0;
    sample_count = 0;
    new_samples_since_prediction = 0;
    first_prediction_done = false;

    for (uint32_t i = 0; i < WINDOW_SIZE; ++i)
    {
        MotionBuffer_AddSample(clockwise_circle_test_samples[i]);
    }
}
