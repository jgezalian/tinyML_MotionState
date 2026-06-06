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
