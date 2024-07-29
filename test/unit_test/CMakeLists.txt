add_executable(emos_unit_tests)
target_common_setup(emos_unit_tests)
target_find_sources_and_add(emos_unit_tests)
target_setup_vs_folders(emos_unit_tests)
target_link_libraries(emos_unit_tests PRIVATE emos_lib gtest)
target_include_directories(emos_unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(emos_unit_tests PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--gtest_filter=*")
set_target_properties(emos_unit_tests PROPERTIES FOLDER tests)

add_test(NAME UnitTests COMMAND emos_unit_tests)
define_test_runner_target(emos_unit_tests)

add_subdirectories()
