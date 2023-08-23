#include <catch2/catch_test_macros.hpp>
#include <tuner/vector.hpp>

TEST_CASE("[new_vector_with_values_between] positive step and start < end") {
    int start = 1;
    int end = 5;
    float step = 1.0;
    std::vector<float> result = tuner::new_vector_with_values_between(start, end, step);
    std::vector<float> expected = {1.0, 2.0, 3.0, 4.0};
    REQUIRE(result == expected);
}

TEST_CASE("[new_vector_with_values_between] negative step and start < end") {
    try {
        int start = -2;
        int end = 2;
        float step = -0.5;
        tuner::new_vector_with_values_between(start, end, step);
        REQUIRE(false);
    } catch (tuner::NegativeStepException& ne) {
        REQUIRE(std::string(ne.what()) == "Negative step exception");
    }
}

TEST_CASE("[new_vector_with_values_between] zero step and start < end") {
    int start = 0;
    int end = 0;
    float step = 0.0;
    std::vector<float> result = tuner::new_vector_with_values_between(start, end, step);
    std::vector<float> expected = {}; // The result should be an empty vector when step is zero.
    REQUIRE(result == expected);
}

TEST_CASE("[new_vector_with_values_between] positive step and start > end") {
    int start = 5;
    int end = 1;
    float step = 1.0;
    std::vector<float> result = tuner::new_vector_with_values_between(start, end, step);
    std::vector<float> expected = {}; // The result should be an empty vector when start > end and step is positive.
    REQUIRE(result == expected);
}

TEST_CASE("[new_vector_with_values_between] negative step and start > end") {
    try {
        int start = 10;
        int end = 2;
        float step = -0.5;
        tuner::new_vector_with_values_between(start, end, step);
        REQUIRE(false);
    } catch (tuner::NegativeStepException& ne) {
        REQUIRE(std::string(ne.what()) == "Negative step exception");
    }
}

TEST_CASE("[new_vector_with_values_between] positive floating point step and start < end") {
    int start = 1;
    int end = 4;
    float step = 0.5;
    std::vector<float> result = tuner::new_vector_with_values_between(start, end, step);
    std::vector<float> expected = {1.0, 1.5, 2.0, 2.5, 3.0, 3.5};
    REQUIRE(result == expected);
}

TEST_CASE("[sort] empty vector") {
    std::vector<float> m = {};
    std::vector<uint32_t> result = tuner::sort(m);
    std::vector<uint32_t> expected = {};
    REQUIRE(result == expected);
}

TEST_CASE("[sort] vector contains one element") {
    std::vector<float> m = {5.0};
    std::vector<uint32_t> result = tuner::sort(m);
    std::vector<uint32_t> expected = {0};
    REQUIRE(result == expected);
}

TEST_CASE("[sort] vector contains two elements in ascending order") {
    std::vector<float> m = {2.0, 5.0};
    std::vector<uint32_t> result = tuner::sort(m);
    std::vector<uint32_t> expected = {0, 1};
    REQUIRE(result == expected);
}

TEST_CASE("[sort] vector contains two elements in descending order") {
    std::vector<float> m = {10.0, 1.0};
    std::vector<uint32_t> result = tuner::sort(m);
    std::vector<uint32_t> expected = {1, 0};
    REQUIRE(result == expected);
}

TEST_CASE("[sort] vector contains multiple elements in random order") {
    std::vector<float> m = {3.0, 1.0, 7.0, 2.0, 5.0};
    std::vector<uint32_t> result = tuner::sort(m);
    std::vector<uint32_t> expected = {1, 3, 0, 4, 2};
    REQUIRE(result == expected);
}

TEST_CASE("[sort] vector contains multiple elements with duplicate values") {
    std::vector<float> m = {3.0, 2.0, 1.0, 2.0, 3.0};
    std::vector<uint32_t> result = tuner::sort(m);
    std::vector<uint32_t> expected = {2, 1, 3, 0, 4};
    REQUIRE(result == expected);
}

std::vector<float> fill_xp_vector() {
    std::vector<float> xp = {};
    for (int i = 0; i < TUNER_SIZE / 2; i++) {
        xp.push_back(float(i));
    }

    return xp;
}

std::array<float, TUNER_SIZE / 2> fill_fp_array(float last_value) {
    std::array<float, TUNER_SIZE / 2> fp = {};
    for (int i = 0; i < TUNER_SIZE / 2; i++) {
        if (i == (TUNER_SIZE / 2) - 1) {
            fp[i] = last_value;
        } else {
            fp[i] = 0;
        }

    }

    return fp;
}

TEST_CASE("[interpolate] empty x vector") {
    std::vector<float> in_x = {};
    std::vector<float> in_xp = fill_xp_vector();
    std::array<float, TUNER_SIZE / 2> in_fp = {};
    std::vector<float> result = tuner::interpolate(in_x, in_xp, in_fp);
    std::vector<float> expected = {}; // The result should be an empty vector as there are no input values.
    REQUIRE(result == expected);
}

TEST_CASE("[interpolate] fp array contains more elements than xp vector") {
    try {
        std::vector<float> in_x = {1.0};
        std::vector<float> in_xp = {1.0};
        std::array<float, TUNER_SIZE / 2> in_fp = {10.0};
        tuner::interpolate(in_x, in_xp, in_fp);
        REQUIRE(false);
    } catch (tuner::UnequalLengthException& ue) {
        REQUIRE(std::string(ue.what()) == "Unequal length exception");
    }
}

TEST_CASE("[interpolate] x contains index of last fp value") {
    std::vector<float> in_x = {(TUNER_SIZE / 2) - 1};
    std::vector<float> in_xp = fill_xp_vector();
    std::array<float, TUNER_SIZE / 2> in_fp = fill_fp_array(10.0f);
    std::vector<float> result = tuner::interpolate(in_x, in_xp, in_fp);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == 10.0f);
}

TEST_CASE("[interpolate] x contains indexes beyond the last fp value") {
    std::vector<float> in_x = {(TUNER_SIZE / 2), (TUNER_SIZE / 2) + 1, (TUNER_SIZE / 2) + 2, (TUNER_SIZE / 2) + 3};
    std::vector<float> in_xp = fill_xp_vector();
    std::array<float, TUNER_SIZE / 2> in_fp = fill_fp_array(10.0f);
    std::vector<float> result = tuner::interpolate(in_x, in_xp, in_fp);
    REQUIRE(result.size() == 4);
    REQUIRE(result[0] == 10.0f);
    REQUIRE(result[1] == 10.0f);
    REQUIRE(result[2] == 10.0f);
    REQUIRE(result[3] == 10.0f);
}

TEST_CASE("[interpolate] x contains index of known fp value (not last element)") {
    std::vector<float> in_x = {(TUNER_SIZE / 2) - 2};
    std::vector<float> in_xp = fill_xp_vector();
    std::array<float, TUNER_SIZE / 2> in_fp = fill_fp_array(10.0f);
    std::vector<float> result = tuner::interpolate(in_x, in_xp, in_fp);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == 0);
}

TEST_CASE("[interpolate] x contains index of unknown fp value (between N-1 and N elements)") {
    std::vector<float> in_x = {(TUNER_SIZE / 2) - 1.5};
    std::vector<float> in_xp = fill_xp_vector();
    std::array<float, TUNER_SIZE / 2> in_fp = fill_fp_array(10.0f);
    std::vector<float> result = tuner::interpolate(in_x, in_xp, in_fp);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == 5.0f);
}
