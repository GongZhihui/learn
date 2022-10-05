project(d3dwindows)
include(../module.cmake)

find_cur_all_files(CUR_DIRS)
source_group_by_dir(CUR_DIRS)

link_libraries(d3d11
	d3dcompiler
)

set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/shader/vertexshader.hlsl PROPERTIES VS_SHADER_FLAGS "/Fh \"$(OutDir)%(Filename).cso\"" VS_SHADER_TYPE Vertex VS_SHADER_MODEL 5.0 VS_SHADER_ENTRYPOINT main)
set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/shader/pixshader.hlsl PROPERTIES VS_SHADER_FLAGS "/Fh \"$(OutDir)%(Filename).cso\"" VS_SHADER_TYPE Pixel VS_SHADER_MODEL 5.0 VS_SHADER_ENTRYPOINT main)

find_package(Qt5
	COMPONENTS
	core 
	gui 
	widgets 
	REQUIRED
)

set(EXECUTABLE_OUTPUT_PATH ${INSTALL_BIN_PATH})
add_executable(${PROJECT_NAME} ${CUR_DIRS})

target_link_libraries(${PROJECT_NAME}
	PRIVATE
	Qt5::Core 
	Qt5::Gui 
	Qt5::Widgets 
)