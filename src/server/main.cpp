#include "MessageServer.hpp"

int main() {
  hst::MessageServer server("tcp://*:5555", "tcp://*:5556");
  server.run();
  return 0;
}
