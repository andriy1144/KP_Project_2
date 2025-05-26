/*
#pragma once

#include <string>
#include <vector>

#include "Reader.hpp"

class StudentReader : public Reader {
private:
    std::string university;
    std::string faculty;
    int course;
    std::string group;

    std::vector<std::pair<std::string, std::string>> _attributes;
public:
    StudentReader(int id, const std::string& name, int libraryId,
        const std::string& university, const std::string& faculty, int course, const std::string& group);
    StudentReader(int id, const std::string& name, int libraryId);

    std::string getSpecificAttributes() const override;

    std::string serialize() const override;
    std::string toString() const override;
};
*/