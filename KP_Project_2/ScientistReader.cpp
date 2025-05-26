/*
#include "ScientistReader.hpp"

ScientistReader::ScientistReader(int id, const std::string& name, int libraryId,
    const std::string& organization, const std::string& researchTopic)
    : Reader(id, name, "Researcher", libraryId), organization(organization), researchTopic(researchTopic) {}

std::string ScientistReader::getSpecificAttributes() const {
    return "Organization: " + organization + ", Research Topic: " + researchTopic;
}

std::string ScientistReader::serialize() const {
    return Reader::serialize() + "|" + organization + "|" + researchTopic;
}

std::string ScientistReader::toString() const {
	return Reader::toString() + ", " + getSpecificAttributes();
}
*/