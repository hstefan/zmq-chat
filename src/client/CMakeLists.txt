set(SOURCE
  main.cpp
)

set(HEADER
)


set(LIBRARY
  ${ZMQ_LIBRARIES}
  zmqchat
)

add_executable(chatclient ${SOURCE} ${HEADER})
target_link_libraries(chatclient ${LIBRARY})
