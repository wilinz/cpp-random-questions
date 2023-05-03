#include "person.hpp"

Person::Person(const std::string& first,
               const std::string& last,
               unsigned short age) : first_(first), second_(last), age_(age) {};

const std::string& Person::first() const {
    return first_;
}

const std::string& Person::last() const {
    return second_;
}

unsigned short Person::age() const {
    return age_;
}

void Person::age(unsigned short a) {
    age_ = a;
}