#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

#include <tuner/wa_tuner.hpp>
#include <tuner/tuner.hpp>
#include <tuner/global.hpp>

constexpr int SAMPLE_RATE  = 48000;
constexpr char ANSI_RESET[] = "\033[0m";
constexpr char ANSI_RED[] = "\033[31m";
constexpr char ANSI_GREEN[] = "\033[32m";
constexpr char ANSI_BLUE[] = "\033[34m";

/**
 * Retrieves audio data from a file and returns it as a vector of floating-point values.
 *
 * This function reads audio data from the specified file path and converts it into
 * a sequence of floating-point values, representing the audio samples. The file should
 * be in a format supported by this function (e.g., WAV, MP3, etc.), and the audio data
 * should be in a format that can be represented using single-precision floating-point values.
 *
 * @param file_path The path to the audio file to be read.
 * @return A std::vector<float> containing the audio samples from the file.
 *         If the file cannot be read or the audio format is unsupported, an empty vector is returned.
 * @throws Any exceptions related to file I/O operations or audio decoding may be thrown.
 *         It's recommended to handle these exceptions in the calling code.
 */
std::vector<float> get_audio_buffer_from_file(std::string file_path) {
    bool capture_number = false;
    std::vector<float> out_buffer = {};

    std::ifstream audio_file;
    audio_file.open(file_path);

    if (audio_file.is_open()) { // always check whether the file is open
        char current_char;
        std::string temp_buffer;
        while (audio_file) {
            current_char = audio_file.get();

            if (capture_number) {
                if (current_char == ',') {
                    out_buffer.push_back(std::stof(temp_buffer));
                    temp_buffer = "";
                    capture_number = false;
                    continue;
                }

                temp_buffer += current_char;
            }
            if (current_char == '(') {
                capture_number = true;
            }
        }
    }

    audio_file.clear();
    audio_file.close();

    return out_buffer;
}

/**
 * Logs test metrics for a series of iterations.
 *
 * This function is used to log the success metrics of a test over a number of iterations.
 * It provides information about the success count and total iteration count, allowing
 * monitoring of test performance and progress.
 *
 * @param success_count The number of successful outcomes or events observed during the test.
 * @param iteration_count The total number of iterations or trials conducted for the test.
 * @note It is assumed that both success_count and iteration_count are non-negative integers.
 * @note The function does not perform any validation on the input parameters.
 * @note This function does not return any values; it only logs the metrics.
 */
void log_test_metrics(int success_count, int iteration_count) {
    std::cout << ANSI_BLUE << std::left << std::setw(20) <<"Iteration Count"
              << std::setw(20) << "Success Count"
              << std::setw(20) << "Success Percentage"
              << std::endl;

    std::cout << std::setw(20) << iteration_count
              << std::setw(20) << success_count
              << std::setw(20) << std::to_string((float)success_count / (float)iteration_count * 100) + "%"
              << ANSI_RESET
              << std::endl;
}

