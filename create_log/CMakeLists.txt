# 设置最低 CMake 版本
cmake_minimum_required(VERSION 3.15)

# 设置项目名称
project(logcreate)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 配置输出路径，指定可执行文件输出到项目根目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# 查找 vcpkg 安装的库（如 zlib）
# 假设你已经安装了 zlib，通过 vcpkg 或其他包管理器

# 找到 spdlog 库
find_package(spdlog REQUIRED)
# 添加源文件
add_executable(main create_log.cpp)

# 链接 zlib 库
target_link_libraries(main PRIVATE spdlog::spdlog)

# 可选：添加 include 目录（如果需要的话）
# target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 如果你有多个源文件，可以直接在这里添加
# add_executable(main src/main.cpp src/other_file.cpp)

# 设置输出路径为项目根目录
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)