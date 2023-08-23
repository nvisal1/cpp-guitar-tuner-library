#include <iostream>

#include <tuner/dsp.hpp>
#include <tuner/math.hpp>

bool tuner::signal_energy_is_too_low(std::array<float, TUNER_SIZE> m) {
    float signal_pow = (std::pow(tuner::euclidean_norm(m), float(2)) / float(m.size()));
    return signal_pow < tuner::SIGNAL_POWER_THRESHOLD;
}

std::array<float, TUNER_SIZE> tuner::apply_hanning_window(std::array<float, TUNER_SIZE> audio_stream_buffer) {
    std::array<float, TUNER_SIZE> with_hanning_window = {};
    for (int i = 0; i < audio_stream_buffer.size(); i++) {
        // https://en.wikipedia.org/wiki/Hann_function
        float multiplier = 0.5f * (1.0f - cos(2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(audio_stream_buffer.size())));
        with_hanning_window[i] = multiplier * audio_stream_buffer[i];
    }

    return with_hanning_window;
}

std::array<float, TUNER_SIZE / 2> tuner::calculate_magnitude_spec(float audio_buffer_stream_freq[TUNER_SIZE / 2]) {
    std::array<float, TUNER_SIZE / 2> out = {};
    for (int i = 0; i < TUNER_SIZE / 2; i++) {
        out[i] = abs(audio_buffer_stream_freq[i]);
    }

    return out;
}

float tuner::calculate_delta_frequency(int sample_rate, int fft_size) {
    if (fft_size == 0) {
        throw tuner::DivisionByZeroException();
    }
    return float(sample_rate) / float(fft_size);
}

std::array<float, TUNER_SIZE / 2> tuner::suppress_below_octave_bands(std::array<float, TUNER_SIZE / 2> mag_spec, float delta_frequency) {
    if (delta_frequency == 0) {
        throw tuner::DivisionByZeroException();
    }
    std::array<float, TUNER_SIZE / 2> out = mag_spec;
    for (int i = 0; i < tuner::OCTAVE_BANDS.size(); i++) {
        int start_index = tuner::OCTAVE_BANDS[i] / int(delta_frequency);
        int end_index = tuner::OCTAVE_BANDS[i + 1] / int(delta_frequency);
        if (mag_spec.size() <= end_index) {
            end_index = mag_spec.size();
        }

        float p_n = std::pow(tuner::euclidean_norm(mag_spec, start_index, end_index), float(2));
        int dividend = end_index - start_index;
        float avg_energy_per_freq = p_n / float(dividend);
        avg_energy_per_freq = std::pow(avg_energy_per_freq, float(0.5));
        for (int j = start_index; j < end_index; j++) {
            if (mag_spec[j] <= tuner::WHITE_NOISE_THRESHOLD * avg_energy_per_freq) {
                out[j] = 0;
            }
        }
    }

    return out;
}

std::vector<float> tuner::calculate_hps(std::vector<float> input) {
    std::vector<float> copy = input;

    for (int i = 0; i < tuner::NUM_HPS; i++) {
        int hps_len = int(std::ceil(input.size() / (i + 1)));
        int every_n = i + 1;
        std::vector<float> temp;
        for (int j = 0; j < hps_len; j++) {
            temp.push_back(copy[j] * input[j * every_n]);
        }
        if (temp.size() == 0) {
            break;
        }
        copy = temp;
    }

    return copy;
}

float tuner::get_max_frequency(std::vector<float> m, int sample_rate) {
    int max_index = 0;
    float tmp_max_freq = 0;
    for (int i = 0; i < m.size(); i++) {
        if (m[i] > tmp_max_freq) {
            tmp_max_freq = m[i];
            max_index = i;
        }
    }

    float max_freq = float(max_index) * (float(sample_rate) / float(TUNER_SIZE)) / float(tuner::NUM_HPS);

    return max_freq;
}

