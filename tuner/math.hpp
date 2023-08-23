#ifndef TUNER_MATH_H
#define TUNER_MATH_H

#include <array>
#include <cmath>
#include <vector>

#include <tuner/global.hpp>

namespace tuner {

    struct DivisionByZeroException : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Division by zero exception";
        }
    };

    /**
     * @brief Performs the floored modulo operation on the given integer 'n' with respect to 'm',
     *        and returns the result as an integer.
     *
     * @param n The dividend for the modulo operation.
     * @param m The divisor for the modulo operation.
     *
     * @return An integer representing the result of the floored modulo operation of 'n' with respect to 'm'.
     */
    int floored_modulo(int n, int m);

    /**
     * @brief Calculates the euclidean_norm of a subarray 'm' within the specified range [from_index, to_index],
     *        and returns the result as a float.
     *
     * @param m The input std::array<float, TUNER_SIZE / 2> representing the array of values.
     * @param from_index The starting index of the subarray (inclusive).
     * @param to_index The ending index of the subarray (exclusive).
     *
     * @return A float representing the euclidean_norm of the subarray 'm' within the specified range.
     */
    float euclidean_norm(std::array<float, TUNER_SIZE / 2> m, int from_index, int to_index);

    /**
     * @brief Calculates the euclidean_norm of the given std::array 'm', and returns the result as a float.
     *
     * @param m The input std::array<float, TUNER_SIZE> representing the array of values.
     *
     * @return A float representing the p-norm of the std::array 'm'.
     */
    float euclidean_norm(std::array<float, TUNER_SIZE> m);

    /**
     * @brief Calculates the euclidean_norm of the given std::vector 'm', and returns the result as a float.
     *
     * @param m The input std::vector<float> representing the vector of values.
     *
     * @return A float representing the p-norm of the std::vector 'm'.
     */
    float euclidean_norm(std::vector<float> m);
}

#endif //TUNER_MATH_H
