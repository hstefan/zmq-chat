#include "ChatServer.hpp"

int main() {
  hst::ChatServer server("tcp://*:5555");
  server.run();
  return 0;
}
