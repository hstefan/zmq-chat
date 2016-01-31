#pragma once

#include <zmq.hpp>
#include <google/protobuf/message.h>
#include <memory>

namespace hst {

inline zmq::message_t wrapMessage(const google::protobuf::Message &message) {
  const auto ser = message.SerializeAsString();
  zmq::message_t msg(ser.size());
  std::memcpy(msg.data(), ser.data(), ser.length());
  return std::move(msg);
}
}
