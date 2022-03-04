#ifndef PROCESSTREE_UTIL_H
#define PROCESSTREE_UTIL_H

#include <vector>
#include <unordered_map>
#include <chrono>
#include <magic_enum.hpp>

// can use const pair with clang++ but not g++
typedef std::vector<std::pair<const std::string, const std::chrono::steady_clock::time_point> > ts_vector;

void mark_time(ts_vector& timestamps, const std::string_view label);
void print_timestamps(const ts_vector& timestamps);

template <typename E> std::unordered_map<std::string, E> enum_rentries() {
    const auto& entries = magic_enum::enum_entries<E>();
    std::unordered_map<std::string, E> rentries;
    for (const auto& e: entries)
        rentries.insert(std::pair(e.second, e.first));
    return rentries;
}

#endif // PROCESSTREE_UTIL_H
