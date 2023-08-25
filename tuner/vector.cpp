#include <iostream>
#include <numeric>
#include <algorithm>

#include <tuner/vector.hpp>

std::vector<float> tuner::new_vector_with_values_between(int start, int end, float step) {
    std::vector<float> v;

    if (step < 0) {
        throw NegativeStepException();
    }

    auto current_value = float(start);
    float counter = 1;

    while (current_value < float(end)) {
        v.push_back(current_value);
        current_value = float(start) + step * counter++;
    }

    return v;
}

std::vector<uint32_t> tuner::sort(std::vector<float> m) {
    std::vector <uint32_t> idx(m.size());
    std::iota(idx.begin(), idx.end(), 0);

    const auto function = [&](int i1, int i2)
    noexcept->
    bool{
        return m[i1] < m[i2];
    };

    std::stable_sort(idx.begin(), idx.end(), function);
    return idx;
}

std::vector<float> tuner::interpolate(std::vector<float> in_x, std::vector<float> in_xp, std::array<float, TUNER_SIZE / 2> in_fp) {

    if (in_xp.size() != in_fp.size()) {
        throw tuner::UnequalLengthException();
    }

    std::vector <uint32_t> sorted_xp_idxs = tuner::sort(in_xp);
    auto sorted_xp = std::vector<float>(in_fp.size());
    auto sorted_fp = std::vector<float>(in_fp.size());
    uint32_t counter = 0;

    for (auto sorted_xp_idx: sorted_xp_idxs) {
        sorted_xp[counter] = in_xp[sorted_xp_idx];
        sorted_fp[counter++] = in_fp[sorted_xp_idx];
    }

    std::vector <uint32_t> sorted_x_idxs = tuner::sort(in_x);
    auto out = std::vector<float>(in_x.size());

    uint32_t curr_x_index = 0;
    uint32_t curr_xp_index = 0;
    while (curr_x_index < in_x.size()) {
        const auto sorted_x_idx = sorted_x_idxs[curr_x_index];
        const auto x = in_x[sorted_x_idx];
        const auto xp_low = sorted_xp[curr_xp_index];
        const auto xp_high = sorted_xp[curr_xp_index + 1];
        const auto fp_low = sorted_fp[curr_xp_index];
        const auto fp_high = sorted_fp[curr_xp_index + 1];

        if (curr_xp_index >= (sorted_xp.size() - 1)) {
            out[sorted_x_idx] = fp_low;
            ++curr_x_index;
        } else {
            if (xp_low <= x && x <= xp_high) {
                const double percent = static_cast<double>(x - xp_low) / static_cast<double>(xp_high - xp_low);
                out[sorted_x_idx] = fp_low * (1. - percent) + fp_high * percent;
                ++curr_x_index;
            } else {
                ++curr_xp_index;
            }
        }
    }

    return out;
}
