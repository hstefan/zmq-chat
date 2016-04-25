set(SOURCE
  main.cpp
  ChatClient.cpp
  MessageUpdater.cpp
)

set(HEADER
  ChatClient.hpp
  MessageUpdater.hpp
)


set(LIBRARY
  ${ZMQ_LIBRARIES}
  zmqchat
)

add_executable(chatclient ${SOURCE} ${HEADER})
target_link_libraries(chatclient ${LIBRARY})
