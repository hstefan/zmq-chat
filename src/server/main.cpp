#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include "shared/hello.pb.h"

int main() {
  using namespace std::chrono;

  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REP);
  socket.bind("tcp://*:5555");

  for (;;) {
    zmq::message_t request;
    socket.recv(&request);
    zmqchat::Hello helloRequest;
    if (helloRequest.ParseFromArray(request.data(), request.size())) {
      zmqchat::Hello helloResponse;
      helloResponse.set_timestamp(
          duration_cast<microseconds>(
              high_resolution_clock::now().time_since_epoch())
              .count());
      helloResponse.set_msgcontent("Out there in the cold?");

      std::string messageContent;
      if (helloResponse.SerializeToString(&messageContent)) {
        zmq::message_t okResponse(messageContent.size());
        memcpy(okResponse.data(), messageContent.c_str(),
               messageContent.size());
        socket.send(okResponse);
      } else {
        zmqchat::InvalidRequest invalidRequest;
        invalidRequest.set_errorcode(-1);
        auto invalidRequestContent = invalidRequest.SerializeAsString();
        zmq::message_t badResponse(invalidRequestContent.size());
        memcpy(badResponse.data(), invalidRequestContent.c_str(),
               invalidRequestContent.size());
        std::cerr << "Something went wrong with the server serialization"
                  << std::endl;
      }
    }
  }

  return 0;
}
