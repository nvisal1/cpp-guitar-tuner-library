#ifndef TUNER_NOTE_HPP
#define TUNER_NOTE_HPP

#include <string>

namespace tuner {
    const float a1_hz = 440;
    const float c0_hz = 16.3516;
    const float b8_hz = 7902.13184;
    const std::string notes[12] = {
            "A",
            "A#",
            "B",
            "C",
            "C#",
            "D",
            "D#",
            "E",
            "F",
            "F#",
            "G",
            "G#",
    };

    struct note_context {
        std::string name;
        float closest_note_frequency;
        float actual_frequency;
    };

    /**
     * @brief Retrieves the note context for a given pitch value, represented by the float 'pitch', and returns a pointer to the note_context struct.
     *
     * @param frequency The float value representing the pitch for which the note context is to be retrieved.
     *
     * @return A pointer to the note_context struct representing the note information (name and pitch) corresponding to the given pitch value.
     */
    tuner::note_context* get_note_for_frequency(float frequency);
}

#endif //TUNER_NOTE_HPP
