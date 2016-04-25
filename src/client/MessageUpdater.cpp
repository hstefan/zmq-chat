#include "MessageUpdater.hpp"
#include "shared/Debug.hpp"
#include <array>

using namespace hst;

MessageUpdater::MessageUpdater(const char *repServer, const char *pubServer)
    : _messages(),
      _runThread(&MessageUpdater::run, this, repServer, pubServer) {
}

void MessageUpdater::run(const char *repServer, const char *pubServer) {
  LOG("Client starting to run!");
  zmq::context_t ctx;

  // TODO: subscribe only to specific topics
  zmq::socket_t sub(ctx, ZMQ_SUB);
  sub.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  sub.connect(pubServer);

  // catch-up requester socket
  zmq::socket_t req(ctx, ZMQ_REQ);
  req.connect(repServer);

  // setup socket polling
  zmq::pollitem_t items[] = {
    { static_cast<void*>(sub), 0, ZMQ_POLLIN, 0},
  };
  const size_t itemCount = sizeof(items) / sizeof(zmq::pollitem_t);
  zmq::pollitem_t& subPoll = items[0];

  zmq_pollitem_t it;
  while (true) {
    zmq::message_t msg;
    zmq::poll(&items[0], itemCount, -1);

    if (subPoll.revents & ZMQ_POLLIN) {
      // handle subscription message
      sub.recv(&msg);
      handleSubscription(msg);
    }
  }
}

void MessageUpdater::handleSubscription(const zmq::message_t& msg) {
  ChatMessage chatMessage;
  // the only event the updater should care about is ChatMessage
  if (chatMessage.ParseFromArray(msg.data(), msg.size())) {
    storeMessage(chatMessage);
  }
}

void MessageUpdater::storeMessage(const ChatMessage &msg) {
  LOG_F("Received message from %s\n", msg.user().nickname().c_str());
  _messages.insert(std::make_pair(msg.messageid(), msg));
}
