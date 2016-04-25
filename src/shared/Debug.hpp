#pragma once
#include <cstdio>
#define LOG_F(fmt, ...)                                                        \
  do {                                                                         \
    fprintf(stdout, fmt, ##__VA_ARGS__);                                       \
    fflush(stdout);                                                            \
  } while (0)
#define LOG_WF(fmt, ...)                                                       \
  do {                                                                         \
    fprintf(stdout, fmt, ##__VA_ARGS__);                                       \
    fflush(stdout);                                                            \
  } while (0)
#define LOG_EF(fmt, ...)                                                       \
  do {                                                                         \
    fprintf(stderr, fmt, ##__VA_ARGS__);                                       \
    fflush(stderr);                                                            \
  } while (0)
#define LOG(x) LOG_F("%s\n", x)
#define LOG_W(x) LOG_WF("%s\n", x)
#define LOG_E(x) LOG_EF("%s\n", x)
