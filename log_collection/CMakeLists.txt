# 设置最低 CMake 版本
cmake_minimum_required(VERSION 3.15)

# 设置项目名称
project(logcollection)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 配置输出路径，指定可执行文件输出到项目根目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# 查找 vcpkg 安装的库（如 zlib）
# 假设你已经安装了 zlib，通过 vcpkg 或其他包管理器
find_package(httplib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(hiredis CONFIG REQUIRED)
find_package(RdKafka CONFIG REQUIRED)

# 手动查找kafka
find_library(RDKAFKA_LIB rdkafka HINTS /root/vcpkg/installed/x64-linux/lib)
include_directories(/root/vcpkg/installed/x64-linux/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加源文件
add_executable(main main.cpp)
# 链接  库
target_link_libraries(main PRIVATE 
    httplib::httplib 
    nlohmann_json::nlohmann_json
    hiredis  # 链接 hiredis 库
    ${RDKAFKA_LIB}
    RdKafka::rdkafka++
)

# 可选：添加 include 目录（如果需要的话）
# target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 如果你有多个源文件，可以直接在这里添加
# add_executable(main src/main.cpp src/other_file.cpp)


# 设置输出路径为项目根目录
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)




