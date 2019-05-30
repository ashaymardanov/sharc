
set(Protobuf_USE_STATIC_LIBS ON)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS model.proto)
SET_SOURCE_FILES_PROPERTIES(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

add_library(proto STATIC ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(proto ${Protobuf_LIBRARIES})

#configure_file(pack.hpp.in pack.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/proto/)
configure_file(${CMAKE_SOURCE_DIR}/src/proto/pack.hpp.in ${CMAKE_SOURCE_DIR}/src/proto/pack.hpp)
configure_file(${CMAKE_SOURCE_DIR}/src/proto/pack.cpp.in ${CMAKE_SOURCE_DIR}/src/proto/pack.cpp)
SET_SOURCE_FILES_PROPERTIES(pack.hpp PROPERTIES GENERATED TRUE)
SET_SOURCE_FILES_PROPERTIES(pack.cpp PROPERTIES GENERATED TRUE)
add_library(pack STATIC ${CMAKE_SOURCE_DIR}/src/proto/pack.cpp ${CMAKE_SOURCE_DIR}/src/proto/pack.hpp)

target_link_libraries(pack proto)