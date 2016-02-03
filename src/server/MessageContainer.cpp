#include "MessageContainer.hpp"
#include "shared/TimestampUtil.hpp"
#include "shared/Algorithms.hpp"
#include <algorithm>

using namespace hst;
using namespace std::chrono;
using namespace chat::common;

MessageContainer::MessageId MessageContainer::put(ChatMessage message) {
  message.set_messageid(static_cast<MessageId>(_messages.size()));
  _messages.push_back(message);
  return _messages.back().messageid();
}

MessageContainer::ContainerType MessageContainer::getAll() const {
  return _messages;
}

MessageContainer::ContainerType
MessageContainer::getAfterId(MessageId id) const {
  // message ids are also the index of the array, so this should be as easy
  // as slicing the array after the id through the end
  ContainerType answer;
  size_t index = static_cast<size_t>(id);
  if (index < _messages.size())
    answer.insert(answer.begin(), std::next(_messages.begin(), index),
                  _messages.end());
  return answer;
}
