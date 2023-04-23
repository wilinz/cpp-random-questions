cmake_minimum_required(VERSION 3.25)
project(question_library)

find_package(oatpp 1.3.0 REQUIRED)
set(CMAKE_CXX_STANDARD 17)

add_executable(question_library src/main.cpp src/handler.h)
target_link_libraries(question_library oatpp::oatpp)