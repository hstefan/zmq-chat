#pragma once

#include <string>
#include <zmq.hpp>
#include <chrono>
#include <vector>
#include "shared/responses.pb.h"
#include "shared/common.pb.h"

namespace hst {

class ChatClient {
public:
  ChatClient(const char* serverAddress);
  void sendMessage(std::string nickname, std::string message);
  std::vector<chat::common::ChatMessage> receiveMessages();
private:
  zmq::context_t _context;
  zmq::socket_t _socket;
  ::google::protobuf::int64 _lastId;
  std::chrono::system_clock::time_point _lastUpdate;
  std::vector<chat::common::ChatMessage> _messages;
};

} // namespace hst
