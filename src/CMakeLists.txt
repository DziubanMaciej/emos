add_library(emos_lib STATIC)
target_common_setup(emos_lib)
target_find_sources_and_add(emos_lib)
target_setup_vs_folders(emos_lib)
target_include_directories(emos_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_subdirectories()
