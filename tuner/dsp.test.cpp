#include <array>

#include <catch2/catch_test_macros.hpp>
#include <tuner/dsp.hpp>
#include <tuner/math.hpp>

#include <cmath>

TEST_CASE("[signal_energy_is_too_low] signal energy is above the threshold") {
    std::array<float, TUNER_SIZE> m = {1.0, 2.0, 3.0, 4.0, 5.0};
    bool result = tuner::signal_energy_is_too_low(m);

    REQUIRE(result == false);
}

TEST_CASE("[signal_energy_is_too_low] signal energy is below the threshold") {
    float epsilon = 1e-6;
    std::array<float, TUNER_SIZE> m = {0.5f * epsilon, 0.6f * epsilon, 0.7f * epsilon, 0.8f * epsilon, 0.9f * epsilon};
    bool result = tuner::signal_energy_is_too_low(m);

    REQUIRE(result == true);
}

TEST_CASE("[signal_energy_is_too_low] signal energy is an empty array") {
    std::array<float, TUNER_SIZE> m = {};
    bool result = tuner::signal_energy_is_too_low(m);

    REQUIRE(result == true);
}

TEST_CASE("[signal_energy_is_too_low] signal energy contains all negative values") {
    std::array<float, TUNER_SIZE> m = {-1.0, -2.0, -3.0, -4.0, -5.0};
    bool result = tuner::signal_energy_is_too_low(m);

    REQUIRE(result == false);
}

TEST_CASE("[signal_energy_is_too_low] signal energy contains one value") {
    std::array<float, TUNER_SIZE> m = {1.0};
    bool result = tuner::signal_energy_is_too_low(m);

    REQUIRE(result == false);
}

TEST_CASE("[apply_hanning_window] all elements are zero") {
    std::array<float, TUNER_SIZE> audio_stream_buffer = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::array<float, TUNER_SIZE> result = tuner::apply_hanning_window(audio_stream_buffer);
    for (size_t i = 0; i < TUNER_SIZE; i++) {
        REQUIRE(result[i] == 0.0f);
    }
}

TEST_CASE("[apply_hanning_window] all elements are one") {
    std::array<float, TUNER_SIZE> audio_stream_buffer = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    std::array<float, TUNER_SIZE> result = tuner::apply_hanning_window(audio_stream_buffer);
    for (size_t i = 0; i < TUNER_SIZE; i++) {
        REQUIRE(result[i] == audio_stream_buffer[i] * (0.5f * (1.0f - cos(2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(audio_stream_buffer.size())))));
    }
}

TEST_CASE("[apply_hanning_window] alternate positive and negative elements") {
    std::array<float, TUNER_SIZE> audio_stream_buffer = {1.0f, -1.0f, 1.0f, -1.0f, 1.0f};
    std::array<float, TUNER_SIZE> result = tuner::apply_hanning_window(audio_stream_buffer);
    for (size_t i = 0; i < TUNER_SIZE; i++) {
        float expected = audio_stream_buffer[i] * (0.5f * (1.0f - cos(2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(audio_stream_buffer.size()))));
        REQUIRE(std::abs(expected - result[i]) < 1e-6);
    }
}

TEST_CASE("[calculate_magnitude_spec] all elements are zero") {
    float audio_stream_buffer[TUNER_SIZE / 2] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::array<float, TUNER_SIZE / 2> result = tuner::calculate_magnitude_spec(audio_stream_buffer);
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        REQUIRE(result[i] == 0.0f);
    }
}

TEST_CASE("[calculate_magnitude_spec] all elements are one") {
    float audio_stream_buffer[TUNER_SIZE / 2] = {};
    std::fill_n(audio_stream_buffer, TUNER_SIZE / 2, 1.0f);
    std::array<float, TUNER_SIZE / 2> result = tuner::calculate_magnitude_spec(audio_stream_buffer);
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        REQUIRE(audio_stream_buffer[i] == 1.0f);
    }
}

TEST_CASE("[calculate_magnitude_spec] alternate positive and negative elements") {
    float audio_stream_buffer[TUNER_SIZE / 2] = {};
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        if (i % 2 == 0) {
            audio_stream_buffer[i] = 1.0f;
        } else {
            audio_stream_buffer[i] = -1.0f;
        }
    }
    std::array<float, TUNER_SIZE / 2> result = tuner::calculate_magnitude_spec(audio_stream_buffer);
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        REQUIRE(result[i] == 1.0f);
    }
}


