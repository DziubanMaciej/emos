#pragma once

#include <cstdarg>
#include <cstdio>
#include <exception>

namespace Error {
inline void log(FILE *file, const char *label, const char *format, ...) {
    if (label != nullptr) {
        fprintf(file, "%s: ", label);
    }

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fprintf(file, "\n");
}

[[noreturn]] inline void abort() {
    throw std::exception{};
}
} // namespace Error

#define INFO(...)                                 \
    do {                                          \
        Error::log(stdout, nullptr, __VA_ARGS__); \
    } while (0)

#define FATAL_ERROR(...)                          \
    do {                                          \
        Error::log(stderr, "ERROR", __VA_ARGS__); \
        Error::abort();                           \
    } while (0)

#define FATAL_ERROR_IF(condition, ...) \
    do {                               \
        if (condition) {               \
            FATAL_ERROR(__VA_ARGS__);  \
        }                              \
    } while (0)

#define UNREACHABLE_CODE() FATAL_ERROR("Unreachable code")
