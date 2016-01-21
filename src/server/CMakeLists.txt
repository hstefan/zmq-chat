set(SOURCE
  main.cpp
)

set(HEADER
)


set(LIBRARY
  ${ZMQ_LIBRARIES}
  zmqchat
)

add_executable(chatserver ${SOURCE} ${HEADER})
target_link_libraries(chatserver ${LIBRARY})
