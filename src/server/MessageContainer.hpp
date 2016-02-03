#pragma once

#include <vector>
#include <chrono>
#include "shared/common.pb.h"

namespace hst {
class MessageContainer {
public:
  using MessageId = ::google::protobuf::int64_t;
  using ContainerType = std::vector<chat::common::ChatMessage>;
  MessageId put(chat::common::ChatMessage message);
  ContainerType getAll() const;
  ContainerType getAfterId(MessageId id) const;
private:
  ContainerType _messages;
};

}
