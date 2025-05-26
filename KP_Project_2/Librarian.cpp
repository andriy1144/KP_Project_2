#pragma once
#include "Librarian.hpp"


Librarian::Librarian(int id, int libraryId, const std::string& firstName,
	const std::string& lastName,
	const std::string& email,
	const std::string& phoneNumber,
	const std::string& address,
	const std::string& city,
	unsigned int age, const std::string& hallId) : _id(id) , _libraryId(libraryId) ,
	PersonInfo(firstName, lastName, email, phoneNumber, address, city, age),
	_hallId(hallId) {}


Librarian::~Librarian() = default;

int Librarian::getId() const { return _id; }
int Librarian::getLibraryId() const { return _libraryId; }
std::string Librarian::getHallId() const { return _hallId; }

void Librarian::setLibraryId(int library) { _libraryId = library;}
void Librarian::setHallId(const std::string& hallId) { _hallId = hallId; }

std::string Librarian::serialize() const {
    return std::to_string(_id) + "|" + std::to_string(_libraryId) + "|" + PersonInfo::serialize() + "|" + _hallId;
}
std::string Librarian::toString() const {
	return "ID: " + std::to_string(_id) + ", " + PersonInfo::toString() + ", Бібліотека ID" + std::to_string(_libraryId) + ", Зал ID: " + _hallId;
}