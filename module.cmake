#定义分组宏
MACRO(source_group_by_dir src_files)
    SET(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    FOREACH(group_files ${${src_files}})
        STRING(REGEX REPLACE ${cur_dir}/\(.*\) \\1 sgbd_fpath ${group_files})
        STRING(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        STRING(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        STRING(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        IF(sgbd_nogroup)
            SOURCE_GROUP("" FILES ${group_files})
        ELSE()
            INCLUDE_DIRECTORIES(${sgbd_group_name})
            SOURCE_GROUP(${sgbd_group_name} FILES ${group_files})
        ENDIF(sgbd_nogroup)
    ENDFOREACH(group_files)
ENDMACRO(source_group_by_dir)

# 将下列扩展名的文件添加到项目中
macro(find_cur_all_files CUR_DIR)
    file(GLOB_RECURSE ${CUR_DIR} "*.h" "*.cpp" 
    "*.hpp" "*.ipp" "*.cc" "*.ui" "*.qrc" "*.md")

endmacro(find_cur_all_files)

# 生成exe
macro(create_exe TARGET_NAME CUR_DIR)
    add_executable(${TARGET_NAME} WIN32 ${CUR_DIRS})

    # 修改输出文件的名字
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME lib${TARGET_NAME}_h)

    # debug版本下，给输出文件增加后缀
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d") 

    # 经过上述设置，最后生成的文件格式为libXXXX_h_d.exe
endmacro(create_exe)