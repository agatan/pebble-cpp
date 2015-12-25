file(GLOB_RECURSE SRCS *.cpp)

add_library(pebble-syntax-lib
  ${SRCS}
  ${Boost_LIBRARIES})
