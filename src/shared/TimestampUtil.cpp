#include "TimestampUtil.hpp"

using namespace chat::common;
using namespace std::chrono;

using NativeTimestamp = std::chrono::system_clock::time_point;

Timestamp timestampMessageFromNative(const NativeTimestamp &timestamp) {
  // calculate the proper durations
  const auto ts = timestamp.time_since_epoch();
  const auto timeSeconds = duration_cast<seconds>(timestamp.time_since_epoch());
  const auto timeNanos =
      duration_cast<nanoseconds>(timestamp.time_since_epoch());
  const auto nanosDelta = timeNanos - timeSeconds;
  // builds the message
  Timestamp msg;
  msg.set_seconds(timeSeconds.count());
  msg.set_nanos(nanosDelta.count());
  return msg;
}

NativeTimestamp nativeTimestampFromMessage(const Timestamp &timestamp) {
  // creates the two needed durations;
  const auto totalDuration =
      seconds(timestamp.seconds()) + nanoseconds(timestamp.nanos());
  NativeTimestamp ts;
  ts += totalDuration;
  return ts;
}
