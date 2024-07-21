add_executable(emos_unit_tests)
target_common_setup(emos_unit_tests)
target_find_sources_and_add(emos_unit_tests)
target_setup_vs_folders(emos_unit_tests)
target_link_libraries(emos_unit_tests PRIVATE emos_lib gtest)
target_include_directories(emos_unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(emos_unit_tests PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--gtest_filter=*")

add_test(NAME UnitTests COMMAND emos_unit_tests)

set(RUN_UNIT_TESTS_TARGET run_emos_unit_tests)
add_custom_target(${RUN_UNIT_TESTS_TARGET} DEPENDS emos_unit_tests)
add_custom_command(
                    TARGET ${RUN_UNIT_TESTS_TARGET}
                    POST_BUILD
                    COMMAND echo Running emos_unit_tests in $<TARGET_FILE_DIR:emos_unit_tests>
                    COMMAND WORKING_DIRECTORY $<TARGET_FILE_DIR:emos_unit_tests>
                    COMMAND emos_unit_tests --gtest_filter=*
                    )

add_subdirectories()
