#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "shared/hello.pb.h"

int main() {
  using namespace std::chrono;

  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);

  std::cout << "Connecting to hello world server" << std::endl;
  socket.connect("tcp://localhost:5555");

  for (;;) {
    zmqchat::Hello helloRequest;
    helloRequest.set_timestamp(
        duration_cast<microseconds>(
            high_resolution_clock::now().time_since_epoch())
            .count());
    helloRequest.set_msgcontent("Hey you");

    std::string requestContent;
    if (helloRequest.SerializeToString(&requestContent)) {
      zmq::message_t request(requestContent.size());
      memcpy(request.data(), requestContent.c_str(), requestContent.size());
      socket.send(request);
    }

    zmq::message_t response;
    socket.recv(&response);
    zmqchat::Hello helloResponse;
    if (helloResponse.ParseFromArray(response.data(), response.size())) {
      auto elapsedTime = helloResponse.timestamp() - helloRequest.timestamp();
      std::cout << "Received answer \"" << helloResponse.msgcontent()
                << "\" after " << elapsedTime << " us.\n";
    } else {
      zmqchat::InvalidRequest invalidRequest;
      if (invalidRequest.ParseFromArray(response.data(), response.size())) {
        std::cerr << "Server returned error: " << invalidRequest.errorcode()
                  << ".\n";
      }
    }
  }
  return 0;
}

