#include "ChatClient.hpp"
#include <iostream>
#include <thread>

int main() {
  hst::ChatClient client("tcp://localhost:5555");
  client.sendMessage("hstefan", "Hello from the other siiiiide");
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  for (chat::common::ChatMessage m : client.receiveMessages()) {
    std::cout << m.nickname() << ": " << m.content() << std::endl;
  }
}

