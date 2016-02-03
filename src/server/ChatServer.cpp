#include "ChatServer.hpp"
#include "shared/requests.pb.h"
#include "shared/responses.pb.h"
#include "shared/TimestampUtil.hpp"
#include "shared/MessagingUtils.hpp"
#include "shared/Debug.hpp"
#include <cassert>

using namespace hst;
using namespace chat::reqs;
using namespace chat::resps;
using namespace chat::common;
using namespace google;

ChatServer::ChatServer(const char *bindAddress)
    : _context(1), _socket(_context, ZMQ_REP), _storage() {
  _socket.bind(bindAddress);
}

void ChatServer::run() {
  for (;;) {
    zmq::message_t request;
    _socket.recv(&request);
    zmq::message_t response = makeResponse(request);
    _socket.send(response);
  }
}

zmq::message_t ChatServer::makeResponse(const zmq::message_t &request) {
  Request req;
  if (req.ParseFromArray(request.data(), request.size())) {
    switch (req.type()) {
    case RequestType::Put:
      LOG("Received PUT message");
      return handleMessageSending(req);
    case RequestType::Get:
      LOG("Received GET message");
      return handleMessageRetrieval(req);
    default:
      LOG("Received unknown message");
      break;
    }
  } else {
    LOG("Failed to parse from array");
    return makeInvalidRequest(ResponseStatus::FailedToParse);
  }
}

zmq::message_t
ChatServer::handleMessageSending(Request request) {
  // create timestamp message for "now"
  Timestamp recvTs = timestampMessageFromNative(std::chrono::system_clock::now());
  // create response message with received timestamp
  ChatMessage msg = request.messageput().msg();
  *msg.mutable_receivedat() = recvTs;
  _storage.put(std::move(msg));
  // create response message
  ChatResponses response;
  response.set_type(ResponseType::Put);
  response.set_status(ResponseStatus::Ok);
  return wrapMessage(response);
}

zmq::message_t
ChatServer::handleMessageRetrieval(Request request) {
  const GetRequest getRequest = request.messageget();
  // query messages accordingly to the optional lastknownid parameter
  std::vector<ChatMessage> queryResult;
  if (getRequest.has_lastknownid())
    queryResult = _storage.getAfterId(getRequest.lastknownid());
  else
    queryResult = _storage.getAll();

  GetResponse getResponse;
  for (const auto &m : queryResult)
    *getResponse.add_messages() = m;

  ChatResponses resp;
  resp.set_type(ResponseType::Get);
  resp.set_status(ResponseStatus::Ok);
  *resp.mutable_getresponse() = getResponse;

  return wrapMessage(resp);
}

zmq::message_t
ChatServer::makeInvalidRequest(ResponseStatus status,
                               const Request &request) const {
  assert(status != ResponseStatus::Ok &&
         "Attempt to respond to invalid request with Ok status");
  InvalidRequest invalidRequest;
  std::string reqData; // TODO
  invalidRequest.set_originalrequest(reqData);

  ChatResponses resp;
  resp.set_type(ResponseType::Invalid);
  resp.set_status(status);
  *resp.mutable_invalid() = invalidRequest;
  return wrapMessage(resp);
}

zmq::message_t ChatServer::makeInvalidRequest(ResponseStatus status) const {
  InvalidRequest invalidRequest;
  ChatResponses resp;
  resp.set_type(ResponseType::Invalid);
  resp.set_status(status);
  *resp.mutable_invalid() = invalidRequest;
  return wrapMessage(resp);
}
