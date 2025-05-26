#include "Library.hpp"

Library::Library(int id, const std::string& name, const std::string& address, std::vector<std::string> halls)
    : _id(id), _name(name), _address(address), _halls(halls) {}

Library::~Library() = default;

void Library::addHall(const std::string& hallId) {
    _halls.push_back(hallId);
}
void Library::setHalls(const std::vector<std::string>& halls) {
	_halls = halls;
}

int Library::getId() const { return _id; }
std::string Library::getName() const { return _name; }
std::vector<std::string> Library::getHalls() const { return _halls; }
std::string Library::getAddress() const { return _address; }


void Library::setName(const std::string& name) { _name = name; }
void Library::setAddress(const std::string& address) { _address = address; }

std::string Library::serialize() const {
    std::string hallList = "";
    for (const auto& hall : _halls) {
        hallList += hall + ";";
    }
    return std::to_string(_id) + "|" + _name + "|" + _address + "|" + hallList;
}
std::string Library::toString() const {
	std::string hallList = "";
	for (const auto& hall : _halls) {
		hallList += hall + ", ";
	}
	if (!hallList.empty()) {
		hallList.pop_back(); 
		hallList.pop_back();
	}
	return "ID: " + std::to_string(_id) + ", Назва: " + _name + ", Адреса: " + _address + ", Зали: [" + hallList + "]";
}