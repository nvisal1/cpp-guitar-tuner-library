#ifndef TUNER_VECTOR_H
#define TUNER_VECTOR_H

#include <vector>
#include <array>

#include <tuner/global.hpp>

namespace tuner {

    struct NegativeStepException : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Negative step exception";
        }
    };

    struct UnequalLengthException : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Unequal length exception";
        }
    };

    /**
     * @brief Creates a new std::vector<float> and populates it with values between the specified 'start' and 'end' indices.
     *        The values are incremented by the provided 'step' (defaulting to 1 if not specified).
     *
     * @param start The starting index of the range.
     * @param end The ending index of the range.
     * @param step The increment value between consecutive elements (default: 1).
     *
     * @return A std::vector<float> containing the values between 'start' and 'end', with the specified 'step' increment.
     */
    std::vector<float> new_vector_with_values_between(int start, int end, float step = 1);

    /**
     * @brief Sorts the elements of the input std::vector<float> in ascending order and returns a new std::vector<uint32_t>
     *        containing the indices of the sorted elements.
     *
     * @param m The input std::vector<float> to be sorted.
     *
     * @return A std::vector<uint32_t> containing the indices of the sorted elements from the input vector.
     */
    std::vector<uint32_t> sort(std::vector<float> m);

    /**
     * @brief Interpolates the values in 'in_fp' based on the given 'in_x' and 'in_xp' vectors using linear interpolation,
     *        and returns a new std::vector<float> containing the interpolated values.
     *
     * @param in_x The input std::vector<float> representing the x-coordinates of the interpolation points.
     * @param in_xp The input std::vector<float> representing the known x-coordinates of the data points.
     * @param in_fp The input std::array<float, TUNER_SIZE / 2> representing the known function values (y-coordinates of the data points) at 'in_xp'.
     *
     *
     * @return A std::vector<float> containing the interpolated values corresponding to 'in_x' based on linear interpolation.
     */
    std::vector<float>
    interpolate(std::vector<float> in_x, std::vector<float> in_xp, std::array<float, TUNER_SIZE / 2> in_fp);
}

#endif //TUNER_VECTOR_H
