set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(googletest)
set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER third_party)

add_subdirectory(6502_functional_tests)
