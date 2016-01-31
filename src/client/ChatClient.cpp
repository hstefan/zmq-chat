#include "ChatClient.hpp"
#include "shared/requests.pb.h"
#include "shared/TimestampUtil.hpp"
#include "shared/MessagingUtils.hpp"
#include <iostream>

using namespace hst;
using namespace std::chrono;
using namespace chat::resps;
using namespace chat::reqs;
using namespace chat::common;

ChatClient::ChatClient(const char *serverAddress)
    : _context(1), _socket(_context, ZMQ_REQ), _lastUpdate() {
  _socket.connect(serverAddress);
}

void ChatClient::sendMessage(std::string nickname, std::string message) {
  ChatMessage msg;
  msg.set_nickname(nickname);
  msg.set_content(message);
  *msg.mutable_sentat() = timestampMessageFromNative(system_clock::now());
  MessagePut msgPut;
  *msgPut.mutable_msg() = msg;
  ChatRequest req;
  req.set_type(RequestType::Put);
  *req.mutable_messageput() = msgPut;

  _socket.send(wrapMessage(req));

  zmq::message_t response;
  _socket.recv(&response);

  ChatResponses qualResponse;
  if (qualResponse.ParseFromArray(response.data(), response.size())) {
    switch (qualResponse.type()) {
    case Common:
      // we don't need to do anything
      break;
    case Invalid:
      std::cerr << "Invalid request, server returned "
                << qualResponse.invalid().errorcode() << std::endl;
      break;
    case Receive:
      std::cerr << "Unexpected response for put request" << std::endl;
      break;
    }
  }
}

std::vector<ChatMessage> ChatClient::receiveMessages() {
  MessageGet msgGet;
  *msgGet.mutable_fromtime() = timestampMessageFromNative(_lastUpdate);
  ChatRequest req;
  req.set_type(RequestType::Get);
  *req.mutable_messageget() = msgGet;

  _socket.send(wrapMessage(req));

  zmq::message_t response;
  _socket.recv(&response);

  std::vector<ChatMessage> newMessages;

  ChatResponses qualResponse;
  if (qualResponse.ParseFromArray(response.data(), response.size())) {
    switch (qualResponse.type()) {
    case Common:
      std::cerr << "Unexpected response for get request" << std::endl;
      break;
    case Invalid:
      std::cerr << "Invalid request, server returned "
                << qualResponse.invalid().errorcode() << std::endl;
      break;
    case Receive:
      const auto &recv = qualResponse.receivedmessages();
      newMessages.insert(newMessages.end(), recv.messages().begin(),
                         recv.messages().end());
      if (!newMessages.empty()) {
        _lastUpdate =
            nativeTimestampFromMessage(newMessages.back().receivedat());
        _messages.insert(_messages.end(), newMessages.begin(),
                         newMessages.end());
      }
      break;
    }
  }

  return newMessages;
}

