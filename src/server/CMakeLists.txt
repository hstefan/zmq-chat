set(SOURCE
  main.cpp
  ChatServer.cpp
  MessageContainer.cpp
)

set(HEADER
  ChatServer.hpp
  MessageContainer.hpp
)


set(LIBRARY
  ${ZMQ_LIBRARIES}
  zmqchat
)

add_executable(chatserver ${SOURCE} ${HEADER})
target_link_libraries(chatserver ${LIBRARY})
