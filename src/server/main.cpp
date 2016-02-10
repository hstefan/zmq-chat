#include "MessageServer.hpp"

int main() {
  hst::MessageServer server("tcp://*:5555");
  server.run();
  return 0;
}
