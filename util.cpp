#include <spdlog/spdlog.h>

#include "util.h"

void mark_time(ts_vector& timestamps, const std::string_view label) {
    timestamps.push_back(std::pair(label.data(), std::chrono::steady_clock::now()));
}

void print_timestamps(const ts_vector& timestamps) {
    using std::chrono::milliseconds;
    using std::chrono::duration_cast;
    for (auto t = timestamps.begin() + 1; t != timestamps.end(); t++) {
        const auto dur = duration_cast<milliseconds>(t->second - (t - 1)->second).count();
        spdlog::info("{}: {} ms", t->first, dur);
    }
    const auto& start = timestamps.front().second;
    const auto& stop = timestamps.back().second;
    spdlog::info("TOTAL time: {} ms", duration_cast<milliseconds>(stop - start).count());
}