TEST_CASE("[wa_tuner] frequency should be 82.41") {
    float success_count = 0;
    float iteration_count = 0;

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/e2-audio-stream/audio-stream-e2-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            push_value(buf[i]);

            if (i == TUNER_SIZE - 1) {
                float frequency = get_frequency(48000);
                if (frequency > 70 && frequency < 90) {
                    success_count += 1;
                }

                if (frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.50f);
}

TEST_CASE("[tuner] frequency should be 82.41") {
    float success_count = 0;
    float iteration_count = 0;
    std::array<float, TUNER_SIZE> audio_buffer = {};

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/e2-audio-stream/audio-stream-e2-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            audio_buffer[i] = buf[i];

            if (i == TUNER_SIZE - 1) {
                auto nc = tuner::tune(audio_buffer, SAMPLE_RATE);
                if (nc->actual_frequency > 70 && nc->actual_frequency < 90) {
                    success_count += 1;
                }

                if (nc->actual_frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.50f);
}

TEST_CASE("[wa_tuner] frequency should be 329.63") {
    float success_count = 0;
    float iteration_count = 0;

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/e4-audio-stream/audio-stream-e4-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            push_value(buf[i]);

            if (i == TUNER_SIZE - 1) {
                float frequency = get_frequency(48000);
                if (frequency > 310 && frequency < 340) {
                    success_count += 1;
                }

                if (frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.65f);
}

TEST_CASE("[tuner] frequency should be 329.63") {
    float success_count = 0;
    float iteration_count = 0;
    std::array<float, TUNER_SIZE> audio_buffer = {};

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/e4-audio-stream/audio-stream-e4-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            audio_buffer[i] = buf[i];

            if (i == TUNER_SIZE - 1) {
                auto nc = tuner::tune(audio_buffer, SAMPLE_RATE);
                if (nc->actual_frequency > 310 && nc->actual_frequency < 340) {
                    success_count += 1;
                }

                if (nc->closest_note_frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.65f);
}

TEST_CASE("[wa_tuner] frequency should be 246.94") {
    float success_count = 0;
    float iteration_count = 0;

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/b-audio-stream/audio-stream-b-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            push_value(buf[i]);

            if (i == TUNER_SIZE - 1) {
                float frequency = get_frequency(48000);
                if (frequency > 230 && frequency < 255) {
                    success_count += 1;
                }

                if (frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // there aren't any low volume sections in these audio streams
    REQUIRE(iteration_count == 703);

    REQUIRE((success_count / iteration_count) > 0.70f);
}

TEST_CASE("[tuner] frequency should be 246.94") {
    float success_count = 0;
    float iteration_count = 0;
    std::array<float, TUNER_SIZE> audio_buffer = {};

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/b-audio-stream/audio-stream-b-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            audio_buffer[i] = buf[i];

            if (i == TUNER_SIZE - 1) {
                auto nc = tuner::tune(audio_buffer, SAMPLE_RATE);
                if (nc->actual_frequency > 230 && nc->actual_frequency < 255) {
                    success_count += 1;
                }

                if (nc->actual_frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // there aren't any low volume sections in these audio streams
    REQUIRE(iteration_count == 703);

    REQUIRE((success_count / iteration_count) > 0.70f);
}

TEST_CASE("[wa_tuner] frequency should be 196.00") {
    float success_count = 0;
    float iteration_count = 0;

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/g-audio-stream/audio-stream-g-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            push_value(buf[i]);

            if (i == TUNER_SIZE - 1) {
                float frequency = get_frequency(48000);
                if (frequency > 180 && frequency < 205) {
                    success_count += 1;
                }

                if (frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.60f);
}

TEST_CASE("[tuner] frequency should be 196.00") {
    float success_count = 0;
    float iteration_count = 0;
    std::array<float, TUNER_SIZE> audio_buffer = {};

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/g-audio-stream/audio-stream-g-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            audio_buffer[i] =buf[i];

            if (i == TUNER_SIZE - 1) {
                auto nc = tuner::tune(audio_buffer, SAMPLE_RATE);
                if (nc->actual_frequency > 180 && nc->actual_frequency < 205) {
                    success_count += 1;
                }

                if (nc->actual_frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.60f);
}

TEST_CASE("[wa_tuner] frequency should be 146.83") {
    float success_count = 0;
    float iteration_count = 0;

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/d-audio-stream/audio-stream-d-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            push_value(buf[i]);

            if (i == TUNER_SIZE - 1) {
                float frequency = get_frequency(48000);
                if (frequency > 130 && frequency < 155) {
                    success_count += 1;
                }

                if (frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.55f);
}

TEST_CASE("[tuner] frequency should be 146.83") {
    float success_count = 0;
    float iteration_count = 0;
    std::array<float, TUNER_SIZE> audio_buffer = {};

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/d-audio-stream/audio-stream-d-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            audio_buffer[i] = buf[i];

            if (i == TUNER_SIZE - 1) {
                auto nc = tuner::tune(audio_buffer, SAMPLE_RATE);
                if (nc->actual_frequency > 130 && nc->actual_frequency < 155) {
                    success_count += 1;
                }

                if (nc->actual_frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.55f);
}

TEST_CASE("[wa_tuner] frequency should be 110.00") {
    float success_count = 0;
    float iteration_count = 0;

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/a-audio-stream/audio-stream-a-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            push_value(buf[i]);

            if (i == TUNER_SIZE - 1) {
                float frequency = get_frequency(48000);
                if (frequency > 100 && frequency < 120) {
                    success_count += 1;
                }

                if (frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.50f);
}

TEST_CASE("[tuner] frequency should be 110.00") {
    float success_count = 0;
    float iteration_count = 0;
    std::array<float, TUNER_SIZE> audio_buffer = {};

    for (int x = 1; x < 704; x++) {
        std::string file_path = "./acceptance-test-assets/a-audio-stream/audio-stream-a-" + std::to_string(x) + ".txt";
        std::vector<float> buf = get_audio_buffer_from_file(file_path);
        assert(buf.size() == TUNER_SIZE);

        for (int i = 0; i < buf.size(); i++) {
            audio_buffer[i] = buf[i];

            if (i == TUNER_SIZE - 1) {
                auto nc = tuner::tune(audio_buffer, SAMPLE_RATE);
                if (nc->actual_frequency > 100 && nc->actual_frequency < 120) {
                    success_count += 1;
                }

                if (nc->actual_frequency != -1) {
                    iteration_count += 1;
                }


                clear_buffer_offset();
            }
        }

        buf.clear();
    }

    log_test_metrics(success_count, iteration_count);

    // Ensure that the correct number of samples were processed
    REQUIRE(success_count > 300);
    REQUIRE(iteration_count > 600);

    // ensure that at least some low volume sections were captured
    REQUIRE(iteration_count < 703);

    REQUIRE((success_count / iteration_count) > 0.50f);
}
