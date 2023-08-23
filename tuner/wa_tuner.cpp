#include <tuner/tuner.hpp>
#include <tuner/wa_tuner.hpp>

EXTERN float get_frequency(int sample_rate) {
    return tuner::tune(AUDIO_SAMPLES, sample_rate)->actual_frequency;
}

EXTERN void push_value(float audio_sample) {
    AUDIO_SAMPLES[AUDIO_SAMPLE_INDEX] = audio_sample;
    AUDIO_SAMPLE_INDEX += 1;
}

EXTERN void clear_buffer_offset() {
    AUDIO_SAMPLE_INDEX = 0;
}