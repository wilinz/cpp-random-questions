#ifndef __PERSION_HPP__
#define __PERSION_HPP__

#include <string>
#include <odb/core.hxx>
#include <nlohmann/json.hpp>

#pragma db object table("person")

class Person {
public:
    Person(const std::string &first,
           const std::string &last,
           unsigned short age);

    const std::string &first() const;

    const std::string &last() const;

    unsigned short age() const;

    void age(unsigned short);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Person, first_, second_);
private:
    Person() {};

    friend class odb::access;

#pragma db id auto
    unsigned long id_;

    std::string first_;
    std::string second_;
    unsigned short age_;
};



#endif
