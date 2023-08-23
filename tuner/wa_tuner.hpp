#ifndef TUNER_WA_TUNER_H
#define TUNER_WA_TUNER_H

#include <array>
#include <tuner/global.hpp>

#define EXTERN extern "C"

static std::array<float, TUNER_SIZE> AUDIO_SAMPLES = {0};
static int AUDIO_SAMPLE_INDEX = 0;

EXTERN float get_frequency(int sample_rate);

/**
 * @brief Pushes a single audio sample represented by the float 'audio_sample' into the data structure.
 *
 * @param audio_sample The float value representing the audio sample to be pushed.
 *
 * @return None.
 */
EXTERN void push_value(float audio_sample);

/**
 * @brief Resets the buffer offset used in a data structure.
 *
 * @param None.
 *
 * @return None.
 */
EXTERN void clear_buffer_offset();
#endif //TUNER_WA_TUNER_H
