set(SOURCE
  main.cpp
  ChatClient.cpp
)

set(HEADER
  ChatClient.hpp
)


set(LIBRARY
  ${ZMQ_LIBRARIES}
  zmqchat
)

add_executable(chatclient ${SOURCE} ${HEADER})
target_link_libraries(chatclient ${LIBRARY})
