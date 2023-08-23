#include <cmath>

#include <tuner/note.hpp>
#include <tuner/math.hpp>

tuner::note_context* tuner::get_note_for_frequency(float frequency) {
    int i = int(std::round(std::log2(frequency / tuner::a1_hz) * 12));

    if (frequency > tuner::b8_hz) {
        auto *n = new tuner::note_context();
        n->name = "B8";
        n->closest_note_frequency = tuner::b8_hz;
        n->actual_frequency = frequency;
        return n;
    }

    if (frequency < tuner::c0_hz) {
        auto *n = new tuner::note_context();
        n->name = "C0";
        n->closest_note_frequency = tuner::c0_hz;
        n->actual_frequency = frequency;
        return n;
    }

    std::string octave = std::to_string(int(4 + std::floor((i + 9) / 12)));
    std::string note = tuner::notes[tuner::floored_modulo(i, 12)];
    float frequency_of_closest_note = tuner::a1_hz * std::pow(float(2), (float(i) / float(12)));
    auto *n = new tuner::note_context();
    n->name = note + octave;
    n->closest_note_frequency = frequency_of_closest_note;
    n->actual_frequency = frequency;
    return n;
}