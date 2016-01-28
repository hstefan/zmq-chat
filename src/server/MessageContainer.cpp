#include "MessageContainer.hpp"
#include "shared/TimestampUtil.hpp"
#include "shared/Algorithms.hpp"
#include <algorithm>

using namespace hst;
using namespace std::chrono;
using namespace chat::common;

void MessageContainer::put(ChatMessage message) {
  _messages.push_back(message);
}

std::vector<ChatMessage> MessageContainer::getFromTo(Timestamp from,
                                                     Timestamp to) const {
  std::vector<ChatMessage> results;
  const auto firstIt = std::lower_bound(
      _messages.begin(), _messages.end(), from,
      [](const auto &lhs, const auto &rhs) {
        // TODO: remove the need for converting each timestamp to a chrono
        // duration
        return nativeTimestampFromMessage(lhs.receivedat()) < rhs;
      });

  copy_while(firstIt, _messages.end(), std::back_inserter(results),
             [&](const auto &m) {
               return nativeTimestampFromMessage(m.receivedat()) <= to;
             });
  return results;
}

std::vector<ChatMessage> MessageContainer::getFrom(Timestamp from) const {
  if (!_messages.empty())
    return getFromTo(from,
                     nativeTimestampFromMessage(_messages.back().receivedat()));
  else
    return std::vector<ChatMessage>{};
}

std::vector<ChatMessage> MessageContainer::getTo(Timestamp to) const {
  if (!_messages.empty())
    return getFromTo(nativeTimestampFromMessage(_messages.front().receivedat()),
                     to);
  else
    return std::vector<ChatMessage>{};
}

