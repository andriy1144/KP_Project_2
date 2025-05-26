#include "Reader.hpp"


Reader::Reader(int id,
	const std::string& firstName,
	const std::string& lastName,
	const std::string& email,
	const std::string& phoneNumber,
	const std::string& address,
	const std::string& city,
	unsigned int age, const std::string& category, int libraryId,
	int subscriptionId,	std::vector<std::pair<std::string, std::string>> attributes) : 
	_id(id), PersonInfo(firstName,lastName,email,phoneNumber,address, city, age),
	_category(category), _libId(libraryId), _subscriptionId(subscriptionId), _attributes(attributes) { }

Reader::~Reader() = default;

int Reader::getId() const { return _id; }
std::string Reader::getCategory() const { return _category; }
int Reader::getLibId() const { return _libId; }
int Reader::getSubscriptionId() const { return _subscriptionId; }
int Reader::getBooksCount() const { return _booksCount; }

void Reader::setCategory(const std::string& category) { _category = category; }
void Reader::setLibId(int libraryId) { _libId = libraryId; }
void Reader::setSubscriptionId(int subscriptionId) { _subscriptionId = subscriptionId; }
void Reader::setBooksCount(int booksCount) { _booksCount = booksCount; }

void Reader::setAttribute(const std::string& key, const std::string& value) {
	_attributes.push_back({ key, value });
}
std::vector<std::pair<std::string, std::string>> Reader::Reader::getAttributes() const {
	return _attributes;
}


std::string Reader::serialize() const {
	std::string serializedEntity = std::to_string(_id) 
		+ "|" + PersonInfo::serialize() 
		+ "|" + _category 
		+ "|" + std::to_string(_libId)
		+ "|" + std::to_string(_subscriptionId) 
		+ "|" + std::to_string(_booksCount);

	if (!_attributes.empty()) {
		serializedEntity += "|";
		for (const auto& attr : _attributes) {
			serializedEntity += attr.first + ":" + attr.second + ";";
		}
	}
	return serializedEntity;
}
std::string Reader::toString() const {
	std::string res = "ID: " + std::to_string(_id) + ", " 
		+ PersonInfo::toString() 
		+ ", Бібліотека ID: " + std::to_string(_libId) 
		+ ", Абонимент ID: " + std::to_string(_subscriptionId) 
		+ ", Кількість виданих книг на даний момент: " + std::to_string(_booksCount)
		+ ", Категорія: " + _category;
	for (const auto& attr : _attributes) {
		res += ", " + attr.first + ": " + attr.second;
	}
	return res;
}