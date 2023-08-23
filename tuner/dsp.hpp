#ifndef TUNER_DSP_H
#define TUNER_DSP_H

#include <array>
#include <vector>

#include <tuner/global.hpp>

namespace tuner {
    static int NUM_HPS = 5;
    static float SIGNAL_POWER_THRESHOLD = 1e-6;
    static float WHITE_NOISE_THRESHOLD = 0.2;

    const std::array<int, 10> OCTAVE_BANDS = {
            50,
            100,
            200,
            400,
            800,
            1600,
            3200,
            6400,
            12800,
            25600
    };

    /**
     * @brief Checks if the signal energy is too low based on the values in the std::array 'm',
     *        and returns a bool indicating whether the signal energy is too low or not.
     *
     * @param m The input std::array<float, TUNER_SIZE> representing the signal values.
     *
     * @return A bool value indicating whether the signal energy is too low (true) or not (false).
     */
    bool signal_energy_is_too_low(std::array<float, TUNER_SIZE> m);

    /**
     * @brief Applies a Hanning window function to the audio stream buffer represented by the input std::array 'audio_stream_buffer',
     *        and returns a new std::array<float, TUNER_SIZE> containing the windowed samples.
     *
     * @param audio_stream_buffer The input std::array<float, TUNER_SIZE> representing the audio stream buffer to which the Hanning window will be applied.
     *
     * @return A new std::array<float, TUNER_SIZE> containing the windowed samples after applying the Hanning window function.
     */
    std::array<float, TUNER_SIZE> apply_hanning_window(std::array<float, TUNER_SIZE> audio_stream_buffer);

    /**
     * @brief Calculates the magnitude spectrum of the audio buffer stream frequencies represented by the input array 'audio_buffer_stream_freq',
     *        and returns a new std::array<float, TUNER_SIZE / 2> containing the magnitude values.
     *
     * @param audio_buffer_stream_freq The input array of size TUNER_SIZE / 2 representing the frequencies of the audio buffer stream.
     *
     * @return A new std::array<float, TUNER_SIZE / 2> containing the calculated magnitude spectrum of the audio buffer stream frequencies.
     */
    std::array<float, TUNER_SIZE / 2> calculate_magnitude_spec(float audio_buffer_stream_freq[TUNER_SIZE / 2]);

    /**
     * @brief Calculates the delta frequency, i.e., the frequency resolution, based on the given 'sample_rate' and 'fft_size',
     *        and returns the result as a float.
     *
     * @param sample_rate The sample rate of the audio signal.
     * @param fft_size The size of the FFT (Fast Fourier Transform) used for frequency analysis.
     *
     * @return A float representing the delta frequency, which is the frequency resolution of the given audio signal.
     */
    float calculate_delta_frequency(int sample_rate, int fft_size);

    /**
     * @brief Suppresses values below the octave bands in the magnitude spectrum represented by the input std::array 'mag_spec',
     *        based on the given 'delta_frequency', and returns a new std::array<float, TUNER_SIZE / 2> with suppressed values.
     *
     * @param mag_spec The input std::array<float, TUNER_SIZE / 2> representing the magnitude spectrum of the audio signal.
     * @param delta_frequency The delta frequency, i.e., the frequency resolution used for determining the octave bands.
     *
     * @return A new std::array<float, TUNER_SIZE / 2> with values below the octave bands suppressed in the magnitude spectrum.
     */
    std::array<float, TUNER_SIZE / 2>
    suppress_below_octave_bands(std::array<float, TUNER_SIZE / 2> mag_spec, float delta_frequency);

    /**
     * @brief Calculates the Harmonic Product Spectrum (HPS) of the input std::vector 'input' and returns the result as a new std::vector<float>.
     *
     * @param input The input std::vector<float> representing the signal or audio data.
     *
     * @return A new std::vector<float> containing the Harmonic Product Spectrum (HPS) calculated from the input data.
     */
    std::vector<float> calculate_hps(std::vector<float> input);

    /**
     * @brief Retrieves the maximum frequency from the given std::vector 'm' representing the frequency spectrum,
     *        based on the provided 'sample_rate', and returns the result as a float.
     *
     * @param m The input std::vector<float> representing the frequency spectrum.
     * @param sample_rate The sample rate of the audio signal or frequency spectrum.
     *
     * @return A float representing the maximum frequency present in the frequency spectrum.
     */
    float get_max_frequency(std::vector<float> m, int sample_rate);
}

#endif //TUNER_DSP_H
