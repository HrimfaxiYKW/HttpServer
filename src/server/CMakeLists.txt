cmake_minimum_required (VERSION 2.8)

project (server)

include_directories(${PROJECT_SOURCE_DIR}/src/common)

# 将当前目录所有源文件名赋值给 DIR_SRCS
aux_source_directory(. DIR_SRCS)

# 生成目标
if (${BUILD_SERVER})
  add_executable(server ${DIR_SRCS})
  target_link_libraries(server libcommon)
endif()