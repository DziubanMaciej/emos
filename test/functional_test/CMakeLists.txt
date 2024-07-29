set(PROGRAMS_DIRECTORY ${CMAKE_BINARY_DIR}/third_party/6502_functional_tests/programs)

function (define_functional_test TEST_NAME TARGET_NAME BIN_FILE TEST_INDEX)
    add_executable(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER tests)
    target_common_setup(${TARGET_NAME})
    target_link_libraries(${TARGET_NAME} PRIVATE emos_lib)
    target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_compile_definitions(${TARGET_NAME} PRIVATE
        -DTEST_BINARY_FILE="${BIN_FILE}"
        -DTEST_INDEX=${TEST_INDEX}
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TARGET_NAME})
    add_dependencies(${TARGET_NAME} compile_functional_tests)
endfunction()

define_functional_test(FunctionalTest emos_functional_test ${PROGRAMS_DIRECTORY}/6502_functional_test.bin 0)
# define_functional_test(DecimalTest emos_decimal_test ${PROGRAMS_DIRECTORY}/6502_decimal_test.bin 1)
# define_functional_test(InterruptTest emos_interrupt_test ${THIRD_PARTY_DST_DIR}/6502_interrupt_test.bin 2)
