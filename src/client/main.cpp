#include "ChatClient.hpp"
#include "shared/TimestampUtil.hpp"
#include <iostream>

void doNicknameCommand(std::string *nickname) {
  std::cout << ">> new nickname: ";
  std::getline(std::cin, *nickname);
}

void doSendCommand(std::string nickname, hst::ChatClient *client) {
  std::string message;
  std::cout << ">> Message: ";
  std::getline(std::cin, message);
  client->sendMessage(nickname, message);
  std::cout << "<< Message sent." << std::endl;
}

void doHelpCommand() {
  std::cout << "<< q: quit" << std::endl
            << "<< h: print this text" << std::endl
            << "<< n: set new nickname" << std::endl
            << "<< s: send message" << std::endl
            << "<< g: get new message" << std::endl;
}

void doGetCommand(hst::ChatClient *client) {
  auto messages = client->receiveMessages();
  if (messages.empty())
    std::cout << "<< No new messages!" << std::endl;
  else {
    for (const auto &m : messages)
      std::cout << "<< " << m.nickname() << ": " << m.content() << std::endl;
  }
}

int main() {
  hst::ChatClient client("tcp://localhost:5555");
  std::string nickname = "unknown";

  bool quit = false;
  while (!quit) {
    std::string command;
    std::cout << "> ";
    std::getline(std::cin, command);
    if (command == "q")
      quit = true;
    else if (command == "h")
      doHelpCommand();
    else if (command == "n")
      doNicknameCommand(&nickname);
    else if (command == "s")
      doSendCommand(nickname, &client);
    else if (command == "g")
      doGetCommand(&client);
    else
      std::cout << "<< Uknown command, " << command << std::endl;
    ;
  }
}
