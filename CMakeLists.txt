cmake_minimum_required(VERSION 3.10)  

# 项目名称  
project(raytracing)  

# 设置 C++17 标准  
set(CMAKE_CXX_STANDARD 17)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用 C++17 标准  

# 添加可执行文件，包含 main.cpp 和 vector.cpp  
add_executable(raytracing main.cpp vector.cpp)  

# 设置 Visual Studio 的启动项目为 raytracing  
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT raytracing)  

# 针对 Visual Studio 目标，设置预处理宏  
target_compile_definitions(raytracing PRIVATE  
    _CRT_SECURE_NO_DEPRECATE  
    _SCL_SECURE_NO_DEPRECATE  
)  
