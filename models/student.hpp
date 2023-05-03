#ifndef __STUDENT_HPP__
#define __STUDENT_HPP__

#include <string>
#include <odb/core.hxx>
#include <nlohmann/json.hpp>

#pragma db object table("users")

using namespace std;

class Student {
private:
    Student() {};
    friend class odb::access;

#pragma db id auto
    unsigned long id_;
    unsigned long long student_id;
    string my_answer;
    unsigned long selected_question_id;

public:

    Student(unsigned long id, unsigned long long int studentId, const string &myAnswer, unsigned long selectedQuestionId)
            : id_(id), student_id(studentId), my_answer(myAnswer), selected_question_id(selectedQuestionId) {}

    unsigned long getId() const {
        return id_;
    }

    unsigned long long int getStudentId() const {
        return student_id;
    }

    const string &getMyAnswer() const {
        return my_answer;
    }

    unsigned long getSelectedQuestionId() const {
        return selected_question_id;
    }

    void setId(unsigned long id) {
        Student::id_ = id;
    }

    void setStudentId(unsigned long long int studentId) {
        student_id = studentId;
    }

    void setMyAnswer(const string &myAnswer) {
        my_answer = myAnswer;
    }

    void setSelectedQuestionId(unsigned long selectedQuestionId) {
        selected_question_id = selectedQuestionId;
    }
};



#endif
