set(PROTO_SOURCE
  requests.proto
  responses.proto
  common.proto
)

set(SOURCE
  TimestampUtil.cpp
)

set(HEADER
  Algorithms.hpp
  TimestampUtil.hpp
  MessagingUtils.hpp
  Debug.hpp
)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_SOURCE})
set(PROTO_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

add_library(zmqchat STATIC ${SOURCE} ${HEADER}
  ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(zmqchat ${PROTOBUF_LIBRARIES})
