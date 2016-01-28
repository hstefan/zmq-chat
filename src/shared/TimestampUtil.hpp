#include "shared/common.pb.h"
#include <chrono>

// converts from timestamp messages to native type
chat::common::Timestamp timestampMessageFromNative(
    const std::chrono::system_clock::time_point &timestamp);

// converst from native type to timestamp message
std::chrono::system_clock::time_point
nativeTimestampFromMessage(const chat::common::Timestamp &timestamp);
