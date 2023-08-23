#include <kiss_fftr.h>

#include <tuner/tuner.hpp>
#include <tuner/dsp.hpp>
#include <tuner/vector.hpp>
#include <tuner/math.hpp>

std::vector<float> interpolate_spec(std::array<float, TUNER_SIZE / 2> mag_s) {
    std::vector<float> mag_s_i = tuner::interpolate(
            tuner::new_vector_with_values_between(0, mag_s.size(), float(1) / float(tuner::NUM_HPS)),
            tuner::new_vector_with_values_between(0, mag_s.size()),
            mag_s);

    float norm_val = tuner::euclidean_norm(mag_s_i);

    for (int i = 0; i < mag_s_i.size(); i++) {
        mag_s_i[i] = mag_s_i[i] / norm_val;
    }

    return mag_s_i;
}

struct tuner::note_context* tuner::tune(std::array<float, TUNER_SIZE> audio_stream_buffer, int sample_rate) {
    if (tuner::signal_energy_is_too_low(audio_stream_buffer)) {
        auto n = new tuner::note_context;
        n->name = "LOW";
        n->closest_note_frequency = -1;
        n->actual_frequency = -1;
        return n;
    }

    std::array<float, TUNER_SIZE> with_hanning = tuner::apply_hanning_window(audio_stream_buffer);
    kiss_fftr_cfg kiss_fftr_state = kiss_fftr_alloc(TUNER_SIZE, 0, nullptr, nullptr);
    kiss_fft_cpx fft_res[TUNER_SIZE / 2] = {};
    float in[TUNER_SIZE] = {};
    for (int i = 0; i < with_hanning.size(); i++) {
        in[i] = with_hanning[i];
    }
    kiss_fftr(kiss_fftr_state, in, fft_res);
    float out[TUNER_SIZE / 2] = {};
    for (int i = 0; i < TUNER_SIZE / 2; i++) {
        out[i] = fft_res[i].r;
    }
    std::array<float, TUNER_SIZE / 2> mag_s = tuner::calculate_magnitude_spec(out);

    // suppress hums
    float delta_frequency = tuner::calculate_delta_frequency(sample_rate, audio_stream_buffer.size());
    for (int i = 0; i < int(62 / delta_frequency); i++) {
        mag_s[i] = 0;
    }

    mag_s = tuner::suppress_below_octave_bands(mag_s, delta_frequency);

    std::vector<float> i = interpolate_spec(mag_s);

    std::vector<float> hps_spec = tuner::calculate_hps(i);
    i.clear();

    float max_frequency = tuner::get_max_frequency(hps_spec, sample_rate);

    tuner::note_context* n = tuner::get_note_for_frequency(max_frequency);

    return n;
}
