#include "ChatServer.hpp"
#include "shared/requests.pb.h"
#include "shared/responses.pb.h"
#include "shared/TimestampUtil.hpp"
#include "shared/MessagingUtils.hpp"
#include "shared/Debug.hpp"

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

    zmq::message_t response = makeResponse(request, &response);
    _socket.send(response);
  }
}

zmq::message_t ChatServer::makeResponse(const zmq::message_t &request,
                      zmq::message_t *response) {
  ChatRequest req;
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
    return makeInvalidRequest(GeneralError);
  }
}

zmq::message_t
ChatServer::handleMessageSending(chat::reqs::ChatRequest request) {
  using namespace std::chrono;
  // create timestamp message for "now"
  Timestamp recvTs = timestampMessageFromNative(system_clock::now());
  // create response message with received timestamp
  ChatMessage msg = request.messageput().msg();
  *msg.mutable_receivedat() = recvTs;
  _storage.put(std::move(msg));
  ChatResponses okResponse;
  okResponse.set_type(ResponseType::Common);
  return wrapMessage(okResponse);
}

zmq::message_t
ChatServer::handleMessageRetrieval(chat::reqs::ChatRequest request) {
  auto qualReq = request.messageget();
  // query for messages in interval
  std::vector<ChatMessage> queryResult;
  if (qualReq.has_totime() && qualReq.has_fromtime()) {
    queryResult = _storage.getFromTo(
        nativeTimestampFromMessage(qualReq.fromtime()),
        nativeTimestampFromMessage(qualReq.totime()));
  } else if (qualReq.has_totime()) {
    queryResult = _storage.getTo(
        nativeTimestampFromMessage(qualReq.totime()));
  } else if (qualReq.has_fromtime()) {
    queryResult = _storage.getFrom(
        nativeTimestampFromMessage(qualReq.fromtime()));
  } else {
    // invalid request, at least one of the optional fields must be present
    return makeInvalidRequest(GeneralError, request);
  }

  ReceivedChatMessages msgs;
  for(const auto& m : queryResult)
    *msgs.add_messages() = m;
  return wrapMessage(msgs);
}

  return wrapMessage(resp);
}

zmq::message_t
ChatServer::makeInvalidRequest(int errorCode,
                               const ChatRequest &request) const {
  InvalidRequest r;
  std::string reqData; // TODO
  r.set_errorcode(errorCode);
  r.set_originalrequest(reqData);
  return wrapMessage(r);
}

zmq::message_t ChatServer::makeInvalidRequest(int errorCode)  const {
  InvalidRequest r;
  r.set_errorcode(errorCode);
  return wrapMessage(r);
}
