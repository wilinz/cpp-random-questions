//
// Created by wilinz on 23-4-29.
//
#include <odb/mysql/database.hxx>
#include <memory>
#include "mysql.h"


std::unique_ptr<odb::core::database> initMysql(){
    return std::make_unique<odb::mysql::database>("root", "", "questions", "home.wilinz.com", 5506, "", "utf8");
}

