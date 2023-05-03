cmake_minimum_required(VERSION 3.0.0)

project(mtest VERSION 0.1.0)


INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/models/
        ${CMAKE_SOURCE_DIR}/generated/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

find_package(oatpp 1.3.0 REQUIRED)
# odb -o generated/ --std c++1 -d mysql --generate-query --generate-schema --show-sloc models/questions_db.hpp
#
#
add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        models/person.cpp
        generated/person-odb.cxx
        generated/questions_db-odb.cxx
        generated/student-odb.cxx
        db/mysql.cpp
        db/mysql.h
        models/person.cpp
        models/questions.hpp
        models/questions_db.hpp models/student.hpp)

target_link_libraries(
        mtest
        odb
        odb-mysql
        oatpp::oatpp
)