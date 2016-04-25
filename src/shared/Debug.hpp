#pragma once
#include <cstdio>
#define LOG_F(fmt, ...)                                                        \
  fprintf(stdout, fmt, ##__VA_ARGS__);                                         \
  fflush(stdout)
#define LOG_WF(fmt, ...)                                                       \
  fprintf(stdout, fmt, ##__VA_ARGS__);                                         \
  fflush(stdout)
#define LOG_EF(fmt, ...)                                                       \
  fprintf(stdout, fmt, ##__VA_ARGS__);                                         \
  fflush(stderr)
#define LOG(x) LOG_F("%s\n", x)
#define LOG_W(x) LOG_WF("%s\n", x)
#define LOG_E(x) LOG_EF("%s\n", x)