TEST_CASE("[calculate_magnitude_spec] all negative elements") {
    float audio_stream_buffer[TUNER_SIZE / 2] = {};
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        audio_stream_buffer[i] = -1.0f;
    }
    std::array<float, TUNER_SIZE / 2> result = tuner::calculate_magnitude_spec(audio_stream_buffer);
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        REQUIRE(result[i] == 1.0f);
    }
}

TEST_CASE("[calculate_delta_frequency] sample rate of 44100 Hz and an FFT size of 1024") {
    int sample_rate = 44100;
    int fft_size = 1024;
    float result = tuner::calculate_delta_frequency(sample_rate, fft_size);
    float expected = static_cast<float>(sample_rate) / static_cast<float>(fft_size);
    REQUIRE(std::abs(result - expected) < 1e-6);
}

TEST_CASE("[calculate_delta_frequency] sample rate of 48000 Hz and an FFT size of 2048") {
    int sample_rate = 48000;
    int fft_size = 2048;
    float result = tuner::calculate_delta_frequency(sample_rate, fft_size);
    float expected = static_cast<float>(sample_rate) / static_cast<float>(fft_size);
    REQUIRE(std::abs(result - expected) < 1e-6);
}

TEST_CASE("[calculate_delta_frequency] sample rate of 22050 Hz and an FFT size of 512") {
    int sample_rate = 22050;
    int fft_size = 512;
    float result = tuner::calculate_delta_frequency(sample_rate, fft_size);
    float expected = static_cast<float>(sample_rate) / static_cast<float>(fft_size);
    REQUIRE(std::abs(result - expected) < 1e-6);
}

TEST_CASE("[calculate_delta_frequency] sample rate of 8000 Hz and an FFT size of 128") {
    int sample_rate = 8000;
    int fft_size = 128;
    float result = tuner::calculate_delta_frequency(sample_rate, fft_size);
    float expected = static_cast<float>(sample_rate) / static_cast<float>(fft_size);
    REQUIRE(std::abs(result - expected) < 1e-6);
}

TEST_CASE("[calculate_delta_frequency] large sample rate and FFT size") {
    int sample_rate = 96000;
    int fft_size = 4096;
    float result = tuner::calculate_delta_frequency(sample_rate, fft_size);
    float expected = static_cast<float>(sample_rate) / static_cast<float>(fft_size);
    REQUIRE(std::abs(result - expected) < 1e-6);
}

TEST_CASE("[calculate_delta_frequency] small sample rate and FFT size") {
    int sample_rate = 8000;
    int fft_size = 64;
    float result = tuner::calculate_delta_frequency(sample_rate, fft_size);
    float expected = static_cast<float>(sample_rate) / static_cast<float>(fft_size);
    REQUIRE(std::abs(result - expected) < 1e-6);
}

TEST_CASE("[calculate_delta_frequency] FFT size is zero") {
    try {
        int sample_rate = 8000;
        int fft_size = 0;
        tuner::calculate_delta_frequency(sample_rate, fft_size);
        REQUIRE(false);
    } catch (tuner::DivisionByZeroException& de) {
        REQUIRE(std::string(de.what()) == "Division by zero exception");
    }
}

TEST_CASE("[suppress_below_octave_bands] all magnitude spectrum elements are zero") {
    std::array<float, TUNER_SIZE / 2> mag_spec = {};
    float delta_frequency = 10.0; // Arbitrary value for delta frequency
    std::array<float, TUNER_SIZE / 2> result = tuner::suppress_below_octave_bands(mag_spec, delta_frequency);
    for (size_t i = 0; i < mag_spec.size(); i++) {
        REQUIRE(result[i] == 0.0f);
    }
}

TEST_CASE("[suppress_below_octave_bands] elements under the first octave band are below the delta frequency") {
    std::array<float, TUNER_SIZE / 2> mag_spec = {
            5.0, 8.0, 3.0, 2.0, 1.0, // not included
            5.0, 8.0, 3.0, 0.4, 0.7, // under octave band 50
    };
    float delta_frequency = 10.0; // target octave band 50 elements 5-10
    std::array<float, TUNER_SIZE / 2> result = tuner::suppress_below_octave_bands(mag_spec, delta_frequency);
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        if (i > 0 && i < 5) {
            REQUIRE(result[i] == mag_spec[i]);
        } else {
            if (mag_spec[i] < 1) {
                REQUIRE(result[i] == 0); // anything under ~0.88 should be set to 0
            } else {
                REQUIRE(result[i] == mag_spec[i]);
            }
        }
    }
}

