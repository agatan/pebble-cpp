add_executable(parse_test parse_test.cpp)
add_test(NAME parse COMMAND $<TARGET_FILE:parse_test>)
target_link_libraries(parse_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  pebble-syntax-lib)

add_executable(ast_test ast_test.cpp)
add_test(NAME ast COMMAND $<TARGET_FILE:ast_test>)
target_link_libraries(ast_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  pebble-syntax-lib)
