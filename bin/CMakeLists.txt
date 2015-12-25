file(GLOB_RECURSE SRC *.cpp)
add_executable(
  pebblec
  ${SRC}
  )
target_link_libraries(pebblec
  pebble-syntax-lib
  ${Boost_LIBRARIES}
  )