TEST_CASE("[suppress_below_octave_bands] elements under the first octave band are above the delta frequency") {
    std::array<float, TUNER_SIZE / 2> mag_spec = {
            5.0, 8.0, 3.0, 2.0, 1.0, // not included
            5.0, 8.0, 3.0, 1.4, 1.7, // under octave band 50
    };
    float delta_frequency = 10.0; // target octave band 50 elements 5-10
    std::array<float, TUNER_SIZE / 2> result = tuner::suppress_below_octave_bands(mag_spec, delta_frequency);
    for (size_t i = 0; i < TUNER_SIZE / 2; i++) {
        REQUIRE(result[i] == mag_spec[i]);
    }
}

TEST_CASE("[suppress_below_octave_bands] delta frequency is zero") {
    try {
        std::array<float, TUNER_SIZE / 2> mag_spec = {};
        float delta_frequency = 0.0f;
        std::array<float, TUNER_SIZE / 2> result = tuner::suppress_below_octave_bands(mag_spec, delta_frequency);
        REQUIRE(false);
    } catch (tuner::DivisionByZeroException& de) {
        REQUIRE(std::string(de.what()) == "Division by zero exception");
    }
}

TEST_CASE("[calculate_hps] empty input vector") {
    std::vector<float> input = {};
    std::vector<float> result = tuner::calculate_hps(input);
    REQUIRE(result.empty());
}

TEST_CASE("[calculate_hps] input vector contains single element") {
    std::vector<float> input = {3.15};
    std::vector<float> result = tuner::calculate_hps(input);
    REQUIRE(result.size() == 1);
    REQUIRE(std::abs(result[0] - std::pow(input[0], 2)) < 1e-6);
}

TEST_CASE("[calculate_hps] input vector contains less than NUM_HPS elements") {
    std::vector<float> input = {1.0, 2.0, 3.0};
    std::vector<float> result = tuner::calculate_hps(input);
    REQUIRE(result.size() == 1);
    REQUIRE(std::abs(result[0] - std::pow(input[0], 2)) < 1e-6);
}

TEST_CASE("[calculate_hps] input vector contains 2(NUM_HPS) elements") {
    std::vector<float> input = {1.0, 2.0, 3.0, 4.0, 5.0,
                                1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<float> result = tuner::calculate_hps(input);
    REQUIRE(result.size() == 2);
    REQUIRE(std::abs(result[0] - std::pow(input[0], 2)) < 1e-6);
}

TEST_CASE("[get_max_frequency] empty magnitude spectrum") {
    std::vector<float> m = {};
    int sample_rate = 44100;
    float result = tuner::get_max_frequency(m, sample_rate);
    REQUIRE(result == 0.0);
}

TEST_CASE("[get_max_frequency] magnitude spectrum contains all zeroes") {
    std::vector<float> m = {0.0, 0.0, 0.0, 0.0, 0.0};
    int sample_rate = 44100;
    float result = tuner::get_max_frequency(m, sample_rate);
    REQUIRE(result == 0.0);
}

float convert_to_frequency(int index, int sample_rate) {
    return (float(index) * (float(sample_rate) / float(TUNER_SIZE)) / float(tuner::NUM_HPS));
}

TEST_CASE("[get_max_frequency] magnitude spectrum contains single peak") {
    std::vector<float> m = {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0};
    int sample_rate = 44100;
    float result = tuner::get_max_frequency(m, sample_rate);
    REQUIRE(result == convert_to_frequency(3, sample_rate));
}

TEST_CASE("[get_max_frequency] magnitude spectrum contains multiple peaks") {
    std::vector<float> m = {0.0, 0.0, 5.0, 0.0, 8.0, 0.0, 0.0};
    int sample_rate = 44100;
    float result = tuner::get_max_frequency(m, sample_rate);
    REQUIRE(result == convert_to_frequency(4, sample_rate));
}

TEST_CASE("[get_max_frequency] larger magnitude spectrum and sample size") {
    std::vector<float> m = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    int sample_rate = 96000;
    float result = tuner::get_max_frequency(m, sample_rate);
    REQUIRE(result == convert_to_frequency(6, sample_rate));
}

TEST_CASE("[get_max_frequency] magnitude spectrum contains negative value") {
    std::vector<float> m = {1.0, 2.0, 3.0, -4.0, 5.0};
    int sample_rate = 8000;
    float result = tuner::get_max_frequency(m, sample_rate);
    REQUIRE(result == convert_to_frequency(4, sample_rate));
}







