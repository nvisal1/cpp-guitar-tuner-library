#include <array>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <tuner/math.hpp>

#include <cmath>

TEST_CASE("[floored_modulo] positive dividend and positive divisor") {
    int n = 10;
    int m = 3;
    int r = tuner::floored_modulo(n, m);
    REQUIRE(r == 1);
}

TEST_CASE("[floored_modulo] positive dividend and negative divisor") {
    int n = 10;
    int m = -3;
    int r = tuner::floored_modulo(n, m);
    REQUIRE(r == -2);
}

TEST_CASE("[floored_modulo] negative dividend and positive divisor") {
    int n = -10;
    int m = 3;
    int r = tuner::floored_modulo(n, m);
    REQUIRE(r == 2);
}

TEST_CASE("[floored_modulo] negative dividend and negative divisor") {
    int n = -10;
    int m = -3;
    int r = tuner::floored_modulo(n, m);
    REQUIRE(r == -1);
}

TEST_CASE("[floored_modulo] zero dividend") {
    int n = 0;
    int m = 3;
    int r = tuner::floored_modulo(n, m);
    REQUIRE(r == 0);
}

TEST_CASE("[floored_modulo] zero divisor") {
    int n = 10;
    int m = 0;

    try {
        int r = tuner::floored_modulo(n, m);
        REQUIRE(false);
    } catch (tuner::DivisionByZeroException& de) {
        REQUIRE(std::string(de.what()) == "Division by zero exception");
    }
}

TEST_CASE("[floored_modulo] divisor larger than dividend") {
    int n = 5;
    int m = 10;
    int r = tuner::floored_modulo(n, m);
    REQUIRE(r == 5);
}

TEST_CASE("[euclidean_norm | array | sliced] with non-empty array and full range") {
    std::array<float, TUNER_SIZE / 2> m = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    int from_index = 0;
    int to_index = m.size();
    float result = tuner::euclidean_norm(m, from_index, to_index);
    float expected = std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f + 4.0f*4.0f + 5.0f*5.0f);
    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | array | sliced] with non-empty array and sliced range") {
    std::array<float, TUNER_SIZE / 2> m = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    int from_index = 0;
    int to_index = 4;
    float result = tuner::euclidean_norm(m, from_index, to_index);
    float expected = std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f + 4.0f*4.0f);
    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | array | sliced] with empty array") {
    std::array<float, TUNER_SIZE / 2> m = {};
    int from_index = 0;
    int to_index = 4;
    float result = tuner::euclidean_norm(m, from_index, to_index);
    float expected = 0;
    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | array | sliced] with non-empty array and negative values") {
    std::array<float, TUNER_SIZE / 2> m = {-1.0, -2.0, 3.0, -4.0, 5.0};
    int from_index = 0;
    int to_index = 4;
    float expected = std::sqrt((-1.0)*(-1.0) + (-2.0)*(-2.0) + 3.0*3.0 + (-4.0)*(-4.0));

    float result = tuner::euclidean_norm(m, from_index, to_index);

    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | array | full] with non-empty array") {
    std::array<float, TUNER_SIZE> m = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

    float expected = std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f + 4.0f*4.0f + 5.0f*5.0f);

    float result = tuner::euclidean_norm(m);

    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | array | full] with empty array") {
    std::array<float, TUNER_SIZE> m = {};

    float expected = 0;

    float result = tuner::euclidean_norm(m);

    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | array | full] with non-empty array and negative values") {
    std::array<float, TUNER_SIZE> m = {-1.0, -2.0, 3.0, -4.0, 5.0};

    float expected = std::sqrt((-1.0)*(-1.0) + (-2.0)*(-2.0) + 3.0*3.0 + (-4.0)*(-4.0) + 5.0*5.0);

    float result = tuner::euclidean_norm(m);

    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | vector | full] with non-empty array") {
    std::vector<float> m = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

    float expected = std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f + 4.0f*4.0f + 5.0f*5.0f);

    float result = tuner::euclidean_norm(m);

    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | vector | full] with empty array") {
    std::vector<float> m = {};

    float expected = 0;

    float result = tuner::euclidean_norm(m);

    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}

TEST_CASE("[euclidean_norm | vector | full] with non-empty array and negative values") {
    std::vector<float> m = {-1.0, -2.0, 3.0, -4.0, 5.0};

    float expected = std::sqrt((-1.0)*(-1.0) + (-2.0)*(-2.0) + 3.0*3.0 + (-4.0)*(-4.0) + 5.0*5.0);

    float result = tuner::euclidean_norm(m);

    REQUIRE(std::abs(result - expected) < 1e-6); // use epsilon for floating-point comparison
}









