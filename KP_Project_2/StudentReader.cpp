/*
#include "StudentReader.hpp"


StudentReader::StudentReader(int id, const std::string& name, int libraryId,
    const std::string& university, const std::string& faculty, int course, const std::string& group)
    : Reader(id, name, "Student", libraryId), university(university), faculty(faculty), course(course), group(group) {}

StudentReader::StudentReader(int id, const std::string& name, int libraryId) :
	Reader(id, name, "Student", libraryId), university(""), faculty(""), course(0), group("") {}


void StudentReader::setAttribute(const std::string& key, const std::string& value) {
	_attributes.push_back({ key, value });
}

std::string StudentReader::getSpecificAttributes() const {
    return "University: " + university + ", Faculty: " + faculty + ", Course: " + std::to_string(course) + ", Group: " + group;
}
std::string StudentReader::serialize() const {
    std::string serializedEntity = Reader::serialize();
	for (const auto& attr : _attributes) {
		serializedEntity += "|" + attr.second;
	}
	return serializedEntity;
}
std::string StudentReader::toString() const {
	return Reader::toString() + ", " + getSpecificAttributes();
}
*/