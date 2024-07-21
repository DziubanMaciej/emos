set(THIRD_PARTY_SRC_DIR ${PROJECT_SOURCE_DIR}/third_party/6502_65C02_functional_tests)
set(THIRD_PARTY_DST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
add_custom_command(
        POST_BUILD
        MAIN_DEPENDENCY ${THIRD_PARTY_SRC_DIR}/as65_142.zip
        OUTPUT ${THIRD_PARTY_DST_DIR}/as65/as65
        COMMAND unzip -o ${THIRD_PARTY_SRC_DIR}/as65_142.zip -d ${THIRD_PARTY_DST_DIR}/as65 >/dev/null
        COMMENT "Unzipping assembler ${FILE_NAME}"
)
function (add_custom_command_copy_and_build_file FILE_NAME)
    add_custom_command(
        POST_BUILD
        MAIN_DEPENDENCY ${THIRD_PARTY_SRC_DIR}/${FILE_NAME}
        OUTPUT ${THIRD_PARTY_DST_DIR}/${FILE_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy ${THIRD_PARTY_SRC_DIR}/${FILE_NAME} ${THIRD_PARTY_DST_DIR}/${FILE_NAME}
        COMMENT "Copying ${FILE_NAME}"
    )
    get_filename_component(FILE_NAME_BUILT ${FILE_NAME} NAME_WE)
    set(FILE_NAME_BUILT ${FILE_NAME_BUILT}.bin)
    add_custom_command(
        POST_BUILD
        MAIN_DEPENDENCY ${THIRD_PARTY_DST_DIR}/${FILE_NAME}
        DEPENDS ${THIRD_PARTY_DST_DIR}/as65/as65
        OUTPUT ${THIRD_PARTY_DST_DIR}/${FILE_NAME_BUILT}
        COMMAND ${THIRD_PARTY_DST_DIR}/as65/as65 -l -m -w -h0 ${THIRD_PARTY_DST_DIR}/${FILE_NAME}
        COMMENT "Assembling ${FILE_NAME}"
    )
endfunction()
add_custom_command_copy_and_build_file(6502_functional_test.a65)
add_custom_command_copy_and_build_file(6502_decimal_test.a65)
add_custom_command_copy_and_build_file(6502_interrupt_test.a65)

add_executable(emos_functional_tests
        ${THIRD_PARTY_DST_DIR}/6502_functional_test.bin
        ${THIRD_PARTY_DST_DIR}/6502_decimal_test.bin
        ${THIRD_PARTY_DST_DIR}/6502_interrupt_test.bin
)
target_common_setup(emos_functional_tests)
target_find_sources_and_add(emos_functional_tests)
target_setup_vs_folders(emos_functional_tests)
target_link_libraries(emos_functional_tests PRIVATE emos_lib)
target_include_directories(emos_functional_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_definitions(emos_functional_tests PRIVATE
    -DTEST_BINARY_FILE="${THIRD_PARTY_DST_DIR}/6502_functional_test.bin"
)

add_test(NAME FunctionalTests COMMAND emos_functional_tests)

add_subdirectories()
