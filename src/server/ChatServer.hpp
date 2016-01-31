#pragma once

#include <zmq.hpp>
#include "MessageContainer.hpp"
#include "shared/requests.pb.h"

namespace hst {

class ChatServer {
public:
  static const int GeneralError = 0;
  ChatServer(const char *bindAddress);
  ChatServer(ChatServer &) = delete;
  ChatServer &operator=(ChatServer &) = delete;
  void run();

private:
  zmq::message_t makeResponse(const zmq::message_t &request,
                              zmq::message_t *response);
  zmq::message_t handleMessageSending(chat::reqs::ChatRequest request);
  zmq::message_t handleMessageRetrieval(chat::reqs::ChatRequest request);
  zmq::message_t
  makeInvalidRequest(int errorCode,
                     const chat::reqs::ChatRequest &request) const;
  zmq::message_t makeInvalidRequest(int errorCode) const;
  zmq::context_t _context;
  zmq::socket_t _socket;
  MessageContainer _storage;
};

} // namespace hst
