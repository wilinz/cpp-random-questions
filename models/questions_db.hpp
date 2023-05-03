//
// Created by wilinz on 23-5-2.
//

#include <string>
#include <odb/core.hxx>

#ifndef MTEST_QUESTIONS_DB_HPP
#define MTEST_QUESTIONS_DB_HPP

#pragma db object table("questions")

using namespace std;
class Question {

private:
    friend class odb::access;
#pragma db id auto
    unsigned int id;
    string text;
    string answer;
    bool can_view_answer;
    bool selected;

    Question() {}

public:

    Question(const string &text, const string &answer = "", bool canViewAnswer = false, bool selected = false)
            : text(text), answer(answer), can_view_answer(canViewAnswer), selected(selected) {}

    unsigned int getId() const {
        return id;
    }

    const string &getText() const {
        return text;
    }

    const string &getAnswer() const {
        return answer;
    }

    bool isCanViewAnswer() const {
        return can_view_answer;
    }

    bool isSelected() const {
        return selected;
    }

    void setId(unsigned int id) {
        Question::id = id;
    }

    void setText(const string &text) {
        Question::text = text;
    }

    void setAnswer(const string &answer) {
        Question::answer = answer;
    }

    void setCanViewAnswer(bool canViewAnswer) {
        can_view_answer = canViewAnswer;
    }

    void setSelected(bool selected) {
        Question::selected = selected;
    }
};

#endif //MTEST_QUESTIONS_DB_HPP