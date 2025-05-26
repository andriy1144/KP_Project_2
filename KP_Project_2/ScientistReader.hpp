/*
#pragma once

#include <string>
#include "Reader.hpp"

class ScientistReader : public Reader {
private:
    std::string organization;
    std::string researchTopic;
public:
    ScientistReader(int id, const std::string& name, int libraryId,
        const std::string& organization, const std::string& researchTopic);
    std::string getSpecificAttributes() const override;

    std::string serialize() const override;
    std::string toString() const override;
};
*/