# Setup assembler
set(ASSEMBLER_URL "https://www.kingswood-consulting.co.uk/assemblers/as65_142.zip")
set(ASSEMBLER_DIR "${CMAKE_CURRENT_BINARY_DIR}/as65")
set(ASSEMBLER_DIR_UNPACKED "${ASSEMBLER_DIR}/unpacked")
if(NOT EXISTS "${ASSEMBLER_FILE}")
    # Download
    include(FetchContent)
    cmake_policy(SET CMP0135 NEW)
    FetchContent_Declare(
        as65
        URL ${ASSEMBLER_URL}
        PREFIX ${ASSEMBLER_DIR}
        URL_HASH MD5=421d68823b460b5e3f05254632f8aed1
        DOWNLOAD_NAME ${ASSEMBLER_FILE}
    )
    FetchContent_MakeAvailable(as65)

    # Unpack
    if (UNIX)
        execute_process(
            COMMAND unzip -o "${ASSEMBLER_DIR}/src/as65_142.zip" -d ${ASSEMBLER_DIR_UNPACKED}
            OUTPUT_QUIET
        )
        set(ASSEMBLER_BINARY "${ASSEMBLER_DIR_UNPACKED}/as65")
    elseif (WIN32)
        # TODO implement unpacking
        message(FATAL_ERROR "Windows not implemented")
        set(ASSEMBLER_BINARY "${ASSEMBLER_DIR_UNPACKED}/as65.exe")
    endif()
endif()

add_custom_target(CompileFunctionalTests)

set(COMPILATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/programs")
function (add_custom_command_copy_and_build_file FILE_NAME)
    get_filename_component(FILE_NAME_BUILT ${FILE_NAME} NAME_WE)
    set(FILE_NAME_BUILT ${FILE_NAME_BUILT}.bin)

    add_custom_command(
        TARGET CompileFunctionalTests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${FILE_NAME}" "${COMPILATION_DIR}/${FILE_NAME}"
        COMMAND ${ASSEMBLER_BINARY} -l -m -w -h0 ${COMPILATION_DIR}/${FILE_NAME}
    )
endfunction()
add_custom_command_copy_and_build_file(6502_functional_test.a65)
add_custom_command_copy_and_build_file(6502_decimal_test.a65)
add_custom_command_copy_and_build_file(6502_interrupt_test.a65)
