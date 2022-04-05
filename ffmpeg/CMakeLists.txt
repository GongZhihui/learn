include(../module.cmake)

find_cur_all_files(CUR_DIRS)
source_group_by_dir(CUR_DIRS)

set(EXECUTABLE_OUTPUT_PATH ${INSTALL_BIN_PATH})
add_executable(ffmpeg ${CUR_DIRS})
