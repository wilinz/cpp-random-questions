//
// Created by wilinz on 23-5-2.
//

#ifndef MTEST_QUESTIONS_HPP
#define MTEST_QUESTIONS_HPP

#endif //MTEST_QUESTIONS_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <ostream>

using namespace std;

class ImportQuestionRequest{
public:
    string password;
    vector<string> questions;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ImportQuestionRequest, password, questions);

    friend ostream &operator<<(ostream &os, const ImportQuestionRequest &request) {
        os << "password: " << request.password << " questions: " << request.questions.size();
        return os;
    }
};