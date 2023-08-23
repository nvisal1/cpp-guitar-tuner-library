#include <tuner/math.hpp>

int tuner::floored_modulo(int n, int m) {
    if (m == 0) {
        throw tuner::DivisionByZeroException();
    }
    return ((n % m) + m) % m;
}

float tuner::euclidean_norm(std::array<float, TUNER_SIZE / 2> m, int from_index, int to_index) {
    float sum = 0;
    int order = 2;
    for (int i = from_index; i < to_index; i++) {
        sum += std::pow(std::abs(m[i]), float(order));
    }

    sum = std::pow(sum, (float(1) / float(order)));

    return sum;
}


float tuner::euclidean_norm(std::array<float, TUNER_SIZE> m) {
    float sum = 0;
    int order = 2;
    for (float i: m) {
        sum += std::pow(std::abs(i), float(order));
    }

    sum = std::pow(sum, (float(1) / float(order)));

    return sum;
}

float tuner::euclidean_norm(std::vector<float> m) {
    float sum = 0;
    int order = 2;
    for (float i: m) {
        sum += std::pow(std::abs(i), float(order));
    }

    sum = std::pow(sum, (float(1) / float(order)));

    return sum;
}