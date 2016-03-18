#pragma once

#include <zmq.hpp>
#include "MessageContainer.hpp"
#include "shared/requests.pb.h"
#include "shared/responses.pb.h"

namespace hst {

class MessageServer {
public:
  static const int GeneralError = 0;
  MessageServer(const char *repBindAddress, const char *pubBindAddress);
  MessageServer(MessageServer &) = delete;
  MessageServer &operator=(MessageServer &) = delete;
  void run();
private:
  using ChatResponses = chat::resps::ChatResponses;
  using Request = chat::reqs::Request;
  using ResponseStatus = chat::resps::ResponseStatus;

  ChatResponses makeResponse(const Request &request);
  ChatResponses handleMessageSending(Request request);
  ChatResponses
  handleMessageRetrieval(Request request);
  ChatResponses makeInvalidRequest(ResponseStatus status,
                                   const chat::reqs::Request &request) const;
  ChatResponses makeInvalidRequest(ResponseStatus errorCode) const;
  void handleUpdatePublish(Request request, ChatResponses response);
  bool tryParseRequest(const zmq::message_t& msg, Request* request) const;
  zmq::context_t _context;
  zmq::socket_t _repSocket;
  zmq::socket_t _pubSocket;
  MessageContainer _storage;
};

} // namespace hst
