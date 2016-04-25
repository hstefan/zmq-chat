#pragma once

#include <thread>
#include <zmq.hpp>
#include <unordered_map>
#include <functional>
#include "shared/common.pb.h"

namespace hst {

class MessageUpdater {
public:
  using ChatMessage = chat::common::ChatMessage;
  MessageUpdater(const char* repServer, const char* pubServer);
private:
  void run(const char* repServer, const char* pubServer);
  void handleSubscription(const zmq::message_t& msg);
  void storeMessage(const ChatMessage& msg);
  std::unordered_map<long, ChatMessage> _messages;
  std::thread _runThread;
};
}
