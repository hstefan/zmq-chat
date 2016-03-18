#include "MessageServer.hpp"
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

MessageServer::MessageServer(const char *repBindAddress,
                             const char *pubBindAddress)
    : _context(1), _repSocket(_context, ZMQ_REP), _pubSocket(_context, ZMQ_PUB),
      _storage() {
  _repSocket.bind(repBindAddress);
  _pubSocket.bind(pubBindAddress);
}

void MessageServer::run() {
  for (;;) {
    zmq::message_t reqMsg;
    _repSocket.recv(&reqMsg);
    Request req;
    if (tryParseRequest(reqMsg, &req)) {
      ChatResponses response = makeResponse(req);
      _repSocket.send(wrapMessage(response));
      handleUpdatePublish(req, response);
    }
    else {
      LOG("Failed to parse from array");
      _repSocket.send(
          wrapMessage(makeInvalidRequest(ResponseStatus::FailedToParse)));
    }
  }
}

ChatResponses MessageServer::makeResponse(const Request &request) {
  switch (request.type()) {
  case RequestType::Put:
    LOG("Received PUT message");
    return handleMessageSending(request);
  case RequestType::Get:
    LOG("Received GET message");
    return handleMessageRetrieval(request);
  default:
    LOG("Received unknown message");
    break;
  }
}

ChatResponses MessageServer::handleMessageSending(Request request) {
  // create timestamp message for "now"
  Timestamp recvTs =
      timestampMessageFromNative(std::chrono::system_clock::now());
  // create response message with received timestamp
  ChatMessage msg = request.messageput().msg();
  *msg.mutable_receivedat() = recvTs;
  _storage.put(std::move(msg));
  // create response message
  ChatResponses response;
  response.set_type(ResponseType::Put);
  response.set_status(ResponseStatus::Ok);
  return response;
}

ChatResponses MessageServer::handleMessageRetrieval(Request request) {
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
  return resp;
}

ChatResponses MessageServer::makeInvalidRequest(ResponseStatus status,
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
  return resp;
}

ChatResponses MessageServer::makeInvalidRequest(ResponseStatus status) const {
  InvalidRequest invalidRequest;
  ChatResponses resp;
  resp.set_type(ResponseType::Invalid);
  resp.set_status(status);
  *resp.mutable_invalid() = invalidRequest;
  return resp;
}

void MessageServer::handleUpdatePublish(Request request,
                                        ChatResponses response) {
  if (request.type() == RequestType::Put &&
      response.status() == ResponseStatus::Ok) {
    assert(request.has_messageput() && "Attempt to broadcast empty message");
    const ChatMessage msg = request.messageput().msg();
    _pubSocket.send(wrapMessage(msg));
  }
}

bool MessageServer::tryParseRequest(const zmq::message_t &msg,
                                    Request *request) const {
  return request->ParseFromArray(msg.data(), msg.size());
}
