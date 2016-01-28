#pragma once

#include <vector>
#include <chrono>
#include "shared/common.pb.h"

namespace hst {
class MessageContainer {
public:
  using Timestamp = std::chrono::system_clock::time_point;
  using ChatMessage = chat::common::ChatMessage;

  void put(ChatMessage message);
  std::vector<ChatMessage> getFromTo(Timestamp from, Timestamp to) const;
  std::vector<ChatMessage> getFrom(Timestamp from) const;
  std::vector<ChatMessage> getTo(Timestamp to) const;
private:
  std::vector<ChatMessage> _messages;
};

}
