#pragma once
#include <cstdio>
#define LOG(x)                                                                 \
  fprintf(stdout, "%s\n", x);                                                  \
  fflush(stdout)
#define LOG_W(x)                                                               \
  fprintf(stdout, "%s\n", x);                                                  \
  fflush(stdout)
#define LOG_E(x)                                                               \
  fprintf(stderr, "%s\n", x);                                                  \
  fflush(stderr)
