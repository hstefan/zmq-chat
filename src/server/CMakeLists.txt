set(SOURCE
  main.cpp
  MessageServer.cpp
  MessageContainer.cpp
)

set(HEADER
  MessageServer.hpp
  MessageContainer.hpp
)


set(LIBRARY
  ${ZMQ_LIBRARIES}
  zmqchat
)

add_executable(chatserver ${SOURCE} ${HEADER})
target_link_libraries(chatserver ${LIBRARY})
