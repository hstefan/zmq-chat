#pragma once

#include <zmq.hpp>
#include "MessageContainer.hpp"
#include "shared/requests.pb.h"
#include "shared/responses.pb.h"

namespace hst {

class ChatServer {
public:
  static const int GeneralError = 0;
  ChatServer(const char *bindAddress);
  ChatServer(ChatServer &) = delete;
  ChatServer &operator=(ChatServer &) = delete;
  void run();

private:
  zmq::message_t makeResponse(const zmq::message_t &request);
  zmq::message_t handleMessageSending(chat::reqs::Request request);
  zmq::message_t handleMessageRetrieval(chat::reqs::Request request);
  zmq::message_t makeInvalidRequest(chat::resps::ResponseStatus status,
                                    const chat::reqs::Request &request) const;
  zmq::message_t
  makeInvalidRequest(chat::resps::ResponseStatus errorCode) const;
  zmq::context_t _context;
  zmq::socket_t _repSocket;
  MessageContainer _storage;
};

} // namespace hst
