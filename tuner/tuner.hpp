#ifndef TUNER_TUNER_H
#define TUNER_TUNER_H

#include <array>
#include <string>

#include <tuner/global.hpp>
#include <tuner/note.hpp>



namespace tuner {

    /**
     * @brief Performs tuning on the audio stream buffer represented by the std::array 'audio_stream_buffer' with the specified 'sample_rate',
     *        and returns a const char* representing the tuned result.
     *
     * @param audio_stream_buffer The input std::array<float, TUNER_SIZE> representing the audio stream buffer to be tuned.
     * @param sample_rate The sample rate of the audio stream buffer.
     *
     * @return A const char* representing the tuned result of the audio stream buffer.
     */
    struct tuner::note_context *tune(std::array<float, TUNER_SIZE> audio_stream_buffer, int sample_rate);
}
#endif //TUNER_TUNER_H
