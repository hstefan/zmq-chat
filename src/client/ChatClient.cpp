#include "ChatClient.hpp"
#include "shared/requests.pb.h"
#include "shared/TimestampUtil.hpp"
#include "shared/MessagingUtils.hpp"
#include "shared/Debug.hpp"
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
  User user;
  user.set_nickname(nickname);

  ChatMessage chatMessage;
  *chatMessage.mutable_user() = user;
  chatMessage.set_content(message);

  PutRequest putRequest;
  *putRequest.mutable_msg() = chatMessage;

  Request req;
  req.set_type(RequestType::Put);
  *req.mutable_messageput() = putRequest;

  _socket.send(wrapMessage(req));

  zmq::message_t response;
  _socket.recv(&response);

  ChatResponses qualResponse;
  if (qualResponse.ParseFromArray(response.data(), response.size())) {
    switch (qualResponse.type()) {
    case ResponseType::Get:
      if (qualResponse.status() != ResponseStatus::Ok)
        LOG_E("Server responded with error status for Get.\n");
      break;
    case ResponseType::Invalid:
      switch (qualResponse.status()) {
        case ResponseStatus::MissingArguments:
          LOG_E("Server responded with MissingArguments error.\n");
          break;
        case ResponseStatus::FailedToParse:
          LOG_E("Server responded with FailedToParse error.\n");
          break;
        case ResponseStatus::GeneralError:
          LOG_E("Server responded with GeneralError.\n");
          break;
        case ResponseStatus::Ok:
          LOG_W("Server answer with Ok status for invalid request.\n");
          break;
      }
      break;
    case ResponseType::Put:
      _lastId = qualResponse.putresponse().messageid();
      break;
    }
  }
}

std::vector<ChatMessage> ChatClient::receiveMessages() {
  GetRequest getRequest;
  if (_lastId != -1)
    getRequest.set_lastknownid(_lastId);
  Request req;
  req.set_type(RequestType::Get);
  *req.mutable_messageget() = getRequest;

  _socket.send(wrapMessage(req));

  zmq::message_t response;
  _socket.recv(&response);

  std::vector<ChatMessage> newMessages;

  ChatResponses qualResponse;
  if (qualResponse.ParseFromArray(response.data(), response.size())) {
    switch (qualResponse.type()) {
    case ResponseType::Invalid:
      switch (qualResponse.status()) {
      case ResponseStatus::MissingArguments:
        LOG_E("Server responded with MissingArguments error.\n");
        break;
      case ResponseStatus::FailedToParse:
        LOG_E("Server responded with FailedToParse error.\n");
        break;
      case ResponseStatus::GeneralError:
        LOG_E("Server responded with GeneralError.\n");
        break;
      case ResponseStatus::Ok:
        LOG_W("Server answer with Ok status for invalid request.\n");
        break;
      }
      break;
    case ResponseType::Put:
      LOG_E("Unexpected response for get request.\n");
      break;
    case ResponseType::Get:
      const auto &messages = qualResponse.getresponse().messages();
      newMessages.insert(newMessages.end(), messages.begin(), messages.end());
      break;
    }
  }

  return newMessages;
}

