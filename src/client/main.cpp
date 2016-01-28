#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
  using namespace std::chrono;

  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);

  std::cout << "Connecting to hello world server" << std::endl;
  socket.connect("tcp://localhost:5555");

  for (;;) {
  }
  return 0;
}

