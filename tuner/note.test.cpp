#include <catch2/catch_test_macros.hpp>

#include <tuner/note.hpp>

TEST_CASE("[get_note_for_frequency] pitch within a note's frequency range") {
    float pitch = 440.0; // A4 (frequency: 440 Hz)
    tuner::note_context* result = tuner::get_note_for_frequency(pitch);
    REQUIRE(result != nullptr);
    REQUIRE(result->name == "A4");
    REQUIRE((result->closest_note_frequency - 440.0f) < 1e-6);
}

TEST_CASE("[get_note_for_frequency] pitch slightly above a note's frequency range") {
    float pitch = 445.0; // Slightly above A4's frequency (440 Hz)
    tuner::note_context* result = tuner::get_note_for_frequency(pitch);
    REQUIRE(result != nullptr);
    REQUIRE(result->name == "A4");
    REQUIRE((result->closest_note_frequency - 440.0f) < 1e-6);
}

TEST_CASE("[get_note_for_frequency] pitch slightly below a note's frequency range") {
    float pitch = 435.0;  // Slightly below A4's frequency (440 Hz)
    tuner::note_context *result = tuner::get_note_for_frequency(pitch);
    REQUIRE(result != nullptr);
    REQUIRE(result->name == "A4");
    REQUIRE((result->closest_note_frequency - 440.0f) < 1e-6);
}

TEST_CASE("[get_note_for_frequency] pitch at the lower boundary of the valid frequency range") {
    float pitch = 16.35; // The lowest valid frequency for a note (C0's frequency)
    tuner::note_context *result = tuner::get_note_for_frequency(pitch);
    REQUIRE(result != nullptr);
    REQUIRE(result->name == "C0");
    REQUIRE((result->closest_note_frequency - tuner::c0_hz) < 1e-6);
}

TEST_CASE("[get_note_for_frequency] pitch at the upper boundary of the valid frequency range") {
    float pitch = 7902.13; // The highest valid frequency for a note (B8's frequency)
    tuner::note_context *result = tuner::get_note_for_frequency(pitch);
    REQUIRE(result != nullptr);
    REQUIRE(result->name == "B8");
    REQUIRE((result->closest_note_frequency - tuner::b8_hz) < 1e-6);
}

TEST_CASE("[get_note_for_frequency] pitch outside the valid frequency range (below the lowest note's frequency)") {
    float pitch = 10.0; // Below the valid frequency range
    tuner::note_context* result = tuner::get_note_for_frequency(pitch);
    REQUIRE(result != nullptr);
    REQUIRE(result->name == "C0");
    REQUIRE((result->closest_note_frequency - tuner::c0_hz) < 1e-6);
}

TEST_CASE("[get_note_for_frequency] pitch outside the valid frequency range (above the highest note's frequency)") {
    float pitch = 8000.0; // Above the valid frequency range
    tuner::note_context* result = tuner::get_note_for_frequency(pitch);
    REQUIRE(result != nullptr);
    REQUIRE(result->name == "B8");
    REQUIRE((result->closest_note_frequency - tuner::b8_hz) < 1e-6);
}